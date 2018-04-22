#ifndef __SUBSYSTEM_NET__
#define __SUBSYSTEM_NET__

#include "Subsystem.h"

class SubsystemNet : public Subsystem {
    static SubsystemNet *instance;
    SubsystemNet() = default;
    static std::string info;

public:
    // get once instance.
    static SubsystemNet *inst()
    {
        if (instance == nullptr) {
            instance = new SubsystemNet;
            info = _getDataFromCommand("./lshw -c network");
        }

        return instance;
    }

//    // get structuring info for device
//    virtual ListInfoT getDeviceInfo() const override {
//        return ListInfoT();
//    }

    virtual string getPrintableInfo() const {
        return info;
    }
};

#endif // __SUBSYSTEM_NET__
