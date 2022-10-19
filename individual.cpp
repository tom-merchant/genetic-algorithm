//
// Created by tg2-merchant on 04/10/2022.
//

#pragma once

template<size_t genome_size, GeneType genome_type>
individual<genome_size, genome_type>::individual( Rng& rand_source ) : fitness( 0 ) {
    for ( int i = 0; i < chromosomes; ++i ) {
        genes[i] = genome_type == BINARY
                ? Chromosome{ rand_source.rand() }
                : Chromosome{ .decimal = rnd_double( rand_source ) };
    }

    if( genome_type == BINARY )
    {
        // mask off the last few bits that aren't valid genes
        genes[ chromosomes - 1 ].binary &= final_chromosome_mask;
    }
}

template<size_t genome_size, GeneType genome_type>
Gene individual<genome_size, genome_type>::get_gene( size_t n ) {
    if( genome_type == FLOAT )
    {
        return Gene{ .decimal = get_chromosome( n ).decimal };
    }
    auto chromosome = n / 64;
    auto bit = n % 64;

    return Gene{ ( 0b1 & ( genes[ chromosome ].binary >> 63 - bit ) ) == 1 };
}

template<size_t genome_size, GeneType genome_type>
Chromosome individual<genome_size, genome_type>::get_chromosome( size_t n ) {
    return genes[ n ];
}

template<size_t genome_size, GeneType genome_type>
void individual<genome_size, genome_type>::mutate( double probability, Rng& rand_source ) {

    switch (genome_type) {
        case BINARY:
            for ( int i = 0 ; i < chromosomes ; ++i ) {
                auto bits_to_mutate = rng_sparse ( rand_source, probability );
                genes[ i ].binary ^= bits_to_mutate;
            }

            genes[ chromosomes - 1 ].binary &= final_chromosome_mask;
            break;
        case FLOAT:
            uint64_t mutation_decisions;
            for( int i = 0; i < chromosomes; i++ )
            {
                if( i % 64 == 0 ) {
                    mutation_decisions = rng_sparse( rand_source, probability );
                }

                if( ( mutation_decisions >> i ) & 0b1 ) {
                    auto mut_amt = 2 * mut_step * ( rnd_double( rand_source ) - 0.5 );
                    genes[ i ].decimal += mut_amt;
                }
            }
            break;
    }
}

template<size_t genome_size, GeneType genome_type>
void individual<genome_size, genome_type>::set_chromosome ( size_t n, Chromosome chromosome ){
    genes[ n ] = chromosome;

    if( genome_type == BINARY && n == chromosomes - 1 )
    {
        genes[ n ].binary &= final_chromosome_mask;
    }
}


template<size_t genome_size, GeneType genome_type>
individual<genome_size, genome_type>::individual ( ) : fitness( 0 ){
}

template<size_t genome_size, GeneType type>
void individual<genome_size, type>::set_mutation_step( double mut_step ) {
    this->mut_step = mut_step;
}
