

#include "jobject.h"
#include "jserver.h"

JServer * JObject::root()
{
    JObject * r = this;
    while (r->parent())
        r = r->parent();

    return dynamic_cast<JServer*>(r);
}

