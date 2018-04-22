#ifndef SUBSYSTEMOS_H
#define SUBSYSTEMOS_H

#include "Subsystem.h"

class OS
{
public:
    class Processes : public Subsystem {
        static Processes *instance;

    public:
        static Processes *inst() { return (instance ? instance : (instance = new Processes)); }
        virtual string getPrintableInfo() const;
    };

    class Modules : public Subsystem {
        static Modules *instance;

    public:
        static Modules *inst() { return (instance ? instance : (instance = new Modules)); }
        virtual string getPrintableInfo() const;
    };

    class RAMMemory : public Subsystem {
        static constexpr auto PATH_MEMORY_INFO = "/proc/meminfo";
        static RAMMemory *instance;
        RAMMemory() = default;
    public:
        inline static RAMMemory *inst() {
            return (instance ? instance : (instance = new RAMMemory));
        }

        // get formatting info for `cout'
        virtual string getPrintableInfo() const;
        ListInfoT getDeviceInfo() const;
    };
};

#endif // SUBSYSTEMOS_H
