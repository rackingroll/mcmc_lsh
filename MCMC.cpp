#include <iostream>
#include "MCMC.h"

MCMC::MCMC()
{
	_data = NULL;
	_label = NULL;
}

MCMC::~MCMC()
{
	delete[] arr;
}
