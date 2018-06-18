#include "TestHardware.h"
#include "forms.h"
#include "utils.h"
#include "senderfile.h"
#include "Utils/HtmlUtils.h"
#include "SaverFile.h"
#include "Tests/TestRunner.h"
#include "Tests/CPUOperations.h"
#include "Tests/HardDriveSpeed.h"
#include "Tests/RAMReadWriteSpeed.h"
#include "Tests/Queen.h"
#include "Tests/Zipping.h"

TestHardware::TestHardware(QWidget *parent)
    : QWidget(parent),
      cpuTestComplete(false),
      ramTestComplete(false),
      hdiskTestComplete(false),
      queenTestComplete(false),
      zipTestComplete(false),
      cpuUseCache(false),
      cpuNoUseCache(false),
      isNeedToUpdateHtmlView(false),
      wasSentFile(false),
      htmlPage(HtmlUtils::getEmptyHtmlPage())
{
    setWindowTitle("Test Hardware");

    init();
    initConnections();
}

void TestHardware::init()
{
    timerForWatchdogTests = new QTimer(this);
    timerForWatchdogSend  = new QTimer(this);
    timerForWatchdogSend->start(300);
    auto horisontalLayout = new QHBoxLayout;
    auto verticalLayout   = new QVBoxLayout;
    verticalLayout->addLayout(initCheckboxes());
    verticalLayout->addLayout(initButtons());

    QSpacerItem *spacer = new QSpacerItem(buttonSendToServer->size().width() * 1.5, INT16_MAX,
                                          QSizePolicy::Minimum, QSizePolicy::Maximum);

    verticalLayout->addSpacerItem(spacer);

    auto framer = new QWidget(this);
    framer->setLayout(verticalLayout);

    horisontalLayout->addWidget(framer);
    horisontalLayout->addLayout(initWebview());

    setLayout(horisontalLayout);
}

template <typename WidgetType, typename LayoutType>
static QLayout* abstractInit(std::initializer_list<std::tuple<WidgetType **, QString>> initArgs, QWidget *parent = nullptr)
{
    auto *layoutObj = new LayoutType;
    for (auto tpl : initArgs)
        layoutObj->addWidget(*std::get<0>(tpl) = new WidgetType(std::get<1>(tpl), parent));

    return static_cast<QLayout*>(layoutObj);
}

QLayout *TestHardware::initCheckboxes()
{
    return abstractInit<QCheckBox, QVBoxLayout>(
    {
        std::make_tuple(&checkCPU, "ЦП АЛП"),
        std::make_tuple(&checkCPUUseCache, "ЦП Обробка даних + кеш"),
        std::make_tuple(&checkCPUNoUseCache, "ЦП Проста обробка даних"),
        std::make_tuple(&checkRAM, "Оперативна пам'ять"),
        std::make_tuple(&checkHardDrive, "Жорсткий диск"),
        std::make_tuple(&checkQueenTest, "Тест Queen"),
        std::make_tuple(&checkZIPTest, "Архівування ZIP")
    }, this);
}

QLayout *TestHardware::initButtons()
{
    return abstractInit<QPushButton, QVBoxLayout>(
    {
        std::make_tuple(&buttonStartTest, "Тестувати"),
        std::make_tuple(&buttonGoToMenu, "Меню"),
        std::make_tuple(&buttonSendToServer, "Надіслати на сервер"),
        std::make_tuple(&buttonSaveAsFile, "Зберегти у файл...")
    }, this);
}

QLayout *TestHardware::initWebview()
{
    auto layoutObj = new QHBoxLayout;
    webView = new QWebEngineView(this);
    layoutObj->addWidget(webView);

    return layoutObj;
}

void TestHardware::connectWatcherTimer()
{
    connect(timerForWatchdogTests, SIGNAL(timeout()), this, SLOT(slotDogwatcherOnCompleteTests()));
    timerForWatchdogTests->start(300);
}

void TestHardware::disconnectWatcherTimer()
{
    disconnect(timerForWatchdogTests, SIGNAL(timeout()), this, SLOT(slotDogwatcherOnCompleteTests()));
}

void TestHardware::initConnections()
{
    connect(buttonStartTest, SIGNAL(clicked(bool)), this, SLOT(slotStartTest()));
    connect(buttonGoToMenu,  SIGNAL(clicked(bool)), this, SLOT(slotGoToMenu()));
    connect(buttonSendToServer, SIGNAL(clicked(bool)), this, SLOT(slotSendToServer()));
    connect(buttonSaveAsFile, SIGNAL(clicked(bool)), this, SLOT(slotSaveAsFile()));
    connect(timerForWatchdogSend, SIGNAL(timeout()), this, SLOT(slotDogwatcherOnCompleteSend()));
}

