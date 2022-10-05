//
// Created by tg2-merchant on 04/10/2022.
//

#pragma once

#include <vector>
#include <cstdint>
#include <memory>
#include "individual.h"
#include "evaluator.h"

enum SelectionMethod {
    Tournament,
    Probabilistic
};

template<size_t genome_size, SelectionMethod method>
class population {
public:
    population( std::unique_ptr<evaluator<genome_size>>&& fitness_evaluator );
    void spawn( size_t N, Rng& rand_source );
    individual<genome_size> sample( Rng& rand_source );
    individual<genome_size> best_candidate();
    size_t size();
    double pop_fitness();
    double pop_mean_fitness();
    void add( individual<genome_size> );
private:
    std::unique_ptr<evaluator<genome_size>> fitness_evaluator;
    std::vector<individual<genome_size>> candidates;

    // stores the cumulative probability of the candidate, or a prior candidate being chosen for quick pairing
    std::vector<double> cumulative_probability;

    double population_fitness;
    double mean_fitness;
    double best_fitness;

    void sort_candidates();
    void update_probabilities();
};

#include "population.cpp"