#include <vector>
#pragma once
using namespace std;

class L2LSH 
{
private:
	int _dim;
	int _numhashes, _samSize;
    double ** _rand_vec;
	double _w = 5.0;  // The size of the bins on the normalized vector
	
public:
	L2LSH();
	L2LSH(int dimention, int numOfHashes);
	void Initialize(int dimention, int numOfHashes);
	int * getHash(double * vector, int length);
	double getProb(double * q, double * vector, int length);
	~L2LSH();
};

