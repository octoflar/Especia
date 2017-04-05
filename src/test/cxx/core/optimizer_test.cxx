/// @file optimizer_test.cxx
/// Unit tests
/// Copyright (c) 2017 Ralf Quast
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
#include "../../../main/cxx/core/base.h"
#include "../../../main/cxx/core/optimizer.h"
#include "../unittest.h"

using especia::N_type;
using especia::R_type;
using especia::Optimizer;


class Optimizer_Test : public Unit_Test {
private:

    static R_type sphere(const R_type x[], N_type n) {
        using especia::sq;

        R_type y = 0.0;

        for (N_type i = 0; i < n; ++i) {
            y += sq(x[i]);
        }

        return y;
    }

    static R_type cigar(const R_type x[], N_type n) {
        using especia::sq;

        R_type y = 0.0;

        for (N_type i = 1; i < n; ++i) {
            y += sq(x[i]);
        }

        return 1.0E+06 * y + sq(x[0]);
    }

    static R_type rosenbrock(const R_type x[], N_type n) {
        using especia::sq;

        R_type y = 0.0;

        for (N_type i = 0; i < n - 1; ++i) {
            y += 100.0 * sq(x[i + 1] - sq(x[i])) + sq(1.0 - x[i]);
        }

        return y;
    }

    void before() {
        builder.with_problem_dimension(10).
                with_population_size(40).
                with_parent_number(10).
                with_accuracy_goal(1.0E-06).
                with_random_seed(31415);
    }

    void after() {
        builder.with_defaults();
    }

    void test_minimize_sphere() {
        using especia::No_Constraint;
        using especia::No_Tracing;

        const valarray<R_type> x(1.0, 10);
        const valarray<R_type> d(1.0, 10);
        const R_type s = 1.0;

        const Optimizer optimizer = builder.with_stop_generation(200).build();
        const Optimizer::Result result = optimizer.minimize(sphere, x, d, s);

        assert_true(result.is_optimized(), "test minimize sphere (optimized)");
        assert_false(result.is_underflow(), "test minimize sphere (underflow)");
        assert_equals(0.0, result.get_fitness(), 1.0E-10, "test minimize sphere (fitness)");
        assert_equals(0.0, result.get_parameter_values()[0], 1.0E-06, "test minimize sphere (0)");
        assert_equals(0.0, result.get_parameter_values()[1], 1.0E-06, "test minimize sphere (1)");
        assert_equals(0.0, result.get_parameter_values()[2], 1.0E-06, "test minimize sphere (2)");
        assert_equals(0.0, result.get_parameter_values()[3], 1.0E-06, "test minimize sphere (3)");
        assert_equals(0.0, result.get_parameter_values()[4], 1.0E-06, "test minimize sphere (4)");
        assert_equals(0.0, result.get_parameter_values()[5], 1.0E-06, "test minimize sphere (5)");
        assert_equals(0.0, result.get_parameter_values()[6], 1.0E-06, "test minimize sphere (6)");
        assert_equals(0.0, result.get_parameter_values()[7], 1.0E-06, "test minimize sphere (7)");
        assert_equals(0.0, result.get_parameter_values()[8], 1.0E-06, "test minimize sphere (8)");
        assert_equals(0.0, result.get_parameter_values()[9], 1.0E-06, "test minimize sphere (9)");
    }

    void test_minimize_cigar() {
        using especia::No_Constraint;
        using especia::No_Tracing;

        const valarray<R_type> x(1.0, 10);
        const valarray<R_type> d(1.0, 10);
        const R_type s = 1.0;

        const Optimizer optimizer = builder.with_stop_generation(400).build();
        const Optimizer::Result result = optimizer.minimize(cigar, x, d, s);

        assert_true(result.is_optimized(), "test minimize cigar (optimized)");
        assert_false(result.is_underflow(), "test minimize cigar (underflow)");
        assert_equals(0.0, result.get_fitness(), 1.0E-10, "test minimize cigar (fitness)");
        assert_equals(0.0, result.get_parameter_values()[0], 1.0E-06, "test minimize cigar (0)");
        assert_equals(0.0, result.get_parameter_values()[1], 1.0E-06, "test minimize cigar (1)");
        assert_equals(0.0, result.get_parameter_values()[2], 1.0E-06, "test minimize cigar (2)");
        assert_equals(0.0, result.get_parameter_values()[3], 1.0E-06, "test minimize cigar (3)");
        assert_equals(0.0, result.get_parameter_values()[4], 1.0E-06, "test minimize cigar (4)");
        assert_equals(0.0, result.get_parameter_values()[5], 1.0E-06, "test minimize cigar (5)");
        assert_equals(0.0, result.get_parameter_values()[6], 1.0E-06, "test minimize cigar (6)");
        assert_equals(0.0, result.get_parameter_values()[7], 1.0E-06, "test minimize cigar (7)");
        assert_equals(0.0, result.get_parameter_values()[8], 1.0E-06, "test minimize cigar (8)");
        assert_equals(0.0, result.get_parameter_values()[9], 1.0E-06, "test minimize cigar (9)");
    }

    void test_minimize_rosenbrock() {
        using especia::No_Constraint;
        using especia::No_Tracing;

        const valarray<R_type> x(0.0, 10);
        const valarray<R_type> d(1.0, 10);
        const R_type s = 0.1;

        const Optimizer optimizer = builder.with_stop_generation(400).build();
        const Optimizer::Result result = optimizer.minimize(rosenbrock, x, d, s);

        assert_true(result.is_optimized(), "test minimize Rosenbrock (optimized)");
        assert_false(result.is_underflow(), "test minimize Rosenbrock (underflow)");
        assert_equals(0.0, result.get_fitness(), 1.0E-10, "test minimize Rosenbrock (fitness)");
        assert_equals(1.0, result.get_parameter_values()[0], 1.0E-06, "test minimize Rosenbrock (0)");
        assert_equals(1.0, result.get_parameter_values()[1], 1.0E-06, "test minimize Rosenbrock (1)");
        assert_equals(1.0, result.get_parameter_values()[2], 1.0E-06, "test minimize Rosenbrock (2)");
        assert_equals(1.0, result.get_parameter_values()[3], 1.0E-06, "test minimize Rosenbrock (3)");
        assert_equals(1.0, result.get_parameter_values()[4], 1.0E-06, "test minimize Rosenbrock (4)");
        assert_equals(1.0, result.get_parameter_values()[5], 1.0E-06, "test minimize Rosenbrock (5)");
        assert_equals(1.0, result.get_parameter_values()[6], 1.0E-06, "test minimize Rosenbrock (6)");
        assert_equals(1.0, result.get_parameter_values()[7], 1.0E-06, "test minimize Rosenbrock (7)");
        assert_equals(1.0, result.get_parameter_values()[8], 1.0E-06, "test minimize Rosenbrock (8)");
        assert_equals(1.0, result.get_parameter_values()[9], 1.0E-06, "test minimize Rosenbrock (9)");
    }

    void run_all() {
        run(this, &Optimizer_Test::test_minimize_sphere);
        run(this, &Optimizer_Test::test_minimize_cigar);
        run(this, &Optimizer_Test::test_minimize_rosenbrock);
    }

    Optimizer::Builder builder;
};


int main() {
    return Optimizer_Test().run_testsuite();
}
