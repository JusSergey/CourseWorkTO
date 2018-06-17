#include "form.h"
#include "ui_form.h"
#include "forms.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
}

Form::~Form()
{
    delete ui;
}

void Form::on_buttonExit_clicked()
{
    if (Forms::inst()->getServerHW())
        Forms::inst()->getServerHW()->stopServer();

    QApplication::exit(0);
}

void Form::on_buttonConfigure_clicked()
{
    Forms::inst()->showConfigure();
}

void Form::on_buttonTests_clicked()
{
    Forms::inst()->showTestHW();
}

void Form::on_pushButton_clicked()
{
    Forms::inst()->showServer();
}
