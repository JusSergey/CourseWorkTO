#include "widget.h"
#include "ui_widget.h"
#include "SysInfo/All.h"
#include "forms.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    applySettings();
    initConnections();

    printResultTest();
}

Widget::~Widget()
{
    qDebug() << "widget destructor";
    delete ui;
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    lockedUpdateInfo = true;
    qDebug() << "mouse press";
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    lockedUpdateInfo = false;
    qDebug() << "mouse release";
}

void Widget::initConnections()
{
    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(slotClickTreeView(QTreeWidgetItem*, int)));



//    timerForUpdateInfo = new QTimer(this);
//    connect(timerForUpdateInfo, SIGNAL(timeout()), this, SLOT(slotUpdateInfoFromDevice()));
//    timerForUpdateInfo->start(1000);
}

Widget::DEVICE Widget::decodeTextNameItem(const QString &name) const
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

Subsystem *Widget::getDevice(Widget::DEVICE dev) const
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
    default:          return nullptr;
    };
}

void Widget::applySettings()
{
    ui->infoFromDevice->setMouseTracking(true);
    QFont f("monospace");
    f.setPixelSize(16);
    ui->infoFromDevice->setFont(f);
    setWindowTitle("Hardware");
    currentView = getDevice(DEVICE::CPU);
}

void Widget::printResultTest()
{
//    auto ram = (SubsystemRAM*)getDevice(DEVICE::RAM);
//    std::cout << "test ram: " << ram->test();
//    auto cpu = (SubsystemCPU*)getDevice(DEVICE::CPU);
//    auto fss = (SubsystemFilesystem*)getDevice(DEVICE::CPU);

//    std::cout << "test cache: " << cpu->testCache() << '\n'
//              << "test hddwr: " << fss->testWrite() << '\n';

//    std::cout << "write finish...\n"
//              << "test hddrd: " << fss->testRead() << '\n';

//    std::cout << "cpu iteration  : " << cpu->testIteration() << '\n'
//              << "cpu float      : " << cpu->testTemplateValue<float>()   << '\n'
//              << "cpu double     : " << cpu->testTemplateValue<double>()   << '\n'
//              << "cpu long double: " << cpu->testTemplateValue<long double>()   << '\n'

//              << "cpu int8_t     : " << cpu->testTemplateValue<int8_t>()   << '\n'
//              << "cpu int16_t    : " << cpu->testTemplateValue<int16_t>()   << '\n'
//              << "cpu int32_t    : " << cpu->testTemplateValue<int32_t>()   << '\n'
//              << "cpu int64_t    : " << cpu->testTemplateValue<int64_t>()   << '\n'

//              << "cpu uint8_t    : " << cpu->testTemplateValue<uint8_t>()   << '\n'
//              << "cpu uint16_t   : " << cpu->testTemplateValue<uint16_t>()   << '\n'
//              << "cpu uint32_t   : " << cpu->testTemplateValue<uint32_t>()   << '\n'
//              << "cpu uint64_t   : " << cpu->testTemplateValue<uint64_t>()   << '\n'
//    ;
}

void Widget::slotClickTreeView(QTreeWidgetItem *item, int n)
{
    qDebug() << "click tree view..." << n << ": " << item->text(0);
    currentView = getDevice(decodeTextNameItem(item->text(0)));
    slotUpdateInfoFromDevice();
}

void Widget::slotUpdateInfoFromDevice()
{
    if (currentView && !lockedUpdateInfo) {
        ui->infoFromDevice->setText(currentView->getPrintableInfo().c_str());
    }
}

void Widget::on_pushButton_clicked()
{
    Forms::inst()->showMenu();
}

#include "senderfile.h"
#include <fstream>
using std::ofstream;

void Widget::on_buttonSend_clicked()
{
    string result;
    for (DEVICE dev :  {
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

    ofstream file(filename);
    if (!file.is_open()) {
        QMessageBox::information(nullptr, "error", "can't create file for send");
        return;
    }

    file << result;
    file.flush();
    file.close();

    (new SenderFile(filename))->show();

}