void TestHardware::startTestCPU(HtmlUtils::IGF inputDataForHtmlPage)
{
    static constexpr size_t TITLE = 0;
    static constexpr size_t TYPE  = 1;

    enum class TestTypes { x8 = 0, x16, x32, x64, ux8, ux16, ux32, ux64, _float, _double, _ldouble, _ufloat, _udouble, _uldouble };

    auto nth = new ThreadPool(4);
    ThreadPool &threads = *nth;
    auto cv = new condition_variable;

    map<TestTypes, ResultTest::Ptr> df;
    df[TestTypes::x8] = TestRunner<CPU::CPUOperations<int8_t>>::startInNewThread(*cv, threads);
    df[TestTypes::x16] = TestRunner<CPU::CPUOperations<int16_t>>::startInNewThread(*cv, threads);
    df[TestTypes::x32] = TestRunner<CPU::CPUOperations<int32_t>>::startInNewThread(*cv, threads);
    df[TestTypes::x64] = TestRunner<CPU::CPUOperations<int64_t>>::startInNewThread(*cv, threads);
    df[TestTypes::ux8] = TestRunner<CPU::CPUOperations<uint8_t>>::startInNewThread(*cv, threads);
    df[TestTypes::ux16] = TestRunner<CPU::CPUOperations<uint16_t>>::startInNewThread(*cv, threads);
    df[TestTypes::ux32] = TestRunner<CPU::CPUOperations<uint32_t>>::startInNewThread(*cv, threads);
    df[TestTypes::ux64] = TestRunner<CPU::CPUOperations<uint64_t>>::startInNewThread(*cv, threads);
    df[TestTypes::_float] = TestRunner<CPU::CPUOperations<float>>::startInNewThread(*cv, threads);
    df[TestTypes::_double] = TestRunner<CPU::CPUOperations<double>>::startInNewThread(*cv, threads);
    df[TestTypes::_ldouble] = TestRunner<CPU::CPUOperations<long double>>::startInNewThread(*cv, threads);

    threads.waitAll();
    threads.stopAllThreads();

    using TupleT = std::tuple<string, TestTypes>;

    HtmlUtils::Table testCPUTable;

    testCPUTable.nameTable = " CPU Test";

    testCPUTable.fieldsNames.push_back("Тип даних");
    testCPUTable.fieldsNames.push_back("Результат");

    float accumulateBalls(0);
    int countTests(0);
    for (TupleT t : {
         std::make_tuple("8  бітне ціле знакове", TestTypes::x8),
         std::make_tuple("16 бітне ціле знакове", TestTypes::x16),
         std::make_tuple("32 бітне ціле знакове", TestTypes::x32),
         std::make_tuple("64 бітне ціле знакове", TestTypes::x64),
         std::make_tuple("8  бітне ціле беззнакове", TestTypes::ux8),
         std::make_tuple("16 бітне ціле беззнакове", TestTypes::ux16),
         std::make_tuple("32 бітне ціле беззнакове", TestTypes::ux32),
         std::make_tuple("64 бітне ціле беззнакове", TestTypes::ux64),
         std::make_tuple("32 бітне дробове", TestTypes::_float),
         std::make_tuple("64 бітне дробове", TestTypes::_double),
         std::make_tuple("80 бітне дробове", TestTypes::_ldouble)})
    {
        TestTypes type = std::get<TYPE>(t);
        accumulateBalls += static_cast<float>(df[type]->ball);
        testCPUTable.push_back(
            StringVector { std::get<TITLE>(t), Number<float>::toStr(df[type]->sec) }
        );
        countTests++;
    }
    const int MediumBall = accumulateBalls / countTests;
    testCPUTable.push_back(StringVector { "Середній Бал",  Number<int>::toStr(MediumBall) } );

    vector<int> balls;
    for (const auto &dfp : df)
        balls.push_back(dfp.second->ball);

    balls.push_back(MediumBall);

    testCPUTable.getColorByPos = [balls](int row, int col, string text) -> string {
        std::cout << "apu ar: " << text << ':' << balls[row-1] << std::endl;
        return col == 1 && row ? HtmlUtils::getColotFromBall(balls[row-1])
                               : HtmlUtils::Table::getDefaultColorByPos()(row, col, text);
    };

    inputDataForHtmlPage.listTables.emplace_back(std::move(testCPUTable));

    cpuTestComplete.store(true);
}

