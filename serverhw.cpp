#include "serverhw.h"
#include "ui_serverhw.h"
#include "Utils/StringUtilities.h"
#include <QtWidgets>
ServerHW::ServerHW(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerHW),
    frecver(nullptr)
{
    ui->setupUi(this);
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setEnabled(false);
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

void ServerHW::startServer()
{
    if (!frecver){
        std::string IP = ui->lineIP->text().toStdString();
        u_short PORT = ui->linePORT->text().toUShort();

        try {
            if (!StringUtil::isValidIPv4(IP) || !StringUtil::isValidPort(ui->linePORT->text().toStdString())) {
                throw NoValidIpPortException();
            }
            frecver = new FileReceiver(IP, PORT);
            frecver->setCallbackFileSendComplete([](const std::string &filename){
                (std::cout << "ЗВІТ ПРИЙНЯТО[" << filename << "]\n").flush();
            });
        }
        catch (std::exception &ex) {
            ui->lineIP->clear();
            ui->linePORT->clear();
            qDebug() << "std::exception" << ex.what();
            QMessageBox::critical(nullptr, "Помилка", "Можливо ви ввели неправильні дані");
            return;
        }
        ui->buttonStart->setEnabled(false);
        ui->buttonStop->setEnabled(true);
    }
}

void ServerHW::stopServer()
{
    if (frecver){
        frecver->stop();
        delete frecver;
        frecver = nullptr;
        ui->buttonStart->setEnabled(true);
        ui->buttonStop->setEnabled(false);
    }
}
