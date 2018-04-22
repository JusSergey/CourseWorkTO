#include "SubsystemDMI.h"

using std::make_pair;

SubsystemDMI *SubsystemDMI::instance = nullptr;

static constexpr auto PATH_VENDOR_BIOS   = "/sys/devices/virtual/dmi/id/bios_vendor";
static constexpr auto PATH_VERSION_BIOS  = "/sys/devices/virtual/dmi/id/bios_version";
static constexpr auto PATH_VERSION_BOARD = "/sys/devices/virtual/dmi/id/board_version";
static constexpr auto PATH_NAME_PRODUCT  = "/sys/devices/virtual/dmi/id/product_name";
static constexpr auto PATH_DATE_BIOS     = "/sys/devices/virtual/dmi/id/bios_date";

//SubsystemDMI *SubsystemDMI::inst() { return (instance == nullptr ? (instance = nullptr) : instance); }

std::string SubsystemDMI::getPrintableInfo() const
{
//    new SubsystemDMI();

    return Subsystem::getPrintableInfo(getDeviceInfo(), {/*all lines*/});

//    return (getVendorBIOS() + '\n' +
//            getVersionBIOS() + '\n' +
//            getBoardVersion() + '\n' +
//            getProductName() + '\n' +
//            getDateBios() + '\n');
}

std::string SubsystemDMI::getVendorBIOS() const
{
    return _getDataFromFile(PATH_VENDOR_BIOS);
}

std::string SubsystemDMI::getVersionBIOS() const
{
    return _getDataFromFile(PATH_VERSION_BIOS);
}

std::string SubsystemDMI::getBoardVersion() const
{
    return _getDataFromFile(PATH_VERSION_BOARD);
}

std::string SubsystemDMI::getProductName() const
{
    return _getDataFromFile(PATH_NAME_PRODUCT);
}

std::string SubsystemDMI::getDateBios() const
{
    return _getDataFromFile(PATH_DATE_BIOS);
}


ListInfoT SubsystemDMI::getDeviceInfo() const {
    return {
        std::make_pair(string("bios_vendor"),   string(getVendorBIOS())),
        std::make_pair(string("bios_version"),  string(getVersionBIOS())),
        std::make_pair(string("board_version"), string(getBoardVersion())),
        std::make_pair(string("product_name"),  string(getProductName())),
        std::make_pair(string("bios_date"),     string(getDateBios()))
    };
}
