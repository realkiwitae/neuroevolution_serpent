#ifndef DNA_H
#define DNA_H
#pragma once
	
#define MAX_DATA_SIZE 2000

#include <stdint.h>
#include <iostream>

inline int fastRandInt(int maxSize);
inline int fastRandInt(int a, int b);
inline double fastRandDouble();
inline double fastRandDouble(double a, double b);

class DNA  
{
	public:
	
	uint8_t data[MAX_DATA_SIZE];
	int length;
	double fitness;

	public:

		DNA(bool random, int size);
		~DNA();

	 /**
	 * Crossover function which combines this DNA with another DNA object.
	 * Process is done byte-wise and a gaussian noise is added to each byte-value 
	 * Bits flip according to mutation probability
	 */
	void crossoverNoise(const DNA* other,DNA* newDNA, double mutationprob);

	/**
	 * Gaussian mutation function
	*/
	void mutateNoise(double prob, double mag);

	/**
	 * Crossover function which combines this DNA with another DNA object.
	 * Process is done bit-wise
	 * Bits flip according to mutation probability
	 */
	void crossover(const DNA* other,DNA* newDNA ,double mutationprob);

	/**
	 * Crossover function which combines this DNA with another DNA object.
	 * Process is only done byte-wise, so less noise is added
	 * Bits flip according to mutation probability
	 */
	void crossoverBytewise(const DNA* other,DNA* newDNA, double mutationprob);

	friend std::ostream &operator<<(std::ostream &os, const DNA& a);
};
#endif