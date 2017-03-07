/// @file model.h
/// Parametric model for fitting absorption line regions.
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
#ifndef ESPECIA_MODEL_H
#define ESPECIA_MODEL_H

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <valarray>
#include <vector>

#include "config.h"
#include "profiles.h"
#include "readline.h"
#include "section.h"

namespace especia {

    template<class Profile>
    class Model {
    public:

        /**
         * A bounded constraint.
         *
         * @tparam T The number type.
         */
        template<class T = Real_t>
        class Bounded_Constraint {
        public:
            /**
             * Constructs a new strict-bound prior constraint.
             *
             * @param[in] lower_bounds The lower bounds.
             * @param[in] upper_bounds The upper bounds.
             * @param[in] n The number of bounds.
             */
            Bounded_Constraint(const T lower_bounds[], const T upper_bounds[], Nint_t n)
                    : a(lower_bounds, n), b(upper_bounds, n) {
            }

            /**
             * Destructor.
             */
            ~Bounded_Constraint() {
            }

            /**
             * Tests if a given parameter vector violates the constraint.
             *
             * @param[in] x The parameter vector.
             * @param[in] n The number of parameters to test.
             * @return @c true, if the parameter vector violates the constraint.
             */
            bool is_violated(const T x[], Nint_t n) const {
                for (Nint_t i = 0; i < n; ++i) {
                    if (x[i] < a[i] || x[i] > b[i]) {
                        return true;
                    }
                }
                return false;
            }

            /**
             * Computes the cost associated with the constraint.
             *
             * @param[in] x The parameter vector.
             * @param[in] n The number of parameters to take account of.
             * @return always zero.
             */
            T cost(const T x[], Nint_t n) const {
                return T(0);
            }

        private:
            const std::valarray<T> a;
            const std::valarray<T> b;
        };

