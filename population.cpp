//
// Created by tg2-merchant on 04/10/2022.
//

#pragma once
#include <limits>
#include <random>

template<size_t genome_size, GeneType genome_type, SelectionMethod method, size_t num_parents>
void population<genome_size, genome_type, method, num_parents>::spawn( size_t N, Rng& rand_source ) {

    for (int i = 0; i < N; ++i) {
        individual<genome_size, genome_type> candidate( rand_source );
        candidate.fitness = fitness_evaluator->test( candidate );
        candidates.push_back( candidate );
        cumulative_probability.push_back( 0 );
    }

    sort_candidates();
    update_probabilities();
}

template<size_t genome_size, GeneType genome_type, SelectionMethod method, size_t num_parents>
void population<genome_size, genome_type, method, num_parents>::sort_candidates() {
    std::sort (
        candidates.begin(), candidates.end(),
        [ this ]( auto a, auto b ){
            if( fitness_evaluator->maximise )
            {
                return a.fitness > b.fitness;
            }
            else
            {
                return a.fitness < b.fitness;
            }
        }
    );
}

template<size_t genome_size, GeneType genome_type, SelectionMethod method, size_t num_parents>
population<genome_size, genome_type, method, num_parents>::population(
        std::shared_ptr<evaluator<genome_size, genome_type>> eval,
        std::shared_ptr<combinator<genome_size, num_parents, genome_type>> recombinator )
    : fitness_evaluator( eval ), recombinator( recombinator )
{
}

template<size_t genome_size, GeneType genome_type, SelectionMethod method, size_t num_parents>
size_t population<genome_size, genome_type, method, num_parents>::size() {
    return candidates.size();
}

template<size_t genome_size, GeneType genome_type, SelectionMethod method, size_t num_parents>
void population<genome_size, genome_type, method, num_parents>::update_probabilities() {
    population_fitness = 0;
    best_fitness = std::numeric_limits<double>::infinity() * -1;

    for ( auto candidate : candidates ) {
        population_fitness += candidate.fitness;
        best_fitness = std::max( best_fitness, candidate.fitness );
    }

    mean_fitness = population_fitness / candidates.size();

    if( method == RouletteWheel )
    {
        cumulative_probability[0] = candidates[0].fitness / population_fitness;

        if( fitness_evaluator->maximise == false )
        {
            cumulative_probability[0] = 1 - cumulative_probability[0];
        }

        for ( int i = 1; i < candidates.size(); ++i ) {
            if( fitness_evaluator-> maximise )
            {
                cumulative_probability[i] = cumulative_probability[i - 1] + candidates[i].fitness / population_fitness;
            } else {
                cumulative_probability[i] = cumulative_probability[i - 1] + 1 - (candidates[i].fitness / population_fitness);
            }
        }
    }
}

template<size_t genome_size, GeneType genome_type, SelectionMethod method, size_t num_parents>
individual<genome_size, genome_type> population<genome_size, genome_type, method, num_parents>::sample( Rng& rand_source ) {

    if( method == Tournament )
    {
        auto contestants = {rnd_double( rand_source ), rnd_double( rand_source )};
        return candidates[ int( min(contestants) * (double)candidates.size() ) ];
    }
    else if( method == RouletteWheel )
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

template<size_t genome_size, GeneType genome_type, SelectionMethod method, size_t num_parents>
individual<genome_size, genome_type> population<genome_size, genome_type, method, num_parents>::best_candidate() {
    return candidates[0];
}

template<size_t genome_size, GeneType genome_type, SelectionMethod method, size_t num_parents>
double population<genome_size, genome_type, method, num_parents>::pop_fitness() {
    return population_fitness;
}

template<size_t genome_size, GeneType genome_type, SelectionMethod method, size_t num_parents>
double population<genome_size, genome_type, method, num_parents>::pop_mean_fitness() {
    return mean_fitness;
}

template<size_t genome_size, GeneType genome_type, SelectionMethod method, size_t num_parents>
void population<genome_size, genome_type, method, num_parents>::add( individual<genome_size, genome_type> candidate ) {
    candidate.fitness = fitness_evaluator->test( candidate );
    candidates.push_back( candidate );

    sort_candidates();
    update_probabilities();
}

template<size_t genome_size, GeneType genome_type, SelectionMethod method, size_t num_parents>
population<genome_size, genome_type, method, num_parents> population<genome_size, genome_type, method, num_parents>::breed (
        size_t generation_size, double mutation_rate, Rng& rand_source )
{
    std::array<individual<genome_size, genome_type>, num_parents> parents;
    population<genome_size, genome_type, method, num_parents> new_pop( fitness_evaluator, recombinator );

    for ( auto i = 0 ; i < generation_size ; ++i ){
        for ( auto j = 0 ; j < num_parents ; ++j ){
            parents[j] = sample ( rand_source );
        }

        individual<genome_size, genome_type> child = recombinator->combine( parents, rand_source );
        child.mutate ( mutation_rate, rand_source, mutation_step );
        new_pop.add_unordered ( child );
    }

    new_pop.candidates.erase( new_pop.candidates.end() - 1 );
    new_pop.candidates.insert( new_pop.candidates.begin(), best_candidate() );

    new_pop.sort_candidates();
    new_pop.update_probabilities();

    return new_pop;
}

template<size_t genome_size, GeneType genome_type, SelectionMethod method, size_t num_parents>
void population<genome_size, genome_type, method, num_parents>::add_unordered ( individual<genome_size, genome_type> candidate ){
    candidate.fitness = fitness_evaluator->test( candidate );
    candidates.push_back( candidate );
    cumulative_probability.push_back( 0 );
}

template < size_t genome_size, GeneType genome_type, SelectionMethod method, size_t num_parents >
void population<genome_size, genome_type, method, num_parents>::set_mut_step ( double mutation_step ){
    this->mutation_step = mutation_step;
}
