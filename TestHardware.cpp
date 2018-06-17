#include "TestHardware.h"
#include "forms.h"
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
      isNeedToUpdateHtmlView(false),
      htmlPage(HtmlUtils::getEmptyHtmlPage())
{
    setWindowTitle("Test Hardware");

    init();
    initConnections();
}

void TestHardware::init()
{
    timerForWatcher = new QTimer(this);
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
        std::make_tuple(&checkCPU, "CPU"),
        std::make_tuple(&checkRAM, "RAM"),
        std::make_tuple(&checkHardDrive, "HardDrive"),
        std::make_tuple(&checkQueenTest, "Queen Test"),
        std::make_tuple(&checkZIPTest, "ZIP Test")
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
    connect(timerForWatcher, SIGNAL(timeout()), this, SLOT(slotWatcherOnCompleteTests()));
    timerForWatcher->start(300);
}

void TestHardware::disconnectWatcherTimer()
{
    disconnect(timerForWatcher, SIGNAL(timeout()), this, SLOT(slotWatcherOnCompleteTests()));
}

void TestHardware::initConnections()
{
    connect(buttonStartTest, SIGNAL(clicked(bool)), this, SLOT(slotStartTest()));
    connect(buttonGoToMenu,  SIGNAL(clicked(bool)), this, SLOT(slotGoToMenu()));
    connect(buttonSendToServer, SIGNAL(clicked(bool)), this, SLOT(slotSendToServer()));
    connect(buttonSaveAsFile, SIGNAL(clicked(bool)), this, SLOT(slotSaveAsFile()));
}

void TestHardware::saveToFile(QString namefile)
{

}

void TestHardware::startTestCPU(HtmlUtils::IGF inputDataForHtmlPage)
{
    static constexpr size_t TITLE = 0;
    static constexpr size_t TIME  = 1;
    static constexpr size_t TYPE  = 2;
    static constexpr size_t DESC  = 3;
    static constexpr size_t END   = 4;

    enum class TestTypes { x8, x16, x32, x64, ux8, ux16, ux32, ux64, _float, _double, _ldouble, _ufloat, _udouble, _uldouble };

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

    using TupleT = std::tuple<string, float, string>;

    HtmlUtils::Table testCPUTable;

    testCPUTable.nameTest = " CPU Test";

    testCPUTable.headTitle.push_back("Тип даних");
    testCPUTable.headTitle.push_back("Результат");
    testCPUTable.headTitle.push_back("Тип");

    for (TupleT t : {
         std::make_tuple("8 bit integer", (float)df[TestTypes::x8]->microsec / 1000000.f, "signed"),
         std::make_tuple("16 bit integer", (float)df[TestTypes::x16]->microsec / 1000000.f, "signed"),
         std::make_tuple("32 bit integer", (float)df[TestTypes::x32]->microsec / 1000000.f, "signed"),
         std::make_tuple("64 bit integer", (float)df[TestTypes::x64]->microsec / 1000000.f, "signed"),
         std::make_tuple("8 bit integer", (float)df[TestTypes::ux8]->microsec / 1000000.f, "unsigned"),
         std::make_tuple("16 bit integer", (float)df[TestTypes::ux16]->microsec / 1000000.f, "unsigned"),
         std::make_tuple("32 bit integer", (float)df[TestTypes::ux32]->microsec / 1000000.f, "unsigned"),
         std::make_tuple("64 bit integer", (float)df[TestTypes::ux64]->microsec / 1000000.f, "unsigned"),
         std::make_tuple("float", (float)df[TestTypes::_float]->microsec / 1000000.f, "only signed"),
         std::make_tuple("double", (float)df[TestTypes::_double]->microsec / 1000000.f, "only signed"),
         std::make_tuple("long double", (float)df[TestTypes::_ldouble]->microsec / 1000000.f, "only signed")})
    {
        testCPUTable.push_back(
                     StringVector {std::get<TITLE>(t), Number<float>::toStr(std::get<TIME>(t)), std::get<TYPE>(t)}
        );
    }

    inputDataForHtmlPage.listTables.emplace_back(std::move(testCPUTable));

    cpuTestComplete.store(true);
}

