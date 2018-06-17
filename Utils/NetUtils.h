#ifndef NETUTILS_H
#define NETUTILS_H
#include "types.h"

class NetUtils
{
public:
    NetUtils() = delete;

    static string getHostName();
    static string getLocalIP();
    static bool isValidIPv4(const std::string &src);
    static bool isValidPort(const std::string &src);
};

#endif // NETUTILS_H
