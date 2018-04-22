#ifndef __SUBSYSTEM_GPU__
#define __SUBSYSTEM_GPU__

#include "Subsystem.h"

class SubsystemGPU : public Subsystem,  public NoActiveDev
{
    static SubsystemGPU *instance;
    SubsystemGPU() = default;
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
    virtual ListInfoT getDeviceInfo() const override;

    // get ball for device
//    virtual double test() override;

    // get formatting info for `cout'
    virtual string getPrintableInfo() const;
};

#endif // __SUBSYSTEM_GPU__
