#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H
#pragma once
	
#include "Stage.hpp"

#define MAX_STAGE_NB 10

class NeuralNetwork  
{
private:
	Stage stages[MAX_STAGE_NB];
	int nb_stages;
public:

	NeuralNetwork(int stage_lengths[],int n);
	~NeuralNetwork();

	/**
	 * Loads the weights / coefficients from the linear array sequentially into the network
	 * @param coeffs	array with the coefficients ranging -128 to +127.
	 * Note: no dimension/length check is done, will crash when given wrong sized array! 
	 */
	void loadCoeffs(uint8_t coeffs[]);

	
	/**
	 * Same like loadCoeffs(), but will fill the network symmetrically
	 * Only use when all stage sizes are even
	 * @param coeffs array with the coefficients ranging -128 to +127.
	 */

	void loadCoeffsSymmetrical(uint8_t coeffs[]);
	void calc(double input[]);
	double* getOutput();
};
#endif