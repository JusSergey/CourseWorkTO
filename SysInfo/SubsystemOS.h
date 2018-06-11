#ifndef SUBSYSTEMOS_H
#define SUBSYSTEMOS_H

#include "Subsystem.h"

class OS
{
    class SubOS : public Subsystem {
    public:
        SubOS(DEVICE dev) : Subsystem(dev) {}
        virtual string getHTMLCode() const override;
    };
public:
    class Processes : public SubOS {
        static Processes *instance;
        Processes() : SubOS(DEVICE::PROCESSES) {}

    public:
        static Processes *inst() { return (instance ? instance : (instance = new Processes)); }
        virtual string getPrintableInfo() const override;
        virtual ListStrVects getDeviceInfo() const override;

    };

    class Modules : public SubOS {
        static Modules *instance;
        Modules() : SubOS(DEVICE::MODULES) {}

    public:
        static Modules *inst() { return (instance ? instance : (instance = new Modules)); }
        virtual string getPrintableInfo() const override;
        virtual ListStrVects getDeviceInfo() const override;
    };

    class RAMMemory : public SubOS {
        static constexpr auto PATH_MEMORY_INFO = "/proc/meminfo";
        static RAMMemory *instance;
        RAMMemory() : SubOS(DEVICE::RAM_MEMORY) {}
    public:
        inline static RAMMemory *inst() {
            return (instance ? instance : (instance = new RAMMemory));
        }

        // get formatting info for `cout'
        virtual string getPrintableInfo() const;
        ListStrVects getDeviceInfo() const;
    };
};

#endif // SUBSYSTEMOS_H
