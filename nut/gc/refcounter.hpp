/**
 * @file -
 * @author jingqi
 * @date 2011-11-12 11:52
 * @brief
 *  ���ü����ļ�����
 */

#ifndef ___HEADFILE___9D7DF0EC_0FB5_48E2_8771_6F52D286DE11_
#define ___HEADFILE___9D7DF0EC_0FB5_48E2_8771_6F52D286DE11_

namespace nut
{

/**
 * һ������ü�����
 */
struct RefCounter
{
    int m_value;

    RefCounter(int v) : m_value(v) {}
    inline int operator++ () { return ++m_value; }
    inline int operator-- () { return --m_value; }
};

/**
 * ���̻߳����µ����ü�����
 */
struct RefCounterSync
{
    volatile int m_value;

    RefCounterSync(int v) : m_value(v) {}
    inline int operator++ () { return ++m_value; }
    inline int operator-- () { return --m_value; }
};

}

#endif /* head file guarder */