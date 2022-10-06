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
    population<1024, Tournament, 2> environment(
            std::make_shared<CountOnesEval<1024>>(),
            std::make_shared<OnePointCrossover<1024>>() );

    DefaultRand rng;

    environment.spawn( 500, rng );

    for ( int i = 0 ; i < 250 ; ++i ){
        std::cout
            << "epoch " << i
            << ". mean fitness " << environment.pop_mean_fitness()
            << ". total fitness " << environment.pop_fitness()
            << ". best fitness " << environment.best_candidate().fitness << std::endl;
        environment = environment.breed ( 500, 0.001, rng );
    }
}