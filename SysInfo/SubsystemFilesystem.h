#ifndef SUBSYSTEMFILESYSTEM_H
#define SUBSYSTEMFILESYSTEM_H
#include "Subsystem.h"

class SubsystemFilesystem : public ActiveDev, public Subsystem
{
    static SubsystemFilesystem *instance;
    SubsystemFilesystem() : Subsystem(DEVICE::HARD_DRIVE) {}

private:
    string readDeviceInfo();

public:
    // get once instance.
    inline static SubsystemFilesystem *inst() { return (instance == nullptr ? (instance = new SubsystemFilesystem) : instance) ; }

    // get ball for device
    virtual TestResult test() override;

    // get formatting info for `cout'
    virtual string getPrintableInfo() const;

public:
    double testWrite();
    double testRead();
};

#endif // SUBSYSTEMFILESYSTEM_H
