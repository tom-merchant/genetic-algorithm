//
// Created by tg2-merchant on 04/10/2022.
//

#pragma once

#include <vector>
#include <cstdint>
#include <memory>
#include "individual.h"
#include "evaluator.h"
#include "combinator.h"

enum SelectionMethod {
    Tournament,
    Probabilistic
};

template<size_t genome_size, GeneType genome_type, SelectionMethod method, size_t num_parents>
class population {
public:
    population(
            std::shared_ptr<evaluator<genome_size, genome_type>> fitness_evaluator,
            std::shared_ptr<combinator<genome_size, num_parents, genome_type>> recombinator );
    void spawn( size_t N, Rng& rand_source );
    individual<genome_size, genome_type> sample( Rng& rand_source );
    individual<genome_size, genome_type> best_candidate();
    size_t size();
    double pop_fitness();
    double pop_mean_fitness();
    void add( individual<genome_size, genome_type> );
    population<genome_size, genome_type, method, num_parents> breed( size_t generation_size, double mutation_rate, Rng& rand_source );
private:
    std::shared_ptr<evaluator<genome_size, genome_type>> fitness_evaluator;
    std::shared_ptr<combinator<genome_size, num_parents, genome_type>> recombinator;
    std::vector<individual<genome_size, genome_type>> candidates;

    // stores the cumulative probability of the candidate, or a prior candidate being chosen for quick pairing
    std::vector<double> cumulative_probability;

    double population_fitness;
    double mean_fitness;
    double best_fitness;

    void add_unordered( individual<genome_size, genome_type> );
    void sort_candidates();
    void update_probabilities();
};

#include "population.cpp"