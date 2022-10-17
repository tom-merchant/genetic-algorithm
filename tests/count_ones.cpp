//
// Created by tg2-merchant on 04/10/2022.
//

#include <bit>
#include <iostream>
#include <SDL.h>
#include "../evaluator.h"
#include "../population.h"
#include "../graphing/GraphModal.h"
#include "../graphing/SDLPolling.h"

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

int main( int argc, char *argv[] ) {
    SDL_Init(SDL_INIT_VIDEO);
    GraphModal graph( 800, 800, "Fitness" );
    auto polling = SDLPolling::get_instance();

    population<50, Tournament, 2> environment(
            std::make_shared<CountOnesEval<50>>(),
            std::make_shared<OnePointCrossover<50>>() );

    DefaultRand rng;

    environment.spawn( 50, rng );

    for ( int i = 0 ; i < 50 ; ++i ){
        std::cout
            << "epoch " << i
            << ". mean fitness " << environment.pop_mean_fitness()
            << ". total fitness " << environment.pop_fitness()
            << ". best fitness " << environment.best_candidate().fitness << std::endl;
        environment = environment.breed ( 50, 0.01, rng );
        polling->poll();
    }

    while( graph.open )
    {
        polling->block();
    }

    SDL_Quit();

    return 0;
}