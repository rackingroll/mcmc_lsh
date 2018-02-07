#include <iostream>
#include <cmath>
#include <limits>
#include "MCMC.h"
//#include "L2LSH.h"
//#include "LSH.h"
//#include "HashFunction.h"

using namespace std;

/*
MCMC::MCMC()
{
	srand(time(0));
	_data = NULL;
	_label = NULL;
}
*/

double THRESHOLD = 0.01;

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

MCMC::MCMC(double ** data, int * label, int num, int dim, int clusnum, int K, int L)
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

	lsh = new LSH(K,L);
	l2lsh = new L2LSH(_dim, K*L) ;
	
	for (int j=0; j<_num; j++ )
	{
		lsh->add(l2lsh->getHash(_data[j], _dim), j);
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

int * MCMC::EM_GMM(int clusnum)
{
	int * label = new int[_num];

	// Randomly assign labels
	for (int i = 0; i < _num; i++)
	{
		label[i] = rand() % clusnum;
	}

	double likelihood = std::numeric_limits<double>::max();
	double plikelihood = 0.0;

	//for (int i=0;i<_num;i++) cout << label[i] ;
	//cout<< endl;

	while (true)
	{

		double ** centers = GetCenters(label, clusnum);

		label = Reassignment(label, centers);
		
		
		//for (int i=0;i<_num;i++) cout << label[i] ;
		//cout<< endl;
		//for (int i=0;i<_num;i++) cout << labelCurrent[i] ;
		//cout<< endl;

		likelihood = Likelihood(label, _clusnum);
		
		cout << "likelihoodCurrent" << likelihood << endl;
		
		if (abs(likelihood - plikelihood) <= THRESHOLD) break;
		//if (likelihood <= THRESHOLD) break;

		plikelihood = likelihood;
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
	int iter = 0;
	while (likelihood >= THRESHOLD)
	{
		iter++;
		int clusnumCurrent = clusnum;
		labelCurrent = OperationSM(label, &clusnumCurrent);
		//for (int i=0;i<_num;i++) cout << label[i] ;
		//cout<< endl;
		//for (int i=0;i<_num;i++) cout << labelCurrent[i] ;
		//cout<< endl;
	
		double likelihoodCurrent = Likelihood(labelCurrent, clusnumCurrent);
		if (likelihoodCurrent < likelihood)
		{
			//cout << "likelihood: " << likelihood << endl;
			likelihood = likelihoodCurrent;
			label = labelCurrent;
			clusnum = clusnumCurrent;
		}
		//cout << "likelihood: " << likelihood << endl;
		//cout << "likelihoodCurrent" << likelihoodCurrent << endl;
	}
	cout<< iter << endl;
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
	
	double likelihood = std::numeric_limits<double>::max();
	int * labelCurrent;
	int iter = 0;
	while (likelihood >= THRESHOLD)
	{
		iter++;
		int clusnumCurrent = clusnum;
		labelCurrent = OperationSDDSSM(label, &clusnumCurrent);
		//for (int i=0;i<_num;i++) cout << label[i] ;
		//cout<< endl;
		//for (int i=0;i<_num;i++) cout << labelCurrent[i] ;
		//cout<< endl;
	
		double likelihoodCurrent = Likelihood(labelCurrent, clusnumCurrent);
		if (likelihoodCurrent < likelihood)
		{
			likelihood = likelihoodCurrent;
			label = labelCurrent;
			clusnum = clusnumCurrent;
			//cout << "likelihood: " << likelihood << endl;
		}
		cout << "likelihood: " << likelihood << endl;
		//cout << "likelihoodCurrent" << likelihoodCurrent << endl;
	}
	
	cout<< iter <<endl;
	return label;	
}

int * MCMC::LSHSM_GMM()
{
	int * label = new int[_num];
	int clusnum = 1;

	// All the 
	for (int i = 0; i < _num; i++)
	{
		label[i] = 0;
	}
	
	double likelihood = std::numeric_limits<double>::max();
	int * labelCurrent;
	int iter = 0;
	while (likelihood >= THRESHOLD)
	{
		iter++;
		int clusnumCurrent = clusnum;
		labelCurrent = OperationLSHSM(label, &clusnumCurrent);
		for (int i=0;i<_num;i++) cout << label[i] ;
		cout<< endl;
		for (int i=0;i<_num;i++) cout << labelCurrent[i] ;
		cout<< endl;
	
		double likelihoodCurrent = Likelihood(labelCurrent, clusnumCurrent);
		
		if (likelihoodCurrent < likelihood)
		{
			likelihood = likelihoodCurrent;
			label = labelCurrent;
			clusnum = clusnumCurrent;
			cout << "likelihood: " << likelihood << endl;
		}
		//cout << "likelihood: " << likelihood << endl;
		//cout << "likelihoodCurrent" << likelihoodCurrent << endl;
	}
	
	cout<< iter <<endl;
	return label;	
}

// Reassignment the labels to the corresponding centers
int * MCMC::Reassignment(int * label, double ** centers)
{
	int * labelCurrent = new int[_num];
	
	for (int i=0;i<_num;i++)
	{
		double min_dis = std::numeric_limits<double>::max();
		for (int j=0; j< _clusnum; j++)
		{
			double dist = L2Distance(_data[i], centers[j]) ;
			if ( L2Distance(_data[i], centers[j]) < min_dis) 
			{
				min_dis = dist;
				labelCurrent[i] = j;
			}
		}
	}

	return labelCurrent;
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

int * MCMC::OperationSM(int * label, int *clusnum)
{
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
			//cout << "Split!" << endl;
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
			//cout << "Merge!" << endl;
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
			//cout << "!!!!!" << *clusnum << endl;
		}
	}

	return labelCurrent;
}

int * MCMC::OperationSDDSSM(int * label, int *clusnum)
{
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
			//cout << "Split!" << endl;
			int split_label = rand() % (*clusnum);
			bool splited = false;
			
			for (int i=0;i<_num;i++)
			{
				if (labelCurrent[i] == split_label)
				{
					int labelOld = labelCurrent[i];
					double likelihoodOld = 0.0;
					if (splited)	likelihoodOld = Likelihood(labelCurrent, *clusnum+1);
					else	likelihoodOld = Likelihood(labelCurrent, *clusnum);
					labelCurrent[i] = (*clusnum);
					double likelihoodCurrent = Likelihood(labelCurrent, (*clusnum) + 1);
					
					if (likelihoodOld < likelihoodCurrent)	labelCurrent[i] = labelOld; 
					else splited = true;
					
				}
			}
			
			if (splited)
				*clusnum = *clusnum + 1;
		}
		else // Merge
		{
			//cout << "Merge!" << endl;
			if (*clusnum == 1) return labelCurrent;
			int mlabel = 0;
			mlabel = rand() % *clusnum;
			int mlabel1 = 0;
			int mlabel2 = 0;

			double likelihoodOld = std::numeric_limits<double>::max();

			for (int i=0;i< (*clusnum);i++)
			{
				if (i != mlabel)
				{
					if (i<mlabel) 
					{
						mlabel1 = i; mlabel2 = mlabel;
					}
					else
					{
						mlabel1 = mlabel; mlabel2 = i;
					}

					int * labelUpdate = new int[_num];
					for (int i=0;i<_num;i++)
					{
						labelUpdate[i] = label[i];
					}

					for (int i=0;i<_num;i++)
					{
						if (labelUpdate[i] == mlabel2)
						{
							labelUpdate[i] = mlabel1;
						}
						if (labelUpdate[i] > mlabel2)
						{
							labelUpdate[i] --;
						}
					}

					double likelihoodCurrent = Likelihood(labelUpdate, (*clusnum) - 1);

					if (likelihoodCurrent < likelihoodOld)
					{
						labelCurrent = labelUpdate;
						likelihoodOld = likelihoodCurrent;
					}

				}
			}
			*clusnum = *clusnum -1;
		}
	}

	return labelCurrent;	
}

