//
// Created by tg2-merchant on 04/10/2022.
//

#include <bit>
#include <iostream>
#include "../evaluator.h"
#include "../population.h"

template<size_t genome_size>
class CountOnesEval : public evaluator<genome_size> {
    double test( individual<genome_size> individual ) override {
        auto ones = 0;

        for ( int i = 0; i < individual.chromosome_count(); ++i ) {
            ones += std::popcount( individual.get_chromosome( i ) );
        }

        return (double)ones;
    }
};

int main() {
    population<1024, Probabilistic> environment( std::make_unique<CountOnesEval<1024>>() );
    DefaultRand rng;

    environment.spawn( 512, rng );

    std::cout << environment.pop_mean_fitness() << std::endl;

    return 0;
}