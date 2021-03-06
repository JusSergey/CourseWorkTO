#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <err.h>
#include <cstring>

#include <functional>
#include <future>
#include <iostream>
#include <vector>
#include <type_traits>
#include <exception>
#include "typedefs.h"

//namespace Net {

using namespace std::chrono;

#include "buffer.h"
#include "device.h"
#include "socketfd.h"
#include "callbacktypes.h"

constexpr auto MAX_SIZE_PACKET = UINT16_MAX - sizeof(uint16) - sizeof(TypeMsg) - 512 /*for safe*/;
constexpr auto DELAY_TIME = 1; //ms

class CreateSocketException : public std::exception {
public:
    virtual const char* what() const throw()
    {
        return "failed while creating new socket";
    }
};

class NoValidIpPortException : public std::exception {
public:
    virtual const char* what() const throw()
    {
        return "No valid Ip or Port";
    }
};

class UndefinedSpecifiedMessage : public std::exception {
public:
    virtual const char* what() const throw()
    {
        return "received undefined specified msg";
    }
};

class TCPSocket : public SocketFD
{
public:
    std::string _name;
    std::future<void> _fut;
    TCPSocket(const std::string &ip, uint16 port);
    virtual ~TCPSocket();

protected:
    void initLoop(CallbackLoop callbackLoop);
    bool readMessage(SocketFD &fd);
    bool readMessage();

public:
    bool isRunning() const { return _running.load(); }
    void stop();
    CallbackRead getCallbackRead() const;
    void setCallbackRead(const CallbackRead &value);

    inline int64 getTimeConfirmConnectionMSEC() const
    { return (duration_cast<milliseconds>(steady_clock::now() - _lastConfirmConnection)).count(); }

    template <typename Enum, typename EnumType = typename std::underlying_type<Enum>::type>
    constexpr inline EnumType toUnderlying(Enum value) const
    { return static_cast<typename std::underlying_type<Enum>::type>(value); }

    void setName(const std::string &name);
    std::string getName() const;

protected: /* data info to init */
    std::string _ip;
    u_short _port;

protected: /* data read/write buffers */
    CallbackRead _callbackRead = [] (Buffer, SocketFD) {};
    Buffer _buffer;
    TypeMsg _typeMsg;
    time_point<steady_clock> _lastConfirmConnection;

protected: /* data to init socket */
    std::atomic_bool _running;
    sockaddr_in _sock;

protected:
    void unlockfd();

protected:
    virtual void specifiedDisconnect(Buffer &buff, SocketFD &socket) = 0;
    virtual void specifiedTectConnection(Buffer &buff, SocketFD &socket) = 0;
    virtual void specifiedConfirmConnection(Buffer &buff, SocketFD &socket) = 0;

private:
    void procSpecifiedMsg(Buffer &buff, SocketFD &socket);
};

//}

#endif // TCPSOCKET_H
