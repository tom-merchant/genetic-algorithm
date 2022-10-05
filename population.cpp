//
// Created by tg2-merchant on 04/10/2022.
//

#pragma once
#include <limits>
#include <random>

template<size_t genome_size, SelectionMethod method>
void population<genome_size, method>::spawn( size_t N, Rng& rand_source ) {

    for (int i = 0; i < N; ++i) {
        individual<genome_size> candidate( rand_source );
        candidate.fitness = fitness_evaluator->test( candidate );
        candidates.push_back( candidate );
        cumulative_probability.push_back( 0 );
    }

    sort_candidates();
    update_probabilities();
}

template<size_t genome_size, SelectionMethod method>
void population<genome_size, method>::sort_candidates() {
    std::sort (
        candidates.begin(), candidates.end(),
        []( auto a, auto b ){
            return a.fitness > b.fitness;
        }
    );
}

template<size_t genome_size, SelectionMethod method>
population<genome_size, method>::population( std::unique_ptr<evaluator<genome_size>>&& eval )
    : fitness_evaluator( std::move( eval ) )
{
}

template<size_t genome_size, SelectionMethod method>
size_t population<genome_size, method>::size() {
    return candidates.size();
}

template<size_t genome_size, SelectionMethod method>
void population<genome_size, method>::update_probabilities() {
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

template<size_t genome_size, SelectionMethod method>
individual<genome_size> population<genome_size, method>::sample( Rng& rand_source ) {

    if( method == Tournament )
    {
        auto contestants = {rnd_double( rand_source ), rnd_double( rand_source )};
        return candidates[ min(contestants) ];
    }
    else if( method == Probabilistic )
    {
        auto rand = rnd_double( rand_source );
        auto interval = {0, candidates.size()};

        while ( interval[0] != interval[1] ) {
            auto mid = ( interval[0] + interval[1] ) / 2;
            if( cumulative_probability[mid] >= rand )
            {
                interval[1] = mid;
                continue;
            }
            interval[0] = mid;
        }
        return candidates[ interval[1] ];
    }
}

template<size_t genome_size, SelectionMethod method>
individual<genome_size> population<genome_size, method>::best_candidate() {
    return candidates[0];
}

template<size_t genome_size, SelectionMethod method>
double population<genome_size, method>::pop_fitness() {
    return population_fitness;
}

template<size_t genome_size, SelectionMethod method>
double population<genome_size, method>::pop_mean_fitness() {
    return mean_fitness;
}

template<size_t genome_size, SelectionMethod method>
void population<genome_size, method>::add( individual<genome_size> candidate ) {
    candidate.fitness = fitness_evaluator->test( candidate );
    candidates.push_back( candidate );

    sort_candidates();
    update_probabilities();
}
