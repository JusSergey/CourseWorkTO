#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets>
#include "SysInfo/Subsystem.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    enum class DEVICE : int { UNDEFINED,
                              CPU, DMI, GPU, USB, NETWORK,
                              HARD_DRIVE, RAM,
                              PROCESSES, MODULES, RAM_MEMORY };

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::Widget *ui;
    bool lockedUpdateInfo = false;
    QTimer *timerForUpdateInfo;
    Subsystem *currentView = nullptr;
    void initConnections();
    DEVICE decodeTextNameItem(const QString &name) const;
    Subsystem* getDevice(DEVICE dev) const;

    void applySettings();

    void printResultTest();

private slots:
    void slotClickTreeView(QTreeWidgetItem*, int);
    void slotUpdateInfoFromDevice();
    void on_pushButton_clicked();
    void on_buttonSend_clicked();
};

#endif // WIDGET_H
