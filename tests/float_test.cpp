//
// Created by tg2-merchant on 19/10/2022.
//

#include <cstddef>
#include <cmath>
#include <iostream>
#include "../individual.h"
#include "../evaluator.h"
#include "SDL.h"
#include "../graphing/GraphModal.h"
#include "../graphing/SDLPolling.h"
#include "../population.h"

template<size_t genome_size>
class FloatTestEval : public evaluator<genome_size, FLOAT> {
public:
    FloatTestEval(){
        this->maximise = false;
    }

    double test( individual<genome_size, FLOAT> individual ) override {
        double fitness = 10 * genome_size;

        for ( int i = 0; i  < genome_size; ++i ) {
            fitness += pow(individual.get_gene( i ).decimal, 2) - (10 * cos( 2 * M_PI * individual.get_gene( i ).decimal ));
        }

        return fitness;
    }
};


int main( int argc, char *argv[] ) {
    SDL_Init(SDL_INIT_VIDEO);
    GraphModal graph( 800, 600, "Fitness" );
    graph.set_origin({1, 0});
    graph.series_colour( "mean", {255, 255, 0} );
    graph.series_colour( "best", {255, 0, 255} );
    auto polling = SDLPolling::get_instance();

    population<20, FLOAT, RouletteWheel, 2> environment(
    std::make_shared<FloatTestEval<20>>(),
            std::make_shared<OnePointCrossover<20>>() );

    DefaultRand rng;

    environment.set_mut_step (10);
    environment.spawn( 50, rng );

    for ( int i = 0 ; i < 50 && graph.open; ++i ){
        std::cout
                << "epoch " << i
                << ". mean fitness " << environment.pop_mean_fitness()
                << ". total fitness " << environment.pop_fitness()
                << ". best fitness " << environment.best_candidate().fitness << std::endl;

        graph.add_point( i+1, environment.pop_mean_fitness(), "mean" );
        graph.add_point( i+1, environment.best_candidate().fitness, "best" );

        environment = environment.breed ( 50, 0.05, rng );
        polling->poll();
    }

    while( graph.open )
    {
        polling->block();
    }

    SDL_Quit();

    return 0;
}