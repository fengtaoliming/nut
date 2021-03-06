﻿
#ifndef ___HEADFILE___31A42BCF_A2BF_4E7C_8387_751B0648CFBA_
#define ___HEADFILE___31A42BCF_A2BF_4E7C_8387_751B0648CFBA_

#include <assert.h>
#include <stdint.h>

namespace nut
{

class DestroyChecker
{
    enum
    {
        CONSTRUCTED = 0x12344321,
        DESTRUCTED = 0xDEADBEEF /* magic dead-beaf */
    };

    int32_t _tag = CONSTRUCTED;

public:
    DestroyChecker() = default;

    ~DestroyChecker()
    {
        assert(CONSTRUCTED == _tag);
        _tag = DESTRUCTED;
    }

    void assert_alive() const
    {
        assert(CONSTRUCTED == _tag);
    }
};

}

#ifndef NDEBUG
/** 析构检查器 */
#   define NUT_DEBUGGING_DESTROY_CHECKER    nut::DestroyChecker __destroy_checker_;
/** 检查析构 */
#   define NUT_DEBUGGING_ASSERT_ALIVE       __destroy_checker_.assert_alive()
#else
#   define NUT_DEBUGGING_DESTROY_CHECKER
#   define NUT_DEBUGGING_ASSERT_ALIVE       ((void)0)
#endif

#endif /* head file guarder */
