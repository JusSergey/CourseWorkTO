#ifndef SUBSYSTEMCPU_H
#define SUBSYSTEMCPU_H

#include <string>
#include "Subsystem.h"

/* /proc/22108/net
 * /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies - доступні частоти
 * /sys/devices/system/cpu/cpu0/cpufreq/stats/time_in_state - скільки часу на якій частоті працювало
 * /sys/devices/system/cpu/cpu0/cpufreq/stats/total_trans - скільки було переходів з одної частоти на іншу
 * /sys/devices/system/cpu/cpu0/cpufreq/stats/trans_table - табличка, на перетині стовбця і рядка показано к-сть переходів
 * /proc/cpuinfo - заг. інф. про процесор
*/

constexpr auto PATH_AVAILABLE_FREQ = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies";
constexpr auto PATH_GENERAL_INFO = "/proc/cpuinfo";

class SubsystemCPU : public Subsystem, public ActiveDev
{
    static SubsystemCPU *instance;
    SubsystemCPU() : Subsystem(DEVICE::CPU) {}
    static string cache_info;

public:
    // get once instance.
    inline static SubsystemCPU *inst() {
        if (instance == nullptr) {
            instance = new SubsystemCPU;
            cache_info = _getDataFromCommand("./lshw -c memory");

            auto in = cache_info.find("*-cache"),
                 to = cache_info.find("*-memory");

            if (in != string::npos && to != string::npos)
                cache_info = _alignText(string("   ") + cache_info.substr(in, to-in));

            else cache_info.clear();
        }
        return instance;
    }

    // get structuring info for device
    virtual ListStrVects getDeviceInfo() const override;

    // get ball for device
    virtual TestResult test() override { return TestResult(); }

    std::string testIteration() const;

    template <typename Value_T>
    TestResult testTemplateValue();

    std::string testCache() const;


    // get formatting info for `cout'
    virtual string getPrintableInfo() const;
};

template<typename Value_T>
ActiveDev::TestResult SubsystemCPU::testTemplateValue()
{
    static constexpr auto limit = 1024*1024*32;
    auto delay_time_calc = _getItervalBetweenCalculate([&]{

#define ValName value
#define def ValName += ValName; ValName -= ValName; \
            ValName *= ValName; ValName /= 3; \
          ++ValName; --ValName;

        Value_T ValName = static_cast<Value_T>(123);

        for (register int i = 0; i < limit; ++i)
        {
                def       def def def
            def def       def     def
                def       def     def
            def def def   def def def
        }
    });
    (std::cout << "time: " << delay_time_calc << '\n').flush();
    int64_t n = (int64_t(6) * 17 * limit);
    return std::make_pair(Number<>::toStr(int64_t(n/delay_time_calc)) + " operation/sec", delay_time_calc);
}

#endif // SUBSYSTEMCPU_H
