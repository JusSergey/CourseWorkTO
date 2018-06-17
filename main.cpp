#include "forms.h"
#include <QApplication>

#include "SysInfo/SubsystemFilesystem.h"

#include "Tests/TestRunner.h"

#include "Tests/Queen.h"
#include "Tests/Zipping.h"


#include "utils.h"
#include "SysInfo/SubsystemCPU.h"
#include "SysInfo/SubsystemDMI.h"
#include "SysInfo/SubsystemRAM.h"
#include "SysInfo/SubsystemGPU.h"
#include "Utils/HtmlUtils.h"
#include "SystemOverview.h"
#include "TestHardware.h"

#include "Tests/CPUOperations.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Forms::inst()->showMenu();
    //auto testHW = new TestHardware;
    //testHW->show();

    return a.exec();
}
