#include "forms.h"
#include <QApplication>

#include "SysInfo/SubsystemFilesystem.h"

#include "Tests/TestRunner.h"

class Tst {
public:
    void startTest(){
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 3));
    }
};
#include "Tests/Queen.h"
#include "Tests/Zipping.h"

class MyRun : public Runnable {
public:
    virtual void run() override {
        std::condition_variable cv;
        TestRunner<ZipCompress>::ResultTest::Ptr result = TestRunner<ZipCompress>::startInNewThread(cv);
        std::mutex m;
        std::unique_lock<std::mutex> ul(m);
        while (!result->finished)
            cv.wait(ul);

        std::cout << "complete" << std::endl;
    }
};


void start_new_thr(){
    ThreadPool::defaultPool()->start(Runnable::Ptr(new MyRun));
}

#include "utils.h"
#include "SysInfo/SubsystemCPU.h"
#include "SysInfo/SubsystemDMI.h"
#include "SysInfo/SubsystemRAM.h"
#include "SysInfo/SubsystemGPU.h"
#include "Utils/HtmlUtils.h"
#include "SystemOverview.h"
#include "TestHardware.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Forms::inst()->showMenu();
    //auto testHW = new TestHardware;
    //testHW->show();

    return a.exec();
}
