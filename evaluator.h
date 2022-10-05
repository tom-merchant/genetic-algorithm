//
// Created by tg2-merchant on 04/10/2022.
//

#pragma once

#include "individual.h"

template<size_t genome_size>
class evaluator {
public:
    virtual double test( individual<genome_size> individual ) = 0;
};