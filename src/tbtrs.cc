// Copyright (c) 2017-2023, University of Tennessee. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause
// This program is free software: you can redistribute it and/or modify it under
// the terms of the BSD 3-Clause license. See the accompanying LICENSE file.

#include "lapack.hh"
#include "lapack_internal.hh"
#include "lapack/fortran.h"

#include <vector>

namespace lapack {

using blas::max;
using blas::min;
using blas::real;

// -----------------------------------------------------------------------------
int64_t tbtrs(
    lapack::Uplo uplo, lapack::Op trans, lapack::Diag diag, int64_t n, int64_t kd, int64_t nrhs,
    float const* AB, int64_t ldab,
    float* B, int64_t ldb )
{
    char uplo_ = to_char( uplo );
    char trans_ = to_char( trans );
    char diag_ = to_char( diag );
    lapack_int n_ = to_lapack_int( n );
    lapack_int kd_ = to_lapack_int( kd );
    lapack_int nrhs_ = to_lapack_int( nrhs );
    lapack_int ldab_ = to_lapack_int( ldab );
    lapack_int ldb_ = to_lapack_int( ldb );
    lapack_int info_ = 0;

    LAPACK_stbtrs(
        &uplo_, &trans_, &diag_, &n_, &kd_, &nrhs_,
        AB, &ldab_,
        B, &ldb_, &info_
    );
    if (info_ < 0) {
        throw Error();
    }
    return info_;
}

// -----------------------------------------------------------------------------
int64_t tbtrs(
    lapack::Uplo uplo, lapack::Op trans, lapack::Diag diag, int64_t n, int64_t kd, int64_t nrhs,
    double const* AB, int64_t ldab,
    double* B, int64_t ldb )
{
    char uplo_ = to_char( uplo );
    char trans_ = to_char( trans );
    char diag_ = to_char( diag );
    lapack_int n_ = to_lapack_int( n );
    lapack_int kd_ = to_lapack_int( kd );
    lapack_int nrhs_ = to_lapack_int( nrhs );
    lapack_int ldab_ = to_lapack_int( ldab );
    lapack_int ldb_ = to_lapack_int( ldb );
    lapack_int info_ = 0;

    LAPACK_dtbtrs(
        &uplo_, &trans_, &diag_, &n_, &kd_, &nrhs_,
        AB, &ldab_,
        B, &ldb_, &info_
    );
    if (info_ < 0) {
        throw Error();
    }
    return info_;
}

// -----------------------------------------------------------------------------
int64_t tbtrs(
    lapack::Uplo uplo, lapack::Op trans, lapack::Diag diag, int64_t n, int64_t kd, int64_t nrhs,
    std::complex<float> const* AB, int64_t ldab,
    std::complex<float>* B, int64_t ldb )
{
    char uplo_ = to_char( uplo );
    char trans_ = to_char( trans );
    char diag_ = to_char( diag );
    lapack_int n_ = to_lapack_int( n );
    lapack_int kd_ = to_lapack_int( kd );
    lapack_int nrhs_ = to_lapack_int( nrhs );
    lapack_int ldab_ = to_lapack_int( ldab );
    lapack_int ldb_ = to_lapack_int( ldb );
    lapack_int info_ = 0;

    LAPACK_ctbtrs(
        &uplo_, &trans_, &diag_, &n_, &kd_, &nrhs_,
        (lapack_complex_float*) AB, &ldab_,
        (lapack_complex_float*) B, &ldb_, &info_
    );
    if (info_ < 0) {
        throw Error();
    }
    return info_;
}

// -----------------------------------------------------------------------------
int64_t tbtrs(
    lapack::Uplo uplo, lapack::Op trans, lapack::Diag diag, int64_t n, int64_t kd, int64_t nrhs,
    std::complex<double> const* AB, int64_t ldab,
    std::complex<double>* B, int64_t ldb )
{
    char uplo_ = to_char( uplo );
    char trans_ = to_char( trans );
    char diag_ = to_char( diag );
    lapack_int n_ = to_lapack_int( n );
    lapack_int kd_ = to_lapack_int( kd );
    lapack_int nrhs_ = to_lapack_int( nrhs );
    lapack_int ldab_ = to_lapack_int( ldab );
    lapack_int ldb_ = to_lapack_int( ldb );
    lapack_int info_ = 0;

    LAPACK_ztbtrs(
        &uplo_, &trans_, &diag_, &n_, &kd_, &nrhs_,
        (lapack_complex_double*) AB, &ldab_,
        (lapack_complex_double*) B, &ldb_, &info_
    );
    if (info_ < 0) {
        throw Error();
    }
    return info_;
}

}  // namespace lapack
