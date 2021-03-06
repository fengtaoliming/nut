﻿
#include <nut/unittest/unit_test.h>

#include <stdio.h>
#include <nut/threading/lockfree/atomic.h>

using namespace nut;

NUT_FIXTURE(TestAtomic)
{
    NUT_CASES_BEGIN()
    NUT_CASE(test_bug1)
    NUT_CASES_END()

    /**
     * bug:
     * 处理
     *  __sync_val_compare_and_swap()
     *  InterlockedCompareExchange128()
     * 等函数的返回值出错
     */
    void test_bug1()
    {
        {
            void* volatile p = nullptr;
            void *p1 = nullptr, *p2 = reinterpret_cast<void*>(65);
            NUT_TA(atomic_cas(&p, p1, p2));
            NUT_TA(p == p2);
            NUT_TA(atomic_cas(&p, p2, p1));
            NUT_TA(p == p1);
        }

#if NUT_HAS_INT128
        {
            volatile int128_t d = 0;
            int128_t d1 = 0, d2 = 45;
            NUT_TA(atomic_cas(&d, d1, d2));
            NUT_TA(d == d2);
            NUT_TA(atomic_cas(&d, d2, d1));
            NUT_TA(d == d1);
        }

        {
            volatile uint128_t d = 0;
            uint128_t d1 = 0, d2 = 45;
            NUT_TA(atomic_cas(&d, d1, d2));
            NUT_TA(d == d2);
            NUT_TA(atomic_cas(&d, d2, d1));
            NUT_TA(d == d1);
        }

        {
            volatile int128_t d = 0;
            NUT_TA(atomic_add(&d, -1) == 0);
            NUT_TA(d == -1);
            NUT_TA(atomic_add(&d, -1) == -1);
            NUT_TA(d == -2);
        }

        {
            volatile uint128_t d = 0;
            NUT_TA(atomic_add(&d, 1) == 0);
            NUT_TA(d == 1);
            NUT_TA(atomic_add(&d, 1) == 1);
            NUT_TA(d == 2);
        }
#endif

        {
            volatile int64_t d = 0;
            int64_t d1 = 0, d2 = 45;
            NUT_TA(atomic_cas(&d, d1, d2));
            NUT_TA(d == d2);
            NUT_TA(atomic_cas(&d, d2, d1));
            NUT_TA(d == d1);
        }

        {
            volatile uint64_t d = 0;
            uint64_t d1 = 0, d2 = 45;
            NUT_TA(atomic_cas(&d, d1, d2));
            NUT_TA(d == d2);
            NUT_TA(atomic_cas(&d, d2, d1));
            NUT_TA(d == d1);
        }

        {
            volatile int64_t d = 0;
            NUT_TA(atomic_add(&d, -1) == 0);
            NUT_TA(d == -1);
            NUT_TA(atomic_add(&d, -1) == -1);
            NUT_TA(d == -2);
        }

        {
            volatile uint64_t d = 0;
            NUT_TA(atomic_add(&d, 1) == 0);
            NUT_TA(d == 1);
            NUT_TA(atomic_add(&d, 1) == 1);
            NUT_TA(d == 2);
        }

        {
            volatile int32_t d = 0;
            int32_t d1 = 0, d2 = 45;
            NUT_TA(atomic_cas(&d, d1, d2));
            NUT_TA(d == d2);
            NUT_TA(atomic_cas(&d, d2, d1));
            NUT_TA(d == d1);
        }

        {
            volatile uint32_t d = 0;
            uint32_t d1 = 0, d2 = 45;
            NUT_TA(atomic_cas(&d, d1, d2));
            NUT_TA(d == d2);
            NUT_TA(atomic_cas(&d, d2, d1));
            NUT_TA(d == d1);
        }

        {
            volatile int32_t d = 0;
            NUT_TA(atomic_add(&d, -1) == 0);
            NUT_TA(d == -1);
            NUT_TA(atomic_add(&d, -1) == -1);
            NUT_TA(d == -2);
        }

        {
            volatile uint32_t d = 0;
            NUT_TA(atomic_add(&d, 1) == 0);
            NUT_TA(d == 1);
            NUT_TA(atomic_add(&d, 1) == 1);
            NUT_TA(d == 2);
        }

        {
            volatile int16_t d = 0;
            int16_t d1 = 0, d2 = 45;
            NUT_TA(atomic_cas(&d, d1, d2));
            NUT_TA(d == d2);
            NUT_TA(atomic_cas(&d, d2, d1));
            NUT_TA(d == d1);
        }

        {
            volatile uint16_t d = 0;
            uint16_t d1 = 0, d2 = 45;
            NUT_TA(atomic_cas(&d, d1, d2));
            NUT_TA(d == d2);
            NUT_TA(atomic_cas(&d, d2, d1));
            NUT_TA(d == d1);
        }

        {
            volatile int16_t d = 0;
            NUT_TA(atomic_add(&d, -1) == 0);
            NUT_TA(d == -1);
            NUT_TA(atomic_add(&d, -1) == -1);
            NUT_TA(d == -2);
        }

        {
            volatile uint16_t d = 0;
            NUT_TA(atomic_add(&d, 1) == 0);
            NUT_TA(d == 1);
            NUT_TA(atomic_add(&d, 1) == 1);
            NUT_TA(d == 2);
        }
    }
};

NUT_REGISTER_FIXTURE(TestAtomic, "threading, lockfree, quiet")
