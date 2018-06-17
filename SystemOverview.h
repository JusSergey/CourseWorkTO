#ifndef SYSTEMOVERVIEW_H
#define SYSTEMOVERVIEW_H

#include <QtWidgets>
#include <QWebEngineView>
#include "SysInfo/Subsystem.h"
#include <map>



static std::map<DEVICE, QString> AllModulesMap {
    std::make_pair(DEVICE::UNDEFINED, "UNDEFINED"),
    std::make_pair(DEVICE::PC, "PC"),
    std::make_pair(DEVICE::HARDWARE, "HARDWARE"),
    std::make_pair(DEVICE::OS, "OS"),
    std::make_pair(DEVICE::CPU, "CPU"),
    std::make_pair(DEVICE::DMI, "DMI"),
    std::make_pair(DEVICE::GPU, "GPU"),
    std::make_pair(DEVICE::USB, "USB"),
    std::make_pair(DEVICE::NETWORK, "NETWORK"),
    std::make_pair(DEVICE::HARD_DRIVE, "HARD_DRIVE"),
    std::make_pair(DEVICE::RAM, "RAM"),
    std::make_pair(DEVICE::MEMORY, "MEMORY"),
    std::make_pair(DEVICE::PROCESSES, "PROCESSES"),
    std::make_pair(DEVICE::MODULES, "MODULES"),
    std::make_pair(DEVICE::RAM_MEMORY, "RAM_MEMORY"),
};

static QString getDeviceName(DEVICE dev) {
    return AllModulesMap[dev];
}

class MyTreeView : public QTreeWidget {
    Q_OBJECT
public:
    explicit MyTreeView(QWidget *parent = nullptr);

private:
    void init();

private:
    std::list<QTreeWidgetItem *> treeItems;
};

///////////////////////////////////
/// \brief The SystemOverview class
///
///
class SystemOverview : public QWidget
{
    Q_OBJECT
public:
    explicit SystemOverview(QWidget *parent = nullptr);

private:
    void init();
    void initTimerUpdater();
    void initConnections();
    QLayout *initButtons();
    QLayout *initTreeWidget();
    QLayout *initWebView();
    Subsystem *getDevice(DEVICE dev) const;
    DEVICE decodeTextNameItem(const QString &name) const;
    void applySettings();
    void printResultTest();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    string htmlPage;
    QPointF currentPosition;
    bool lockedUpdateInfo             = false  ;
    QTimer *timerForUpdateInfo        = nullptr;
    Subsystem *currentView            = nullptr;
    MyTreeView *treeView              = nullptr;
    QPushButton *gotoMenu             = nullptr;
    QPushButton *sendToServer         = nullptr;
    QPushButton *buttonSaveAsFile     = nullptr;
    QPushButton *buttonSaveAllAsFile  = nullptr;
    QWebEngineView *webView           = nullptr;
    QTimer *timerWatchdogCompleteSend = nullptr;
    std::atomic_bool wasSentFile;

private slots:
    void slotClickTreeView(QTreeWidgetItem *item, int);
    void slotUpdateInfoFromDevice();
    void slotScrollChangePosition(QPointF newPosition);
    void slotSaveAllAsFile();
    void slotSaveAsFile();
    void slotCheckIsSend();
    void slotSendCurrentDataToServer();
    void slotSendAllDataToServer();
    void on_pushButton_clicked();
    void on_buttonSend_clicked();
};

#endif // SYSTEMOVERVIEW_H
