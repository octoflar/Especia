//! @file random.h
//! Function-like class templates to generate random numbers.
/// @author Ralf Quast
/// @date 2021
/// @copyright MIT License
#ifndef ESPECIA_RANDOM_H
#define ESPECIA_RANDOM_H

#include <algorithm>
#include <limits>
#include <valarray>

#include "base.h"

namespace especia {

    /// A maximally equidistributed F2-linear generator (MELG).
    ///
    /// Further reading:
    ///
    /// S. Harase and T. Kimoto (2018).
    /// Implementing 64-bit maximally equidistributed F2-linear generators with Mersenne prime period.
    /// ACM Transactions on Mathematical Software, 44, 3, 30.
    /// <http://doi.acm.org/10.1145/3159444>, <http://arxiv.org/abs/1505.06582>
    ///
    /// @tparam w The the number of bits in a word.
    /// @tparam n The parameter n.
    /// @tparam m The parameter m.
    /// @tparam l The parameter l.
    /// @tparam mult1 A multiplier (used for initialisation).
    /// @tparam mult2 A multiplier (used for initialisation).
    /// @tparam mult3 A multiplier (used for initialisation).
    template<natural w, natural n, natural m, natural l, word64 mult1, word64 mult2, word64 mult3>
    class Melg {
    public:
        /// Constructs a new instance of this functor.
        ///
        /// @param[in] seed The seed.
        explicit Melg(const word64 seed) : state(n + 1) { // NOLINT
            const word64 seeds[] = {seed & 0x00000000FFFFFFFFull, seed & 0xFFFFFFFF00000000ull};

            reset(2, seeds);
        }

        /// Constructs a new instance of this functor.
        ///
        /// @param[in] seed_count The number of seeds.
        /// @param[in] seeds The seeds.
        Melg(const natural seed_count, const word64 seeds[]) : state(n + 1) { // NOLINT
            reset(seed_count, seeds);
        }

        /// The destructor.
        ~Melg() = default;

        /// Returns a new real-valued  random number in the interval  [0, 1].
        ///
        /// @return a real-valued random number in [0, 1].
        real operator()() const {
            using std::numeric_limits;
            // the maximum mantissa value for a real number
            const real max_mantissa =
                    numeric_limits<word64>::max() >> (numeric_limits<word64>::digits - (w < numeric_limits<real>::digits ? w : numeric_limits<real>::digits));

            return (w < numeric_limits<real>::digits ? rand() : rand() >> (w - numeric_limits<real>::digits)) * (1.0 / max_mantissa);
        }

        /// Returns a new  random word.
        ///
        /// @return a random word.
        word64 rand() const {
            word64 next = 0ull;

            switch (cycle) {
                case 1:
                    next = rock(index, index + 1);
                    roll(next, index + m);
                    next = twist(next, index, index + l);
                    index++;
                    if (index == n - m) {
                        cycle = 2;
                    }
                    break;
                case 2:
                    next = rock(index, index + 1);
                    roll(next, index + m - n);
                    next = twist(next, index, index + l);
                    index++;
                    if (index == n - l) {
                        cycle = 3;
                    }
                    break;
                case 3:
                    next = rock(index, index + 1);
                    roll(next, index + m - n);
                    next = twist(next, index, index - (n - l));
                    index++;
                    if (index == n - 1) {
                        cycle = 4;
                    }
                    break;
                case 4:
                    next = rock(n - 1, 0);
                    roll(next, m - 1);
                    next = twist(next, n - 1, index - (n - l));
                    index = 0;
                    cycle = 1;
                    break;
            }

            return next;
        }

    private:
        /// Resets this algorithm.
        ///
        /// @param[in] seed The seed.
        void reset(const word64 seed) {
            state[0] = seed;

            for (index = 1; index < n + 1; index++) {
                state[index] = (state[index - 1] ^ (state[index - 1] >> (w - 2))) * mult1 + index;
            }

            index = 0;
            cycle = 1;
        }

