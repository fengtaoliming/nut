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
#include <nut/platform/platform.hpp>

#if defined(NUT_PLATFORM_OS_WINDOWS)
#   include <windows.h>
#endif

namespace nut
{

inline bool atomic_cas(void * volatile *dest, void *oldval, void *newval)
{
#if defined(NUT_PLATFORM_OS_LINUX)
    return __sync_val_compare_and_swap(dest, oldval, newval);
#elif defined(NUT_PLATFORM_OS_WINDOWS)
    static_assert(sizeof(long) == sizeof(uint32_t));
    return InterlockedCompareExchangePointer(dest, newval, oldval) == oldval;
#else
#   error platform not supported!
#endif
}

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
#else
    uint16_t old;
    do {
        old = *addend;
    } while (!atomic_cas(addend, old, old + value));
    return old;
#endif
}

/**
 * Ϊ�˱���ABA���������Ĵ���ǩ��ָ��
 */
template <typename T>
union TagedPtr
{
    /** �����Ӧ��CAS���������� */
#if defined(NUT_PLATFORM_BITS_64)
    typedef uint128_t cas_type;
#elif defined(NUT_PLATFORM_BITS_32)
    typedef uint64_t cas_type;
#elif defined(NUT_PLATFORM_BITS_16)
    typedef uint32_t cas_type;
#else
#   error platform not supported!
#endif

    typedef struct
    {
        T *ptr;
        unsigned int tag;
    } tagedptr;

    cas_type cas;

    TagedPtr() : cas(0) {}

    TagedPtr(T *p, unsigned int t) : ptr(p), tag(t) {}

    inline bool operator==(const TagedPtr<T>& x) const { return cas == x.cas; }

    inline bool operator!=(const TagedPtr<T>& x) const { return cas != x.cas; }
};
static_assert(sizeof(TagedPtr<void>::tagedptr) == sizeof(TagedPtr<void>::cas_type), "size not match");
static_assert(sizeof(TagedPtr<void>) == sizeof(TagedPtr<void>::cas_type), "size not match");

}

#endif /* head file guarder */

