#include "senderfile.h"
#include "ui_senderfile.h"
#include "Utils/StringUtilities.h"
#include "Utils/NetUtils.h"

SenderFile::SenderFile(std::string fileToSend, std::atomic_bool &setInFinish, QWidget *parent) :
    QWidget(parent),
    fileSend(fileToSend),
    ui(new Ui::SenderFile),
    fsender(nullptr),
    _setInFinish(setInFinish)
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

    try {
        if (!NetUtils::isValidIPv4(IP) || !NetUtils::isValidPort(ui->linePORT->text().toStdString())) {
            throw NoValidIpPortException();
        }
        fsender = new FileSender(IP, PORT);
        fsender->asyncSendFile(std::string("received_")+fileSend, fileSend, [&](const std::string & filename){
            (std::cout << "ЗВІТ НАДІСЛАНО[" << filename << "]\n").flush();
            _setInFinish.store(true);
        });
    } catch (std::exception &ex) {
        ui->lineIP->clear();
        ui->linePORT->clear();
        qDebug() << "std::exception: " << ex.what();
        QMessageBox::critical(nullptr, "Помилка", "Можливо ви ввели неправильні дані");
    }
}
