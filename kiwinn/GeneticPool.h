#ifndef GENETICPOOL_H
#define GENETICPOOL_H

#pragma once

#include "DNA.hpp"

#define POOL_SIZE 1000

class GeneticPool
{
public:
    GeneticPool();
    ~GeneticPool();

    void init(int _size,int dna_size);
    void breed();
    DNA* getDNA(int i){return dnas[i];}
    int getGen(){return gen;}
private:
    DNA* dnas[POOL_SIZE];
    int gen;
    int size;
};

#endif