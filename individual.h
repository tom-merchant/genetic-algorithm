//
// Created by tg2-merchant on 04/10/2022.
//

#pragma once

#include <vector>
#include <cstdint>
#include <array>
#include "rng.h"

template<size_t genome_size>
class individual {
public:
    double fitness;
    bool get_gene( size_t n );
    uint64_t get_chromosome( size_t n );
    individual( Rng& rand_source );
    void mutate( double probability, Rng& rand_source );

    size_t gene_count() { return genome_size; }
    size_t chromosome_count() { return chromosomes; }
private:
    static constexpr size_t chromosomes = 1 + genome_size/64;
    static constexpr uint64_t final_chromosome_mask = genome_size % 64 == 0 ? ~0 : ~0 << 64 - ( genome_size % 64 );
    std::array<uint64_t, chromosomes> genes;
};

#include "individual.cpp"