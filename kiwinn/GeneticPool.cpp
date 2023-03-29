#include "GeneticPool.h"
#include "./utils.hpp"

#include <stdio.h>
#include <algorithm>
#include <iostream>

GeneticPool::GeneticPool()
{
}

GeneticPool::~GeneticPool()
{

}

void GeneticPool::init(int _size,int dna_size){
    std::ofstream outdbg = std::ofstream("gen.dbg",std::ofstream::out | std::ofstream::trunc);
    size = _size;
    for(size_t i = 0; i < size; i++){
        dnas[i] = new DNA(true, dna_size);
        outdbg << *dnas[i] << std::endl;
    }
    gen = 1;


}

void GeneticPool::breed(){
    /* 
        Select top 3% mutate to 20%
        breed to 95% 
        add random 3%
    */
    std::sort(dnas, dnas + size,
    [](const DNA* dna_a, const DNA* dna_b )
        { return  dna_a->fitness > dna_b->fitness;});

    std::cout << "Gen :" << gen;
    std::cout << " max fitness: " << dnas[0]->fitness << std::endl;

    for(int i = (int)(size*0.03f); i < (int)(size*.2f) ;i++){
        int a = fastRandInt(0,(int)(size*0.03f));
        delete dnas[i];
        dnas[i] = new DNA(*dnas[a]);
        dnas[i]->mutateNoise(.1f,.1f);
    }
    for(int i = (int)(size*.2f); i < (int)(size*.95f) ;i++){
        int a = fastRandInt(0,(int)(size*0.2f));
        int b = fastRandInt(0,(int)(size*0.2f));
        dnas[a]->crossover(dnas[b],dnas[i] ,0.05f);      
    }
    for(int i = (int)(size*.95f) ; i < size;i++){
        delete dnas[i];
        dnas[i] = new DNA(true,dnas[0]->length);
    }

    gen++;
}