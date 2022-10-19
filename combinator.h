//
// Created by tmerchant on 05/10/22.
//

#pragma once

#include <cstddef>
#include "individual.h"

template<size_t genome_size, size_t num_parents, GeneType genome_type>
class combinator {
public:

    virtual individual<genome_size, genome_type> combine( std::array<individual<genome_size, genome_type>, num_parents> parents, Rng& rand_source ) = 0;
};

template<size_t genome_size>
class OnePointCrossover : public combinator<genome_size, 2, FLOAT>{
public:
    individual<genome_size, FLOAT> combine( std::array<individual<genome_size, FLOAT>, 2> parents, Rng& rand_source ) override;
};

template<size_t genome_size>
individual<genome_size, FLOAT>
OnePointCrossover<genome_size>::combine(std::array<individual<genome_size, FLOAT>, 2> parents, Rng &rand_source) {
    individual<genome_size, FLOAT> child;
    auto crossover_point = static_cast<size_t>( rnd_double (rand_source) * genome_size );

    for ( int i = 0 ; i < parents[0].chromosome_count() ; ++i ) {
        child.set_chromosome( i, ( i < crossover_point ? parents[0] : parents[1] ).get_chromosome(i)  );
    }

    return child;
}

template<size_t genome_size>
class BinOnePointCrossover : public combinator<genome_size, 2, BINARY>{
public:
    individual<genome_size, BINARY> combine( std::array<individual<genome_size, BINARY>, 2> parents, Rng& rand_source ) override;
};

template < size_t genome_size >
individual<genome_size, BINARY> BinOnePointCrossover<genome_size>::combine ( std::array<individual<genome_size, BINARY>, 2> parents, Rng& rand_source ) {
    individual<genome_size, BINARY> child;

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
            new_chromosome.binary &= crossover_mask;
            new_chromosome.binary |= parents[1].get_chromosome ( i ).binary & ~crossover_mask;
        }

        child.set_chromosome ( i, new_chromosome );
    }

    return child;
}

template<size_t genome_size>
class BinUniformCrossover : public combinator<genome_size, 2, BINARY>{
public:
    individual<genome_size, BINARY> combine( std::array<individual<genome_size, BINARY>, 2> parents, Rng& rand_source ) override;
};

template < size_t genome_size >
individual<genome_size, BINARY> BinUniformCrossover<genome_size>::combine (std::array<individual<genome_size, BINARY>, 2> parents, Rng& rand_source ) {
    individual<genome_size, BINARY> child;

    for ( int i = 0 ; i < parents[0].chromosome_count() ; ++i ){
        auto a_genes = parents[0].get_chromosome ( i );
        auto b_genes = parents[1].get_chromosome ( i );
        auto mask = rand_source.rand();

        child.set_chromosome ( i, a_genes & mask | b_genes & ~mask );
    }

    return child;
}