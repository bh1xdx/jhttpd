//
// This file is part of thttpd
// Copyright (c) Antonino Calderone (antonino.calderone@gmail.com)
// All rights reserved.  
// Licensed under the MIT License. 
// See COPYING file in the project root for full license information.
//


/* -------------------------------------------------------------------------- */

#include "jsocket.h"
#include "jtools.h"

#include <vector>


/* -------------------------------------------------------------------------- */

HttpSocket& HttpSocket::operator=(TcpSocket::Handle handle)
{
    _socketHandle = handle;
    return *this;
}


/* -------------------------------------------------------------------------- */

HttpRequest::Handle HttpSocket::recv()
{
    HttpRequest::Handle handle(new HttpRequest);

    char c = 0;
    int ret = 1;

    enum class CrLfSeq { CR1, LF1, CR2, LF2, IDLE } s = CrLfSeq::IDLE;

    auto crlf = [&s](char c) -> bool {
        switch (s) {
        case CrLfSeq::IDLE:
            s = (c == '\r') ? CrLfSeq::CR1 : CrLfSeq::IDLE;
            break;
        case CrLfSeq::CR1:
            s = (c == '\n') ? CrLfSeq::LF1 : CrLfSeq::IDLE;
            break;
        case CrLfSeq::LF1:
            s = (c == '\r') ? CrLfSeq::CR2 : CrLfSeq::IDLE;
            break;
        case CrLfSeq::CR2:
            s = (c == '\n') ? CrLfSeq::LF2 : CrLfSeq::IDLE;
            break;
        default:
            break;
        }

        return s == CrLfSeq::LF2;
    };

    std::string line;

    while (ret > 0 && _connUp && _socketHandle) {
        std::chrono::seconds sec(getConnectionTimeout());

        auto recvEv = _socketHandle->waitForRecvEvent(sec);

        switch (recvEv) {
        case TransportSocket::RecvEvent::RECV_ERROR:
        case TransportSocket::RecvEvent::TIMEOUT:
            _connUp = false;
            break;
        default:
            break;
        }

        if (!_connUp)
            break;

        ret = _socketHandle->recv(&c, 1);

        if (ret > 0) {
            line += c;
        } else if (ret <= 0) {
            _connUp = false;
            break;
        }

        if (crlf(c)) {
            break;
        }

        if (s == CrLfSeq::LF1) {
            if (!line.empty()) {
                handle->addHeader(line);
                line.clear();
            }
        }
    }

    if (ret < 0 || !_socketHandle || handle->get_header().empty()) {
        return handle;
    }

    std::string request = *handle->get_header().cbegin();
    std::vector<std::string> tokens;

    if (!Tools::splitLineInTokens(request, tokens, " ")) {
        return handle;
    }

    if (tokens.size() != 3) {
        return handle;
    }

    handle->parseMethod(tokens[0]);
    handle->parseUri(tokens[1]);
    handle->parseVersion(tokens[2]);

    return handle;
}

int HttpSocket::sendHeader(const JResponse& response)
{
    const std::string resp_head = response.header();
    int sent = _socketHandle->send(resp_head);
    if (send < 0 || (uint32_t)send < resp_head.size())
        _connUp = false;
    return 0;
}

int HttpSocket::sendBody(const JResponse& response)
{
    uint64_t readed = 0;
    const uint64_t count = 1 << 20;
    do
    {
        const std::string content = response.body(readed, count);
        if (content.empty())
            break;
        readed += content.size();

        int sent = _socketHandle->send(content);
        if (send < 0 || (uint32_t)send < content.size())
        {
            _connUp = false;
            break;
        }
        if (readed >= response.bodysize())
            break;
    } while (true);

    if (readed < response.bodysize())
        _connUp = false;
    return _connUp ? 0: -1;
}