void TestHardware::startCPUUseCache(HtmlUtils::IGF inputDataForHtmlPage)
{
    condition_variable cv;
    ResultTest::Ptr result = TestRunner<CPU::UseCache>::startInCurrentThread(cv);

    HtmlUtils::Table tableCPUUseCache;
    tableCPUUseCache.nameTable = "Обробка з оптимізацією кешу";
    tableCPUUseCache.fieldsNames = {"Опис", "Значення"};
    tableCPUUseCache.push_back({Number<size_t>::toStr(CPU::AbstractCache::getSizeComputingDataMBytes()) + " МБайт",
                                Number<float>::toStr(result->sec) + " сек"});

    tableCPUUseCache.push_back(StringVector { "Бал",  Number<float>::toStr(result->ball) } );

    int ball = result->ball;

    tableCPUUseCache.getColorByPos = [ball](int row, int col, string text) -> string {
        return col == 1 && row ? HtmlUtils::getColotFromBall(ball)
                               : HtmlUtils::Table::getDefaultColorByPos()(row, col, text);
    };

    inputDataForHtmlPage.listTables.push_back(std::move(tableCPUUseCache));

    cpuUseCache.store(true);
}

void TestHardware::startCPUNoUseCache(HtmlUtils::IGF inputDataForHtmlPage)
{
    condition_variable cv;
    ResultTest::Ptr result = TestRunner<CPU::NoUseCache>::startInCurrentThread(cv);

    HtmlUtils::Table tableCPUNoUseCache;
    tableCPUNoUseCache.nameTable = "Обробка без оптимізації кешу";
    tableCPUNoUseCache.fieldsNames = {"Опис", "Значення"};
    tableCPUNoUseCache.push_back({Number<size_t>::toStr(CPU::AbstractCache::getSizeComputingDataMBytes()) + " МБайт",
                                  Number<float>::toStr(result->sec) + " сек"});

    tableCPUNoUseCache.push_back(StringVector { "Бал",  Number<float>::toStr(result->ball) } );

    int ball = result->ball;

    tableCPUNoUseCache.getColorByPos = [ball](int row, int col, string text) -> string {
        return col == 1 && row ? HtmlUtils::getColotFromBall(ball)
                               : HtmlUtils::Table::getDefaultColorByPos()(row, col, text);
    };

    inputDataForHtmlPage.listTables.push_back(std::move(tableCPUNoUseCache));

    cpuNoUseCache.store(true);
}

void TestHardware::startTestRAM(HtmlUtils::IGF inputDataForHtmlPage)
{
    condition_variable cv;
    ResultTest::Ptr result = TestRunner<RAMReadWriteSpeed>::startInCurrentThread(cv);

    int rwSpeed = (float)RAMReadWriteSpeed::getTestDataSizeInMBytes() / result->sec;

    HtmlUtils::Table testRAMTable;
    testRAMTable.nameTable = "Ram Test";
    testRAMTable.fieldsNames.push_back("Description");
    testRAMTable.fieldsNames.push_back("Speed");
    testRAMTable.push_back({"Test Read/Write speed", Number<int64_t>::toStr(rwSpeed) + "MB/sec"});

    testRAMTable.push_back(StringVector { "Бал",  Number<float>::toStr(result->ball) } );

    int ball = result->ball;

    testRAMTable.getColorByPos = [ball](int row, int col, string text) -> string {

        return col == 1 && row ? HtmlUtils::getColotFromBall(ball)
                               : HtmlUtils::Table::getDefaultColorByPos()(row, col, text);
    };

    inputDataForHtmlPage.listTables.emplace_back(std::move(testRAMTable));

    ramTestComplete.store(true);
}

