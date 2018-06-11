#include "TestHardware.h"
#include "forms.h"
#include "senderfile.h"

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

void TestHardware::slotStartTest()
{
    cout << "slotStartTest" << std::endl;
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
