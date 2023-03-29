#include "Stage.hpp"  
#include <cmath>

Stage::Stage()
{
	
}

Stage::Stage(Stage* prev, int size){
    this->prev = prev;
    output_length = size;
}
	
Stage::~Stage()
{
	
}

void Stage::calc(){
    if (prev == nullptr) return;
    for (int i = 0; i < output_length; i++){
        double sum = 0;
        for (int j = 0; j < prev->output_length-1; j++){
            sum += (coeffs[i][j]/255.f - .5f)*prev->output[j];
        }
        sum += (coeffs[i][prev->output_length-1]/255.f - .5f)*signalMultiplier;  //constant bias
        output[i] = sigmoid(sum);
    }
}

double Stage::sigmoid(double x) {
    return signalMultiplier/(1+exp(-x));  //range: 0 .. multiplier
}