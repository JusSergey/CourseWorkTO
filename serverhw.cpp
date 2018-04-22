#include "serverhw.h"
#include "ui_serverhw.h"
#include <QtWidgets>
ServerHW::ServerHW(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerHW)
{
    ui->setupUi(this);
    if (frecver) {
        frecver->stop();
        delete frecver;
    }

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

        frecver = new FileReceiver(IP, PORT);

        frecver->setCallbackFileSendComplete([](const std::string &filename){
            (std::cout << "ЗВІТ ПРИЙНЯТО[" << filename << "]\n").flush();
        });
    }
}

void ServerHW::stopServer()
{
    if (frecver){
        frecver->stop();
        delete frecver;
        frecver = nullptr;
    }
}
