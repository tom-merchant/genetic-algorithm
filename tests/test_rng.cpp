//
// Created by tmerchant on 05/10/22.
//

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <cmath>
#include "../rng.h"

double actual_bit_density( double requested_density ) {
    DefaultRand rand;
    auto ones = 0;
    auto total_bits = 0;

    for ( int i = 0 ; i < 100000 ; ++i ){
        ones += std::popcount ( rng_sparse ( rand, requested_density ) );
        total_bits += 64;
    }

    return (double)ones / (double)total_bits;
}

#define EXPECTED_TOLERANCE 0.001

TEST_CASE( "Sparse rand has accurate bit density", "[rng]" ) {
    REQUIRE( actual_bit_density(0) == 0 );
    REQUIRE( actual_bit_density(1) == 1 );
    auto i = GENERATE(0.3, 0.0351, 0.012, 0.0004, 0.3141579);
    REQUIRE( fabs( actual_bit_density( i ) - i ) <= EXPECTED_TOLERANCE );
}