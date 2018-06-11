#ifndef SUBSYSTEMRAM_H
#define SUBSYSTEMRAM_H

#include "Subsystem.h"

constexpr auto PATH_MEMORY_INFO = "/proc/meminfo";

class SubsystemRAM : public Subsystem, public ActiveDev
{
    static SubsystemRAM *instance;
    SubsystemRAM() : Subsystem(DEVICE::RAM){}
    static string info;

public:
    // get once instance.
    inline static SubsystemRAM *inst() {
        if (instance == nullptr) {
            instance = new SubsystemRAM;
        }
        return instance;
    }

    // get formatting info for `cout'
    virtual string getPrintableInfo() const override;
    virtual ListStrVects getDeviceInfo() const override;

    virtual TestResult test();
    double testWrite();
    double testRead();

};

#endif // SUBSYSTEMRAM_H
