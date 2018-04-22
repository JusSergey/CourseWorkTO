#ifndef SERVERHW_H
#define SERVERHW_H

#include <QWidget>
#include "Net/net.h"

namespace Ui {
class ServerHW;
}

class ServerHW : public QWidget
{
    Q_OBJECT

public:
    explicit ServerHW(QWidget *parent = 0);
    ~ServerHW();

private slots:
    void on_buttonStart_clicked();

    void on_buttonStop_clicked();

public:
    void startServer();
    void stopServer();

private:
    Ui::ServerHW *ui;
    FileReceiver *frecver = nullptr;
};

#endif // SERVERHW_H
