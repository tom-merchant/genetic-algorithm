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
    GraphModal graph( 800, 600, "Fitness" );
    graph.series_colour( "mean", {255, 255, 0} );
    graph.series_colour( "best", {255, 0, 255} );
    auto polling = SDLPolling::get_instance();

    population<2048, Tournament, 2> environment(
            std::make_shared<CountOnesEval<2048>>(),
            std::make_shared<OnePointCrossover<2048>>() );

    DefaultRand rng;

    environment.spawn( 500, rng );

    for ( int i = 0 ; i < 500 ; ++i ){
        std::cout
            << "epoch " << i
            << ". mean fitness " << environment.pop_mean_fitness()
            << ". total fitness " << environment.pop_fitness()
            << ". best fitness " << environment.best_candidate().fitness << std::endl;

        graph.add_point( i, environment.pop_mean_fitness(), "mean" );
        graph.add_point( i, environment.best_candidate().fitness, "best" );

        environment = environment.breed ( 250, 0.001, rng );
        polling->poll();
    }

    while( graph.open )
    {
        polling->block();
    }

    SDL_Quit();

    return 0;
}