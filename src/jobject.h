


#ifndef __JHTTPD_JOBJECT_H__
#define __JHTTPD_JOBJECT_H__


#include "nocopyable.h"

#include "jhttpd.h"
#include <mutex>
#include <string>

class JServer;

class JObject 
{
public:
    JObject(const std::string & name, JObject *parent = nullptr)
        : m_name(name)
        , m_parent(parent)
    {
    };
    virtual ~JObject() {}

    JObject * parent() const { return m_parent; };
    JServer * root();
 
protected:
    std::string name() const { return m_name; };
private:
    std::string m_name;
    JObject * m_parent;
};

#endif // __JHTTPD_JOBJECT_H__