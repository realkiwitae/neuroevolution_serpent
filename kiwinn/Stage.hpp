#ifndef STAGE_H
#define STAGE_H
#pragma once

#include <stdint.h>

#define MAX_SIZE 500
	
class Stage  
{

public:
	double signalMultiplier = .1;
	Stage* prev;
	int output_length;
	double output[MAX_SIZE];
	uint8_t coeffs[MAX_SIZE][MAX_SIZE];
	
public:

	Stage();
	Stage(Stage* prev, int size);
	~Stage();

	/**
	 * calculates the outputs based on the input values
	 */
	void calc();
	double sigmoid(double x);
};
#endif