        std::istream &get(std::istream &is,
                          std::ostream &os,
                          char comment_mark = '%',
                          char begin_of_section = '{',
                          char end_of_section = '}') {
            using namespace std;

            typedef map<string, Nint_t>::const_iterator id_index_map_ci;

            const char errmsg[] = "especia::Model<>::get(): Error: ";
            const char dlimsg[] = "duplicate line identifier";
            const char dsimsg[] = "duplicate section identifier";
            const char fnfmsg[] = "file not found";
            const char infmsg[] = "input failed";
            const char srfmsg[] = "self reference";
            const char synmsg[] = "syntax error";
            const char rnfmsg[] = "reference not found";

            vector<especia::Section> sec;

            vector<Nint_t> isc;
            vector<Nint_t> nle;
            vector<Nint_t> nli;

            vector<Real_t> val;
            vector<Real_t> lo;
            vector<Real_t> up;

            vector<bool> msk;
            vector<Nint_t> ind;

            map<string, Nint_t> pim;
            map<string, Nint_t> sim;

            vector<string> ref;

            stringstream ss;
            stringstream st;
            string line;

            os << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n";
            os << "<html>\n";
            os << "<!--\n";
            os << "<model>\n";

            // Read all lines and write them to standard output.
            while (readline(is, line)) {
                ss << line << endl;
                os << line << endl;
            }

            os << "</model>\n";
            os << "-->\n";
            os << "</html>\n";

            // Now strip empty lines and comments
            while (readline(ss, line, comment_mark)) {
                st << line << '\n';
            }

            Nint_t i = 0;
            Nint_t j = 0;

            while (getline(st, line, end_of_section))
                if (!st.eof()) {
                    j = line.find(begin_of_section);

                    if (j != string::npos) {
                        istringstream ist(line.substr(j + 1));

                        string sid, pid, fn, s2;
                        Real_t a, b;
                        Nint_t p;

                        // Parse section head
                        if (ist >> sid >> fn >> a >> b >> p and getline(ist, s2)) {
                            if (sim.find(sid) == sim.end()) {
                                sim[sid] = sec.size();

                                ifstream ifs(fn.c_str());

                                if (ifs) {
                                    especia::Section s;

                                    if (s.get(ifs, a, b)) {
                                        istringstream is2(s2);
                                        while (is2 >> a >> b)
                                            s.mask(a, b);

                                        sec.push_back(s);
                                        isc.push_back(i);
                                        nle.push_back(p);
                                    } else {
                                        is.setstate(ios_base::badbit | ios_base::failbit);
                                        cerr << errmsg << fn << ": " << infmsg << endl;

                                        return is;
                                    }
                                } else {
                                    is.setstate(ios_base::badbit | ios_base::failbit);
                                    cerr << errmsg << fn << ": " << fnfmsg << endl;

                                    return is;
                                }
                            } else {
                                is.setstate(ios_base::badbit | ios_base::failbit);
                                cerr << errmsg << sid << ": " << dsimsg << endl;

                                return is;
                            }
                        } else {
                            is.setstate(ios_base::badbit | ios_base::failbit);
                            cerr << errmsg << infmsg << endl;

                            return is;
                        }

                        // Read resolution parameter specification
                        if (read(ist, val, lo, up, msk, ref, 1, '\n', true))
                            ++i;
                        else {
                            is.setstate(ios_base::badbit | ios_base::failbit);
                            cerr << errmsg << infmsg << endl;

                            return is;
                        }

                        Nint_t k = 0;

                        // Read profile function parameter specification
                        while (ist >> pid)
                            if (pim.find(pid) == pim.end()) {
                                pim[pid] = i;

                                if (read(ist, val, lo, up, msk, ref, Profile::parameter_count, '\n', true)) {
                                    i += Profile::parameter_count;
                                    k += 1;
                                } else {
                                    is.setstate(ios_base::badbit | ios_base::failbit);
                                    cerr << errmsg << infmsg << endl;

                                    return is;
                                }
                            } else {
                                is.setstate(ios_base::badbit | ios_base::failbit);
                                cerr << errmsg << pid << ": " << dlimsg << endl;

                                return is;
                            }

                        nli.push_back(k);
                    } else {
                        is.setstate(ios_base::badbit | ios_base::failbit);
                        cerr << errmsg << synmsg << endl;

                        return is;
                    }
                }

            if (!st.bad() and st.eof()) {
                // Index independent parameters
                for (Nint_t i = 0, k = 0; i < msk.size(); ++i)
                    if (msk[i] and ref[i].empty()) {
                        if (lo[i] > up[i])
                            swap(lo[i], up[i]);

                        ind.push_back(k++);
                    } else {
                        lo[i] = 0.0;
                        up[i] = 0.0;

                        ind.push_back(0);
                    }

                // Dereference resolution parameter references
                for (id_index_map_ci i = sim.begin(); i != sim.end(); ++i) {
                    const Nint_t j = isc[i->second];

                    while (!ref[j].empty()) {
                        if (sim.find(ref[j]) != sim.end()) {
                            const Nint_t k = isc[sim[ref[j]]];

                            if (j != k) {
                                if (ref[k].empty()) {
                                    val[j] = val[k];

                                    lo[j] = lo[k];
                                    up[j] = up[k];

                                    msk[j] = msk[k];
                                    ind[j] = ind[k];
                                }
                                ref[j] = ref[k];
                            } else {
                                is.setstate(ios_base::failbit);
                                cerr << errmsg << ref[j] << srfmsg << endl;

                                return is;
                            }
                        } else {
                            is.setstate(ios_base::failbit);
                            cerr << errmsg << ref[j] << rnfmsg << endl;

                            return is;
                        }
                    }
                }

                // Dereference line parameter references
                for (id_index_map_ci i = pim.begin(); i != pim.end(); ++i)
                    for (Nint_t j = 0; j < Profile::parameter_count; ++j) {
                        const Nint_t k = i->second + j;

                        while (!ref[k].empty()) {
                            if (pim.find(ref[k]) != pim.end()) {
                                const Nint_t l = pim[ref[k]] + j;

                                if (k != l) {
                                    if (ref[l].empty()) {
                                        val[k] = val[l];

                                        lo[k] = lo[l];
                                        up[k] = up[l];

                                        msk[k] = msk[l];
                                        ind[k] = ind[l];
                                    }
                                    ref[k] = ref[l];
                                } else {
                                    is.setstate(ios_base::failbit);
                                    cerr << errmsg << ref[j] << srfmsg << endl;

                                    return is;
                                }
                            } else {
                                is.setstate(ios_base::failbit);
                                cerr << errmsg << ref[j] << rnfmsg << endl;

                                return is;
                            }
                        }
                    }

                const Nint_t m = sec.size();
                const Nint_t n = msk.size();

                this->sec = sec;

                this->isc.resize(m);
                this->nle.resize(m);
                this->nli.resize(m);

                copy(isc.begin(), isc.end(), &(this->isc[0]));
                copy(nle.begin(), nle.end(), &(this->nle[0]));
                copy(nli.begin(), nli.end(), &(this->nli[0]));

                this->val.resize(n);
                this->err.resize(n);

                copy(val.begin(), val.end(), &(this->val[0]));

                this->lo.resize(n);
                this->up.resize(n);

                copy(lo.begin(), lo.end(), &(this->lo[0]));
                copy(up.begin(), up.end(), &(this->up[0]));

                this->msk.resize(n);
                this->ind.resize(n);

                copy(msk.begin(), msk.end(), &(this->msk[0]));
                copy(ind.begin(), ind.end(), &(this->ind[0]));

                this->sim = sim;
                this->pim = pim;

                is.clear(is.rdstate() & ~ios_base::failbit);
            } else
                is.setstate(ios_base::badbit | ios_base::failbit);

            return is;
        }

