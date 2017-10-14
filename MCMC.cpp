#include <iostream>
#include "MCMC.h"

int MCMC::_size = 0;

MCMC::MCMC()
{
	a = 1;
}

MCMC::~MCMC()
{
	delete[] arr;
}
