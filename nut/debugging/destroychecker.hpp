/**
 * @file -
 * @author jingqi
 * @date 2011-11-11 18:49
 */

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

    int32_t m_tag;

public:
    DestroyChecker() : m_tag(CONSTRUCTED) {}

    ~DestroyChecker()
    {
        assert(CONSTRUCTED == m_tag);
        m_tag = DESTRUCTED;
    }

    inline void assertAlive() const { assert(CONSTRUCTED == m_tag); }
};

}

#ifndef NDEBUG
/** ��������� */
#   define NUT_DEBUGGING_DESTROY_CHECKER nut::DestroyChecker __destroy_checker_;
/** ������� */
#   define NUT_DEBUGGING_ASSERT_ALIVE __destroy_checker_.assertAlive()
#else
#   define NUT_DEBUGGING_DESTROY_CHECKER ((void)0);
#   define NUT_DEBUGGING_ASSERT_ALIVE ((void)0)
#endif

#endif /* head file guarder */
