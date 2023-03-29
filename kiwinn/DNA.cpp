#include "DNA.hpp" 

#include "./utils.hpp"

#include <cstdint>
#include <cmath>
#include <cstdlib>
#include <bits/stdc++.h>
#include <random>
#include <chrono>

DNA::DNA(bool random, int size):length(size){
    
    for (int i = 0; i < size; i++){
        data[i] = random?floor(fastRandInt(256)):0;
    }
    fitness = 0.f;
}
	
DNA::~DNA()
{
//	delete data;
}

void DNA::crossoverNoise(const DNA* other,DNA* newDNA, double mutationprob){  //byte-wise, noise applied to each value
    newDNA = new DNA(false, length);
    int numswaps = length*.1; 
    int swaps[numswaps+1];
    for (int i = 0; i < numswaps; i++){
        swaps[i] = (int)floor(fastRandInt(length));
    }
    swaps[numswaps] = length;  //save last
    std::sort( swaps, swaps+numswaps);
    int swapidx = 0;
    bool that = true;
    for (int i = 0; i < length; i++){
        if (i >= swaps[swapidx]){
            swapidx++;
            that = !that;
        }
        uint8_t d = 0;
        if (that){
            d = data[i];
        }
        else {
            d = other->data[i];
        }
        d += (uint8_t)(fast_white_noise()*mutationprob*256);
        newDNA->data[i] = d;
    }
}

void DNA::mutateNoise(double prob, double mag){
    for (int i = 0; i < length; i++){
        if (fastRandDouble() < prob) data[i] += (uint8_t)(fast_white_noise()*mag*256);
    }
}

void DNA::crossover(const DNA* other,DNA* newDNA ,double mutationprob){
    newDNA = new DNA(false, length);
    int numswaps = length/8; 
    int swaps[numswaps+1];
    for (int i = 0; i < numswaps; i++){
        swaps[i] = (int)floor(fastRandInt(length)*8);
    }
    swaps[numswaps] = 8*length;  //save last
    std::sort( swaps, swaps+numswaps);
    int swapidx = 0;
    bool that = true;
    for (int i = 0; i < 8*length; i++){
        if (i >= swaps[swapidx]){
            swapidx++;
            that = !that;
        }
        int bit = 0;
        if (that){
            bit = ((data[i/8] >> (i%8)) & 1);
        }
        else {
            bit = ((other->data[i/8] >> (i%8)) & 1);
        }
        if (fastRandDouble() < mutationprob) bit = 1-bit;
        newDNA->data[int(i/8)] |= (bit << (i%8));
    }
}

void DNA::crossoverBytewise(const DNA* other,DNA* newDNA, double mutationprob){
    newDNA = new DNA(false, length);
    int numswaps = length/8; 
    int swaps[numswaps+1];
    for (int i = 0; i < numswaps; i++){
        swaps[i] = 8*(int)floor(fastRandInt(length));
    }
    swaps[numswaps] = 8*length;  //save last
    std::sort( swaps, swaps+numswaps);
    int swapidx = 0;
    bool that = true;
    for (int i = 0; i < 8*length; i++){
        if (i >= swaps[swapidx]){
            swapidx++;
            that = !that;
        }
        int bit = 0;
        if (that){
            bit = ((data[i/8] >> (i%8)) & 1);
        }
        else {
            bit = ((other->data[i/8] >> (i%8)) & 1);
        }
        if (fastRandDouble() < mutationprob) bit = 1-bit;
        newDNA->data[i/8] |= (bit << (i%8));
    }
}

std::ostream &operator<<(std::ostream &os, const DNA& a){
    os << "dna: f=" << a.fitness << " data:{";  
    for(size_t i = 0; i < a.length; i++){
        os << (int)a.data[i] << " ";
    }
    os << "}";
}