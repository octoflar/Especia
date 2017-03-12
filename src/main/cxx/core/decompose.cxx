/// @file decompose.cxx
/// Symmetric eigenproblem solvers calling the LAPACK routines.
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
#include <algorithm>

#include "decompose.h"

using std::copy;
using std::invalid_argument;
using std::max;
using std::runtime_error;
using std::string;
using std::swap;
using std::valarray;

using especia::R_type;
using especia::Z_type;


#define LAPACK_NAME_DOUBLE(x) d##x##_
#define LAPACK_NAME_SINGLE(x) s##x##_
#define LAPACK_NAME_R_TYPE(x) LAPACK_NAME_DOUBLE(x)

extern "C" {
R_type LAPACK_NAME_R_TYPE(lamch)(const char &cmach);

void LAPACK_NAME_R_TYPE(syevd)(const char &job,
                               const char &uplo,
                               const Z_type &n,
                               R_type A[],
                               const Z_type &lda,
                               R_type w[],
                               R_type work[],
                               const Z_type &lwork,
                               Z_type iwork[],
                               const Z_type &liwork,
                               Z_type &info);

void LAPACK_NAME_R_TYPE(syevr)(const char &job,
                               const char &range,
                               const char &uplo,
                               const Z_type &n,
                               R_type A[],
                               const Z_type &lda,
                               const R_type &vl,
                               const R_type &vu,
                               const Z_type &il,
                               const Z_type &iu,
                               const R_type &abstol,
                               Z_type &m,
                               R_type w[],
                               R_type Z[],
                               const Z_type &ldz,
                               Z_type isupp[],
                               R_type work[],
                               const Z_type &lwork,
                               Z_type iwork[],
                               const Z_type &liwork,
                               Z_type &info);

void LAPACK_NAME_R_TYPE(syevx)(const char &job,
                               const char &range,
                               const char &uplo,
                               const Z_type &n,
                               R_type A[],
                               const Z_type &lda,
                               const R_type &vl,
                               const R_type &vu,
                               const Z_type &il,
                               const Z_type &iu,
                               const R_type &abstol,
                               Z_type &m,
                               R_type w[],
                               R_type Z[],
                               const Z_type &ldz,
                               R_type work[],
                               const Z_type &lwork,
                               Z_type iwork[],
                               Z_type ifail[],
                               Z_type &info);
}

/**
 * The job parameter (here: compute eigenvalues and eigenvectors).
 */
static const char job = 'V';

/**
 * The range parameter (here: compute all eigenvalues and eigenvectors).
 */
static const char range = 'A';

/**
 * The matrix parameter (here: use the upper triangular part)
 */
static const char uplo = 'U';

/**
 * The lower range limit (here: not used).
 */
static const R_type vl = 0.0;

/**
 * The upper range limit (here: not used).
 */
static const R_type vu = 0.0;

/**
 * The lower range index (here: not used).
 */
static const Z_type il = 0;

/**
 * The upper range index (here: not used).
 */
static const Z_type iu = 0;

/**
 * The minimum positive real number such that its reciprocal does not overflow.
 */
static const R_type safe_minimum = LAPACK_NAME_R_TYPE(lamch)('S');


especia::D_Decompose::D_Decompose(N_type n_in)
        : n(Z_type(n_in)), work(1), iwork(1) {
    allocate_workspace();
}

especia::D_Decompose::~D_Decompose() {
}

void especia::D_Decompose::operator()(const R_type A[], R_type Z[], R_type w[]) const throw(invalid_argument, runtime_error) {
    copy(&A[0], &A[n * n], Z);

    Z_type info = 0;
    LAPACK_NAME_R_TYPE(syevd)(job, uplo, n, &Z[0], n, w, &work[0], lwork, &iwork[0], liwork, info);

    if (info == 0) {
        // To convert from column-major into row-major layout
        transpose(Z);
    } else if (info > 0) {
        throw runtime_error(message_int_err);
    } else {
        throw invalid_argument(message_ill_arg);
    }
}

void especia::D_Decompose::allocate_workspace() {
    Z_type info = 0;

    LAPACK_NAME_R_TYPE(syevd)(job, uplo, n, 0, n, 0, &work[0], -1, &iwork[0], -1, info);

    if (info == 0) {
        lwork = static_cast<Z_type>(work[0]);
        work.resize(static_cast<N_type>(lwork));
        liwork = iwork[0];
        iwork.resize(static_cast<N_type>(liwork));
    } else if (info > 0) {
        throw runtime_error(message_int_err);
    } else {
        throw invalid_argument(message_ill_arg);
    }
}

void especia::D_Decompose::transpose(R_type A[]) const {
    for (Z_type i = 0, i0 = 0; i < n; ++i, i0 += n) {
        for (Z_type j = 0, ij = i0, ji = i; j < i; ++j, ++ij, ji += n) {
            swap(A[ij], A[ji]);
        }
    }
}

