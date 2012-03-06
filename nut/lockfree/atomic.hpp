/**
 * @file -
 * @author jingqi
 * @date 2012-03-05
 * @last-edit 2012-03-05 20:19:43 jingqi
 * @brief
 *      �����ϴ󲿷ֵ������������ݽṹ���������������ṩ��CAS(compare and swap)������
 * ʵ�ֵ�; ����ԭ�Ӽӡ�ԭ�Ӽ�����������Ҳ�����������ʵ�ֵġ�
 */

#ifndef ___HEADFILE_CBAE01C9_CF0C_4836_A4DC_E7B0934DEA6E_
#define ___HEADFILE_CBAE01C9_CF0C_4836_A4DC_E7B0934DEA6E_

#include <stdint.h>
#include <nut/platform/platforms.hpp>

#if defined(NUT_PLATFORM_OS_WINDOWS)
#   include <windows.h>
#endif

namespace nut
{

#if defined(NUT_PLATFORM_BITS_64)

inline bool atomic_cas(uint128_t volatile *dest, uint64_t oldval, uint64_t newval)
{
#if defined(NUT_PLATFORM_OS_LINUX)
    return __sync_val_compare_and_swap(dest, oldval, newval);
#elif defined(NUT_PLATFORM_OS_WINDOWS)
    return InterlockedCompareExchange128(dest, newval, oldval) == oldval;
#else
#   error platform not supported!
#endif
}

#endif

/**
 * 64λCAS����
 *
 * @return �����ɹ��򷵻�true
 */
inline bool atomic_cas(uint64_t volatile *dest, uint64_t oldval, uint64_t newval)
{
#if defined(NUT_PLATFORM_OS_LINUX)
    return __sync_val_compare_and_swap(dest, oldval, newval);
#elif defined(NUT_PLATFORM_OS_WINDOWS)
    return InterlockedCompareExchange64(dest, newval, oldval) == oldval;
#else
#   error platform not supported!
#endif
}

/**
 * 32λCAS����
 *
 * @return �����ɹ��򷵻�true
 */
inline bool atomic_cas(uint32_t volatile *dest, uint32_t oldval, uint32_t newval)
{
#if defined(NUT_PLATFORM_OS_LINUX)
    return __sync_val_compare_and_swap(dest, oldval, newval);
#elif defined(NUT_PLATFORM_OS_WINDOWS)
    static_assert(sizeof(long) == sizeof(uint32_t));
    return InterlockedCompareExchange(dest, newval, oldval) == oldval;
#else
#   error platform not supported!
#endif
}

/**
 * 16λCAS����
 *
 * @return �����ɹ��򷵻�true
 */
inline bool atomic_cas(uint16_t volatile *dest, uint16_t oldval, uint16_t newval)
{
#if defined(NUT_PLATFORM_OS_LINUX)
    return __sync_val_compare_and_swap(dest, oldval, newval);
#elif defined(NUT_PLATFORM_OS_WINDOWS)
    return InterlockedCompareExchange16(dest, newval, oldval) == oldval;
#else
#   error platform not supported!
#endif
}

/**
 * 64λԭ�Ӽ�
 *
 * @return ���ؾ�ֵ
 */
inline uint64_t atomic_add(uint64_t volatile *addend, uint64_t value)
{
#if defined(NUT_PLATFORM_OS_LINUX)
    return __sync_fetch_and_add(addend, value);
#elif defined(NUT_PLATFORM_OS_WINDOWS)
    return InterlockedExchangeAdd64(addend, value);
#else
    uint64_t old;
    do {
        old = *addend;
    } while (!atomic_cas(addend, old, old + value));
    return old;
#endif
}

/**
 * 32λԭ�Ӽ�
 *
 * @return ���ؾ�ֵ
 */
inline uint32_t atomic_add(uint32_t volatile *addend, uint32_t value)
{
#if defined(NUT_PLATFORM_OS_LINUX)
    return __sync_fetch_and_add(addend, value);
#elif defined(NUT_PLATFORM_OS_WINDOWS)
    static_assert(sizeof(long) == sizeof(uint32_t));
    return InterlockedExchangeAdd(addend, value);
#else
    uint32_t old;
    do {
        old = *addend;
    } while (!atomic_cas(addend, old, old + value));
    return old;
#endif
}

/**
 * 16λԭ�Ӽ�
 *
 * @return ���ؾ�ֵ
 */
inline uint16_t atomic_add(uint16_t volatile *addend, uint16_t value)
{
#if defined(NUT_PLATFORM_OS_LINUX)
    return __sync_fetch_and_add(addend, value);
#elif defined(NUT_PLATFORM_OS_WINDOWS)
    return InterlockedExchangeAdd16(addend, value);
#else
    uint16_t old;
    do {
        old = *addend;
    } while (!atomic_cas(addend, old, old + value));
    return old;
#endif
}

}

#endif /* head file guarder */

