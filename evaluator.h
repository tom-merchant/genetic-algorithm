//
// Created by tg2-merchant on 04/10/2022.
//

#pragma once

#include "individual.h"

template<size_t genome_size, GeneType genome_type>
class evaluator {
public:
    virtual double test( individual<genome_size, genome_type> individual ) = 0;
};