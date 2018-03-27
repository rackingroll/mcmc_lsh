#include <vector>
#pragma once
using namespace std;

class WeightedMinHash 
{
private:
	int _dim;
	int _numhashes, _samSize;
    double ** _rand_vec;
	double _w = 20.0;  // The size of the bins on the normalized vector
    
    double uniform(double a, double b);
    double rgamma(double a, double b);
    double gauss(double mu, double sigma);
    pair<int, double> CWS (double * vector, int length);

public:
	WeightedMinHash();
	WeightedMinHash(int dimention, int numOfHashes);
	void Initialize(int dimention, int numOfHashes);
	int * getHash(double * vector, int length);
	double getProb(double * q, double * vector, int length);
	~WeightedMinHash();
};

