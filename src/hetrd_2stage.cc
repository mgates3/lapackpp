#include "lapack.hh"
#include "lapack_fortran.h"

#if LAPACK_VERSION >= 30700  // >= v3.7

#include <vector>

namespace lapack {

using blas::max;
using blas::min;
using blas::real;

// -----------------------------------------------------------------------------
/// @ingroup heev_computational
int64_t hetrd_2stage(
    lapack::Job jobz, lapack::Uplo uplo, int64_t n,
    std::complex<float>* A, int64_t lda,
    float* D,
    float* E,
    std::complex<float>* tau,
    std::complex<float>* hous2, int64_t lhous2 )
{
    // check for overflow
    if (sizeof(int64_t) > sizeof(blas_int)) {
        lapack_error_if( std::abs(n) > std::numeric_limits<blas_int>::max() );
        lapack_error_if( std::abs(lda) > std::numeric_limits<blas_int>::max() );
        lapack_error_if( std::abs(lhous2) > std::numeric_limits<blas_int>::max() );
    }
    char jobz_ = job2char( jobz );
    char uplo_ = uplo2char( uplo );
    blas_int n_ = (blas_int) n;
    blas_int lda_ = (blas_int) lda;
    blas_int lhous2_ = (blas_int) lhous2;
    blas_int info_ = 0;

    // query for workspace size
    std::complex<float> qry_work[1];
    blas_int ineg_one = -1;
    LAPACK_chetrd_2stage( &jobz_, &uplo_, &n_, A, &lda_, D, E, tau, hous2, &lhous2_, qry_work, &ineg_one, &info_ );
    if (info_ < 0) {
        throw Error();
    }
    blas_int lwork_ = real(qry_work[0]);

    // allocate workspace
    std::vector< std::complex<float> > work( lwork_ );

    LAPACK_chetrd_2stage( &jobz_, &uplo_, &n_, A, &lda_, D, E, tau, hous2, &lhous2_, &work[0], &lwork_, &info_ );
    if (info_ < 0) {
        throw Error();
    }
    return info_;
}

// -----------------------------------------------------------------------------
/// Reduces a Hermitian matrix A to real symmetric
/// tridiagonal form T by a unitary similarity transformation:
/// \f$ Q1^H Q2^H A Q2 Q1 = T \f$.
///
/// Overloaded versions are available for
/// `float`, `double`, `std::complex<float>`, and `std::complex<double>`.
/// For real matrices, this is an alias for `lapack::sytrd_2stage`.
///
/// @param[in] jobz
///     - lapack::Job::NoVec: No need for the Housholder representation,
///         in particular for the second stage (band to
///         tridiagonal).
///     - lapack::Job::Vec: the Householder representation is needed to
///         either generate \f$ Q1 Q2 \f$ or to apply \f$ Q1 Q2 \f$.
///         Not yet available (as of LAPACK 3.8.0).
///
/// @param[in] uplo
///     - lapack::Uplo::Upper: Upper triangle of A is stored;
///     - lapack::Uplo::Lower: Lower triangle of A is stored.
///
/// @param[in] n
///     The order of the matrix A. n >= 0.
///
/// @param[in,out] A
///     The n-by-n matrix A, stored in an lda-by-n array.
///     On entry, the Hermitian matrix A.
///     - If uplo = Upper, the leading
///     n-by-n upper triangular part of A contains the upper
///     triangular part of the matrix A, and the strictly lower
///     triangular part of A is not referenced.
///
///     - If uplo = Lower, the
///     leading n-by-n lower triangular part of A contains the lower
///     triangular part of the matrix A, and the strictly upper
///     triangular part of A is not referenced.
///
///     - On exit, if uplo = Upper, the diagonal and upper band
///     of A are overwritten by the corresponding elements of the
///     internal band-diagonal matrix AB, and the elements above
///     the KD superdiagonal, with the array tau, represent the unitary
///     matrix Q1 as a product of elementary reflectors.
///
///     - On exit, if uplo = Lower, the diagonal and lower band
///     of A are overwritten by the corresponding elements of the
///     internal band-diagonal matrix AB, and the elements below
///     the KD subdiagonal, with the array tau, represent the unitary
///     matrix Q1 as a product of elementary reflectors. See Further Details.
///
/// @param[in] lda
///     The leading dimension of the array A. lda >= max(1,n).
///
/// @param[out] D
///     The vector D of length n.
///     The diagonal elements of the tridiagonal matrix T.
///
/// @param[out] E
///     The vector E of length n-1.
///     The off-diagonal elements of the tridiagonal matrix T.
///
/// @param[out] tau
///     The vector tau of length n-kd.
///     The scalar factors of the elementary reflectors of
///     the first stage (see Further Details).
///
/// @param[out] hous2
///     The vector hous2 of length lhous2.
///     Stores the Householder representation of the stage2
///     band to tridiagonal.
///
/// @param[in] lhous2
///     The dimension of the array hous2.
///     - If lhous2 = -1,
///     then a query is assumed; the routine
///     only calculates the optimal size of the hous2 array, returns
///     this value as the first entry of the hous2 array, and no error
///     message related to lhous2 is issued.
///     - If jobz=NoVec, lhous2 = max(1, 4*n);
///     - if jobz=Vec, option not yet available.
///
/// @retval = 0: successful exit
///
// -----------------------------------------------------------------------------
/// @par Further Details
///
/// Implemented by Azzam Haidar.
///
/// All details are available on technical report, SC11, SC13 papers.
///
/// Azzam Haidar, Hatem Ltaief, and Jack Dongarra.
/// Parallel reduction to condensed forms for symmetric eigenvalue problems
/// using aggregated fine-grained and memory-aware kernels. In Proceedings
/// of 2011 International Conference for High Performance Computing,
/// Networking, Storage and Analysis (SC '11), New York, NY, USA,
/// Article 8, 11 pages.
/// http://doi.acm.org/10.1145/2063384.2063394
///
/// A. Haidar, J. Kurzak, P. Luszczek, 2013.
/// An improved parallel singular value algorithm and its implementation
/// for multicore hardware, In Proceedings of 2013 International Conference
/// for High Performance Computing, Networking, Storage and Analysis (SC '13).
/// Denver, Colorado, USA, 2013.
/// Article 90, 12 pages.
/// http://doi.acm.org/10.1145/2503210.2503292
///
/// A. Haidar, R. Solca, S. Tomov, T. Schulthess and J. Dongarra.
/// A novel hybrid CPU-GPU generalized eigensolver for electronic structure
/// calculations based on fine-grained memory aware tasks.
/// International Journal of High Performance Computing Applications.
/// Volume 28 Issue 2, Pages 196-209, May 2014.
/// http://hpc.sagepub.com/content/28/2/196
///
/// @ingroup heev_computational
int64_t hetrd_2stage(
    lapack::Job jobz, lapack::Uplo uplo, int64_t n,
    std::complex<double>* A, int64_t lda,
    double* D,
    double* E,
    std::complex<double>* tau,
    std::complex<double>* hous2, int64_t lhous2 )
{
    // check for overflow
    if (sizeof(int64_t) > sizeof(blas_int)) {
        lapack_error_if( std::abs(n) > std::numeric_limits<blas_int>::max() );
        lapack_error_if( std::abs(lda) > std::numeric_limits<blas_int>::max() );
        lapack_error_if( std::abs(lhous2) > std::numeric_limits<blas_int>::max() );
    }
    char jobz_ = job2char( jobz );
    char uplo_ = uplo2char( uplo );
    blas_int n_ = (blas_int) n;
    blas_int lda_ = (blas_int) lda;
    blas_int lhous2_ = (blas_int) lhous2;
    blas_int info_ = 0;

    // query for workspace size
    std::complex<double> qry_work[1];
    blas_int ineg_one = -1;
    LAPACK_zhetrd_2stage( &jobz_, &uplo_, &n_, A, &lda_, D, E, tau, hous2, &lhous2_, qry_work, &ineg_one, &info_ );
    if (info_ < 0) {
        throw Error();
    }
    blas_int lwork_ = real(qry_work[0]);

    // allocate workspace
    std::vector< std::complex<double> > work( lwork_ );

    LAPACK_zhetrd_2stage( &jobz_, &uplo_, &n_, A, &lda_, D, E, tau, hous2, &lhous2_, &work[0], &lwork_, &info_ );
    if (info_ < 0) {
        throw Error();
    }
    return info_;
}

}  // namespace lapack

#endif  // LAPACK >= v3.7