        /// Resets this algorithm.
        ///
        /// @param[in] seed_count The number of seeds.
        /// @param[in] seeds The seeds.
        void reset(const natural seed_count, const word64 seeds[]) {
            using std::max;

            reset(19650218ull);

            natural i = 1;
            natural j = 0;
            for (natural k = max(n, seed_count); k > 0; k--) {
                state[i] = (state[i] ^ ((state[i - 1] ^ (state[i - 1] >> (w - 2))) * mult2)) + seeds[j] + j;
                i++;
                j++;
                if (i >= n) {
                    state[0] = state[n - 1];
                    i = 1;
                }
                if (j >= seed_count) {
                    j = 0;
                }
            }
            for (natural k = n - 1; k > 0; k--) {
                state[i] = (state[i] ^ ((state[i - 1] ^ (state[i - 1] >> (w - 2))) * mult3)) - i;
                i++;
                if (i >= n) {
                    state[0] = state[n - 1];
                    i = 1;
                }
            }
            state[n] = (state[n] ^ ((state[n - 1] ^ (state[n - 1] >> (w - 2))) * mult3)) - n;
            state[0] = (state[0] | (1ull << (w - 1)));

            index = 0;
            cycle = 1;
        }

        word64 rock(const natural i, const natural k) const {
            return (state[i] & 0xFFFFFFFF80000000ull) | (state[k] & 0x7FFFFFFFull);
        }

        void roll(const word64 word, const natural i) const {
            state[n] = (word >> 1) ^ (((word & 1ull) != 0ull) ? 0x5C32E06DF730FC42ull : 0ull) ^ state[i] ^ (state[n] ^ (state[n] << 23));
        }

        word64 twist(const word64 word, const natural i, const natural k) const {
            state[i] = word ^ (state[n] ^ (state[n] >> 33));
            return state[i] ^ (state[i] << 16) ^ (state[k] & 0x6AEDE6FD97B338ECull);
        }

        mutable std::valarray<word64> state;
        mutable natural index;
        mutable natural cycle;
    };

    /// The MELG19937-64 with 2,496 bytes of state and 64-bit output.
    typedef Melg<64, 311, 81, 19, 6364136223846793005ull, 3935559000370003845ull, 2862933555777941757ull> Melg19937_64;


    /// The Mersenne twister algorithm to generate [0,1] uniformly distributed
    /// random deviates.
    ///
    /// This functor template is based on the 2002/01/26 version coded by Takuji
    /// Nishimura and Makoto Matsumoto(Matsumoto and Nishimura, 1998).
    ///
    /// The notation of template parameters follows Matsumoto and
    /// Nishimura (1998, Table 2).
    ///
    /// Further reading:
    ///
    /// m. Matsumoto, T. Nishimura (1998).
    ///  *Mersenne Twister: A 623-dimensionally equidistributed uniform pseudorandom number generator.*
    ///  ACM Transactions on Modeling and Computer Simulation, 8, 3, ISSN 1049-3301.
    ///
    /// D. Knuth (1998).
    ///  *The art of computer programming 2. Seminumerical algorithms*.
    ///  Addison Wesley Longman, ISBN 0-201-89684-2.
    ///
    /// @tparam w The number of bits in a word.
    /// @tparam n The parameter n.
    /// @tparam m The parameter m.
    /// @tparam r The parameter r.
    /// @tparam a The parameter a.
    /// @tparam u The parameter u.
    /// @tparam d The parameter d.
    /// @tparam s The parameter s.
    /// @tparam b The parameter b.
    /// @tparam t The parameter t.
    /// @tparam c The parameter c.
    /// @tparam l The parameter l.
    /// @tparam mult1 A multiplier (used for initialisation).
    /// @tparam mult2 A multiplier (used for initialisation).
    /// @tparam mult3 A multiplier (used for initialisation).
    template<natural w, natural n, natural m, natural r,
            word64 a,
            natural u, word64 d,
            natural s, word64 b,
            natural t, word64 c,
            natural l,
            word64 mult1,
            word64 mult2,
            word64 mult3>
    class Mersenne_Twister {
    public:
        /// Constructs a new instance of this functor.
        ///
        /// @param[in] seed The seed.
        explicit Mersenne_Twister(const word64 seed = 9600629759793949339ull) : state(n) { // NOLINT
            const word64 seeds[] = {seed & 0x00000000FFFFFFFFull, seed & 0xFFFFFFFF00000000ull};

            reset(2, seeds);
        }

