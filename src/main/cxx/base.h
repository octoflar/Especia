// Configuration constants
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
#ifndef ESPECIA_BASE_H
#define ESPECIA_BASE_H

namespace especia {
    /**
     * Pi.
     */
    const double PI = 3.1415926535897932384626433832795028841972; // -
    /**
     * The square root of Pi.
     */
    const double SQRT_OF_PI = 1.7724538509055160272981674833411451827975; // +
    /**
     * The speed of light in vacuum (km s-1).
     */
    const double SPEED_OF_LIGHT = 299792.458;

    /**
     * Returns the square of a number.
     *
     * @tparam number The number type.
     * @param x The number.
     * @return  The square of the number.
     */
    template<class number>
    number sqr(const number &x);
}

template<class number>
inline
number especia::sqr(const number &x) {
    return (x == number(0)) ? number(0) : x * x;
}

#endif //ESPECIA_BASE_H