const string especia::D_Decompose::message_int_err = "especia::D_Decompose() Error: internal error in LAPACK";
const string especia::D_Decompose::message_ill_arg = "especia::D_Decompose() Error: illegal argument(s) in call to LAPACK";


especia::R_Decompose::R_Decompose(N_type n_in)
        : n(Z_type(n_in)), work(1), iwork(1), isupp(2 * max(N_type(1), n_in)), awork(n_in * n_in) {
    allocate_workspace();
}

especia::R_Decompose::~R_Decompose() {
}

void especia::R_Decompose::operator()(const R_type A[], R_type Z[], R_type w[]) const throw(invalid_argument, runtime_error) {
    copy(&A[0], &A[n * n], &awork[0]);

    Z_type m = 0;
    Z_type info = 0;

    LAPACK_NAME_R_TYPE(syevr)(job, range, uplo, n, &awork[0], n, vl, vu, il, iu, abstol, m, w, Z, n,
                              &isupp[0], &work[0], lwork, &iwork[0], liwork,
                              info);

    if (info == 0) {
        // To convert from column-major into row-major layout
        transpose(Z);
    } else if (info > 0) {
        throw runtime_error(message_int_err);
    } else {
        throw invalid_argument(message_ill_arg);
    }
}

void especia::R_Decompose::allocate_workspace() {
    Z_type m = 0;
    Z_type info = 0;

    LAPACK_NAME_R_TYPE(syevr)(job, range, uplo, n, 0, n, vl, vu, il, iu, abstol, m, 0, 0, n,
                              &isupp[0], &work[0], -1, &iwork[0], -1,
                              info);

    if (info == 0) {
        lwork = static_cast<Z_type>(work[0]);
        work.resize(static_cast<N_type>(lwork));
        liwork = iwork[0];
        iwork.resize(static_cast<N_type>(liwork));
    } else if (info > 0) {
        throw runtime_error(message_int_err);
    } else {
        throw invalid_argument(message_ill_arg);
    }
}

void especia::R_Decompose::transpose(R_type A[]) const {
    for (Z_type i = 0, i0 = 0; i < n; ++i, i0 += n) {
        for (Z_type j = 0, ij = i0, ji = i; j < i; ++j, ++ij, ji += n) {
            swap(A[ij], A[ji]);
        }
    }
}

const R_type especia::R_Decompose::abstol = safe_minimum;
const string especia::R_Decompose::message_int_err = "especia::R_Decompose() Error: internal error in LAPACK";
const string especia::R_Decompose::message_ill_arg = "especia::R_Decompose() Error: illegal argument(s) in call to LAPACK";


especia::X_Decompose::X_Decompose(N_type n_in)
        : n(Z_type(n_in)), work(1), iwork(5 * n_in), ifail(n_in), awork(n_in * n_in) {
    allocate_workspace();
}

especia::X_Decompose::~X_Decompose() {
}

void especia::X_Decompose::operator()(const R_type A[], R_type Z[], R_type w[]) const throw(invalid_argument, runtime_error) {
    copy(&A[0], &A[n * n], &awork[0]);

    Z_type m = 0;
    Z_type info = 0;

    LAPACK_NAME_R_TYPE(syevx)(job, range, uplo, n, &awork[0], n, vl, vu, il, iu, abstol, m, w, Z, n,
                              &work[0], lwork, &iwork[0], &ifail[0],
                              info);

    if (info == 0) {
        // To convert from column-major into row-major layout
        transpose(Z);
    } else if (info > 0) {
        throw runtime_error(message_int_err);
    } else {
        throw invalid_argument(message_ill_arg);
    }
}

void especia::X_Decompose::allocate_workspace() {
    Z_type m = 0;
    Z_type info = 0;

    LAPACK_NAME_R_TYPE(syevx)(job, range, uplo, n, 0, n, vl, vu, il, iu, abstol, m, 0, 0, n,
                              &work[0], -1, &iwork[0], &ifail[0],
                              info);

    if (info == 0) {
        lwork = static_cast<Z_type>(work[0]);
        work.resize(static_cast<N_type>(lwork));
    } else if (info > 0) {
        throw runtime_error(message_int_err);
    } else {
        throw invalid_argument(message_ill_arg);
    }
}

void especia::X_Decompose::transpose(R_type A[]) const {
    for (Z_type i = 0, i0 = 0; i < n; ++i, i0 += n) {
        for (Z_type j = 0, ij = i0, ji = i; j < i; ++j, ++ij, ji += n) {
            swap(A[ij], A[ji]);
        }
    }
}

const R_type especia::X_Decompose::abstol = R_type(2) * safe_minimum;
const string especia::X_Decompose::message_int_err = "especia::X_Decompose() Error: internal error in LAPACK";
const string especia::X_Decompose::message_ill_arg = "especia::X_Decompose() Error: illegal argument(s) in call to LAPACK";

#undef LAPACK_NAME_R_TYPE
#undef LAPACK_NAME_SINGLE
#undef LAPACK_NAME_DOUBLE
