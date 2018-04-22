#include "widget.h"
#include "forms.h"
#include <QApplication>

#include "SysInfo/SubsystemFilesystem.h"

#define _TEST

#ifdef _TEST

#include "Threads/All.h"
#include "mutex"
static std::mutex _mutex;

class MyRun : public Runnable {
    int _id;
public:
    MyRun(int id) : _id(id) {}
    virtual void run() {
        for (int i = 0; i < 100; ++i) {
            Thread::sleep(500);
            std::unique_lock<std::mutex> lock_mut(_mutex);
            std::cout << _id << " " << i << std::endl;
        }
    }
};

int main() {
    ThreadPool pool;
    pool.start(std::move(Runnable::Ptr(new MyRun(1))));
    pool.start(std::move(Runnable::Ptr(new MyRun(2))));
    pool.start(std::move(Runnable::Ptr(new MyRun(3))));
    pool.start(std::move(Runnable::Ptr(new MyRun(4))));
    pool.waitAll();
}

#else
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Forms::inst()->showMenu();

    return a.exec();
}
#endif
