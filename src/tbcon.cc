// Copyright (c) 2017-2023, University of Tennessee. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause
// This program is free software: you can redistribute it and/or modify it under
// the terms of the BSD 3-Clause license. See the accompanying LICENSE file.

#include "lapack.hh"
#include "lapack_internal.hh"
#include "lapack/fortran.h"
#include "NoConstructAllocator.hh"

#include <vector>

namespace lapack {

using blas::max;
using blas::min;
using blas::real;

// -----------------------------------------------------------------------------
int64_t tbcon(
    lapack::Norm norm, lapack::Uplo uplo, lapack::Diag diag, int64_t n, int64_t kd,
    float const* AB, int64_t ldab,
    float* rcond )
{
    char norm_ = to_char( norm );
    char uplo_ = to_char( uplo );
    char diag_ = to_char( diag );
    lapack_int n_ = to_lapack_int( n );
    lapack_int kd_ = to_lapack_int( kd );
    lapack_int ldab_ = to_lapack_int( ldab );
    lapack_int info_ = 0;

    // allocate workspace
    lapack::vector< float > work( (3*n) );
    lapack::vector< lapack_int > iwork( (n) );

    LAPACK_stbcon(
        &norm_, &uplo_, &diag_, &n_, &kd_,
        AB, &ldab_, rcond,
        &work[0],
        &iwork[0], &info_
    );
    if (info_ < 0) {
        throw Error();
    }
    return info_;
}

// -----------------------------------------------------------------------------
int64_t tbcon(
    lapack::Norm norm, lapack::Uplo uplo, lapack::Diag diag, int64_t n, int64_t kd,
    double const* AB, int64_t ldab,
    double* rcond )
{
    char norm_ = to_char( norm );
    char uplo_ = to_char( uplo );
    char diag_ = to_char( diag );
    lapack_int n_ = to_lapack_int( n );
    lapack_int kd_ = to_lapack_int( kd );
    lapack_int ldab_ = to_lapack_int( ldab );
    lapack_int info_ = 0;

    // allocate workspace
    lapack::vector< double > work( (3*n) );
    lapack::vector< lapack_int > iwork( (n) );

    LAPACK_dtbcon(
        &norm_, &uplo_, &diag_, &n_, &kd_,
        AB, &ldab_, rcond,
        &work[0],
        &iwork[0], &info_
    );
    if (info_ < 0) {
        throw Error();
    }
    return info_;
}

// -----------------------------------------------------------------------------
int64_t tbcon(
    lapack::Norm norm, lapack::Uplo uplo, lapack::Diag diag, int64_t n, int64_t kd,
    std::complex<float> const* AB, int64_t ldab,
    float* rcond )
{
    char norm_ = to_char( norm );
    char uplo_ = to_char( uplo );
    char diag_ = to_char( diag );
    lapack_int n_ = to_lapack_int( n );
    lapack_int kd_ = to_lapack_int( kd );
    lapack_int ldab_ = to_lapack_int( ldab );
    lapack_int info_ = 0;

    // allocate workspace
    lapack::vector< std::complex<float> > work( (2*n) );
    lapack::vector< float > rwork( (n) );

    LAPACK_ctbcon(
        &norm_, &uplo_, &diag_, &n_, &kd_,
        (lapack_complex_float*) AB, &ldab_, rcond,
        (lapack_complex_float*) &work[0],
        &rwork[0], &info_
    );
    if (info_ < 0) {
        throw Error();
    }
    return info_;
}

// -----------------------------------------------------------------------------
int64_t tbcon(
    lapack::Norm norm, lapack::Uplo uplo, lapack::Diag diag, int64_t n, int64_t kd,
    std::complex<double> const* AB, int64_t ldab,
    double* rcond )
{
    char norm_ = to_char( norm );
    char uplo_ = to_char( uplo );
    char diag_ = to_char( diag );
    lapack_int n_ = to_lapack_int( n );
    lapack_int kd_ = to_lapack_int( kd );
    lapack_int ldab_ = to_lapack_int( ldab );
    lapack_int info_ = 0;

    // allocate workspace
    lapack::vector< std::complex<double> > work( (2*n) );
    lapack::vector< double > rwork( (n) );

    LAPACK_ztbcon(
        &norm_, &uplo_, &diag_, &n_, &kd_,
        (lapack_complex_double*) AB, &ldab_, rcond,
        (lapack_complex_double*) &work[0],
        &rwork[0], &info_
    );
    if (info_ < 0) {
        throw Error();
    }
    return info_;
}

}  // namespace lapack
