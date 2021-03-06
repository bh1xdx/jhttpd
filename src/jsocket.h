//
// This file is part of thttpd
// Copyright (c) Antonino Calderone (antonino.calderone@gmail.com)
// All rights reserved.  
// Licensed under the MIT License. 
// See COPYING file in the project root for full license information.
//

/* -------------------------------------------------------------------------- */

/// \file HttpSocket.h
/// \brief Declaration of HTTP classes


/* -------------------------------------------------------------------------- */

#ifndef __HTTP_SOCKET_H__
#define __HTTP_SOCKET_H__


/* -------------------------------------------------------------------------- */
#include "jconfig.h"
#include "jtcpsocket.h"
#include "jrequest.h"
#include "jresponse.h"

#include <string>


/**
 * This class represents an HTTP connection between a client and a server.
 */
class HttpSocket {
private:
    TcpSocket::Handle _socketHandle;
    bool _connUp = true;
    HttpRequest::Handle recv();
    int _connectionTimeOut = JHTTPD_CONNECTION_TIMEOUT; // secs

public:
    HttpSocket(int connectionTimeout = JHTTPD_CONNECTION_TIMEOUT) noexcept :
        _connectionTimeOut(connectionTimeout) 
    {}

    HttpSocket(const HttpSocket&) = default;

    /**
     * Construct the HTTP connection starting from TCP connected-socket handle.
     */
    HttpSocket(TcpSocket::Handle handle)
        : _socketHandle(handle)
    {
    }

    /**
     * Assigns a new TCP connected socket handle to this HTTP socket.
     */
    HttpSocket& operator=(TcpSocket::Handle handle);

    /**
     * Returns TCP socket handle
     */
    operator TcpSocket::Handle() const { 
       return _socketHandle; 
    }

    /**
     * Receives an HTTP request from remote peer.
     * @param the handle of http request object
     */
    HttpSocket& operator>>(HttpRequest::Handle& handle) {
        handle = recv();
        return *this;
    }

    /**
     * Returns false if last recv/send operation detected
     * that connection was down; true otherwise.
     */
    explicit operator bool() const { 
       return _connUp; 
    }

    int sendHeader(const JResponse& response);
    int sendBody(const JResponse& response);

    int sendFile(const std::string& fileName) {
        return _socketHandle->sendFile(fileName);
    }

    /*
     * Return connection timeout interval in seconds
     */
    const int& getConnectionTimeout() const noexcept {
        return _connectionTimeOut;
    }
};


/* -------------------------------------------------------------------------- */

#endif // __HTTP_SOCKET_H__
