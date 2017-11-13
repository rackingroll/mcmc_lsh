#include <iostream>
#include <cmath>
#include <limits>
#include "MCMC.h"

using namespace std;

/*
MCMC::MCMC()
{
	srand(time(0));
	_data = NULL;
	_label = NULL;
}
*/

MCMC::MCMC(double ** data, int * label, int num, int dim, int clusnum)
{
	srand(time(0));
	_num = num;
	_dim = dim;
	_clusnum = clusnum;

	_label = new int[_num];
	_data = new double*[_num];

	for (int i=0;i< _num; i++)
	{
		_data[i] = new double[_dim];
		_label[i] = label[i];
		for (int j=0;j< _dim; j++)
		{
			_data[i][j] = data[i][j];
		}
	}
}

double ** MCMC::GetCenters(int * label, int clusnum)
{
	double ** centers = new double*[clusnum];
	double * centerSum = new double[clusnum];
	for (int i=0;i<clusnum;i++)
	{
		centers[i] = new double[_dim];
		centerSum[i] = 0.0;
		for (int j=0;j<_dim;j++)
		{
			centers[i][j] = 0.0;
		}
	}

	for (int i=0;i<_num;i++)
	{
		for (int j=0; j<_dim; j++)
		{
			centers[label[i]][j] += _data[i][j];
		}
		centerSum[label[i]] += 1.0;
	}

	for (int i=0;i<clusnum;i++)
	{
		for (int j=0; j<_dim; j++)
		{
			centers[i][j] = centers[i][j] / centerSum[i];
		}
	}

	return centers;
}

// Likelihood of the clusters
// Need garrentee the labels has the same index of centers
double MCMC::Likelihood(int * label, int clusnum)
{
	double likelihood = 0.0;

	double ** centers = GetCenters(label, clusnum);

	for (int i=0;i< _num; i++)
	{
		likelihood += L2Distance(centers[label[i]], _data[i]);
	}
	return likelihood;
}

double MCMC::L2Distance(double *a, double *b)
{
	double l2 = 0.0;
	for (int i=0;i<_dim;i++)
	{
		l2 += (a[i] - b[i])*(a[i] - b[i]);
	}
	return sqrt(l2);
}

int * MCMC::EM_GMM()
{
	int * label = new int[_num];

	for (int i = 0; i < _num; i++)
	{
		// Flip a coin, and randomly assign clusters
		if (rand() % 2 == 0)
			label[i] = 0;
		else
			label[i] = 1;
	}


	double likelihood = std::numeric_limits<double>::max();
	int * labelCurrent;

	while (likelihood >= 0.01)
	{
		labelCurrent = Operation(label);
		
		for (int i=0;i<_num;i++) cout << label[i] ;
		cout<< endl;
		for (int i=0;i<_num;i++) cout << labelCurrent[i] ;
		cout<< endl;

		double likelihoodCurrent = Likelihood(labelCurrent, _clusnum);
		if (likelihoodCurrent <= likelihood)
		{
			//cout << "likelihood: " << likelihood << endl;
			likelihood = likelihoodCurrent;
			label = labelCurrent;
		}
		cout << "likelihood: " << likelihood << endl;
		cout << "likelihoodCurrent" << likelihoodCurrent << endl;

	}

	return label;
}

int * MCMC::SM_GMM()
{
	int * label = new int[_num];
	int clusnum = 1;

	// All the 
	for (int i = 0; i < _num; i++)
	{
		label[i] = 0;
	}
	
	// TBD
	double likelihood = std::numeric_limits<double>::max();
	int * labelCurrent;
	
	while (likelihood >= 0.01)
	{
		int clusnumCurrent = clusnum;
		labelCurrent = OperationSM(label, &clusnumCurrent);
		cout << "Clusnum: " << clusnum << endl;
		cout << "ClusnumCurrent: " << clusnumCurrent << endl;
		for (int i=0;i<_num;i++) cout << label[i] ;
		cout<< endl;
		for (int i=0;i<_num;i++) cout << labelCurrent[i] ;
		cout<< endl;
	
		double likelihoodCurrent = Likelihood(labelCurrent, clusnumCurrent);
		if (likelihoodCurrent < likelihood)
		{
			//cout << "likelihood: " << likelihood << endl;
			likelihood = likelihoodCurrent;
			label = labelCurrent;
			clusnum = clusnumCurrent;
		}
		cout << "likelihood: " << likelihood << endl;
		cout << "likelihoodCurrent" << likelihoodCurrent << endl;
	}
	
	return label;	
}

