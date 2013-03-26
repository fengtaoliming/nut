/**
 * @file -
 * @author jingqi
 * @date 2013-03-26
 * @last-edit 2013-03-26 20:29:45 jingqi
 * @brief
 */

#ifndef ___HEADFILE_F8CA88A7_CD94_4F1D_AA24_A5E53F2E2E96_
#define ___HEADFILE_F8CA88A7_CD94_4F1D_AA24_A5E53F2E2E96_

#include <xutility>

namespace nut
{

/**
 * ��ά����
 * 
 * @param NumT ��������; ������ int��float ��
 * @param DIMENSIONS ά�������ڵ���1
 * @param RealNumT ʵ��������˷�������������õ���float��double��
 */
template <typename NumT, size_t DIMENSIONS = 2, typename RealNumT = double>
class MDArea
{
    typedef MDArea<NumT, DIMENSIONS, RealNumT> self;

public:
    NumT left[DIMENSIONS]; // ��߽�
    NumT right[DIMENSIONS]; // �ұ߽�

    MDArea()
    {
        for (register size_t i = 0; i < DIMENSIONS; ++i)
        {
            left[i] = 0;
            right[i] = 0;
        }
    }

    bool operator==(const self& x) const
    {
        for (register int i = 0; i < DIMENSIONS; ++i)
            if (left[i] != x.left[i] || right[i] != x.right[i])
                return false;
        return true;
    }

    bool operator!=(const self& x) const
    {
        return !(*this == x);
    }

    /**
     * ��չ�����Ա����Ŀ������
     */
    void expandToContain(const self& x)
    {
        for (register int i = 0; i < DIMENSIONS; ++i)
        {
            left[i] = std::min(left[i], x.left[i]);
            right[i] = std::max(right[i], x.right[i]);
        }
    }

    /**
     * ���������Ч��
     */
    bool isValid() const
    {
        for (register int i = 0; i < DIMENSIONS; ++i)
            if (left[i] > right[i])
                return false;
        return true;
    }

    /**
     * ��ռ�Ŀռ�
     */
    RealNumT acreage() const
    {
        RealNumT acr = 1;
        for (register int i = 0; i < DIMENSIONS; ++i)
            acr *= right[i] - left[i];
        return acr;
    }

    /**
     * �鿴�Ƿ���ȫ������һ������
     */
    bool contains(const self& x) const
    {
        for (register int i = 0; i < DIMENSIONS; ++i)
            if (!(left[i] <= x.left[i] && x.right[i] <= right[i]))
                return false;
        return true;
    }

    /**
     * �鿴�Ƿ�����һ�������н���
     */
    bool intersects(const self& x) const
    {
        // ����һ��ά�Ȳ��뽻���������뽻
        for (register size_t i = 0; i < DIMENSIONS; ++i)
            if (left[i] > x.right[i] || right[i] < x.left[i])
                return false;
        return true;
    }
};

}


#endif
