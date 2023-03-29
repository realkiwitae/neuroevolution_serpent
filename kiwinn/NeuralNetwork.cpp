#include "NeuralNetwork.hpp"  

#include <iostream>

NeuralNetwork::NeuralNetwork(int stage_lengths[],int n) {
    nb_stages = n;
    Stage* prev = nullptr;
    for (int i = 0; i < n; i++) {
        stages[i] = Stage(prev, stage_lengths[i]);
        prev = &stages[i];
    }
}
	
NeuralNetwork::~NeuralNetwork()
{
	
}

void NeuralNetwork::loadCoeffs(uint8_t coeffs[]) {
    int idx = 0;
    for (int s = 1; s < nb_stages; s++) {
        for (int i = 0; i < stages[s].output_length; i++) {
            for (int j = 0; j < stages[s].prev->output_length; j++) {
                stages[s].coeffs[i][j] = coeffs[idx++];
            }
        }
    }
}

void NeuralNetwork::loadCoeffsSymmetrical(uint8_t coeffs[]) {
    int idx = 0;
    for (int s = 1; s < nb_stages; s++) {
        if (stages[s].output_length % 2 == 1) {
            std::cerr << "Symmetrical Net without even sized stages. Bad."<< std::endl;
            return;
        }
        for (int i = 0; i < (stages[s].output_length) / 2; i++) {
            for (int j = 0; j < stages[s].prev->output_length; j++) {
                stages[s].coeffs[i][j] = coeffs[idx];
                stages[s].coeffs[stages[s].output_length - 1 - i][stages[s].prev->output_length - 1 - j] = coeffs[idx++];
            }
        }
    }
}

/**
 * Calculates the output of the network for the given input
 * 
 * @param input		input vector (first stage values)
 * @return			output vector
 */

void NeuralNetwork::calc(double input[]) {
    for (int i = 0; i < stages[0].output_length; i++) {
        stages[0].output[i] = input[i];
    }
    for (int i = 1; i < nb_stages; i++) {
        stages[i].calc();
    }
}

double* NeuralNetwork::getOutput(){
    return stages[nb_stages-1].output;
}