#ifndef SENDERFILE_H
#define SENDERFILE_H

#include <QtWidgets>
#include "Net/net.h"

namespace Ui {
class SenderFile;
}

class SenderFile : public QWidget
{
    Q_OBJECT
    std::string fileSend;
public:
    explicit SenderFile(std::string fileToSend, QWidget *parent = 0);
    ~SenderFile();

private slots:
    void on_pushButton_clicked();

private:
    Ui::SenderFile *ui;
    FileSender *fsender;
};

#endif // SENDERFILE_H
