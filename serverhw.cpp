#include "serverhw.h"
#include "ui_serverhw.h"
#include "Utils/StringUtilities.h"
#include "Utils/NetUtils.h"
#include <QtWidgets>
ServerHW::ServerHW(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerHW),
    frecver(nullptr)
{
    ui->setupUi(this);
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setEnabled(false);
    setDefaultFields();
}

ServerHW::~ServerHW()
{
    stopServer();
    delete ui;
}

void ServerHW::on_buttonStart_clicked()
{
    startServer();
}

void ServerHW::on_buttonStop_clicked()
{
    stopServer();
}

void ServerHW::setDefaultFields()
{
    ui->linePORT->clear();
    ui->lineIP->setText(NetUtils::getLocalIP().c_str());
}

void ServerHW::startServer()
{
    if (!frecver){
        std::string IP = ui->lineIP->text().toStdString();
        u_short PORT = ui->linePORT->text().toUShort();

        try {
            if (!NetUtils::isValidIPv4(IP) || !NetUtils::isValidPort(ui->linePORT->text().toStdString())) {
                throw NoValidIpPortException();
            }
            frecver = new FileReceiver(IP, PORT);
            frecver->setCallbackFileSendComplete([](const std::string &filename){
                (std::cout << "ЗВІТ ПРИЙНЯТО[" << filename << "]\n").flush();
            });
        }
        catch (std::exception &ex) {
            setDefaultFields();
            qDebug() << "std::exception" << ex.what();
            QMessageBox::critical(nullptr, "Помилка", "Можливо ви ввели неправильні дані");
            return;
        }
        ui->buttonStart->setEnabled(false);
        ui->buttonStop->setEnabled(true);
    }
    ui->buttonStop->setFocus();
}

void ServerHW::stopServer()
{
    if (frecver){
        frecver->stop();
        delete frecver;
        frecver = nullptr;
        ui->buttonStart->setEnabled(true);
        ui->buttonStop->setEnabled(false);
        ui->buttonStart->setFocus();
    }
}
