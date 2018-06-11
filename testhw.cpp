#include "testhw.h"
#include "ui_testhw.h"
#include "SysInfo/All.h"
#include "forms.h"

TestHW::TestHW(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestHW),
    timer(new QTimer(this))
{
    ui->setupUi(this);
    QFont f("monospace");
    f.setPixelSize(16);
    ui->textEdit->setFont(f);

    connect(timer, SIGNAL(timeout()), this, SLOT(slotCheckCompleteTests()));
    timer->start(300);
}

TestHW::~TestHW()
{
    delete ui;
}

void TestHW::slotCheckCompleteTests() {
    std::cout << "." << std::endl;
}

void TestHW::on_buttonStart_clicked()
{
    string text;
    ui->textEdit->clear();
    ui->textEdit->repaint();

    if (ui->checkCPU->isChecked()) {
        text += "\n~~~~~~CPU~~~~~~~\n";
        text += testCPU();
        ui->textEdit->setText(text.c_str());
        ui->textEdit->repaint();
    }

    if (ui->checkRAM->isChecked()) {
        text += "\n~~~~~~RAM~~~~~~~\n";
        text += testRAM();
        ui->textEdit->setText(text.c_str());
        ui->textEdit->repaint();
    }

    if (ui->checkHARD_DRIVE->isChecked()) {
        text += "\n~~~~~~HARD_DRIVE~~~~~~~\n";
        text += testHARW_DRIVE();
        ui->textEdit->setText(text.c_str());
        ui->textEdit->repaint();
    }

    ui->labelStatus->setText("");
}

std::string TestHW::testCPU()
{
    SubsystemCPU *cpu = SubsystemCPU::inst();
    ui->labelStatus->setText("testing CPU...");
    ui->labelStatus->repaint();

    string result;

    for (auto p : {
         std::make_pair("int_8",  cpu->testTemplateValue<int8_t>()),
         std::make_pair("int_16", cpu->testTemplateValue<int16_t>()),
         std::make_pair("int_32", cpu->testTemplateValue<int32_t>()),
         std::make_pair("int_64", cpu->testTemplateValue<int64_t>()),
         std::make_pair("float", cpu->testTemplateValue<float>()),
         std::make_pair("double", cpu->testTemplateValue<double>()),
         std::make_pair("long double", cpu->testTemplateValue<long double>())
        })
    {
        result += p.second.first + " " + p.first + '\n';
    }

    ((result += "cache test:     ") += cpu->testCache()) += '\n';
    ((result += "iteration test: ") += cpu->testIteration() + "sec") += '\n';

    return result;
}

std::string TestHW::testRAM()
{
    SubsystemRAM *ram = SubsystemRAM::inst();
    ui->labelStatus->setText("testing RAM...");
    ui->labelStatus->repaint();
    auto result = ram->test();
    return result.first + '\n';
}

std::string TestHW::testHARW_DRIVE()
{
    SubsystemFilesystem *fss = SubsystemFilesystem::inst();
    ui->labelStatus->setText("testing HARD DRIVE...");
    ui->labelStatus->repaint();
    return std::string("read:  512MB / ") + Number<float>::toStr(fss->testRead()) + "sec\n" +
           std::string("write: 512MB / ") + Number<float>::toStr(fss->testWrite())+ "sec\n";
}

void TestHW::on_pushButton_clicked()
{
    Forms::inst()->showMenu();
}

#include <fstream>
#include "senderfile.h"
using std::ofstream;

void TestHW::on_pushButton_2_clicked()
{
    auto filename = QDateTime::currentDateTime().toString().toStdString();

    ofstream file(filename);
    if (!file.is_open()) {
        QMessageBox::information(nullptr, "error", "can't create file for send");
        return;
    }

    file << ui->textEdit->toPlainText().toStdString();
    file.flush();
    file.close();

    (new SenderFile(filename))->show();

}
