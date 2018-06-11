#include "SubsystemGPU.h"
#include "../Utils/StringUtilities.h"
#include <iostream>

SubsystemGPU* SubsystemGPU::instance = nullptr;
std::string SubsystemGPU::info;

ListStrVects SubsystemGPU::getDeviceInfo() const
{
    return getStructureInfo(info);
}

std::string SubsystemGPU::getPrintableInfo() const
{
    return info;
}
