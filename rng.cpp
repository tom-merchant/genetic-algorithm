//
// Created by tg2-merchant on 04/10/2022.
//

#include "rng.h"
#include <ctime>

DefaultRand::DefaultRand() {
    seed = time( NULL );
}

uint64_t DefaultRand::rand() {
    uint64_t z = seed + seed_increment;
    seed = z;

    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
    z = (z ^ (z >> 27)) * 0x94d049bb133111eb;

    return  z ^ (z >> 31);
}

double rnd_double( Rng& rng ) {
    return ((double)rng.rand()) / (double)UINT64_MAX;
}

double uniform_double( Rng& rng, double min, double max ) {
    auto rnd = rnd_double( rng );
    rnd *= max - min;
    return rnd + min;
}

// thanks stackoverflow https://stackoverflow.com/a/64168015
uint64_t rng_sparse( Rng& rng, double bit_density ) {
    auto dividend = static_cast<uint16_t>(bit_density * UINT16_MAX);

    if( dividend == 0 ) { return 0; }
    if( dividend == UINT16_MAX ) { return UINT64_MAX; }

    uint64_t accumulator = 0;

    for ( uint8_t bit_idx = std::countr_zero( dividend ) ; bit_idx != 16 ; ++bit_idx )
    {
        if( ( dividend >> bit_idx ) & 0b1 )
        {
            accumulator |= rng.rand();
        }
        else {
            accumulator &= rng.rand();
        }
    }

    return accumulator;
}