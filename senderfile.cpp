#include "senderfile.h"
#include "ui_senderfile.h"

SenderFile::SenderFile(std::string fileToSend, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SenderFile),
    fileSend(fileToSend)
{
    ui->setupUi(this);
}

SenderFile::~SenderFile()
{
    if (fsender) {
        fsender->stop();
        delete fsender;
        fsender = nullptr;
    }
    delete ui;
}

void SenderFile::on_pushButton_clicked()
{
    std::string IP = ui->lineIP->text().toStdString();
    u_short PORT = ui->linePORT->text().toUShort();

    fsender = new FileSender(IP, PORT);

    fsender->asyncSendFile(std::string("received_")+fileSend, fileSend, [&](const std::string & filename){
        (std::cout << "ЗВІТ НАДІСЛАНО[" << filename << "]\n").flush();
    });
}
