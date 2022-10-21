//
// Created by tg2-merchant on 04/10/2022.
//

#pragma once

#include <vector>
#include <cstdint>
#include <array>
#include "rng.h"

enum GeneType{
    BINARY,
    FLOAT
};

union Chromosome {
    uint64_t binary;
    double decimal;
};

union Gene{
    bool binary;
    double decimal;
};

template<size_t genome_size, GeneType type>
class individual {
public:
    double fitness;
    Gene get_gene( size_t n );
    Chromosome get_chromosome( size_t n );
    void set_chromosome( size_t n, Chromosome chromosome );
    individual( Rng& rand_source );
    individual();
    void mutate( double probability, Rng& rand_source, double mut_step );

    size_t gene_count() { return genome_size; }
    size_t chromosome_count() { return chromosomes; }

private:
    static constexpr size_t chromosomes = type == BINARY ? (genome_size + 63)/64 : genome_size;
    static constexpr uint64_t final_chromosome_mask = genome_size % 64 == 0 ? ~0 : (~((uint64_t)0)) << 64 - ( genome_size % 64 );

    std::array<Chromosome, chromosomes> genes;
};

#include "individual.cpp"