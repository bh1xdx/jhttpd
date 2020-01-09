/***************************************************************************************************************
 *          Copyright(C) 2019 by J.L
 ***************************************************************************************************************/


#ifndef __JHTTPD_NO_COPYABLE_H__
#define __JHTTPD_NO_COPYABLE_H__

class NoCopyable
{
    NoCopyable(const NoCopyable &) = delete;
    NoCopyable(NoCopyable &&) = delete;
    NoCopyable & operator = (const NoCopyable &) = delete;
    NoCopyable & operator = (NoCopyable &&) = delete;
public:
    NoCopyable() = default;
    ~NoCopyable() = default;
};

#endif // __JHTTPD_NO_COPYABLE_H__
