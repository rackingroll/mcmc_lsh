#include <iostream>
#include <unordered_map>
#include "HashFunction.h"
//#include "DensifiedMinHash.h"
#include "LSH.h"
//#include <ppl.h>
#include <random>
#include <algorithm>
#include <climits>
//#pragma once
/* Author: Anshumali Shrivastava
*  COPYRIGHT PROTECTION
*  Free for research use. 
*  For commercial use, contact:  RICE UNIVERSITY INVENTION & PATENT or the Author.
*/

// Edited by Chen Luo, remove the parrallel part of the code.

using namespace std;
//using namespace concurrency;

int LSH::_rangePow = 20;
int LSH::_thresh = 0;

LSH::LSH(int K, int L)
{
	_K = K;
	_L = L;
	//_range = 1 << 22;

	// Create a list of
	_bucket = new Bucket*[L];
//#pragma omp parallel for
	for (int i = 0; i < L; i++)
	{
//		_bucket[i] = new Bucket[1<<_K];
		_bucket[i] = new Bucket[1<<_rangePow];
	}

	rand1 = new int[_K*_L];

	std::random_device rd;
	print 
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> dis(1, UINT_MAX);

//#pragma omp parallel for
	for (int i = 0; i < _K*_L; i++)
	{
		rand1[i] = dis(gen);
		if (rand1[i] % 2 == 0)
			rand1[i]++;
	}
}

void LSH::add(int *hashes, int id)
{
	for (int i = 0; i < _L; i++)
	{
		unsigned int index = 0;
		for (int j = 0; j < _K; j++)
		{
			unsigned int h = hashes[_K*i + j];
			h *= rand1[_K*i + j];
			h ^= h >> 13;
			h ^= rand1[_K*i + j];
			index += h*hashes[_K*i + j];
		}
		index = (index << 2) >> (32 - LSH::_rangePow);
		_bucket[i][index].add(id);
	}
}

int * LSH::retrieve(int *hashes)
{ 

	std::unordered_map<int, int> m;

	int count = 0;

	for (int i = 0; i < _L; i++)
	{
		unsigned int index = 0;
		for (int j = 0; j < _K; j++)
		{
			unsigned int h = hashes[_K*i + j];
			h *= rand1[_K*i + j];
			h ^= h >> 13;
			h ^= rand1[_K*i + j];
			index += h*hashes[_K*i + j];
		}
		index = (index << 2) >> (32 - LSH::_rangePow);
		if (_bucket[i][index].getAll() == NULL)
		{
			continue;
		}
		for (size_t a = 0; a < _bucket[i][index].getSize(); a++)
		{
			if (_bucket[i][index].getAll()[a] == 0)
				continue;
			if (_bucket[i][index].getAll()[a] < 0)
				continue;
			if (m.find(_bucket[i][index].getAll()[a]) == m.end())
				m[_bucket[i][index].getAll()[a]] = 1;
			else
				m[_bucket[i][index].getAll()[a]] += 1;

		}
	}
	int *uniqeRet = new int[m.size()+2];
	//uniqeRet[0] = m.size(); //reserved for size
	uniqeRet[1] = -1; //reserveed for id
 	int ind = 2;
	for (unordered_map<int,int>::const_iterator it = m.begin(); it != m.end(); ++it) {
		int val = it->first;
		if (m[val] >= LSH::_thresh)
		{
			uniqeRet[ind] = val;
			ind++;
		}
	}
	uniqeRet[0] = ind - 2; //reserved for size
	return uniqeRet;
}

/*
returns an array with ret with 3 values
ret[0] is the sample returned 
Sampling Probability = (1 - (1 - p^K)^ret[2])*(1/returnarray[1])
where p = (1 - 1/(Range))LSHCollprob(q,ret[0]) + 1/(Range)
Range = the range of hashtable which in our case is (1<<_rangePow)
*/

int * LSH::sample(int *hashes)
{
	int * samplewithProb = new int[3];
	samplewithProb[0] = -1;
	samplewithProb[1] = -1;
	samplewithProb[2] = -1;
	
	std::vector<int> v;
	for (int i = 0; i < _L; i++)
	{
		v[i] = i;
	}


	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(v.begin(), v.end(), g);

	for (int i = 0; i < _L; i++)
	{  
		int table = v[i];
		unsigned int index = 0;
		for (int j = 0; j < _K; j++)
		{
			unsigned int h = hashes[_K*table + j];
			h *= rand1[_K*table + j];
			h ^= h >> 13;
			h ^= rand1[_K*table + j];
			index += h*hashes[_K*table + j];
		}
		index = (index << 2) >> (32 - LSH::_rangePow);
		if (_bucket[table][index].getAll() == NULL)
		{
			continue;
		}
		else{
			int * retSamp = _bucket[table][index].sample();

			samplewithProb[0] = retSamp[0];
			samplewithProb[1] = retSamp[1];
			samplewithProb[2] = i;
			return samplewithProb;
		}
		
	}

	return samplewithProb;

}

LSH::~LSH()
{
	
	for (size_t i = 0; i < _L; i++)
	{
		delete[] _bucket[i];
	}
	delete[] _bucket;
}