void TestHardware::startTestHardDrive(HtmlUtils::IGF inputDataForHtmlPage)
{
    const float MBytes = 1024;
    condition_variable cv;
    auto resultWrite = TestRunner<HardDriveSpeedWrite>::startInCurrentThread(cv, MBytes);
    auto resultRead  = TestRunner<HardDriveSpeedRead> ::startInCurrentThread(cv);

    std::cout << "finish hard" << std::endl << ((float)resultWrite->microsec)/1000000.f << std::endl;

    std::cout << resultRead->sec << std::endl;

    // callculate the results
    int writeSpeed = MBytes / resultWrite->sec;
    int readSpeed  = MBytes / resultRead->sec;

    std::cout << writeSpeed << " : " << readSpeed << std::endl;

    // adding result of test
    HtmlUtils::Table testHardDriveTable;
    testHardDriveTable.nameTable = "Hard Disk Test";
    testHardDriveTable.fieldsNames.push_back("Description");
    testHardDriveTable.fieldsNames.push_back("Speed");
    testHardDriveTable.push_back({"Test Write Hard Drive",
                                         Number<int>::toStr(writeSpeed) + "MB/sec"});
    testHardDriveTable.push_back({"Test Read Hard Drive",
                                         Number<int>::toStr(readSpeed) + "MB/sec"});

    testHardDriveTable.push_back(StringVector { "Середній Бал",  Number<int>::toStr((resultWrite->ball + resultRead->ball) / 2) } );

    vector<int> balls {resultWrite->ball, resultRead->ball, (resultWrite->ball + resultRead->ball) / 2};

    testHardDriveTable.getColorByPos = [balls](int row, int col, string text) -> string {
        return col == 1 && row ? HtmlUtils::getColotFromBall(balls[row-1])
                               : HtmlUtils::Table::getDefaultColorByPos()(row, col, text);
    };

    inputDataForHtmlPage.listTables.emplace_back(std::move(testHardDriveTable));

    //set flag of complete test
    hdiskTestComplete.store(true);
}

void TestHardware::startTestQueen(HtmlUtils::IGF inputDataForHtmlPage)
{
    condition_variable cv;
    ResultTest::Ptr result(TestRunner<Queen>::startInCurrentThread(cv));

    HtmlUtils::Table tableOfQueen;
    tableOfQueen.nameTable = "Queen Test";
    tableOfQueen.fieldsNames = {"Опис", "Значення"};
    tableOfQueen.push_back({"Час", Number<float>::toStr(result->sec)});

    tableOfQueen.push_back(StringVector { "Бал",  Number<float>::toStr(result->ball) } );

    const int ball = result->ball;

    tableOfQueen.getColorByPos = [ball](int row, int col, string text) -> string {
        return col == 1 && row ? HtmlUtils::getColotFromBall(ball)
                               : HtmlUtils::Table::getDefaultColorByPos()(row, col, text);
    };

    inputDataForHtmlPage.listTables.push_back(std::move(tableOfQueen));

    queenTestComplete.store(true);
}

void TestHardware::startTestZIP(HtmlUtils::IGF inputDataForHtmlPage)
{
    condition_variable cv;
    ResultTest::Ptr resultCompress(TestRunner<ZipCompress>::startInCurrentThread(cv));
    ResultTest::Ptr resultUncompress(TestRunner<ZipUncompress>::startInCurrentThread(cv));

    HtmlUtils::Table tableOfZip;

    tableOfZip.nameTable = "Стиснення за допомогою zip архіватора";
    tableOfZip.fieldsNames = {"Опис", "Розмір данних", "Час роботи"};

    tableOfZip.push_back({"Стиснення", Number<size_t>::toStr(ZipCompress::getSizeFile())   + "MBytes", Number<float>::toStr(resultCompress->sec)});
    tableOfZip.push_back({"Екстракт" , Number<size_t>::toStr(ZipUncompress::getSizeFile()) + "MBytes", Number<float>::toStr(resultUncompress->sec)});

    tableOfZip.push_back(StringVector { "Середній Бал", "", Number<float>::toStr((resultCompress->ball + resultUncompress->ball) / 2) } );

    vector<int> balls {resultCompress->ball, resultUncompress->ball, (resultCompress->ball+resultUncompress->ball) / 2};

    tableOfZip.getColorByPos = [balls](int row, int col, string text) -> string {
        return col == 2 && row ? HtmlUtils::getColotFromBall(balls[row-1])
                               : HtmlUtils::Table::getDefaultColorByPos()(row, col, text);
    };

    inputDataForHtmlPage.listTables.push_back(std::move(tableOfZip));
    zipTestComplete.store(true);
}

bool TestHardware::isAllTestsComplete() const
{
    return (cpuTestComplete   &&
            ramTestComplete   &&
            zipTestComplete   &&
            hdiskTestComplete &&
            queenTestComplete &&
            cpuUseCache       &&
            cpuNoUseCache);
}

