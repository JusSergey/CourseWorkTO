#include "TestHardware.h"
#include "forms.h"
#include "senderfile.h"
#include "Utils/HtmlUtils.h"
#include "Tests/TestRunner.h"
#include "Tests/CPUOperations.h"
#include "Tests/HardDriveSpeed.h"
#include "Tests/RAMReadWriteSpeed.h"

TestHardware::TestHardware(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Test Hardware");

    init();
    initConnections();
}

void TestHardware::init()
{
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
        std::make_tuple(&checkHardDrive, "HardDrive")
    }, this);
}

QLayout *TestHardware::initButtons()
{
    return abstractInit<QPushButton, QVBoxLayout>(
    {
        std::make_tuple(&buttonStartTest, "Тестувати"),
        std::make_tuple(&buttonGoToMenu, "Меню"),
        std::make_tuple(&buttonSendToServer, "Надіслати на сервер")
    }, this);
}

QLayout *TestHardware::initWebview()
{
    auto layoutObj = new QHBoxLayout;
    webView = new QWebEngineView(this);
    layoutObj->addWidget(webView);

    return layoutObj;
}

void TestHardware::initConnections()
{
    connect(buttonStartTest, SIGNAL(clicked(bool)), this, SLOT(slotStartTest()));
    connect(buttonGoToMenu,  SIGNAL(clicked(bool)), this, SLOT(slotGoToMenu()));
    connect(buttonSendToServer, SIGNAL(clicked(bool)), this, SLOT(slotSendToServer()));
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

    auto lambdaTest = [&] (TestTypes type) -> TimeType {

    };

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

    inputDataForHtmlPage.headTitle.push_back("1");
    inputDataForHtmlPage.headTitle.push_back("12");
    inputDataForHtmlPage.headTitle.push_back("123");

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
        inputDataForHtmlPage.data.push_back(
                     StringVector {std::get<TITLE>(t), Number<float>::toStr(std::get<TIME>(t)), std::get<TYPE>(t)}
        );
    }

}

void TestHardware::startTestRAM(HtmlUtils::IGF inputDataForHtmlPage)
{
    condition_variable cv;
    auto resultRW = TestRunner<RAMReadWriteSpeed>::startInCurrentThread(cv);

    int rwSpeed = (float)RAMReadWriteSpeed::getTestDataSizeInMBytes() / resultRW->sec;

    inputDataForHtmlPage.data.push_back({"Test Read/Write speed", Number<int64_t>::toStr(rwSpeed) + "MB/sec"});
}

void TestHardware::startTestHardDrive(HtmlUtils::IGF inputDataForHtmlPage)
{
    const float MBytes = 1024;
    condition_variable cv;
    auto resultWrite = TestRunner<HardDriveSpeedWrite>::startInCurrentThread(cv, MBytes);
    auto resultRead  = TestRunner<HardDriveSpeedRead> ::startInCurrentThread(cv);

    std::cout << "finish hard" << std::endl << ((float)resultWrite->microsec)/1000000.f << std::endl;

    std::cout << resultRead->sec << std::endl;

    int writeSpeed = MBytes / resultWrite->sec;
    int readSpeed  = MBytes / resultRead->sec;

    std::cout << writeSpeed << " : " << readSpeed << std::endl;

    inputDataForHtmlPage.data.push_back({"Test Write Hard Drive",
                                         Number<int>::toStr(writeSpeed) + "MB/sec"});

    inputDataForHtmlPage.data.push_back({"Test Read Hard Drive",
                                         Number<int>::toStr(readSpeed) + "MB/sec"});
}

void TestHardware::slotStartTest()
{
    HtmlUtils::InfoGenerateFile dataToCreateHtmlPage;
    dataToCreateHtmlPage.pageTitle = "Hardware Test";

    cout << "slotStartTest" << std::endl;
    if (checkCPU->isChecked()) {
        startTestCPU(dataToCreateHtmlPage);
    }

    if (checkHardDrive->isChecked()) {
        startTestHardDrive(dataToCreateHtmlPage);
    }

    if (checkRAM->isChecked()) {
        startTestRAM(dataToCreateHtmlPage);
    }

    string htmlPage = HtmlUtils::createAndGetHtmlPage(dataToCreateHtmlPage);
    webView->page()->setHtml(QString(htmlPage.c_str()));
    //... to do
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