        /// Constructs a new instance of this functor.
        ///
        /// @param[in] seed_count The number of seeds.
        /// @param[in] seeds The seeds.
        Mersenne_Twister(const natural seed_count, const word64 seeds[]) : state(n) { // NOLINT
            reset(seed_count, seeds);
        }

        /// The destructor.
        ~Mersenne_Twister() = default;

        /// Returns a new real-valued  random number in the interval  [0, 1].
        ///
        /// @return a real-valued random number in [0, 1].
        real operator()() const {
            using std::numeric_limits;
            // the maximum mantissa value for a real number
            const real max_mantissa =
                numeric_limits<word64>::max() >> (numeric_limits<word64>::digits - (w < numeric_limits<real>::digits ? w : numeric_limits<real>::digits));

            return (w < numeric_limits<real>::digits ? rand() : rand() >> (w - numeric_limits<real>::digits)) * (1.0 / max_mantissa);
        }

        /// Returns a new  random word.
        ///
        /// @return a random word.
        word64 rand() const {
            if (index == n) {
                for (natural k = 0; k < n - m; ++k) {
                    twist(k + m, k, k + 1);
                }

                for (natural k = n - m; k < n - 1; ++k) {
                    twist(k + m - n, k, k + 1);
                }

                twist(m - 1, n - 1, 0);
                index = 0;
            }

            word64 next = state[index];
            ++index;

            next ^= (next >> u) & d;
            next ^= (next << s) & b;
            next ^= (next << t) & c;
            next ^= (next >> l);

            return next;
        }

    private:
        /// Resets this algorithm.
        ///
        /// @param[in] seed The seed.
        void reset(const word64 seed) {
            using std::numeric_limits;

            state[0] = seed & (numeric_limits<word64>::max() >> (numeric_limits<word64>::digits - w));
            for (natural k = 1; k < n; ++k) {
                state[k] = ((state[k - 1] ^ (state[k - 1] >> (w - 2))) * mult1 + k) &
                           (numeric_limits<word64>::max() >> (numeric_limits<word64>::digits - w));
            }
            index = n;
        }

        /// Resets this algorithm.
        ///
        /// @param[in] seed_count The number of seeds.
        /// @param[in] seeds The seeds.
        void reset(const natural seed_count, const word64 seeds[]) {
            using std::max;
            using std::numeric_limits;

            reset(19650218ull);

            natural i = 1;
            for (natural j = 0, k = max(n, seed_count); k > 0; --k) {
                state[i] = ((state[i] ^ ((state[i - 1] ^ (state[i - 1] >> (w - 2))) * mult2)) + seeds[j] + j) &
                           (numeric_limits<word64>::max() >> (numeric_limits<word64>::digits - w));
                if (++i >= n) {
                    state[0] = state[n - 1];
                    i = 1;
                }
                if (++j >= seed_count) {
                    j = 0;
                }
            }
            for (natural k = n - 1; k > 0; --k) {
                state[i] = ((state[i] ^ ((state[i - 1] ^ (state[i - 1] >> (w - 2))) * mult3)) - i) &
                           (numeric_limits<word64>::max() >> (numeric_limits<word64>::digits - w));
                if (++i >= n) {
                    state[0] = state[n - 1];
                    i = 1;
                }
            }
            state[0] = (1ull << (w - 1));
            index = n;
        }

