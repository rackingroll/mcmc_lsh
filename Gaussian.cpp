#include "Gaussian.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <math.h>

#define PI 3.14159265
//#pragma once
using namespace std;  

Gaussian::Gaussian(){}

Gaussian::Gaussian(double mean, double variance, int size)
{
    // TBD for constructing a set of data point with gaussian distribution.
    _data = new double[size];
    _mean = mean;
    _variance = variance;
    _size = size;

    std::random_device rd;
    std::mt19937 gen(rd());

     // values near the mean are the most likely
    // standard deviation affects the dispersion of generated values from the mean
    std::normal_distribution<> d(_mean, _variance);

    for (int i=0; i< _size; i++) this->_data[i] = d(gen);

}

double * Gaussian::getData()
{
    return _data;
}

Gaussian::~Gaussian()
{
	delete[] _data;
}