
#include <iostream>

#include <nut/unittest/unittest.hpp>

#include <nut/numeric/numeric_algo/fft.hpp>
#include <nut/mem/sys_ma.hpp>

using namespace std;
using namespace nut;

typedef unsigned char word_type;

NUT_FIXTURE(TestFFT)
{
    NUT_CASES_BEGIN()
    NUT_CASE(test_rand_sample)
    NUT_CASES_END()

    void set_up() {}
    void tear_down() {}

    void test_rand_sample()
    {
        const int N = 17;
        word_type a[N], b[N], c[2 * N], d[2 * N];

        for (size_t i = 0; i < sizeof(word_type) * N - 1; ++i)
            reinterpret_cast<uint8_t*>(a)[i] = (uint8_t) ::rand();
        for (size_t i = 0; i < sizeof(word_type) * N - 1; ++i)
            reinterpret_cast<uint8_t*>(b)[i] = (uint8_t) ::rand();
        ::memset(c, 0, sizeof(word_type) * N * 2);
        ::memset(d, 0, sizeof(word_type) * N * 2);

        a[0] = 10003;
        b[0] = 20004;
        multiply<word_type,sys_ma>(a, N, b, N, c, N * 2, NULL);
        fft_multiply<word_type,sys_ma>(a, N, b, N, d, N * 2, NULL);
        cout << endl << (unsigned)a[0] << '*' << (unsigned)b[0] << '=' <<
                (unsigned)c[0] << " <> " << (unsigned)d[0] << endl;
        NUT_TA(0 == ::memcmp(c, d, sizeof(word_type) * N * 2));
    }
};

NUT_REGISTER_FIXTURE(TestFFT, "numeric,quiet")
