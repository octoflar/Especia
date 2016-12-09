// Mersenne Twister function-like class template
// Copyright (c) 2016 Ralf Quast
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#ifndef ESPECIA_MTWISTER_H
#define ESPECIA_MTWISTER_H

#include <algorithm>
#include <limits>
#include <valarray>

namespace especia {
    /**
     * The Mersenne twister algorithm to generate [0,1] uniformly distributed
     * random deviates.
     *
     * The notation of template parameters follows Matsumoto and
     * Nishimura (1998, Table 2).
     *
     * @tparam w The number of bits in a word.
     * @tparam n The parameter n.
     * @tparam m The parameter m.
     * @tparam r The parameter r.
     * @tparam a The parameter a.
     * @tparam u The parameter u.
     * @tparam s The parameter s.
     * @tparam b The parameter b.
     * @tparam t The parameter t.
     * @tparam c The parameter c.
     * @tparam l The parameter l.
     */
    template<unsigned w, unsigned n, unsigned m, unsigned r,
            unsigned long a,
            unsigned u,
            unsigned s, unsigned long b,
            unsigned t, unsigned long c,
            unsigned l>
    class mersenne_twister;

    /**
     * A predefined Mersenne twister algorithm.
     */
    typedef mersenne_twister<32, 351, 175, 19, 0xe4bd75f5, 11, 7, 0x655e5280,
            15, 0xffd58000, 17> mt11213_a;
    /**
     * A predefined Mersenne twister algorithm.
     */
    typedef mersenne_twister<32, 351, 175, 19, 0xccab8ee7, 11, 7, 0x31b6ab00,
            15, 0xffe50000, 17> mt11213_b;
    /**
     * A predefined Mersenne twister algorithm.
     */
    typedef mersenne_twister<32, 624, 397, 31, 0x9908b0df, 11, 7, 0x9d2c5680,
            15, 0xefc60000, 18> mt19937;
}

/**
 * The Mersenne twister algorithm to generate [0,1] uniformly distributed
 * random deviates.
 *
 * This functor template is based on the 2002/01/26 version coded by Takuji
 * Nishimura and Makoto Matsumoto(Matsumoto and Nishimura, 1998).
 *
 * The notation of template parameters follows Matsumoto and
 * Nishimura (1998, Table 2).
 *
 * Further reading:
 *
 * M. Matsumoto, T. Nishimura (1998)
 *   Mersenne Twister: A 623-dimensionally equidistributed uniform
 *   pseudorandom number generator
 *   ACM Transactions on Modeling and Computer Simulation, 8, 3, ISSN 1049-3301
 *
 * D. Knuth (1998)
 *   The art of computer programming 2. Seminumerical algorithms
 *   Addison Wesley Longman, ISBN 0-201-89684-2
 *
 * @tparam w The number of bits in a word.
 * @tparam n The parameter n.
 * @tparam m The parameter m.
 * @tparam r The parameter r.
 * @tparam a The parameter a.
 * @tparam u The parameter u.
 * @tparam s The parameter s.
 * @tparam b The parameter b.
 * @tparam t The parameter t.
 * @tparam c The parameter c.
 * @tparam l The parameter l.
 */
template<unsigned w, unsigned n, unsigned m, unsigned r,
        unsigned long a,
        unsigned u,
        unsigned s, unsigned long b,
        unsigned t, unsigned long c,
        unsigned l>
class especia::mersenne_twister {
public:
    /**
     * Constructs a new instance of this functor from a single seed.
     *
     * @param seed The seed.
     * @param multiplier A multiplier used by the seeding.
     */
    mersenne_twister(unsigned long seed = 5489, unsigned long multiplier = 1812433253)
            : words(n) {
        reset(seed, multiplier);
    }

    /**
     * Constructs a new instance of this functor from many seeds.
     *
     * @param seed_count The number of seeds.
     * @param seeds The seeds.
     */
    mersenne_twister(unsigned seed_count, const unsigned long seeds[])
            : words(n) {
        reset(seed_count, seeds);
    }

    /**
     * The destructor.
     */
    ~mersenne_twister() {
    };

    /**
     * Returns a new random number.
     *
     * @return a random number in [0, 1].
     */
    double operator()() {
        using std::numeric_limits;

        return rand() * (1.0 / double(numeric_limits<word>::max() >> (numeric_limits<word>::digits - w)));
            // division by 2^w - 1
    }

    /**
     * Resets this algorithm with a single seed.
     *
     * @param seed The seed.
     * @param multiplier A multiplier used by the seeding.
     */
    void reset(unsigned long seed = 5489, unsigned long multiplier = 1812433253) {
        using std::max;
        using std::numeric_limits;

        words[0] = seed & (numeric_limits<word>::max() >> (numeric_limits<word>::digits - w));
        for (unsigned k = 1; k < n; ++k) {
            words[k] = (multiplier * (words[k - 1] ^ (words[k - 1] >> (r - 1))) + k) &
                   (numeric_limits<word>::max() >> (numeric_limits<word>::digits - w));
        }

        i = n;
    }

    /**
     * Resets this algorithm with many seeds.
     *
     * @param seeds The seeds.
     * @param seed_count The number of seeds.
     */
    void reset(unsigned seed_count, const unsigned long seeds[]) {
        using std::max;
        using std::numeric_limits;

        reset(19650218);
        i = 1;

        for (unsigned j = 0, k = max(n, seed_count); k > 0; --k) {
            words[i] = ((words[i] ^ ((words[i - 1] ^ (words[i - 1] >> (r - 1))) * 1664525)) + seeds[j] + j) &
                   (numeric_limits<word>::max() >> (numeric_limits<word>::digits - w));
            if (++i >= n) {
                words[0] = words[n - 1];
                i = 1;
            }
            if (++j >= seed_count)
                j = 0;
        }
        for (unsigned k = n - 1; k > 0; --k) {
            words[i] = ((words[i] ^ ((words[i - 1] ^ (words[i - 1] >> (r - 1))) * 1566083941)) - i) &
                   (numeric_limits<word>::max() >> (numeric_limits<word>::digits - w));
            if (++i >= n) {
                words[0] = words[n - 1];
                i = 1;
            }
        }
        words[0] = (1ul << (w - 1));

        i = n;
    }

private:
    typedef unsigned long word;

    unsigned long rand() {
        if (i == n) {
            for (unsigned k = 0; k < n - m; ++k)
                twist(k + m, k, k + 1);

            for (unsigned k = n - m; k < n - 1; ++k)
                twist(k + m - n, k, k + 1);

            twist(m - 1, n - 1, 0);
            i = 0;
        }

        unsigned long y = words[i];
        ++i;

        if (u > 0)
            y ^= (y >> u);

        y ^= (y << s) & b;
        y ^= (y << t) & c;
        y ^= (y >> l);

        return y;
    }

    void twist(unsigned i, unsigned j, unsigned k) {
        using std::numeric_limits;

        words[j] = words[i] ^ (((words[j] & ((numeric_limits<word>::max()
                << (numeric_limits<word>::digits - w + r))
                >> (numeric_limits<word>::digits - w))) | (words[k] & (numeric_limits<word>::max()
                >> (numeric_limits<word>::digits - r)))) >> 1);
        if ((words[k] & 1ul) == 1ul)
            words[j] ^= a;
    }

    std::valarray<unsigned long> words;
    unsigned i;
};

#endif // ESPECIA_MTWISTER_H
