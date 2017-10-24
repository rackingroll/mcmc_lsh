#include <iostream>
#include "Bucket.h"
//#pragma once

int Bucket::_size = 256;

Bucket::Bucket()
{
	isInit = -1;
	totalAdded = 0;
}

Bucket::~Bucket()
{
	delete[] arr;
}
int Bucket::getSize()
{
	return _size;
}
int Bucket::add(int id)
{
	totalAdded++;

	if (isInit == -1) {
		arr = new int[_size];
		for (int i = 0; i < _size; i++)
			arr[i] = 0;
		isInit = +1;
	}
	if (index == _size) {
		int currSamp = rand() % totalAdded;
		if (currSamp == totalAdded);
		{
			int randind = rand() % _size;
			arr[randind] = id;
		}
	}
	else {
		arr[index] = id;
		index++;
	}
	return 1;
}

int Bucket::retrieve(int index)
{
	if (index >= _size)
		return -1;
	return arr[index];
}

int * Bucket::sample()
{
	int * sample = new int[2];
	if (index == 0)
	{
		//no sample found
		sample[0] = -1; 
		sample[1] = -1; 
		return sample; // confuse here.
	}

	int randint = rand() % index;
	sample[0] = arr[randint]; // return sample
	sample[1] = totalAdded; // probability of selecting = 1/totalAdded;
	return sample;
}

int * Bucket::getAll()
{
	if (isInit == -1)
		return NULL;
	return arr;
}