        std::ostream &put(std::ostream &os) const {
            using namespace std;

            typedef map<string, Nint_t>::const_iterator id_index_map_ci;

            const ios_base::fmtflags fmt = os.flags();

            os.setf(ios_base::fmtflags());
            os.setf(ios_base::fixed, ios_base::floatfield);
            os.setf(ios_base::left, ios_base::adjustfield);

            os << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n";
            os << "<html>\n";
            os << "<!--\n";
            os << "<data>\n";
            os << sec;
            os << "</data>\n";
            os << "-->\n";

            os << "<head>\n";
            os << "  <title>Parameter Table</title>\n";
            os << "</head>\n";
            os << "<body>\n";
            os << "<table border=\"1\" cellspacing=\"2\" cellpadding=\"2\" width=\"100%\">\n";
            os << "  <thead align=\"center\" valign=\"middle\">\n";
            os << "    <tr>\n";
            os << "      <td>Section</td>\n";
            os << "      <td>Start<br>Wavelength<br>(&Aring;)</td>\n";
            os << "      <td>End<br>Wavelength<br>(&Aring;)</td>\n";
            os << "      <td>Legendre Basis<br>Polynomials</td>\n";
            os << "      <td>Resolution<br>(10<sup>3</sup>)</td>\n";
            os << "      <td>Data Points</td>\n";
            os << "      <td>Cost</td>\n";
            os << "      <td>Cost per<br>Data Point</td>\n";
            os << "    </tr>\n";
            os << "  </thead>\n";
            os << "  <tbody align=\"left\">\n";

            for (id_index_map_ci i = sim.begin(); i != sim.end(); ++i) {
                const Nint_t j = i->second;

                const string id = i->first;
                const Nint_t px = sec[j].valid_data_count();
                const Real_t st = sec[j].cost();

                os.precision(2);

                os << "    <tr>\n";
                os << "      <td>" << id << "</td>\n";
                os << "      <td>" << sec[j].lower_bound() << "</td>\n";
                os << "      <td>" << sec[j].upper_bound() << "</td>\n";
                os << "      <td>" << nle[j] << "</td>\n";
                os << "      <td>";
                put_parameter(os, ios_base::fixed, 2, isc[j]);
                os << "</td>\n";
                os << "      <td>" << px << "</td>\n";
                os << "      <td><strong>" << st << "</strong></td>\n";
                os << "      <td>" << st / px << "</td>\n";
                os << "    </tr>\n";
            }

            os << "  </tbody>\n";
            os << "</table>\n";
            os << "<br>\n";
            os << "<table border=\"1\" cellspacing=\"2\" cellpadding=\"2\" width=\"100%\">\n";
            os << "  <thead align=\"center\" valign=\"middle\">\n";
            os << "    <tr>\n";
            os << "      <td>Line</td>\n";
            os << "      <td>Observed<br>Wavelength<br>(&Aring;)</td>\n";
            os << "      <td>Rest<br>Wavelength<br>(&Aring;)</td>\n";
            os << "      <td>Oscillator<br>Strength</td>\n";
            os << "      <td>Redshift</td>\n";
            os << "      <td>Radial<br>Velocity<br>(km s<sup>-1</sup>)</td>\n";
            os << "      <td>Broadening<br>Velocity<br>(km s<sup>-1</sup>)</td>\n";
            os << "      <td>Log. Column<br>Density<br>(cm<sup>-2</sup>)</td>\n";
#if defined(ESPECIA_MANY_MULTIPLET_ANALYSIS)
            os << "      <td>&Delta;&alpha;/&alpha;<br>(10<sup>-6</sup>)</td>\n";
#endif
            os << "    </tr>\n";
            os << "  </thead>\n";
            os << "  <tbody align=\"left\">\n";

            for (id_index_map_ci i = pim.begin(); i != pim.end(); ++i) {
                const Nint_t j = i->second;
                const string id = i->first;

                const Real_t c = 1.0E-3 * speed_of_light;
                const Real_t x = val[j];
                const Real_t z = val[j + 2];
                const Real_t v = val[j + 3];
                const Real_t w = x * (1.0 + z) * (1.0 + v / c);
                const Real_t dx = err[j];
                const Real_t dz = err[j + 2];
                const Real_t dv = err[j + 3];
                const Real_t dw = dx + x * sqrt(sqr((1.0 + v / c) * dz) + sqr((1.0 + z) * dv / c));

                os.precision(4);

                os << "    <tr>\n";
                os << "      <td>" << id << "</td>\n";
                os << "      <td>" << w << " &plusmn; " << dw << "</td>\n";
                os << "      <td>";
                put_parameter(os, ios_base::fixed, 4, j);
                os << "</td>\n";
                os << "      <td>";
                put_parameter(os, ios_base::scientific, 3, j + 1);
                os << "</td>\n";
                os << "      <td>";
                put_parameter(os, ios_base::fixed, 7, j + 2);
                os << "</td>\n";
                os << "      <td>";
                put_parameter(os, ios_base::fixed, 3, j + 3);
                os << "</td>\n";
                os << "      <td>";
                put_parameter(os, ios_base::fixed, 3, j + 4);
                os << "</td>\n";
                os << "      <td>";
                put_parameter(os, ios_base::fixed, 3, j + 5);
                os << "</td>\n";
#if defined(ESPECIA_MANY_MULTIPLET_ANALYSIS)
                os << "      <td>";
                put_parameter(os, ios_base::fixed, 3, j + 7);
                os << "</td>\n";
#endif
                os << "    </tr>\n";
            }

            os << "  </tbody>\n";
            os << "</table>\n";
            os << "<address>\n";
            os << " Created by Evolutionary spectrum inversion and analysis (Especia).<br>\n";
            os << " " << PROJECT_LONG_NAME << " " << "<a href=\"" << DOI << "\">" << DOI << "</a>" << "<br>\n";
            os << " " << SYSTEM << " " << "<br>\n";
            os << " " << CXX_COMPILER << " " << CXX_COMPILER_VERSION << "<br>\n";
            os << "</address>\n";
            os << "</body>\n";
            os << "</html>\n";

            os.flush();
            os.flags(fmt);

            return os;
        }

