#include "utils.hpp"  
	
std::default_random_engine generator;
std::normal_distribution<double> white_noise(0.,1.);

double fast_white_noise(){return white_noise(generator);}