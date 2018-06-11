#ifndef __SUBSYSTEM_GPU__
#define __SUBSYSTEM_GPU__

#include "Subsystem.h"

class SubsystemGPU : public Subsystem
{
    static SubsystemGPU *instance;
    SubsystemGPU() : Subsystem(DEVICE::GPU) {}
    static std::string info;

public:
    // get once instance.
    static SubsystemGPU *inst()
    {
        if (instance == nullptr) {
            instance = new SubsystemGPU;
            info = _getDataFromCommand("./lshw -c video");
        }

        return instance;
    }

    // get structuring info for device
    virtual ListStrVects getDeviceInfo() const override;

    // get ball for device
//    virtual double test() override;

    // get formatting info for `cout'
    virtual string getPrintableInfo() const;
};

#endif // __SUBSYSTEM_GPU__
