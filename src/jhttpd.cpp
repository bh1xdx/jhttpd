

/***************************************************************************************************************
 *          Copyright(C) 2019 by J.L
 ***************************************************************************************************************/
#include "jhttpd.h"

#include <iostream>
#include <unordered_map>

#include "nocopyable.h"
#include "jservice.h"

#include "OsSocketSupport.h"

class Env : public NoCopyable
{
private:
    Env() {
        OsSocketSupport::initSocketLibrary(m_msg);
    };

    ~Env() {};
public:
    static bool good() {
        static Env e;
        return e.m_msg.empty();
    }
private:
    std::string m_msg;
};




///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// JHttpService
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

JHttpService * JHttpCreateService(const char * root /*= ""*/, uint32_t port /*= 80*/)
{
    JHttpService *s = nullptr;
    std::cout << __FUNCTION__ << ": start ..." << std::endl;
    do
    {
        if (!Env::good())
            break;

       s = new JService();
    } while (false);
    std::cout << __FUNCTION__ << ": done" << std::endl;
    return s;
}

void JHttpdestroyService(JHttpService * s)
{
    std::cout << __FUNCTION__ << ": start ..." << std::endl;
    if (s)
        delete s;
    std::cout << __FUNCTION__ << ": done" << std::endl;
}
