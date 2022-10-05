//
// Created by tmerchant on 05/10/22.
//

#pragma once

#include <cstddef>
#include "individual.h"

template<size_t genome_size, size_t num_parents>
class combinator {
public:
    virtual individual<genome_size> combine( individual<genome_size> parents[num_parents], Rng& rand_source ) = 0;
};

template<size_t genome_size>
class OnePointCrossover : public combinator<genome_size, 2>{
public:
    individual<genome_size> combine( individual<genome_size> parents[2], Rng& rand_source ) override;
};

template < size_t genome_size >
individual<genome_size> OnePointCrossover<genome_size>::combine ( individual<genome_size> parents[2], Rng& rand_source ) {
    individual<genome_size> child;

    auto crossover_point = static_cast<size_t>( rnd_double (rand_source) * genome_size );
    auto crossover_mask = UINT64_MAX << (crossover_point % 64);
    auto crossover_word = crossover_point / 64;

    for ( int i = 0 ; i < parents[0].chromosome_count() ; ++i ){
        auto new_chromosome =
                i <= crossover_word
                ? parents[0].get_chromosome ( i )
                : parents[1].get_chromosome ( i );

        if( i == crossover_word )
        {
            new_chromosome &= crossover_mask;
            new_chromosome |= parents[1].get_chromosome ( i ) & ~crossover_mask;
        }

        child.set_chromosome ( i, new_chromosome );
    }

    return child;
}

template<size_t genome_size>
class UniformCrossover : public combinator<genome_size, 2>{
public:
    individual<genome_size> combine( individual<genome_size> parents[2], Rng& rand_source ) override;
};

template < size_t genome_size >
individual<genome_size> UniformCrossover<genome_size>::combine ( individual<genome_size> parents[2], Rng& rand_source ) {
    individual<genome_size> child;

    for ( int i = 0 ; i < parents[0].chromosome_count() ; ++i ){
        auto a_genes = parents[0].get_chromosome ( i );
        auto b_genes = parents[1].get_chromosome ( i );
        auto mask = rand_source.rand();

        child.set_chromosome ( i, a_genes & mask | b_genes & ~mask );
    }

    return child;
}