#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "widget.h"

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

private slots:
    void on_buttonExit_clicked();

    void on_buttonConfigure_clicked();

    void on_buttonTests_clicked();

    void on_pushButton_clicked();

private:
    Ui::Form *ui;
};

#endif // FORM_H
