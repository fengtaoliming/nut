/**
 * @file -
 * @author jingqi
 * @date 2015-01-19
 * @brief
 */

#ifndef ___HEADFILE_583C901E_7939_41C8_A24C_AF094D969CD9_
#define ___HEADFILE_583C901E_7939_41C8_A24C_AF094D969CD9_

#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include <nut/platform/stdint.hpp>
#include "../complex.hpp"
#include "../word_array_integer.hpp"

namespace nut
{

inline void bit_rev(Complex *a, unsigned loglen, unsigned len)
{
    assert(NULL != a);

    for (unsigned i = 0; i < len; ++i)
    {
        unsigned t = i, p = 0;
        for (unsigned j = 0; j < loglen; ++j)
        {
            p <<= 1;
            p |=  (t & 1);
            t >>= 1;
        }
        if (p < i)
        {
            Complex temp = a[p];
            a[p] = a[i];
            a[i] = temp;
        }
    }
}

/**
 * 快速傅立叶变换
 *
 * @param rev 是否逆向
 */
inline void FFT(Complex *a, unsigned loglen, unsigned len, bool rev)
{
    assert(NULL != a);

    bit_rev(a, loglen, len);

    const double PI = ::acos(-1.0);
    for (unsigned s = 1, m = 2; s <= loglen; ++s, m <<= 1)
    {
        const Complex wn(::cos(2 * PI / m), (rev ? -::sin(2 * PI / m) : ::sin(2 * PI / m)));
        for (unsigned i = 0; i < len; i += m)
        {
            Complex w(1, 0);
            for (unsigned j = 0; j < m / 2; ++j)
            {
                const Complex u = a[i+j];
                const Complex v = w * a[i + j + m / 2];
                a[i + j] = u + v;
                a[i + j + m / 2] = u - v;
                w *= wn;
            }
        }
    }
    if (rev)
    {
        for (unsigned i = 0; i < len; ++i)
        {
            a[i].real /= len;
            a[i].image /= len;
        }
    }
}

/**
 * 利用快速傅立叶变换求大数的成绩，时间复杂度为 O(nlogn)
 */
template <typename T, typename MemAlloc>
void fft_multiply(const T *a, size_t M, const T *b, size_t N, T *x, size_t P, MemAlloc *ma = NULL)
{
    assert(NULL != a && M > 0 && NULL != b && N > 0 && NULL != x && P > 0);
    typedef typename StdInt<T>::unsigned_type word_type;
    typedef typename StdInt<T>::double_unsigned_type dword_type;

    // 处理负数和截断
    const word_type filla = (is_positive(a, M) ? 0 : ~(word_type)0);
    const size_t MM = ((M > P || 0 != filla) ? P : M);
    const word_type fillb = (is_positive(b, N) ? 0 : ~(word_type)0);
    const size_t NN = ((N > P || 0 != fillb) ? P : N);

    // 取2的幂次长度
    unsigned n = 1, loglen = 0;
    while (n < MM + NN)
    {
        n <<= 1;
        ++loglen;
    }

    // 填充初始数据
    Complex *pa = NULL;
    if (NULL != ma)
        pa = (Complex*) ma->alloc(sizeof(Complex) * n * 2);
    else
        pa = (Complex*) ::malloc(sizeof(Complex) * n * 2);
    assert(NULL != pa);
    Complex *pb = pa + n;
    for (size_t i = 0; i < n; ++i)
    {
        pa[i].real = (i < M ? a[i] : filla);
        pa[i].image = 0;
    }
    for (size_t i = 0; i < n; ++i)
    {
        pb[i].real = (i < N ? b[i] : fillb);
        pb[i].image = 0;
    }

    // FFT 变换
    FFT(pa, loglen, n, false);
    FFT(pb, loglen, n, false);
    for (size_t i = 0; i < n; ++i)
        pa[i] *= pb[i];
    FFT(pa, loglen, n, true);

    // 插值
    double carray = 0;
    const double radix = ::pow(2, 8 * sizeof(word_type));
    for (size_t i = 0; i < P; ++i)
    {
        if (i < n)
        {
            assert(pa[i].real >= 0);

            const double v = ::floor(pa[i].real + 0.5) + carray;
            x[i] = (word_type) v;
            carray = ::floor(v / radix);
        }
        else
        {
            x[i] = (is_positive(x, n - 1) ? 0 : ~(word_type)0);
        }
    }

    // 回收内存
    if (NULL != ma)
        ma->free(pa);
    else
        ::free(pa);
}

}

#endif
