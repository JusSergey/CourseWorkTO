#ifndef __SUBSYSTEM_USB__
#define __SUBSYSTEM_USB__
#include "Subsystem.h"
#include <string>

class SubsystemUSB : public Subsystem
{
    static SubsystemUSB *instance;
    SubsystemUSB() : Subsystem(DEVICE::USB) {}
    static std::string info;

public:
    // get once instance.
    static SubsystemUSB *inst()
    {
        if (instance == nullptr) {
            instance = new SubsystemUSB;
            info = _getDataFromCommand("./lsusb");
        }

        return instance;
    }

    // get structuring info for device
    virtual ListStrVects getDeviceInfo() const override {
        return getStructureInfo(getPrintableInfo());
    }

    virtual string getPrintableInfo() const {
        return SubsystemUSB::info;
    }
};


#endif // __SUBSYSTEM_USB__
