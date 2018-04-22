#include "tcpclient.h"
#include "tcpsocket.h"
#include "socketfd.h"
#include <stdlib.h>
//using namespace Net;

TCPClient::TCPClient(const std::string &ip, u_short port) :
    TCPSocket(ip, port)
{
    int isConnected = connect(_fd, (sockaddr*)&_sock, sizeof(_sock));
    if (isConnected < 0) {
        err(isConnected, "connect(): failed.");
        exit(-1);
    }

    unlockfd();
    initLoop(getCallbackLoopClient());
}

TCPClient::~TCPClient()
{
    if (_running.load())
        disconnectFromHost();
}

CallbackLoop TCPClient::getCallbackLoopClient()
{
    return [this] {
        if (readMessage()) {
            _callbackRead(_buffer, _fd);
        }
    };
}

void TCPClient::disconnectFromHost()
{
    sendMessage("good bye...", TypeMsg::Disconnect);
    specifiedDisconnect(_buffer, *this);
    setValidFalse();
}

void TCPClient::specifiedConfirmConnection(Buffer &, SocketFD &)
{
}

void TCPClient::specifiedDisconnect(Buffer &, SocketFD &)
{
    stop();
}

void TCPClient::specifiedTectConnection(Buffer &, SocketFD &socket)
{
    socket.sendMessage("", TypeMsg::ConfirmConnection);
}
