#include "L2LSH.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <math.h>

#define PI 3.14159265
//#pragma once
using namespace std;

L2LSH::L2LSH(int dimention,int numOfHashes)
{
	_dim = dimention;
	_numhashes = numOfHashes;

    std::random_device rd;
    std::mt19937 gen(rd());
 
    // values near the mean are the most likely
    // standard deviation affects the dispersion of generated values from the mean
    std::normal_distribution<> d(0,1);
    
    double a = d(gen) ;

    printf("%f\n", a);

}

/*
* Calculate the collision probability under sign random projection
*/

double L2LSH::getProb(double * q, double * vector, int length)
{
    return 0.0;
}


int * L2LSH::getHash(double * vector, int length)
{
    return NULL;
}

L2LSH::~L2LSH()
{
	for (size_t i = 0; i < _numhashes; i++)
	{
		delete[] _rand_vec[i];
	}
	delete[] _rand_vec;
}