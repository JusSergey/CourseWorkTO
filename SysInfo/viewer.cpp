#include "viewer.h"
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;
using namespace chrono;

#include "SubsystemCPU.h"
#include "SubsystemRAM.h"
#include "SubsystemDMI.h"
#include "../Utils/StringUtilities.h"

int getMaxLen(const ListStringPair &lst) {
    size_t max = 0;
    for (const auto &l : lst)
        if (l.first.size() > max)
            max = l.first.size();

    return max;
}

bool isContained(const std::list<string> &lst, const string &src) {
    for (const auto &l : lst)
        if (l == src)
            return true;

    return false;
}

Viewer::Viewer()
{

}

int Viewer::start() const
{
    while (true) {
        this_thread::sleep_for(milliseconds(800));
        show();
    }
    return 0;
}

void Viewer::show() const
{
    system("clear");
    showCPU();
    showRAM();
    showDMI();
}

void Viewer::showCPU() const
{
    cout << "~~~~~~~~~~~~~~~~~~~~CPU~~~~~~~~~~~~~~~~~~~\n";
    cout << SubsystemCPU::inst()->getPrintableInfo();
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
}

void Viewer::showRAM() const
{
    cout << "~~~~~~~~~~~~~~~~~~~~RAM~~~~~~~~~~~~~~~~~~~\n";
    cout << SubsystemRAM::inst()->getPrintableInfo();
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
}

void Viewer::showDMI() const
{
    cout << "~~~~~~~~~~~~~~~~~~~~DMI~~~~~~~~~~~~~~~~~~~\n";
    cout << SubsystemDMI::inst()->getPrintableInfo();
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
}
