#include "forms.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Forms::inst()->showMenu();

    return a.exec();
}
