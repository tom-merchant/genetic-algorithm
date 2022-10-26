//
// Created by tg2-merchant on 26/10/2022.
//

#pragma once

#include <cstddef>
#include <array>

template<const size_t genome_size>
struct float_gene_map {
public:
    std::array<double, genome_size> min;
    std::array<double, genome_size> max;
};