        Real_t operator()(const Real_t x[], Nint_t n) const {
            return cost(x, n);
        }

        void set(const Real_t x[], const Real_t z[]) {
            for (Nint_t i = 0; i < val.size(); ++i) {
                if (msk[i]) {
                    val[i] = x[ind[i]];
                    err[i] = z[ind[i]];
                } else {
                    err[i] = 0.0;
                }
            }
            for (Nint_t i = 0; i < sec.size(); ++i) {
                sec[i].apply(Superposition<Profile>(nli[i], &val[isc[i] + 1]), val[isc[i]], nle[i]);
            }
        }

        Real_t cost(const Real_t x[], Nint_t n) const {
            using std::valarray;

            valarray<Real_t> y = val;
            for (Nint_t i = 0; i < y.size(); ++i) {
                if (msk[i]) {
                    y[i] = x[ind[i]];
                }
            }
            Real_t d = 0.0;
            for (Nint_t i = 0; i < sec.size(); ++i) {
                d += sec[i].cost(Superposition<Profile>(nli[i], &y[isc[i] + 1]), y[isc[i]], nle[i]);
            }
            return d;
        }

        Nint_t get_parameter_count() const {
            return ind.max() + 1;
        }

        std::valarray<Real_t> get_initial_parameter_values() const {
            std::valarray<Real_t> x(get_parameter_count());

            for (Nint_t i = 0, j = 0; i < msk.size(); ++i) {
                if (msk[i] and ind[i] == j) {
                    x[j++] = 0.5 * (lo[i] + up[i]);
                }
            }

            return x;
        }

