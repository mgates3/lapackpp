#include "test.hh"
#include "lapack.hh"
#include "lapack_flops.hh"
#include "print_matrix.hh"
#include "error.hh"
#include "lapacke_wrappers.hh"

#include <vector>

#if LAPACK_VERSION >= 30400  // >= 3.4.0

// -----------------------------------------------------------------------------
template< typename scalar_t >
void test_tprfb_work( Params& params, bool run )
{
    using namespace libtest;
    using namespace blas;
    using real_t = blas::real_type< scalar_t >;
    typedef long long lld;

    // get & mark input values
    lapack::Side side = params.side();
    lapack::Op trans = params.trans();
    lapack::Direct direct = params.direct();
    lapack::StoreV storev = params.storev();
    int64_t m = params.dim.m();
    int64_t n = params.dim.n();
    int64_t k = params.dim.k();
    int64_t l = params.l();
    int64_t align = params.align();

    // mark non-standard output values
    params.ref_time();
    //params.ref_gflops();
    //params.gflops();

    if (! run)
        return;

    if (k < l) {
        printf( "skipping because tprfb requires k >= l\n" );
        return;
    }

    // ---------- setup
    // B is m-by-n
    // V is m-by-k (left,  columnwise)
    //   or n-by-k (right, columnwise)
    //   or k-by-m (left,  rowwise)
    //   or k-by-n (right, rowwise)
    // T is k-by-k
    // A is k-by-n (left)
    //   or m-by-k (right)
    int64_t Vm, Vn;
    if (storev == lapack::StoreV::Columnwise) {
        Vm = (side == Side::Left ? m : n);
        Vn = k;
    }
    else {
        Vm = k;
        Vn = (side == Side::Left ? m : n);
    }
    int64_t Am = (side == Side::Left ? k : m);
    int64_t An = (side == Side::Left ? n : k);
    int64_t ldv = roundup( max( 1, Vm ), align );
    int64_t ldt = roundup( max( 1, k  ), align );
    int64_t lda = roundup( max( 1, Am ), align );
    int64_t ldb = roundup( max( 1, m  ), align );
    size_t size_V = (size_t) ldv * Vn;
    size_t size_T = (size_t) ldt * k;
    size_t size_A = (size_t) lda * An;
    size_t size_B = (size_t) ldb * n;

    std::vector< scalar_t > V( size_V );
    std::vector< scalar_t > T( size_T );
    std::vector< scalar_t > A_tst( size_A );
    std::vector< scalar_t > A_ref( size_A );
    std::vector< scalar_t > B_tst( size_B );
    std::vector< scalar_t > B_ref( size_B );

    int64_t idist = 1;
    int64_t iseed[4] = { 0, 1, 2, 3 };
    lapack::larnv( idist, iseed, V.size(), &V[0] );
    lapack::larnv( idist, iseed, T.size(), &T[0] );
    lapack::larnv( idist, iseed, A_tst.size(), &A_tst[0] );
    lapack::larnv( idist, iseed, B_tst.size(), &B_tst[0] );
    A_ref = A_tst;
    B_ref = B_tst;

    // ---------- run test
    libtest::flush_cache( params.cache() );
    double time = get_wtime();
    lapack::tprfb( side, trans, direct, storev, m, n, k, l, &V[0], ldv, &T[0], ldt, &A_tst[0], lda, &B_tst[0], ldb );
    time = get_wtime() - time;
    // internal routine: no argument check so no info.
    //if (info_tst != 0) {
    //    fprintf( stderr, "lapack::tprfb returned error %lld\n", (lld) info_tst );
    //}

    params.time() = time;
    //double gflop = lapack::Gflop< scalar_t >::larfb( side, trans, direct, storev, m, n, k );
    //params.gflops() = gflop / time;

    if (params.ref() == 'y' || params.check() == 'y') {
        // ---------- run reference
        libtest::flush_cache( params.cache() );
        time = get_wtime();
        int64_t info_ref = LAPACKE_tprfb( side2char(side), op2char(trans), direct2char(direct), storev2char(storev), m, n, k, l, &V[0], ldv, &T[0], ldt, &A_ref[0], lda, &B_ref[0], ldb );
        time = get_wtime() - time;
        if (info_ref != 0) {
            fprintf( stderr, "LAPACKE_tprfb returned error %lld\n", (lld) info_ref );
        }

        params.ref_time() = time;
        //params.ref_gflops() = gflop / time;

        // ---------- check error compared to reference
        real_t error = 0;
        //if (info_tst != info_ref) {
        //    error = 1;
        //}
        error += abs_error( A_tst, A_ref );
        error += abs_error( B_tst, B_ref );
        params.error() = error;
        params.okay() = (error == 0);  // expect lapackpp == lapacke
    }
}

// -----------------------------------------------------------------------------
void test_tprfb( Params& params, bool run )
{
    switch (params.datatype()) {
        case libtest::DataType::Integer:
            throw std::exception();
            break;

        case libtest::DataType::Single:
            test_tprfb_work< float >( params, run );
            break;

        case libtest::DataType::Double:
            test_tprfb_work< double >( params, run );
            break;

        case libtest::DataType::SingleComplex:
            test_tprfb_work< std::complex<float> >( params, run );
            break;

        case libtest::DataType::DoubleComplex:
            test_tprfb_work< std::complex<double> >( params, run );
            break;
    }
}

#else

// -----------------------------------------------------------------------------
void test_tprfb( Params& params, bool run )
{
    fprintf( stderr, "tprfb requires LAPACK >= 3.4.0\n\n" );
    exit(0);
}

#endif  // LAPACK >= 3.4.0