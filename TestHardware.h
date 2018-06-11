#ifndef TESTHARDWARE_H
#define TESTHARDWARE_H

#include <QWebEngineView>
#include <QtWidgets>
#include "types.h"

class TestHardware : public QWidget
{
    Q_OBJECT
public:
    explicit TestHardware(QWidget *parent = nullptr);

private:
    void init();
    QLayout *initCheckboxes();
    QLayout *initButtons();
    QLayout *initWebview();
    void initConnections();
    void saveToFile(QString namefile);

private:
    QWebEngineView *webView         = nullptr;
    QCheckBox *checkCPU             = nullptr;
    QCheckBox *checkRAM             = nullptr;
    QCheckBox *checkHardDrive       = nullptr;
    QPushButton *buttonStartTest    = nullptr;
    QPushButton *buttonGoToMenu     = nullptr;
    QPushButton *buttonSendToServer = nullptr;

private slots:
    void slotStartTest();
    void slotGoToMenu();
    void slotSendToServer();

};

#endif // TESTHARDWARE_H
