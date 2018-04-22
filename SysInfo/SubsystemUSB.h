#ifndef __SUBSYSTEM_USB__
#define __SUBSYSTEM_USB__
#include "Subsystem.h"
#include <string>

class SubsystemUSB : public Subsystem,  public NoActiveDev
{
    static SubsystemUSB *instance;
    SubsystemUSB() = default;
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
    virtual ListInfoT getDeviceInfo() const override {
        return ListInfoT();
    }

    virtual string getPrintableInfo() const {
        return info;
    }
};


#endif // __SUBSYSTEM_USB__