void TestHardware::startTestRAM(HtmlUtils::IGF inputDataForHtmlPage)
{
    condition_variable cv;
    auto resultRW = TestRunner<RAMReadWriteSpeed>::startInCurrentThread(cv);

    int rwSpeed = (float)RAMReadWriteSpeed::getTestDataSizeInMBytes() / resultRW->sec;

    HtmlUtils::Table testRAMTable;
    testRAMTable.nameTest = "Ram Test";
    testRAMTable.headTitle.push_back("Description");
    testRAMTable.headTitle.push_back("Speed");
    testRAMTable.push_back({"Test Read/Write speed", Number<int64_t>::toStr(rwSpeed) + "MB/sec"});
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
    testHardDriveTable.nameTest = "Hard Disk Test";
    testHardDriveTable.headTitle.push_back("Description");
    testHardDriveTable.headTitle.push_back("Speed");
    testHardDriveTable.push_back({"Test Write Hard Drive",
                                         Number<int>::toStr(writeSpeed) + "MB/sec"});
    testHardDriveTable.push_back({"Test Read Hard Drive",
                                         Number<int>::toStr(readSpeed) + "MB/sec"});
    inputDataForHtmlPage.listTables.emplace_back(std::move(testHardDriveTable));

    //set flag of complete test
    hdiskTestComplete.store(true);
}

void TestHardware::startTestQueen(HtmlUtils::IGF inputDataForHtmlPage)
{
    condition_variable cv;
    ResultTest::Ptr result(TestRunner<Queen>::startInCurrentThread(cv));

    HtmlUtils::Table tableOfQueen;
    tableOfQueen.nameTest = "Queen Test";
    tableOfQueen.headTitle = {"Опис", "бал 1/10"};
    tableOfQueen.push_back({"бал", Number<float>::toStr(result->sec)});

    inputDataForHtmlPage.listTables.push_back(std::move(tableOfQueen));

    queenTestComplete.store(true);
}

void TestHardware::startTestZIP(HtmlUtils::IGF inputDataForHtmlPage)
{
    condition_variable cv;
    ResultTest::Ptr resultCompress(TestRunner<ZipCompress>::startInCurrentThread(cv));
    ResultTest::Ptr resultUncompress(TestRunner<ZipUncompress>::startInCurrentThread(cv));

    HtmlUtils::Table tableOfZip;

    tableOfZip.nameTest = "Стиснення за допомогою zip архіватора";
    tableOfZip.headTitle = {"Операція", "Розмір данних", "Час роботи"};

    tableOfZip.push_back({"Стиснення", Number<size_t>::toStr(ZipCompress::getSizeFile())   + "MBytes", Number<float>::toStr(resultCompress->sec)});
    tableOfZip.push_back({"Екстракт" , Number<size_t>::toStr(ZipUncompress::getSizeFile()) + "MBytes", Number<float>::toStr(resultUncompress->sec)});

    inputDataForHtmlPage.listTables.push_back(std::move(tableOfZip));

    zipTestComplete.store(true);
}

bool TestHardware::isAllTestsComplete() const
{
    return (cpuTestComplete   &&
            ramTestComplete   &&
            zipTestComplete   &&
            hdiskTestComplete &&
            queenTestComplete);
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
    QString namefile = QDate::currentDate().toString() + ':' + QTime::currentTime().toString();
    saveToFile(namefile);
    (new SenderFile(namefile.toStdString()))->show();
}

void TestHardware::slotSaveAsFile()
{
    SaverFile::saveLocalFile((const char *)htmlPage.c_str(), htmlPage.length());
}

void TestHardware::slotWatcherOnCompleteTests()
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
