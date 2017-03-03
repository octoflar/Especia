/// @file readline.h
/// Data input procedures.
/// Copyright (c) 2016 Ralf Quast
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
#ifndef ESPECIA_READLINE_H
#define ESPECIA_READLINE_H

#include <cstddef>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

namespace especia {

    /**
     * Reads a vector of data from an input stream.
     *
     * @tparam A The data type.
     *
     * @param is The input stream.
     * @param a The data vector.
     * @param n The number of data elements to read.
     * @param append If @c true, the data read are append to the data vector @c a.
     *
     * @return the input stream.
     */
    template<class A>
    std::istream &read(std::istream &is, std::vector<A> &a, size_t n, bool append = false) {
        using namespace std;

        vector<A> ta; ta.reserve(n);

        for (size_t i = 0; i < n; ++i) {
            A aa;

            if (is >> aa)
                ta.push_back(aa);
        }

        if (is) {
            if (append) {
                a.insert(a.end(), ta.begin(), ta.end());
            } else {
                a.assign(ta.begin(), ta.end());
            }
        }
        return is;
    }

    /**
     * Reads two vectors of data from an input stream.
     *
     * The procedure expects that the data for vectors @c a and @c b are alternating.
     *
     * @tparam A A data type.
     * @tparam B A data type.
     *
     * @param is The input stream.
     * @param a A data vector.
     * @param b A data vector.
     * @param n The number of data elements to read for each data vector.
     * @param append If @c true, the data read are append to the data vectors.
     *
     * @return the input stream.
     */
    template<class A, class B>
    std::istream &read(std::istream &is, std::vector<A> &a, std::vector<B> &b, size_t n, bool append = false) {
        using namespace std;

        vector<A> ta;
        vector<B> tb;

        ta.reserve(n);
        tb.reserve(n);

        for (size_t i = 0; i < n; ++i) {
            A aa;
            B bb;

            if (is >> aa >> bb) {
                ta.push_back(aa);
                tb.push_back(bb);
            }
        }

        if (is) {
            if (append) {
                a.insert(a.end(), ta.begin(), ta.end());
                b.insert(b.end(), tb.begin(), tb.end());
            } else {
                a.assign(ta.begin(), ta.end());
                b.assign(tb.begin(), tb.end());
            }
        }
        return is;
    }

    /**
     * Reads three vectors of data from an input stream.
     *
     * The procedure expects that the data for vectors @c a, @c b, and @c c are alternating.
     *
     * @tparam A A data type.
     * @tparam B A data type.
     * @tparam C A data type.
     *
     * @param is The input stream.
     * @param a A data vector.
     * @param b A data vector.
     * @param c A data vector.
     * @param n The number of data elements to read for each data vector.
     * @param append If @c true, the data read are append to the data vectors.
     *
     * @return the input stream.
     */
    template<class A, class B, class C>
    std::istream &
    read(std::istream &is, std::vector<A> &a, std::vector<B> &b, std::vector<C> &c, size_t n, bool append = false) {
        using namespace std;

        vector<A> ta;
        vector<B> tb;
        vector<C> tc;

        ta.reserve(n);
        tb.reserve(n);
        tc.reserve(n);

        for (size_t i = 0; i < n; ++i) {
            A aa;
            B bb;
            C cc;

            if (is >> aa >> bb >> cc) {
                ta.push_back(aa);
                tb.push_back(bb);
                tc.push_back(cc);
            }
        }

        if (is) {
            if (append) {
                a.insert(a.end(), ta.begin(), ta.end());
                b.insert(b.end(), tb.begin(), tb.end());
                c.insert(c.end(), tc.begin(), tc.end());
            } else {
                a.assign(ta.begin(), ta.end());
                b.assign(tb.begin(), tb.end());
                c.assign(tc.begin(), tc.end());
            }
        }
        return is;
    }


    /**
     * Reads four vectors of data from an input stream.
     *
     * The procedure expects that the data for vectors @c a, @c b, c, and @c d are alternating.
     *
     * @tparam A A data type.
     * @tparam B A data type.
     * @tparam C A data type.
     * @tparam D A data type.
     *
     * @param is The input stream.
     * @param a A data vector.
     * @param b A data vector.
     * @param c A data vector.
     * @param d A data vector.
     * @param n The number of data elements to read for each data vector.
     * @param append If @c true, the data read are append to the data vectors.
     *
     * @return the input stream.
     */
    template<class A, class B, class C, class D>
    std::istream &
    read(std::istream &is, std::vector<A> &a, std::vector<B> &b, std::vector<C> &c, std::vector<D> &d, size_t n,
         bool append = false) {
        using namespace std;

        vector<A> ta;
        vector<B> tb;
        vector<C> tc;
        vector<D> td;

        ta.reserve(n);
        tb.reserve(n);
        tc.reserve(n);
        td.reserve(n);

        for (size_t i = 0; i < n; ++i) {
            A aa;
            B bb;
            C cc;
            D dd;

            if (is >> aa >> bb >> cc >> dd) {
                ta.push_back(aa);
                tb.push_back(bb);
                tc.push_back(cc);
                td.push_back(dd);
            }
        }

        if (is) {
            if (append) {
                a.insert(a.end(), ta.begin(), ta.end());
                b.insert(b.end(), tb.begin(), tb.end());
                c.insert(c.end(), tc.begin(), tc.end());
                d.insert(d.end(), td.begin(), td.end());
            } else {
                a.assign(ta.begin(), ta.end());
                b.assign(tb.begin(), tb.end());
                c.assign(tc.begin(), tc.end());
                d.assign(td.begin(), td.end());
            }
        }
        return is;
    }

