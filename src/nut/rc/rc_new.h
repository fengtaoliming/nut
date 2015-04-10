﻿
#ifndef ___HEADFILE___B77B6DC2_6C30_4732_943B_117AF7E35B83_
#define ___HEADFILE___B77B6DC2_6C30_4732_943B_117AF7E35B83_

#include <assert.h>
#include <stdlib.h>
#include <new>

#include "rc_ptr.h"
#include "rc_wrapper.h"
#include "ref_counter.h"

namespace nut
{

#ifndef _LIBCPP_HAS_NO_VARIADICS
template <typename T, typename Counter=int, typename ...Args>
rc_ptr<T> rc_new(Args&& ...args)
{
    RCWrapper<T,Counter> *p = (RCWrapper<T,Counter>*) ::malloc(sizeof(RCWrapper<T,Counter>));
    assert(NULL != p);
    new (p) RCWrapper<T,Counter>(args...);
    return rc_ptr<T>(p);
}

template <typename T, typename Counter=int, typename ...Args>
rc_ptr<T> rca_new(memory_allocator *ma, Args&& ...args)
{
    if (NULL == ma)
        return rc_new<T,Counter>(args...);

    RCAllocWrapper<T,Counter> *p = (RCAllocWrapper<T,Counter>*) ma_alloc(ma, sizeof(RCAllocWrapper<T,Counter>));
    assert(NULL != p);
    new (p) RCAllocWrapper<T,Counter>(ma, args...);
    return rc_ptr<T>(p);
}
#else

template <typename T, typename Counter=int>
rc_ptr<T> rc_new()
{
    RCWrapper<T,Counter> *p = (RCWrapper<T,Counter>*) ::malloc(sizeof(RCWrapper<T,Counter>));
    assert(NULL != p);
    new (p) RCWrapper<T,Counter>();
    return rc_ptr<T>(p);
}

template <typename T, typename Counter=int, typename Arg1>
rc_ptr<T> rc_new(Arg1&& arg1)
{
    RCWrapper<T,Counter> *p = (RCWrapper<T,Counter>*) ::malloc(sizeof(RCWrapper<T,Counter>));
    assert(NULL != p);
    new (p) RCWrapper<T,Counter>(arg1);
    return rc_ptr<T>(p);
}

template <typename T, typename Counter=int, typename Arg1, typename Arg2>
rc_ptr<T> rc_new(Arg1&& arg1, Arg2&& arg2)
{
    RCWrapper<T,Counter> *p = (RCWrapper<T,Counter>*) ::malloc(sizeof(RCWrapper<T,Counter>));
    assert(NULL != p);
    new (p) RCWrapper<T,Counter>(arg1, arg2);
    return rc_ptr<T>(p);
}

template <typename T, typename Counter=int, typename Arg1, typename Arg2, typename Arg3>
rc_ptr<T> rc_new(Arg1&& arg1, Arg2&& arg2, Arg3&& arg3)
{
    RCWrapper<T,Counter> *p = (RCWrapper<T,Counter>*) ::malloc(sizeof(RCWrapper<T,Counter>));
    assert(NULL != p);
    new (p) RCWrapper<T,Counter>(arg1, arg2, arg3);
    return rc_ptr<T>(p);
}

template <typename T, typename Counter=int, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
rc_ptr<T> rc_new(Arg1&& arg1, Arg2&& arg2, Arg3&& arg3, Arg4&& arg4)
{
    RCWrapper<T,Counter> *p = (RCWrapper<T,Counter>*) ::malloc(sizeof(RCWrapper<T,Counter>));
    assert(NULL != p);
    new (p) RCWrapper<T,Counter>(arg1, arg2, arg3, arg4);
    return rc_ptr<T>(p);
}

template <typename T, typename Counter=int, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
rc_ptr<T> rc_new(Arg1&& arg1, Arg2&& arg2, Arg3&& arg3, Arg4&& arg4, Arg5&& arg5)
{
    RCWrapper<T,Counter> *p = (RCWrapper<T,Counter>*) ::malloc(sizeof(RCWrapper<T,Counter>));
    assert(NULL != p);
    new (p) RCWrapper<T,Counter>(arg1, arg2, arg3, arg4, arg5);
    return rc_ptr<T>(p);
}

template <typename T, typename Counter=int, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5,
          typename Arg6>
rc_ptr<T> rc_new(Arg1&& arg1, Arg2&& arg2, Arg3&& arg3, Arg4&& arg4, Arg5&& arg5,
                 Arg6&& arg6)
{
    RCWrapper<T,Counter> *p = (RCWrapper<T,Counter>*) ::malloc(sizeof(RCWrapper<T,Counter>));
    assert(NULL != p);
    new (p) RCWrapper<T,Counter>(arg1, arg2, arg3, arg4, arg5, arg6);
    return rc_ptr<T>(p);
}

template <typename T, typename Counter=int, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5,
          typename Arg6, typename Arg7>
rc_ptr<T> rc_new(Arg1&& arg1, Arg2&& arg2, Arg3&& arg3, Arg4&& arg4, Arg5&& arg5,
                 Arg6&& arg6, Arg7&& arg7)
{
    RCWrapper<T,Counter> *p = (RCWrapper<T,Counter>*) ::malloc(sizeof(RCWrapper<T,Counter>));
    assert(NULL != p);
    new (p) RCWrapper<T,Counter>(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    return rc_ptr<T>(p);
}

template <typename T, typename Counter=int, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5,
          typename Arg6, typename Arg7, typename Arg8>
rc_ptr<T> rc_new(Arg1&& arg1, Arg2&& arg2, Arg3&& arg3, Arg4&& arg4, Arg5&& arg5,
                 Arg6&& arg6, Arg7&& arg7, Arg8&& arg8)
{
    RCWrapper<T,Counter> *p = (RCWrapper<T,Counter>*) ::malloc(sizeof(RCWrapper<T,Counter>));
    assert(NULL != p);
    new (p) RCWrapper<T,Counter>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    return rc_ptr<T>(p);
}

template <typename T, typename Counter=int, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5,
          typename Arg6, typename Arg7, typename Arg8, typename Arg9>
rc_ptr<T> rc_new(Arg1&& arg1, Arg2&& arg2, Arg3&& arg3, Arg4&& arg4, Arg5&& arg5,
                 Arg6&& arg6, Arg7&& arg7, Arg8&& arg8, Arg9&& arg9)
{
    RCWrapper<T,Counter> *p = (RCWrapper<T,Counter>*) ::malloc(sizeof(RCWrapper<T,Counter>));
    assert(NULL != p);
    new (p) RCWrapper<T,Counter>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
    return rc_ptr<T>(p);
}

template <typename T, typename Counter=int, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5,
          typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
rc_ptr<T> rc_new(Arg1&& arg1, Arg2&& arg2, Arg3&& arg3, Arg4&& arg4, Arg5&& arg5,
                 Arg6&& arg6, Arg7&& arg7, Arg8&& arg8, Arg9&& arg9, Arg10&& arg10)
{
    RCWrapper<T,Counter> *p = (RCWrapper<T,Counter>*) ::malloc(sizeof(RCWrapper<T,Counter>));
    assert(NULL != p);
    new (p) RCWrapper<T,Counter>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
    return rc_ptr<T>(p);
}

template <typename T, typename Counter=int>
rc_ptr<T> rca_new(memory_allocator *ma)
{
    if (NULL == ma)
        return rc_new<T,Counter>();

    RCAllocWrapper<T,Counter> *p = (RCAllocWrapper<T,Counter>*) ma_alloc(ma, sizeof(RCAllocWrapper<T,Counter>));
    assert(NULL != p);
    new (p) RCAllocWrapper<T,Counter>(ma);
    return rc_ptr<T>(p);
}

template <typename T, typename Counter=int, typename Arg1>
rc_ptr<T> rca_new(memory_allocator *ma, Arg1&& arg1)
{
    if (NULL == ma)
        return rc_new<T,Counter>(arg1);

    RCAllocWrapper<T,Counter> *p = (RCAllocWrapper<T,Counter>*) ma_alloc(ma, sizeof(RCAllocWrapper<T,Counter>));
    assert(NULL != p);
    new (p) RCAllocWrapper<T,Counter>(ma, arg1);
    return rc_ptr<T>(p);
}

template <typename T, typename Counter=int, typename Arg1, typename Arg2>
rc_ptr<T> rca_new(memory_allocator *ma, Arg1&& arg1, Arg2&& arg2)
{
    if (NULL == ma)
        return rc_new<T,Counter>(arg1, arg2);

    RCAllocWrapper<T,Counter> *p = (RCAllocWrapper<T,Counter>*) ma_alloc(ma, sizeof(RCAllocWrapper<T,Counter>));
    assert(NULL != p);
    new (p) RCAllocWrapper<T,Counter>(ma, arg1, arg2);
    return rc_ptr<T>(p);
}

template <typename T, typename Counter=int, typename Arg1, typename Arg2, typename Arg3>
rc_ptr<T> rca_new(memory_allocator *ma, Arg1&& arg1, Arg2&& arg2, Arg3&& arg3)
{
    if (NULL == ma)
        return rc_new<T,Counter>(arg1, arg2, arg3);

    RCAllocWrapper<T,Counter> *p = (RCAllocWrapper<T,Counter>*) ma_alloc(ma, sizeof(RCAllocWrapper<T,Counter>));
    assert(NULL != p);
    new (p) RCAllocWrapper<T,Counter>(ma, arg1, arg2, arg3);
    return rc_ptr<T>(p);
}

template <typename T, typename Counter=int, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
rc_ptr<T> rca_new(memory_allocator *ma, Arg1&& arg1, Arg2&& arg2, Arg3&& arg3, Arg4&& arg4)
{
    if (NULL == ma)
        return rc_new<T,Counter>(arg1, arg2, arg3, arg4);

    RCAllocWrapper<T,Counter> *p = (RCAllocWrapper<T,Counter>*) ma_alloc(ma, sizeof(RCAllocWrapper<T,Counter>));
    assert(NULL != p);
    new (p) RCAllocWrapper<T,Counter>(ma, arg1, arg2, arg3, arg4);
    return rc_ptr<T>(p);
}

template <typename T, typename Counter=int, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
rc_ptr<T> rca_new(memory_allocator *ma, Arg1&& arg1, Arg2&& arg2, Arg3&& arg3, Arg4&& arg4, Arg5&& arg5)
{
    if (NULL == ma)
        return rc_new<T,Counter>(arg1, arg2, arg3, arg4, arg5);

    RCAllocWrapper<T,Counter> *p = (RCAllocWrapper<T,Counter>*) ma_alloc(ma, sizeof(RCAllocWrapper<T,Counter>));
    assert(NULL != p);
    new (p) RCAllocWrapper<T,Counter>(ma, arg1, arg2, arg3, arg4, arg5);
    return rc_ptr<T>(p);
}

template <typename T, typename Counter=int, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5,
          typename Arg6>
rc_ptr<T> rca_new(memory_allocator *ma, Arg1&& arg1, Arg2&& arg2, Arg3&& arg3, Arg4&& arg4, Arg5&& arg5,
                  Arg6&& arg6)
{
    if (NULL == ma)
        return rc_new<T,Counter>(arg1, arg2, arg3, arg4, arg5, arg6);

    RCAllocWrapper<T,Counter> *p = (RCAllocWrapper<T,Counter>*) ma_alloc(ma, sizeof(RCAllocWrapper<T,Counter>));
    assert(NULL != p);
    new (p) RCAllocWrapper<T,Counter>(ma, arg1, arg2, arg3, arg4, arg5, arg6);
    return rc_ptr<T>(p);
}

template <typename T, typename Counter=int, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5,
          typename Arg6, typename Arg7>
rc_ptr<T> rca_new(memory_allocator *ma, Arg1&& arg1, Arg2&& arg2, Arg3&& arg3, Arg4&& arg4, Arg5&& arg5,
                  Arg6&& arg6, Arg7&& arg7)
{
    if (NULL == ma)
        return rc_new<T,Counter>(arg1, arg2, arg3, arg4, arg5, arg6, arg7);

    RCAllocWrapper<T,Counter> *p = (RCAllocWrapper<T,Counter>*) ma_alloc(ma, sizeof(RCAllocWrapper<T,Counter>));
    assert(NULL != p);
    new (p) RCAllocWrapper<T,Counter>(ma, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    return rc_ptr<T>(p);
}

template <typename T, typename Counter=int, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5,
          typename Arg6, typename Arg7, typename Arg8>
rc_ptr<T> rca_new(memory_allocator *ma, Arg1&& arg1, Arg2&& arg2, Arg3&& arg3, Arg4&& arg4, Arg5&& arg5,
                  Arg6&& arg6, Arg7&& arg7, Arg8&& arg8)
{
    if (NULL == ma)
        return rc_new<T,Counter>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);

    RCAllocWrapper<T,Counter> *p = (RCAllocWrapper<T,Counter>*) ma_alloc(ma, sizeof(RCAllocWrapper<T,Counter>));
    assert(NULL != p);
    new (p) RCAllocWrapper<T,Counter>(ma, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    return rc_ptr<T>(p);
}

template <typename T, typename Counter=int, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5,
          typename Arg6, typename Arg7, typename Arg8, typename Arg9>
rc_ptr<T> rca_new(memory_allocator *ma, Arg1&& arg1, Arg2&& arg2, Arg3&& arg3, Arg4&& arg4, Arg5&& arg5,
                  Arg6&& arg6, Arg7&& arg7, Arg8&& arg8, Arg9&& arg9)
{
    if (NULL == ma)
        return rc_new<T,Counter>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);

    RCAllocWrapper<T,Counter> *p = (RCAllocWrapper<T,Counter>*) ma_alloc(ma, sizeof(RCAllocWrapper<T,Counter>));
    assert(NULL != p);
    new (p) RCAllocWrapper<T,Counter>(ma, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
    return rc_ptr<T>(p);
}

template <typename T, typename Counter=int, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5,
          typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
rc_ptr<T> rca_new(memory_allocator *ma, Arg1&& arg1, Arg2&& arg2, Arg3&& arg3, Arg4&& arg4, Arg5&& arg5,
                  Arg6&& arg6, Arg7&& arg7, Arg8&& arg8, Arg9&& arg9, Arg10&& arg10)
{
    if (NULL == ma)
        return rc_new<T,Counter>(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);

    RCAllocWrapper<T,Counter> *p = (RCAllocWrapper<T,Counter>*) ma_alloc(ma, sizeof(RCAllocWrapper<T,Counter>));
    assert(NULL != p);
    new (p) RCAllocWrapper<T,Counter>(ma, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
    return rc_ptr<T>(p);
}

#endif

}

#endif /* head file guarder */
