//
// This file is part of thttpd
// Copyright (c) Antonino Calderone (antonino.calderone@gmail.com)
// All rights reserved.  
// Licensed under the MIT License. 
// See COPYING file in the project root for full license information.
//

/* -------------------------------------------------------------------------- */

#ifndef __JRESPOSE_H__
#define __JRESPOSE_H__




#include "HttpRequest.h"


#include "jobject.h"

#include <map>
#include <string>
#include <fstream>


class JResponse: public JObject, public NoCopyable
{
public:
     JResponse(JObject * parent, const HttpRequest& request, const std::string& webRootPath);

     const std::string & header() const {
         return _header;
     }

     const std::string & body(uint64_t offset, uint64_t count) const;

     const uint64_t bodysize() const { return _content_length;}

    std::string dump(const std::string& id = "");

private:
    static std::map<std::string, std::string> _s_ext2mime_type;

    std::string _header;
    std::string _content;
    std::string _localUriPath;
    uint64_t _content_length;

    mutable std::string _senddata;
    mutable std::ifstream _ifs;


    static void formatHeader(std::string & header, int code, const std::string & msg, const std::string & content_type, uint64_t content_length, const std::string & last_modified = "");

    // Format an error response
    void formatError(
        std::string& output, 
        int code);

    // Format an positive response
    static void formatPositiveResponse(
        std::string& response, 
        std::string& fileTime,
        std::string& fileExt,
        const uint64_t contentLen);
};

#endif // __JRESPOSE_H__
