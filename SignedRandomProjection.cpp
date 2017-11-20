#include "SignedRandomProjection.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <math.h>

#define PI 3.14159265
//#pragma once
using namespace std;  

SignedRandomProjection::SignedRandomProjection(int dimention,int numOfHashes)
{
	_dim = dimention;
	_numhashes = numOfHashes;
	_samSize = floor(_dim / 3);

	int * a = new int[_dim];
	for (size_t i = 0; i < _dim; i++)
	{
		a[i] = i;
	} 

	srand(time(0));
	_randBits = new short*[_numhashes];
	_indices = new int*[_numhashes];

	for (size_t i = 0; i < _numhashes; i++)
	{
		// re-arrange elements in range 0,_dim randomly.
		random_shuffle(&a[0],&a[_dim]);

		_randBits[i] = new short[_samSize];
		_indices[i] = new int[_samSize];

		for (size_t j = 0; j < _samSize; j++)
		{
			_indices[i][j] = a[j];

			// Flip a coin, and randomly assign different bits to _randBits
			if (rand() % 2 == 0)
				_randBits[i][j] = 1;
			else
				_randBits[i][j] = -1;
		}
	}
}

/*
* Calculate the collision probability under sign random projection
*/

double SignedRandomProjection::getProb(double * q, double * vector, int length)
{
	double inner = 0.0;
	double q_norm = 0.0;
	double v_norm = 0.0;

	for (int i=0;i<length;i++)
	{
		inner += q[i]*vector[i];
		v_norm += vector[i]*vector[i];
		q_norm += q[i]*q[i];
	}
	
	q_norm = sqrt(q_norm) ;
	v_norm = sqrt(v_norm) ;
	
	double param = inner / (q_norm*v_norm) ;

	//cout << "para!!!!!" << acos(param) << endl;
	return 1 - (acos(param) / PI );
	
}


int * SignedRandomProjection::getHash(double * vector, int length)
{
	// lenght should be = to _dim
	int * hashes = new int[_numhashes];
	for (size_t i = 0; i < _numhashes; i++)
	{
		hashes[i] = 0;
		for (size_t j = 0; j < _samSize; j++)
		{
			if(_randBits[i][j] >=0)
			    hashes[i] += vector[_indices[i][j]];
			else
				hashes[i] -= vector[_indices[i][j]];
		}
		
		if (hashes[i] >= 0)
			hashes[i] = 1; //any random number
		else
			hashes[i] = 2; //any other random number
	}


	return hashes; 
}

SignedRandomProjection::~SignedRandomProjection()
{
	for (size_t i = 0; i < _numhashes; i++)
	{
		delete[] _randBits[i];
		delete[] _indices[i];
	}
	delete[] _randBits;
	delete[] _indices;
}