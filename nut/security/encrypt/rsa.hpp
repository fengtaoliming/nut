/**
 * @file -
 * @author jingqi
 * @date 2012-11-25
 * @last-edit 2013-01-25 15:42:08 jingqi
 */

#ifndef ___HEADFILE_12FB3097_3C87_4104_A964_9BF5D45EF28E_
#define ___HEADFILE_12FB3097_3C87_4104_A964_9BF5D45EF28E_

#include <assert.h>

#include <nut/numeric/biginteger.hpp>
#include <nut/numeric/numeric_algo.hpp>

#include <assert.h>

#include <nut/numeric/biginteger.hpp>
#include <nut/numeric/numeric_algo.hpp>

namespace nut
{

class RSA
{
public:
    struct PublicKey
    {
        unsigned e;
        BigInteger n;
    };

    struct PrivateKey
    {
        BigInteger d, n;
    };

    static void genKey(size_t bit_count, PublicKey *publicKey, PrivateKey *privateKey)
    {
        assert(bit_count > 10);

        // ���ѡȡ�������� 
        // NOTE:
        //	Ϊ�˱�����Բ�������ӷֽ��㷨��p��qӦ�д�����ͬ�ı��س��ȣ����㹻��
        //  ͬʱ�� p,q ��Ӧ̫�ӽ�, ��������׷ֽ�, ���ּ������س��Ȳ��ǿ��Ե�
        BigInteger bound(1);
        bound <<= ((bit_count + 1) / 2);
        BigInteger p = BigInteger::rand_between(bound, bound << 1);
        p = nextProbablePrime(p);
        bound <<= 3;
        BigInteger q = BigInteger::rand_between(bound, bound << 1);
        q = nextProbablePrime(q);

        // ѡȡС���� e��ʹ�� e �� gamma_n ����
        const BigInteger n(p * q), gamma_n = (p - 1) * (q - 1);
        // NOTE:
        // 	e ��ȡ 3 �� 65537������λ bit1 �٣�������߼����ٶ�
        unsigned e = 65537;

        // d Ϊ e ��ģ gamma_n �ĳ˷���Ԫ 
        BigInteger d;
        extended_euclid(BigInteger(e), gamma_n, NULL, &d, NULL);
        if (d < 0)
            d = gamma_n + (d % gamma_n); // % ��������뱻����һ��

        // ��Կ (e, n)
        if (NULL != publicKey)
        {
            publicKey->e = e;
            publicKey->n = n;
        }

        // ˽Կ(d, n)
        if (NULL != privateKey)
        {
            privateKey->d = d;
            privateKey->n = n;
        }
    }

    static BigInteger encode(const BigInteger& m, const PublicKey& k)
    {
        return modular_exponentiation(m, BigInteger(k.e), k.n);
    }

    static BigInteger decode(const BigInteger& c, const PrivateKey& k)
    {
        return modular_exponentiation(c, k.d, k.n);
    }
};

}

#endif


