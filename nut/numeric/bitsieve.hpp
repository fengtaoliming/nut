/**
 * @file -
 * @author jingqi
 * @date 2013-01-29
 * @last-edit 
 * @brief
 */

#ifndef ___HEADFILE_B3D1D8B6_CD77_4FB3_A62E_A83D30BA0451_
#define ___HEADFILE_B3D1D8B6_CD77_4FB3_A62E_A83D30BA0451_

#include "unsignedinteger.hpp"
#include "numeric_algo.hpp"

namespace nut
{
    
/**
 * A simple bit sieve used for finding prime number candidates. Allows setting
 * and clearing of bits in a storage array. The size of the sieve is assumed to
 * be constant to reduce overhead. All the bits of a new bitSieve are zero, and
 * bits are removed from it by setting them.
 *
 * To reduce storage space and increase efficiency, no even numbers are
 * represented in the sieve (each bit in the sieve represents an odd number).
 * The relationship between the index of a bit and the number it represents is
 * given by
 * N = offset + (2*index + 1);
 * Where N is the integer represented by a bit in the sieve, offset is some
 * even integer offset indicating where the sieve begins, and index is the
 * index of a bit in the sieve array.
 *
 * @see     BigInteger
 * @version 1.13, 11/07/08
 * @author  Michael McCloskey
 * @since   1.3
 */
template <typename VOID_T>
class _BitSieve
{
    /**
     * Stores the bits in this bitSieve.
     */
    long *m_bits;
    int m_bits_cap;

    /**
     * Length is how many bits this sieve holds.
     */
    int m_length;

    /**
     * A small sieve used to filter out multiples of small primes in a search
     * sieve.
     */
    static _BitSieve<VOID_T> s_smallSieve;

    /**
     * Construct a "small sieve" with a base of 0.  This constructor is 
     * used internally to generate the set of "small primes" whose multiples
     * are excluded from sieves generated by the main (package private)
     * constructor, BitSieve(BigInteger base, int searchLen).  The length
     * of the sieve generated by this constructor was chosen for performance;
     * it controls a tradeoff between how much time is spent constructing
     * other sieves, and how much time is wasted testing composite candidates
     * for primality.  The length was chosen experimentally to yield good
     * performance.
     */
    _BitSieve()
    {
        m_length = 150 * 64;
        m_bits_cap = unitIndex(m_length - 1) + 1;
        m_bits = new long[m_bits_cap];

        // Mark 1 as composite
        set(0);
        int nextIndex = 1;
        int nextPrime = 3;

        // Find primes and remove their multiples from sieve
        do
        {
            sieveSingle(m_length, nextIndex + nextPrime, nextPrime);
            nextIndex = sieveSearch(m_length, nextIndex + 1);
            nextPrime = 2*nextIndex + 1;
        } while((nextIndex > 0) && (nextPrime < m_length));
    }

public:
    /**
     * Construct a bit sieve of searchLen bits used for finding prime number
     * candidates. The new sieve begins at the specified base, which must
     * be even.
     */
    template <size_t N>
    _BitSieve(const UnsignedInteger<N>& base, int searchLen)
    {
        /*
         * Candidates are indicated by clear bits in the sieve. As a candidates
         * nonprimality is calculated, a bit is set in the sieve to eliminate
         * it. To reduce storage space and increase efficiency, no even numbers
         * are represented in the sieve (each bit in the sieve represents an
         * odd number).
         */
        m_bits_cap = unitIndex(searchLen-1) + 1;
        m_bits = new long[m_bits_cap];
        m_length = searchLen;
        int start = 0;
        
        int step = s_smallSieve.sieveSearch(s_smallSieve.m_length, start);
        int convertedStep = (step *2) + 1;

        // Construct the large sieve at an even offset specified by base
        UnsignedInteger<N> b(base);
        UnsignedInteger<N> q;
        do
        {
            // Calculate base mod convertedStep
            divide_unsigned(b.buffer(), N, (uint8_t*)&convertedStep, sizeof(convertedStep), q.buffer(), N, (uint8_t*)&start, sizeof(start));

            // Take each multiple of step out of sieve
            start = convertedStep - start;
            if (start%2 == 0)
                start += convertedStep;
            sieveSingle(searchLen, (start-1)/2, convertedStep);

            // Find next prime from small sieve
            step = s_smallSieve.sieveSearch(s_smallSieve.m_length, step+1);
            convertedStep = (step *2) + 1;
        } while (step > 0);
    }

    ~_BitSieve()
    {
        if (NULL != m_bits)
            delete[] m_bits;
        m_bits = NULL;
        m_bits_cap = 0;
    }

private:   
    /**
     * Given a bit index return unit index containing it.
     */
    static inline int unitIndex(int bitIndex)
    {
        return (int)(((unsigned)bitIndex) >> 6);
    }

    /**
     * Return a unit that masks the specified bit in its unit.
     */
    static inline long bit(int bitIndex)
    {
        return 1L << (bitIndex & ((1<<6) - 1));
    }

    /**
     * Get the value of the bit at the specified index.
     */
    inline boolean get(int bitIndex)
    {
        int ui = unitIndex(bitIndex);
        return ((m_bits[ui] & bit(bitIndex)) != 0);
    }

    /**
     * Set the bit at the specified index.
     */
    inline void set(int bitIndex)
    {
        int ui = unitIndex(bitIndex);
        m_bits[ui] |= bit(bitIndex);
    }

    /**
     * This method returns the index of the first clear bit in the search
     * array that occurs at or after start. It will not search past the
     * specified limit. It returns -1 if there is no such clear bit.
     */
    inline int sieveSearch(int limit, int start)
    {
        if (start >= limit)
            return -1;
        
        int index = start;
        do
        {
            if (!get(index))
                return index;
            index++;
        } while(index < limit-1);
        return -1;
    }

    /**
     * Sieve a single set of multiples out of the sieve. Begin to remove
     * multiples of the specified step starting at the specified start index,
     * up to the specified limit.
     */
    inline void sieveSingle(int limit, int start, int step)
    {
        while(start < limit)
        {
            set(start);
            start += step;
        }
    }

public:
    /**
     * Test probable primes in the sieve and return successful candidates.
     */
    template <size_t N>
    UnsignedInteger<N> retrieve(UnsignedInteger<N> initValue, int certainty)
    {
        // Examine the sieve one long at a time to find possible primes
        int offset = 1;
        for (int i = 0; i < m_bits_cap; ++i)
        {
            long nextLong = ~m_bits[i];
            for (int j = 0; j < 64; ++j)
            {
                if ((nextLong & 1) == 1)
                {
                    UnsignedInteger<N> candidate = initValue + UnsignedInteger<N>(offset);
                    if (miller_rabin(candidate, certainty))
                        return candidate;
                }
                nextLong = (long)(((unsigned long) nextLong) >> 1);
                nextLong >>= 1;
                offset+=2;
            }
        }
        return UnsignedInteger<N>(0);
    }
};

template <typename VOID_T>
_BitSieve<VOID_T> _BitSieve<VOID_T>::s_smallSieve;

typedef _BitSieve<void> BitSieve;

}

#endif
