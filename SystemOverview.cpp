#include "SystemOverview.h"
#include "SysInfo/All.h"
#include "Utils/HtmlUtils.h"
#include "senderfile.h"
#include "SaverFile.h"
#include "forms.h"
#include "utils.h"
#include <fstream>
#include <list>
#include <map>

MyTreeView::MyTreeView(QWidget *parent) : QTreeWidget(parent)
{
    setHeaderLabel("PC");
    init();
}

void MyTreeView::init()
{
    setFixedWidth(200);
#define create_node(VarName, Device) \
    auto VarName = new QTreeWidgetItem(this, QStringList(AllModulesMap[DEVICE:: Device]))
#define add_child(Parent, ChildDevice) \
    Parent->addChild(new QTreeWidgetItem(QStringList(AllModulesMap[DEVICE:: ChildDevice])));
#define add_exist_child(VarName, NewVar) \
    (VarName) ->addChild( NewVar );

    create_node(ROOT_HW, HARDWARE);
    add_child(ROOT_HW, CPU);
    add_child(ROOT_HW, DMI);
    add_child(ROOT_HW, USB);
    add_child(ROOT_HW, GPU);
    add_child(ROOT_HW, NETWORK);
    add_child(ROOT_HW, RAM);
    add_child(ROOT_HW, HARD_DRIVE);

    create_node(OS, OS);
    add_child(OS, MODULES);
    add_child(OS, PROCESSES);
    add_child(OS, RAM_MEMORY);

    expandItem(ROOT_HW);
    expandItem(OS);
}

/////////////////////////////////////////////////////////////////////////////////////////
/// IMPLEMENT SYSTEM_OVERVIEW
///
///
SystemOverview::SystemOverview(QWidget *parent) :
    QWidget(parent),
    htmlPage(HtmlUtils::getEmptyHtmlPage()),
    treeView(new MyTreeView(parent)),
    lockedUpdateInfo(false),
    wasSentFile(false)
{
    init();
}

void SystemOverview::init()
{
    setMinimumSize(QSize(600, 400));

    auto mainLayout = new QVBoxLayout;

    auto horisontal = new QHBoxLayout;

    horisontal->addLayout(initTreeWidget());
    horisontal->addLayout(initWebView());
    mainLayout->addLayout(horisontal);
    mainLayout->addLayout(initButtons());
    setLayout(mainLayout);

    initTimerUpdater();

    initConnections();
}

QLayout *SystemOverview::initButtons()
{
    auto l = new QHBoxLayout;

    gotoMenu = new QPushButton("Меню", this);
    sendToServer = new QPushButton("Надiслати на сервер", this);
    buttonSaveAsFile = new QPushButton("Зберегти у файл...", this);
    buttonSaveAllAsFile = new QPushButton("Зберегти все у файл...", this);

    l->addWidget(buttonSaveAllAsFile);
    l->addWidget(buttonSaveAsFile);
    l->addWidget(gotoMenu);
    l->addWidget(sendToServer);

    return l;
}

QLayout *SystemOverview::initTreeWidget()
{
    auto layout = new QHBoxLayout;
    layout->addWidget(treeView);

    return layout;
}

QLayout *SystemOverview::initWebView()
{
    auto l = new QHBoxLayout;
    webView = new QWebEngineView;
    l->addWidget(webView);
    return l;
}

void SystemOverview::initTimerUpdater()
{
    timerForUpdateInfo = new QTimer(this);
    timerForUpdateInfo->start(1000);

    timerWatchdogCompleteSend = new QTimer(this);
    timerWatchdogCompleteSend->start(100);
}

void SystemOverview::mousePressEvent(QMouseEvent *event)
{
    lockedUpdateInfo = true;
}

void SystemOverview::mouseReleaseEvent(QMouseEvent *event)
{
    lockedUpdateInfo = false;
}

void SystemOverview::initConnections()
{
    connect(this->treeView, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(slotClickTreeView(QTreeWidgetItem*,int)));
    connect(this->gotoMenu, SIGNAL(clicked(bool)), this, SLOT(on_pushButton_clicked()));
    connect(this->sendToServer, SIGNAL(clicked(bool)), this, SLOT(slotSendCurrentDataToServer()));
    connect(this->buttonSaveAsFile, SIGNAL(clicked(bool)), this, SLOT(slotSaveAsFile()));
    connect(this->buttonSaveAllAsFile, SIGNAL(clicked(bool)), this, SLOT(slotSaveAllAsFile()));
    connect(this->webView->page(), SIGNAL(scrollPositionChanged(QPointF)), this, SLOT(slotScrollChangePosition(QPointF)));
    connect(this->timerWatchdogCompleteSend, SIGNAL(timeout()), this, SLOT(slotCheckIsSend()));
}