        void twist(const natural i, const natural j, const natural k) const {
            using std::numeric_limits;

            state[j] = state[i] ^ (((state[j] & ((numeric_limits<word64>::max()
                    << (numeric_limits<word64>::digits - w + r))
                    >> (numeric_limits<word64>::digits - w))) | (state[k] & (numeric_limits<word64>::max()
                    >> (numeric_limits<word64>::digits - r)))) >> 1);
            if ((state[k] & 1ull) == 1ull) {
                state[j] ^= a;
            }
        }

        mutable std::valarray<word64> state;
        mutable natural index = 0;
    };

    /// The MT-1121A-32.
    typedef Mersenne_Twister<32, 351, 175, 19, 0xE4BD75F5ull, 11, 0xFFFFFFFFull, 7, 0x655E5280ull,
            15, 0xFFD58000ull, 17, 1812433253ull, 1664525ull, 1566083941ull> Mt11213a_32;
    /// The MT-1121B-32.
    typedef Mersenne_Twister<32, 351, 175, 19, 0xCCAB8EE7ull, 11, 0xFFFFFFFFull, 7, 0x31B6AB00ull,
            15, 0xFFE50000ull, 17, 1812433253ull, 1664525ull, 1566083941ull> Mt11213b_32;
    /// The MT-19937-32.
    typedef Mersenne_Twister<32, 624, 397, 31, 0x9908B0DFull, 11, 0xFFFFFFFFull, 7, 0x9D2C5680ull,
            15, 0xEFC60000ull, 18, 1812433253ull, 1664525ull, 1566083941ull> Mt19937_32;

    /// The MT-19937-64.
    typedef Mersenne_Twister<64, 312, 156, 31, 0xB5026F5AA96619E9ull, 29, 0x5555555555555555ull, 17, 0x71D67FFFEDA60000ull,
            37, 0xFFF7EEE000000000ull, 43, 6364136223846793005ull, 3935559000370003845ull, 2862933555777941757ull> Mt19937_64;


    /// PCG algorithm  to generate [0,1] uniformly distributed random deviates. Based on
    /// Melissa E. O'Neill (2014) and <https://www.pcg-random.org>.
    ///
    /// Further reading:
    ///
    /// Melissa E. O'Neill (2014).
    ///  *PCG: A Family of Simple Fast Space-Efficient Statistically Good Algorithms for Random Number Generation.*
    ///  <https://www.cs.hmc.edu/tr/hmc-cs-2014-0905.pdf>.
    ///
    /// @tparam m The multiplier.
    template<word64 mult>
    class Pcg {
    public:
        /// Constructs a new instance of this functor.
        ///
        /// @param[in] seed The seed.
        /// @param[in] selector The sequence selector.
        explicit Pcg(const word64 seed = 9600629759793949339ull, const word64 selector = 7863035247680335341ul) : inc((selector << 1) | 1ull) {
            state = 0ull;
            rand();
            state += seed;
            rand();
        }

        /// The destructor.
        ~Pcg() = default;

        /// Returns a new real-valued  random number in the interval  [0, 1].
        ///
        /// @return a real-valued random number in [0, 1].
        real operator()() const {
            return rand() / real(0xFFFFFFFFul);
        }

        /// Returns a new random word.
        ///
        /// @return a random word.
        word32 rand() const {
            const word64 saved = state;
            state = saved * mult + inc;
            const word32 s = (((saved >> 18) ^ saved) >> 27);
            const word32 r = saved >> 59;
            return ((s >> r) | (s << ((-r) & 31ul)));
        }

    private:
        /// The increment.
        const word64 inc;

        /// The state.
        mutable word64 state;
    };

    /// The PCG-XSH-RR with 64-bit state and 32-bit output.
    typedef Pcg<6364136223846793005ull> Pcg_32;

}

#endif // ESPECIA_RANDOM_H
