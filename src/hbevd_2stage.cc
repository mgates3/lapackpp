#include "lapack.hh"
#include "lapack_fortran.h"

#if LAPACK_VERSION >= 30700  // >= 3.7

#include <vector>

namespace lapack {

using blas::max;
using blas::min;
using blas::real;

// -----------------------------------------------------------------------------
int64_t hbevd_2stage(
    lapack::Job jobz, lapack::Uplo uplo, int64_t n, int64_t kd,
    std::complex<float>* AB, int64_t ldab,
    float* W,
    std::complex<float>* Z, int64_t ldz )
{
    // check for overflow
    if (sizeof(int64_t) > sizeof(blas_int)) {
        lapack_error_if( std::abs(n) > std::numeric_limits<blas_int>::max() );
        lapack_error_if( std::abs(kd) > std::numeric_limits<blas_int>::max() );
        lapack_error_if( std::abs(ldab) > std::numeric_limits<blas_int>::max() );
        lapack_error_if( std::abs(ldz) > std::numeric_limits<blas_int>::max() );
    }
    char jobz_ = job2char( jobz );
    char uplo_ = uplo2char( uplo );
    blas_int n_ = (blas_int) n;
    blas_int kd_ = (blas_int) kd;
    blas_int ldab_ = (blas_int) ldab;
    blas_int ldz_ = (blas_int) ldz;
    blas_int info_ = 0;

    // query for workspace size
    std::complex<float> qry_work[1];
    float qry_rwork[1];
    blas_int qry_iwork[1];
    blas_int ineg_one = -1;
    LAPACK_chbevd_2stage( &jobz_, &uplo_, &n_, &kd_, AB, &ldab_, W, Z, &ldz_, qry_work, &ineg_one, qry_rwork, &ineg_one, qry_iwork, &ineg_one, &info_ );
    if (info_ < 0) {
        throw Error();
    }
    blas_int lwork_ = real(qry_work[0]);
    blas_int lrwork_ = real(qry_rwork[0]);
    blas_int liwork_ = real(qry_iwork[0]);

    // allocate workspace
    std::vector< std::complex<float> > work( lwork_ );
    std::vector< float > rwork( lrwork_ );
    std::vector< blas_int > iwork( liwork_ );

    LAPACK_chbevd_2stage( &jobz_, &uplo_, &n_, &kd_, AB, &ldab_, W, Z, &ldz_, &work[0], &lwork_, &rwork[0], &lrwork_, &iwork[0], &liwork_, &info_ );
    if (info_ < 0) {
        throw Error();
    }
    return info_;
}

// -----------------------------------------------------------------------------
int64_t hbevd_2stage(
    lapack::Job jobz, lapack::Uplo uplo, int64_t n, int64_t kd,
    std::complex<double>* AB, int64_t ldab,
    double* W,
    std::complex<double>* Z, int64_t ldz )
{
    // check for overflow
    if (sizeof(int64_t) > sizeof(blas_int)) {
        lapack_error_if( std::abs(n) > std::numeric_limits<blas_int>::max() );
        lapack_error_if( std::abs(kd) > std::numeric_limits<blas_int>::max() );
        lapack_error_if( std::abs(ldab) > std::numeric_limits<blas_int>::max() );
        lapack_error_if( std::abs(ldz) > std::numeric_limits<blas_int>::max() );
    }
    char jobz_ = job2char( jobz );
    char uplo_ = uplo2char( uplo );
    blas_int n_ = (blas_int) n;
    blas_int kd_ = (blas_int) kd;
    blas_int ldab_ = (blas_int) ldab;
    blas_int ldz_ = (blas_int) ldz;
    blas_int info_ = 0;

    // query for workspace size
    std::complex<double> qry_work[1];
    double qry_rwork[1];
    blas_int qry_iwork[1];
    blas_int ineg_one = -1;
    LAPACK_zhbevd_2stage( &jobz_, &uplo_, &n_, &kd_, AB, &ldab_, W, Z, &ldz_, qry_work, &ineg_one, qry_rwork, &ineg_one, qry_iwork, &ineg_one, &info_ );
    if (info_ < 0) {
        throw Error();
    }
    blas_int lwork_ = real(qry_work[0]);
    blas_int lrwork_ = real(qry_rwork[0]);
    blas_int liwork_ = real(qry_iwork[0]);

    // allocate workspace
    std::vector< std::complex<double> > work( lwork_ );
    std::vector< double > rwork( lrwork_ );
    std::vector< blas_int > iwork( liwork_ );

    LAPACK_zhbevd_2stage( &jobz_, &uplo_, &n_, &kd_, AB, &ldab_, W, Z, &ldz_, &work[0], &lwork_, &rwork[0], &lrwork_, &iwork[0], &liwork_, &info_ );
    if (info_ < 0) {
        throw Error();
    }
    return info_;
}

}  // namespace lapack

#endif  // LAPACK >= 3.7