    /**
     * Reads five vectors of data from an input stream.
     *
     * The procedure expects that the data for vectors @c a, @c b, c, d and @c e are alternating.
     *
     * @tparam A A data type.
     * @tparam B A data type.
     * @tparam C A data type.
     * @tparam D A data type.
     * @tparam E A data type.
     *
     * @param is The input stream.
     * @param a A data vector.
     * @param b A data vector.
     * @param c A data vector.
     * @param d A data vector.
     * @param e A data vector.
     * @param n The number of data elements to read for each data vector.
     * @param append If @c true, the data read are append to the data vectors.
     *
     * @return the input stream.
     */
    template<class A, class B, class C, class D, class E>
    std::istream &read(std::istream &is, std::vector<A> &a, std::vector<B> &b,
                       std::vector<C> &c, std::vector<D> &d, std::vector<E> &e, size_t n,
                       bool append = false) {
        using namespace std;

        vector<A> ta;
        vector<B> tb;
        vector<C> tc;
        vector<D> td;
        vector<E> te;

        ta.reserve(n);
        tb.reserve(n);
        tc.reserve(n);
        td.reserve(n);
        te.reserve(n);

        for (size_t i = 0; i < n; ++i) {
            A aa;
            B bb;
            C cc;
            D dd;
            E ee;

            if (is >> aa >> bb >> cc >> dd >> ee) {
                ta.push_back(aa);
                tb.push_back(bb);
                tc.push_back(cc);
                td.push_back(dd);
                te.push_back(ee);
            }
        }

        if (is) {
            if (append) {
                a.insert(a.end(), ta.begin(), ta.end());
                b.insert(b.end(), tb.begin(), tb.end());
                c.insert(c.end(), tc.begin(), tc.end());
                d.insert(d.end(), td.begin(), td.end());
                e.insert(e.end(), te.begin(), te.end());
            } else {
                a.assign(ta.begin(), ta.end());
                b.assign(tb.begin(), tb.end());
                c.assign(tc.begin(), tc.end());
                d.assign(td.begin(), td.end());
                e.assign(te.begin(), te.end());
            }
        }
        return is;
    }

    /**
     * Reads four vectors of data and a single vector of text from an input stream.
     *
     * The procedure expects that the data are alternating and text consists of a single word.
     *
     * @tparam A A data type.
     * @tparam B A data type.
     * @tparam C A data type.
     * @tparam D A data type.
     *
     * @param is The input stream.
     * @param a A data vector.
     * @param b A data vector.
     * @param c A data vector.
     * @param d A data vector.
     * @param s A text vector.
     * @param n The number of data elements to read for each vector.
     * @param eol The character marking the end of text (or end of line).
     * @param append If @c true, the data read are append to the vectors.
     *
     * @return the input stream.
     */
    template<class A, class B, class C, class D>
    std::istream &read(std::istream &is, std::vector<A> &a, std::vector<B> &b, std::vector<C> &c, std::vector<D> &d,
                       std::vector<std::string> &s, size_t n, char eol = '\n', bool append = false) {
        using namespace std;

        vector<A> ta;
        vector<B> tb;
        vector<C> tc;
        vector<D> td;
        vector<string> ts;

        ta.reserve(n);
        tb.reserve(n);
        tc.reserve(n);
        td.reserve(n);
        ts.reserve(n);

        for (size_t i = 0; i < n; ++i) {
            A aa;
            B bb;
            C cc;
            D dd;
            string ss;

            if (is >> aa >> bb >> cc >> dd and getline(is, ss, eol)) {
                istringstream ist(ss);

                ist >> ss;
                if (!ist)
                    ss.erase();

                ta.push_back(aa);
                tb.push_back(bb);
                tc.push_back(cc);
                td.push_back(dd);
                ts.push_back(ss);
            }
        }

        if (is) {
            if (append) {
                a.insert(a.end(), ta.begin(), ta.end());
                b.insert(b.end(), tb.begin(), tb.end());
                c.insert(c.end(), tc.begin(), tc.end());
                d.insert(d.end(), td.begin(), td.end());
                s.insert(s.end(), ts.begin(), ts.end());
            } else {
                a.assign(ta.begin(), ta.end());
                b.assign(tb.begin(), tb.end());
                c.assign(tc.begin(), tc.end());
                d.assign(td.begin(), td.end());
                s.assign(ts.begin(), ts.end());
            }
        }
        return is;
    }

    /**
     * Reads a line of text from an input stream.
     *
     * @param is The input stream.
     * @param s The line of text.
     * @param comment_mark The character that marks the begin of comment. If set, empty lines are ignored, too.
     * @param eol The character that marks the end of line.
     *
     * @return the input stream.
     */
    std::istream &readline(std::istream &is, std::string &s, char comment_mark = '\0', char eol = '\n');

}

#endif // ESPECIA_READLINE_H