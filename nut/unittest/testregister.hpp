/**
 * @file -
 * @author jingqi
 * @date 2011-11-12 13:16
 */

#ifndef ___HEADFILE___53924D54_39B0_4377_B555_4096B8CC3619_
#define ___HEADFILE___53924D54_39B0_4377_B555_4096B8CC3619_

#include <assert.h>

namespace nut
{

template <typename T>
T*& __getListHead()
{
    static T* g_header = NULL;
    return g_header;
}

class TestRegister
{
    typedef TestFixture* (*new_fixture_func)();
    typedef void (*delete_fixture_func)(TestFixture*);

    new_fixture_func m_newFunc;
    delete_fixture_func m_deleteFunc;
    const char *m_fixtureName;
    std::vector<std::string> m_groups;
    TestRegister *m_pnext;

public:
    TestRegister(const char *fixtureName, const char *groups, new_fixture_func n, delete_fixture_func d)
        : m_fixtureName(fixtureName), m_newFunc(n), m_deleteFunc(d)
    {
        assert(NULL != fixtureName);
        assert(NULL != groups);
        assert(NULL != n);
        assert(NULL != d);

        // ��������
        m_groups = splitGroups(groups);

        // ��ʵ����ӵ�������
        m_pnext = __getListHead<TestRegister>();
        __getListHead<TestRegister>() = this;
    }

    bool matchGroup(const char *groupName) const
    {
        assert(NULL != groupName);
        for (std::vector<std::string>::const_iterator iter = m_groups.begin(); iter != m_groups.end(); ++iter)
        {
            if (0 == ::strcmp(iter->c_str(), groupName))
                return true;
        }
        return false;
    }

    const char* getFixtureName() const { return m_fixtureName; }

    TestRegister* getNextRegister() const { return m_pnext; }

    TestFixture* newFixture() const { return m_newFunc(); }

    void deleteFixture(TestFixture *p)
    {
        assert(NULL != p);
        m_deleteFunc(p);
    }

private:
    /** �ж��ַ��Ƿ��ǿհ� */
    static inline bool isBlank(char c)
    {
        return c == ' ' || c == '\r' || c == '\n' || c == '\t';
    }

    /** ȥ���ַ���ĩβ�Ŀհ� */
    static std::string trimEnd(const std::string& s)
    {
        std::string ret = s;
        while (ret.length() > 0)
        {
            char c = ret.at(ret.length() - 1);
            if (!isBlank(c))
                return ret;
            ret.resize(ret.length() - 1);
        }
        return ret;
    }

    /** ����� "test, quiet" ���ַ���Ϊ ["test", "quiet"] */
    static std::vector<std::string> splitGroups(const char *groups)
    {
        assert(NULL != groups);

        const char GROUP_SPLITER = ','; // ����ָ���

        std::vector<std::string> ret;
        std::string s;
        for (int i = 0; '\0' != groups[i]; ++i)
        {
            if (GROUP_SPLITER == groups[i])
            {
                // ȥ��β���ո�
                s = trimEnd(s);

                // ��ӵ����
                if (s.length() > 0)
                {
                    ret.push_back(s);
                    s.clear();
                }
            }
            else if (s.length() > 0 || !isBlank(groups[i]))
            {
                s.push_back(groups[i]);
            }
        }
        s = trimEnd(s);
        if (s.length() > 0)
            ret.push_back(s);

        return ret;
    }
};

}

#endif /* head file guarder */
