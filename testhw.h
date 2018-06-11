#ifndef TESTHW_H
#define TESTHW_H

#include <QWidget>
#include <string>

namespace Ui {
class TestHW;
}

class TestHW : public QWidget
{
    Q_OBJECT

public:
    explicit TestHW(QWidget *parent = 0);
    ~TestHW();

private slots:
    void on_buttonStart_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::TestHW *ui;
    QTimer *timer;

private:
    std::string testCPU();
    std::string testRAM();
    std::string testHARW_DRIVE();

protected slots:
    void slotCheckCompleteTests();
};

#endif // TESTHW_H
