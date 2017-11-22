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
}

double * Gaussian::getData()
{
    return _data;
}

Gaussian::~Gaussian()
{
	delete[] _data;
}