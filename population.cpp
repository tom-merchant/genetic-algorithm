//
// Created by tg2-merchant on 04/10/2022.
//

#pragma once
#include <limits>
#include <random>

template<size_t genome_size, SelectionMethod method, size_t num_parents>
void population<genome_size, method, num_parents>::spawn( size_t N, Rng& rand_source ) {

    for (int i = 0; i < N; ++i) {
        individual<genome_size> candidate( rand_source );
        candidate.fitness = fitness_evaluator->test( candidate );
        candidates.push_back( candidate );
        cumulative_probability.push_back( 0 );
    }

    sort_candidates();
    update_probabilities();
}

template<size_t genome_size, SelectionMethod method, size_t num_parents>
void population<genome_size, method, num_parents>::sort_candidates() {
    std::sort (
        candidates.begin(), candidates.end(),
        []( auto a, auto b ){
            return a.fitness > b.fitness;
        }
    );
}

template<size_t genome_size, SelectionMethod method, size_t num_parents>
population<genome_size, method, num_parents>::population(
        std::shared_ptr<evaluator<genome_size>> eval,
        std::shared_ptr<combinator<genome_size, num_parents>> recombinator )
    : fitness_evaluator( eval ), recombinator( recombinator )
{
}

template<size_t genome_size, SelectionMethod method, size_t num_parents>
size_t population<genome_size, method, num_parents>::size() {
    return candidates.size();
}

template<size_t genome_size, SelectionMethod method, size_t num_parents>
void population<genome_size, method, num_parents>::update_probabilities() {
    population_fitness = 0;
    best_fitness = std::numeric_limits<double>::infinity() * -1;

    for ( auto candidate : candidates ) {
        population_fitness += candidate.fitness;
        best_fitness = std::max( best_fitness, candidate.fitness );
    }

    mean_fitness = population_fitness / candidates.size();

    if( method == Probabilistic )
    {
        cumulative_probability[0] = candidates[0].fitness / population_fitness;

        for ( int i = 1; i < candidates.size(); ++i ) {
            cumulative_probability[i] = cumulative_probability[i - 1] + candidates[i].fitness / population_fitness;
        }
    }
}

template<size_t genome_size, SelectionMethod method, size_t num_parents>
individual<genome_size> population<genome_size, method, num_parents>::sample( Rng& rand_source ) {

    if( method == Tournament )
    {
        auto contestants = {rnd_double( rand_source ), rnd_double( rand_source )};
        return candidates[ min(contestants) ];
    }
    else if( method == Probabilistic )
    {
        auto rand = rnd_double( rand_source );
        size_t interval[2] = {0, candidates.size()};

        while ( interval[1] - interval[0] > 1 ) {
            auto mid = ( interval[0] + interval[1] ) / 2;
            if( cumulative_probability[mid] >= rand )
            {
                interval[1] = mid;
                continue;
            }
            interval[0] = mid;
        }
        return candidates[ interval[0] ];
    }
}

template<size_t genome_size, SelectionMethod method, size_t num_parents>
individual<genome_size> population<genome_size, method, num_parents>::best_candidate() {
    return candidates[0];
}

template<size_t genome_size, SelectionMethod method, size_t num_parents>
double population<genome_size, method, num_parents>::pop_fitness() {
    return population_fitness;
}

template<size_t genome_size, SelectionMethod method, size_t num_parents>
double population<genome_size, method, num_parents>::pop_mean_fitness() {
    return mean_fitness;
}

template<size_t genome_size, SelectionMethod method, size_t num_parents>
void population<genome_size, method, num_parents>::add( individual<genome_size> candidate ) {
    candidate.fitness = fitness_evaluator->test( candidate );
    candidates.push_back( candidate );

    sort_candidates();
    update_probabilities();
}

template < size_t genome_size, SelectionMethod method, size_t num_parents >
population<genome_size, method, num_parents> population<genome_size, method, num_parents>::breed (
        size_t generation_size, double mutation_rate, Rng& rand_source )
{
    individual<genome_size> parents[num_parents];
    population<genome_size, method, num_parents> new_pop( fitness_evaluator, recombinator );

    for ( auto i = 0 ; i < generation_size ; ++i ){
        for ( auto j = 0 ; j < num_parents ; ++j ){
            parents[j] = sample ( rand_source );
        }

        individual<genome_size> child = recombinator->combine( parents, rand_source );
        child.mutate ( mutation_rate, rand_source );
        new_pop.add_unordered ( child );
    }

    new_pop.sort_candidates();
    new_pop.update_probabilities();

    return new_pop;
}

template < size_t genome_size, SelectionMethod method, size_t num_parents >
void population<genome_size, method, num_parents>::add_unordered ( individual<genome_size> candidate ){
    candidate.fitness = fitness_evaluator->test( candidate );
    candidates.push_back( candidate );
    cumulative_probability.push_back( 0 );
}
