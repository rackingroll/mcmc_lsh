#include "Gaussian.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <math.h>
#include <chrono>
#include <random>

#define PI 3.14159265
//#pragma once
using namespace std;  

Gaussian::Gaussian(){}

Gaussian::Gaussian(double mean, double variance, int size)
{
    _data = new double[size];
    _mean = mean;
    _variance = variance;
    _size = size;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    std::default_random_engine generator(seed);
    std::normal_distribution<double> distribution(_mean,_variance);

    for (int i=0; i< _size; i++)
    {
        this->_data[i] = distribution(generator);
    }

}

double * Gaussian::getData()
{
    return _data;
}

Gaussian::~Gaussian()
{
	delete[] _data;
}