void TestHardware::startTest()
{
    HtmlUtils::InfoGenerateFile dataToCreateHtmlPage;
    dataToCreateHtmlPage.pageTitle = "Hardware Test";

    cout << "slotStartTest" << std::endl;
    if (checkCPU->isChecked()) {
        cpuTestComplete.store(false);
        startTestCPU(dataToCreateHtmlPage);
        isNeedToUpdateHtmlView.store(true);
        htmlPage = HtmlUtils::createAndGetHtmlPage(dataToCreateHtmlPage);
    } else cpuTestComplete.store(true);

    if (checkCPUUseCache->isChecked()) {
        cpuUseCache.store(false);
        startCPUUseCache(dataToCreateHtmlPage);
        isNeedToUpdateHtmlView.store(true);
        htmlPage = HtmlUtils::createAndGetHtmlPage(dataToCreateHtmlPage);
    } else cpuUseCache.store(true);

    if (checkCPUNoUseCache->isChecked()) {
        cpuNoUseCache.store(false);
        startCPUNoUseCache(dataToCreateHtmlPage);
        isNeedToUpdateHtmlView.store(true);
        htmlPage = HtmlUtils::createAndGetHtmlPage(dataToCreateHtmlPage);
    } else cpuNoUseCache.store(true);

    if (checkHardDrive->isChecked()) {
        hdiskTestComplete.store(false);
        startTestHardDrive(dataToCreateHtmlPage);
        isNeedToUpdateHtmlView.store(true);
        htmlPage = HtmlUtils::createAndGetHtmlPage(dataToCreateHtmlPage);
    } else hdiskTestComplete.store(true);

    if (checkRAM->isChecked()) {
        ramTestComplete.store(false);
        startTestRAM(dataToCreateHtmlPage);
        isNeedToUpdateHtmlView.store(true);
        htmlPage = HtmlUtils::createAndGetHtmlPage(dataToCreateHtmlPage);
    } else ramTestComplete.store(true);

    if (checkQueenTest->isChecked()) {
        queenTestComplete.store(false);
        startTestQueen(dataToCreateHtmlPage);
        isNeedToUpdateHtmlView.store(true);
        htmlPage = HtmlUtils::createAndGetHtmlPage(dataToCreateHtmlPage);
    } else queenTestComplete.store(true);

    if (checkZIPTest->isChecked()) {
        zipTestComplete.store(false);
        startTestZIP(dataToCreateHtmlPage);
        isNeedToUpdateHtmlView.store(true);
        htmlPage = HtmlUtils::createAndGetHtmlPage(dataToCreateHtmlPage);
    } else zipTestComplete.store(true);

}

void TestHardware::slotStartTest()
{
    Runnable::Ptr ptr(new RunAsyncTests(this));
    ThreadPool::defaultPool()->start(std::move(ptr), true);

    htmlPage = HtmlUtils::getEmptyHtmlPage();
    webView->setHtml(QString(htmlPage.c_str()));

    buttonSendToServer->setEnabled(false);
    buttonStartTest->setEnabled(false);
    buttonSaveAsFile->setEnabled(false);
    connectWatcherTimer();
}

void TestHardware::slotGoToMenu()
{
    cout << "slotGoToMenu" << std::endl;
    Forms::inst()->showMenu();
}

void TestHardware::slotSendToServer()
{
    cout << "slotSendToServer" << std::endl;
    QString namefile = Utils::generateFilename().c_str();
    SaverFile::saveLocalFile(namefile, htmlPage.c_str(), htmlPage.length());
    (new SenderFile(namefile.toStdString(), wasSentFile))->show();
}

void TestHardware::slotSaveAsFile()
{
    SaverFile::saveLocalFile((const char *)htmlPage.c_str(), htmlPage.length());
}

void TestHardware::slotDogwatcherOnCompleteTests()
{
    if (isNeedToUpdateHtmlView.load()) {
        webView->page()->setHtml(QString(htmlPage.c_str()));
        isNeedToUpdateHtmlView.store(false);
    }
    else if (isAllTestsComplete()) {
        webView->page()->setHtml(QString(htmlPage.c_str()));
        disconnectWatcherTimer();
        buttonSendToServer->setEnabled(true);
        buttonStartTest->setEnabled(true);
        buttonSaveAsFile->setEnabled(true);
    }
}

void TestHardware::slotDogwatcherOnCompleteSend()
{
    if (wasSentFile) {
        wasSentFile.store(false);
        QMessageBox::information(nullptr, "", "Звіт надіслано");
    }
}