        std::valarray<Real_t> get_initial_local_step_sizes() const {
            std::valarray<Real_t> z(get_parameter_count());

            for (Nint_t i = 0, j = 0; i < msk.size(); ++i) {
                if (msk[i] and ind[i] == j) {
                    z[j++] = 0.5 * (up[i] - lo[i]);
                }
            }

            return z;
        }

        Bounded_Constraint<Real_t> get_constraint() const {
            std::valarray<Real_t> a(get_parameter_count());
            std::valarray<Real_t> b(get_parameter_count());

            for (Nint_t i = 0, j = 0; i < msk.size(); ++i) {
                if (msk[i] and ind[i] == j) {
                    a[j] = lo[i];
                    b[j] = up[i];
                    ++j;
                }
            }

            return Bounded_Constraint<Real_t>(&a[0], &b[0], get_parameter_count());
        }

    private:
        std::ostream &put_parameter(std::ostream &os, std::ios_base::fmtflags f, Nint_t p, Nint_t parameter_index) const {
            using namespace std;

            const ios_base::fmtflags fmt = os.flags();

            os.setf(f, ios_base::floatfield);
            os.precision(p);

            os << val[parameter_index];
            if (msk[parameter_index])
                os << " &plusmn; " << err[parameter_index];

            os.flags(fmt);

            return os;
        }

        std::vector<especia::Section> sec;

        std::valarray<Nint_t> isc;
        std::valarray<Nint_t> nle;
        std::valarray<Nint_t> nli;

        std::valarray<Real_t> val;
        std::valarray<Real_t> err;
        std::valarray<Real_t> lo;
        std::valarray<Real_t> up;

        std::valarray<bool> msk;
        std::valarray<Nint_t> ind;

        std::map<std::string, Nint_t> sim;
        std::map<std::string, Nint_t> pim;
    };

}

#endif // ESPECIA_MODEL_H