DEVICE SystemOverview::decodeTextNameItem(const QString &name) const
{
    if (name == "CPU") return DEVICE::CPU;
    if (name == "DMI") return DEVICE::DMI;
    if (name == "RAM") return DEVICE::RAM;
    if (name == "GPU") return DEVICE::GPU;
    if (name == "USB") return DEVICE::USB;

    if (name == "NETWORK") return DEVICE::NETWORK;

    if (name == "HARD_DRIVE") return DEVICE::HARD_DRIVE;
    if (name == "PROCESSES")  return DEVICE::PROCESSES;
    if (name == "MODULES")    return DEVICE::MODULES;
    if (name == "RAM_MEMORY") return DEVICE::RAM_MEMORY;

    return DEVICE::UNDEFINED;
}

Subsystem *SystemOverview::getDevice(DEVICE dev) const
{
    switch (dev) {
    case DEVICE::CPU: return SubsystemCPU::inst();
    case DEVICE::DMI: return SubsystemDMI::inst();
    case DEVICE::RAM: return SubsystemRAM::inst();
    case DEVICE::GPU: return SubsystemGPU::inst();
    case DEVICE::USB: return SubsystemUSB::inst();
    case DEVICE::NETWORK: return SubsystemNet::inst();
    case DEVICE::HARD_DRIVE: return SubsystemFilesystem::inst();
    case DEVICE::PROCESSES : return OS::Processes::inst();
    case DEVICE::MODULES   : return OS::Modules::inst();
    case DEVICE::RAM_MEMORY: return OS::RAMMemory::inst();
    default                : return nullptr;
    };
}

void SystemOverview::applySettings()
{
    currentView = getDevice(DEVICE::CPU);
}

void SystemOverview::printResultTest()
{

}

void SystemOverview::slotClickTreeView(QTreeWidgetItem *item, int)
{
    qDebug() << "click tree view..." << ": " << item->text(0);
    currentView = getDevice(decodeTextNameItem(item->text(0)));
    slotUpdateInfoFromDevice();
}

void SystemOverview::slotUpdateInfoFromDevice()
{
    if (currentView) {
        qDebug() << currentView->getPrintableInfo().c_str();
        htmlPage = currentView->getHTMLCode();
        webView->setHtml(QString(htmlPage.c_str()));
        QScroller::scroller(webView->page())->scrollTo(QPointF(-100, -100));
    }
}

void SystemOverview::slotScrollChangePosition(QPointF newPosition)
{
    qDebug() << "scroll";
    currentPosition = newPosition;
}

void SystemOverview::slotSaveAllAsFile()
{
    string allHtmlPage;
    for (Subsystem *subsystem : std::initializer_list<Subsystem*>{
         SubsystemCPU::inst(),
         SubsystemDMI::inst(),
         SubsystemRAM::inst(),
         SubsystemGPU::inst(),
         SubsystemUSB::inst(),
         SubsystemNet::inst(),
         SubsystemFilesystem::inst(),
         OS::Processes::inst(),
         OS::Modules::inst(),
         OS::RAMMemory::inst()
        })
    {
        allHtmlPage += subsystem->getHTMLCode();
        allHtmlPage += HtmlUtils::getBRTag();
    }

    SaverFile::saveLocalFile((const char *)allHtmlPage.c_str(), allHtmlPage.length());
}

void SystemOverview::slotSaveAsFile()
{
    SaverFile::saveLocalFile((const char *)htmlPage.c_str(), htmlPage.length());
}

void SystemOverview::slotCheckIsSend()
{
    if (wasSentFile) {
        wasSentFile.store(false);
        QMessageBox::information(nullptr, "", "Звiт надіслано");
    }
}

void SystemOverview::slotSendCurrentDataToServer()
{
    cout << "slotSendToServer" << std::endl;
    QString namefile = Utils::generateFilename().c_str();
    SaverFile::saveLocalFile(namefile, htmlPage.c_str(), htmlPage.length());
    (new SenderFile(namefile.toStdString(), wasSentFile))->show();
}

void SystemOverview::slotSendAllDataToServer()
{

}

void SystemOverview::on_pushButton_clicked()
{
    Forms::inst()->showMenu();
}

void SystemOverview::on_buttonSend_clicked()
{
    string result;
    for (DEVICE dev : {
         DEVICE::CPU,
         DEVICE::DMI,
         DEVICE::RAM,
         DEVICE::GPU,
         DEVICE::USB,
         DEVICE::NETWORK,
         DEVICE::HARD_DRIVE,
         DEVICE::PROCESSES,
         DEVICE::MODULES,
         DEVICE::RAM_MEMORY
        })
    {
        result += string("~~~~~~~~~~~~~~~~~\n") +
                getDevice(dev)->getPrintableInfo() + "\n\n";
    }

    auto filename = QDateTime::currentDateTime().toString().toStdString();

    std::ofstream file(filename);
    if (!file.is_open()) {
        QMessageBox::information(nullptr, "error", "can't create file for send");
        return;
    }

    file << result;
    file.flush();
    file.close();

    (new SenderFile(filename, wasSentFile))->show();
}
