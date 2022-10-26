//
// Created by tg2-merchant on 04/10/2022.
//

#pragma once

#include <cstdint>
#include <bit>
#include <cstddef>

class Rng {
public:
    virtual uint64_t rand() = 0;
};

class DefaultRand : public Rng {
public:
    DefaultRand();
    uint64_t rand() override;
private:
    uint64_t seed;
    static const uint64_t seed_increment = 0x9E3779B97F4A7C15;
};

double rnd_double( Rng& rng );
uint64_t rng_sparse( Rng& rng, double bit_density );
double uniform_double( Rng& rng, double min, double max );