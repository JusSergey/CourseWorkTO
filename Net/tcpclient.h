#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <string>
#include "socketfd.h"
#include "tcpsocket.h"
#include "typedefs.h"

class ConnectionRefusedException : public std::exception {
public:
    virtual const char* what() const throw()
    {
        return "Connections refused. (connect to server)";
    }
};

class TCPClient : public TCPSocket
{
public:
    TCPClient(const std::string &ip, u_short port);
    virtual ~TCPClient();
    CallbackLoop getCallbackLoopClient();
    void disconnectFromHost();

protected:
    virtual void specifiedDisconnect(Buffer &buff, SocketFD &socket) override;
    virtual void specifiedTectConnection(Buffer &buff, SocketFD &socket) override;
    virtual void specifiedConfirmConnection(Buffer &buff, SocketFD &socket) override;
};

#endif // TCPCLIENT_H
