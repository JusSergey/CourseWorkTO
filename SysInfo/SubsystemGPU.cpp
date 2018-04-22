#include "SubsystemGPU.h"
#include "StringUtilities.h"
#include <iostream>

SubsystemGPU* SubsystemGPU::instance = nullptr;
std::string SubsystemGPU::info;

ListInfoT SubsystemGPU::getDeviceInfo() const
{
    return getStructureInfo(info);
}

//double SubsystemGPU::test()
//{
//    double ball = 0;



//    return ball;
//}

std::string SubsystemGPU::getPrintableInfo() const
{
    return info;
}
