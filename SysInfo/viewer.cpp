#include "viewer.h"
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;
using namespace chrono;

#include "SubsystemCPU.h"
#include "SubsystemRAM.h"
#include "SubsystemDMI.h"
#include "StringUtilities.h"

int getMaxLen(const ListInfoT &lst) {
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
//    list<string> names{"processor", "vendor_id", "model name", "cpuMHz", "microcode", "cachesize", "fpu", "cpuidlevel"};
//    for (const auto &p : SubsystemCPU::inst()->getDeviceInfo()) {
//        if (p.first == "processor") cout.put('\n');
////        if (isContained(names, p.first))
//            cout /*<< ' '*/ << p.first << ":" << p.second << '\n';
//    }
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
}

void Viewer::showRAM() const
{
//    auto devinfo = SubsystemRAM::inst()->getDeviceInfo();
//    auto max = getMaxLen(devinfo);

//    list<string> names{"MemTotal", "MemFree", "MemAvailable", "Buffers", "Cached", "SwapTotal", "SwapFree"};
    cout << "~~~~~~~~~~~~~~~~~~~~RAM~~~~~~~~~~~~~~~~~~~\n";
    cout << SubsystemRAM::inst()->getPrintableInfo();
//    for (const auto &p : devinfo) {
//        if (isContained(names, p.first)){
//            auto kb = Number<double>::toNum(StringUtil::resize(p.second, 9, ' '));
//            auto GB = Number<float>::toStr(kb / 1024 / 1024);
//            cout << ' ' << StringUtil::resize(p.first, max, ' ') << " : "
//                 << StringUtil::resize(GB, 5, ' ') << " Gb" << '\n';
//        }
//    }
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
}

void Viewer::showDMI() const
{
    cout << "~~~~~~~~~~~~~~~~~~~~DMI~~~~~~~~~~~~~~~~~~~\n";
    cout << SubsystemDMI::inst()->getPrintableInfo();
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
}
