#include <vector>
#pragma once
using namespace std;

class L2LSH 
{
private:
	int _dim;
	int _numhashes, _samSize;
    double ** _rand_vec;
    int _numbins = 10;
public:
	L2LSH(int dimention, int numOfHashes);
	int * getHash(double * vector, int length);
	double getProb(double * q, double * vector, int length);
	~L2LSH();
};

