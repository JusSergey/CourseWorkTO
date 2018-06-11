#ifndef SUBSYSTEMDMI_H
#define SUBSYSTEMDMI_H

//#include <string>
#include "Subsystem.h"

/* /proc/22108/net
 * /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies - доступні частоти
 * /sys/devices/system/cpu/cpu0/cpufreq/stats/time_in_state - скільки часу на якій частоті працювало
 * /sys/devices/system/cpu/cpu0/cpufreq/stats/total_trans - скільки було переходів з одної частоти на іншу
 * /sys/devices/system/cpu/cpu0/cpufreq/stats/trans_table - табличка, на перетині стовбця і рядка показано к-сть переходів
 * /proc/cpuinfo - заг. інф. про процесор
*/

//constexpr auto PATH_AVAILABLE_FREQ = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies";
//constexpr auto PATH_GENERAL_INFO = "/proc/cpuinfo";

class SubsystemDMI : public Subsystem/*, public ActiveDev*/
{
    static SubsystemDMI *instance;
    SubsystemDMI() : Subsystem(DEVICE::DMI) {}

public:
    // get once instance.
    inline static SubsystemDMI *inst() { return (instance == nullptr ? (instance = new SubsystemDMI) : instance) ; }

    // get structuring info for device
    virtual ListStrVects getDeviceInfo() const override;

    string getPrintableInfo() const;

public:
    inline string getVendorBIOS() const;
    inline string getVersionBIOS() const;
    inline string getBoardVersion() const;
    inline string getProductName() const;
    inline string getDateBios() const;
};

#endif // SUBSYSTEMDMI_H

