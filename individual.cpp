//
// Created by tg2-merchant on 04/10/2022.
//

#pragma once

template<size_t genome_size>
individual<genome_size>::individual( Rng& rand_source ) {
    for ( int i = 0; i < chromosomes; ++i ) {
        genes[i] = rand_source.rand();
    }

    // mask off the last few bits that aren't valid genes
    genes[ chromosomes - 1 ] &= final_chromosome_mask;
}

template<size_t genome_size>
bool individual<genome_size>::get_gene( size_t n ) {
    auto chromosome = n / 64;
    auto bit = n % 64;

    return ( 0b1 & ( genes[ chromosome ] >> 63 - bit ) ) == 1;
}

template<size_t genome_size>
uint64_t individual<genome_size>::get_chromosome( size_t n ) {
    return genes[ n ];
}

template<size_t genome_size>
void individual<genome_size>::mutate( double probability, Rng& rand_source ) {
    for ( int i = 0 ; i < chromosomes ; ++i ) {
        auto bits_to_mutate = rng_sparse ( rand_source, probability );
        genes[ i ] ^= bits_to_mutate;
    }

    genes[ chromosomes - 1 ] &= final_chromosome_mask;
}