int * MCMC::OperationLSHSM(int * label, int *clusnum)
{
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
			//cout << "Split!" << endl;
			int d1 = rand() % _num;
			int * candidates = lsh->retrieve(l2lsh->getHash(Inverse(_data[d1], _dim), _dim));
			int d2 = candidates[2];

			if (labelCurrent[d1] == labelCurrent[d2])
			{
				int split_label = labelCurrent[d1];
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
		}
		else // Merge
		{
			//cout << "Merge!" << endl;
			if (*clusnum == 1) return labelCurrent;

			int d1 = rand() % _num;
			int * candidates = lsh->retrieve(l2lsh->getHash(_data[d1], _dim));
			int d2 = candidates[2];
			int mlabel1 = 0;
			int mlabel2 = 0;
			if (labelCurrent[d1] != labelCurrent[d2])
			{
				if (labelCurrent[d1] > labelCurrent[d2])
				{
					mlabel1 = labelCurrent[d2]; mlabel2 = labelCurrent[d1];
				}
				else
				{
					mlabel2 = labelCurrent[d2]; mlabel1 = labelCurrent[d1];
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
				//cout << "!!!!!" << *clusnum << endl;
			}
		}
	}

	return labelCurrent;
}

double * MCMC::Inverse (double * a, int dim)
{
	double * b = new double[dim];
	for (int i=0;i<dim;i++)
	{
		b[i] = -a[i];
	}
	return b;
}
MCMC::~MCMC()
{
}