int * MCMC::SDDSSM_GMM()
{
	int * label = new int[_num];
	int clusnum = 1;

	// All the 
	for (int i = 0; i < _num; i++)
	{
		label[i] = 0;
	}
	
	// TBD
	double likelihood = std::numeric_limits<double>::max();
	int * labelCurrent;
	
	while (likelihood >= 0.01)
	{
		int clusnumCurrent = clusnum;
		labelCurrent = OperationSDDSSM(label, &clusnumCurrent);
		cout << "Clusnum: " << clusnum << endl;
		cout << "ClusnumCurrent: " << clusnumCurrent << endl;
		for (int i=0;i<_num;i++) cout << label[i] ;
		cout<< endl;
		for (int i=0;i<_num;i++) cout << labelCurrent[i] ;
		cout<< endl;
	
		double likelihoodCurrent = Likelihood(labelCurrent, clusnumCurrent);
		if (likelihoodCurrent < likelihood)
		{
			//cout << "likelihood: " << likelihood << endl;
			likelihood = likelihoodCurrent;
			label = labelCurrent;
			clusnum = clusnumCurrent;
		}
		cout << "likelihood: " << likelihood << endl;
		cout << "likelihoodCurrent" << likelihoodCurrent << endl;
	}
	
	return label;	
}

// Design two operations, move or switch
int * MCMC::Operation(int * label)
{
	int * labelCurrent = new int[_num];

	for (int i=0;i<_num;i++)
	{
		labelCurrent[i] = label[i];
	}

	int id1 = 0;
	int id2 = 0;
	while(true)
	{
		// randomly pick one node
		id1 = rand() % _num;
		id2 = rand() % _num;
		if (label[id1] != label[id2])
		{
			break;
		}
	}

	int clus1 = label[id1];
	int clus2 = label[id2];

	// Flip a coin, move or switch
	if (rand() % 2 == 0) // Move
	{
		labelCurrent[id1] = clus2;
	}
	else // switch
	{
		labelCurrent[id1] = clus2;
		labelCurrent[id2] = clus1;
	}

	return labelCurrent;
}

// Design two operations, move or switch
int * MCMC::OperationSM(int * label, int *clusnum)
{
	cout << "-----------" << *clusnum << endl;
	int * labelCurrent = new int[_num];

	for (int i=0;i<_num;i++)
	{
		labelCurrent[i] = label[i];
	}

	if (rand() % 2 == 0)
	{
		// If only has one cluster, then do nothing!
		if (*clusnum == 1) return labelCurrent;
		labelCurrent = Operation(label);
		return labelCurrent;
	}
	else
	{
		if (rand() % 2 == 0)
		{
			// Split
			cout << "Split!" << endl;
			int split_label = rand() % (*clusnum);
			for (int i=0;i<_num;i++)
			{
				if (labelCurrent[i] == split_label)
				{
					if (rand() % 2 == 0) labelCurrent[i] = (*clusnum);
					else;
				}
			}
			*clusnum = *clusnum + 1;
		}
		else // Merge
		{
			cout << "Merge!" << endl;
			if (*clusnum == 1) return labelCurrent;
			int mlabel1 = 0;
			int mlabel2 = 0;
			while(true)
			{
				// randomly pick one node
				mlabel1 = rand() % *clusnum;
				mlabel2 = rand() % *clusnum;
				if (mlabel1 < mlabel2)
				{
					break;
				}
			}

			for (int i=0;i<_num;i++)
			{
				if (labelCurrent[i] == mlabel2)
				{
					labelCurrent[i] = mlabel1;
				}
				if (labelCurrent[i] > mlabel2)
				{
					labelCurrent[i] --;
				}

			}
			*clusnum = *clusnum -1;
			cout << "!!!!!" << *clusnum << endl;
		}
	}

	return labelCurrent;
}

int * MCMC::OperationSDDSSM(int * label, int *clusnum)
{
	cout << "-----------" << *clusnum << endl;
	int * labelCurrent = new int[_num];

	for (int i=0;i<_num;i++)
	{
		labelCurrent[i] = label[i];
	}

	if (rand() % 2 == 0)
	{
		// If only has one cluster, then do nothing!
		if (*clusnum == 1) return labelCurrent;
		labelCurrent = Operation(label);
		return labelCurrent;
	}
	else
	{
		if (rand() % 2 == 0)
		{
			// Split
			cout << "Split!" << endl;
			int split_label = rand() % (*clusnum);
			for (int i=0;i<_num;i++)
			{
				if (labelCurrent[i] == split_label)
				{
					if (rand() % 2 == 0) labelCurrent[i] = (*clusnum);
					else;
				}
			}
			*clusnum = *clusnum + 1;
		}
		else // Merge
		{
			cout << "Merge!" << endl;
			if (*clusnum == 1) return labelCurrent;
			int mlabel1 = 0;
			int mlabel2 = 0;
			while(true)
			{
				// randomly pick one node
				mlabel1 = rand() % *clusnum;
				mlabel2 = rand() % *clusnum;
				if (mlabel1 < mlabel2)
				{
					break;
				}
			}

			for (int i=0;i<_num;i++)
			{
				if (labelCurrent[i] == mlabel2)
				{
					labelCurrent[i] = mlabel1;
				}
				if (labelCurrent[i] > mlabel2)
				{
					labelCurrent[i] --;
				}

			}
			*clusnum = *clusnum -1;
			cout << "!!!!!" << *clusnum << endl;
		}
	}

	return labelCurrent;	
}

MCMC::~MCMC()
{
}
