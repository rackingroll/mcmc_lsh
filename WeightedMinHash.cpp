#include "WeightedMinHash.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <math.h>

#define PI 3.14159265
//#pragma once
using namespace std;

WeightedMinHash::WeightedMinHash()
{
}

void WeightedMinHash::Initialize(int dimention,int numOfHashes)
{
	_dim = dimention;
	_numhashes = numOfHashes;

    std::random_device rd;
    std::mt19937 gen(rd());
 
    // values near the mean are the most likely
    // standard deviation affects the dispersion of generated values from the mean
    std::normal_distribution<> d(0,1);
    
    _rand_vec = new double*[_numhashes];

    for (size_t i = 0; i < _numhashes; i++ )
    {
        _rand_vec[i] = new double[_dim];
        double sum = 0.0;
        for (size_t j = 0; j < _dim; j++)
        {
            _rand_vec[i][j] = d(gen);
            sum += _rand_vec[i][j];
        }
        // Normalize
        for (size_t j = 0; j < _dim; j++)
        {
            _rand_vec[i][j] /= sum;
            //printf("%f \n", _rand_vec[i][j]);
        }
    }    
}

WeightedMinHash::WeightedMinHash(int dimention,int numOfHashes)
{
	_dim = dimention;
	_numhashes = numOfHashes;

    std::random_device rd;
    std::mt19937 gen(rd());
 
    // values near the mean are the most likely
    // standard deviation affects the dispersion of generated values from the mean
    std::normal_distribution<> d(0,1);
    
    _rand_vec = new double*[_numhashes];

    for (size_t i = 0; i < _numhashes; i++ )
    {
        _rand_vec[i] = new double[_dim];
        double sum = 0.0;
        for (size_t j = 0; j < _dim; j++)
        {
            _rand_vec[i][j] = d(gen);
            sum += _rand_vec[i][j];
        }
        // Normalize
        for (size_t j = 0; j < _dim; j++)
        {
            _rand_vec[i][j] /= sum;
        }
    }
   
}

/*
* Calculate the collision probability under sign random projection
*/

double WeightedMinHash::getProb(double * q, double * vector, int length)
{
    return 0.0;
}


int * WeightedMinHash::getHash(double * vector, int length)
{
    int * hashes = new int[_numhashes];
    for (size_t i = 0; i < _numhashes; i++)
    {
        double inner_product = 0.0;
        hashes[i] = 0;
        double _b = (double) (rand() % int(_w));
        
        for (size_t j=0;j<length;j++)
        {
            inner_product += vector[j]* _rand_vec[i][j];
        }

        //printf("%f ", inner_product);

        hashes[i] = floor((inner_product+_b) / _w);    
        //printf ("%d ", hashes[i]);
    }
    //printf ("\n");
    return hashes;
}

/*
The function CWS is Consistent weighted sampling, for the details of this function, please refer the following paper:
Ioffe, Sergey. "Improved consistent sampling, weighted minhash and l1 sketching." Data Mining (ICDM), 2010 IEEE 10th International Conference on. IEEE, 2010.
*/
pair<int, double> WeightedMinHash::CWS (double * vector, int length)
{

    double *r = new double[length]; 
    double *c = new double[length]; 
    double *b = new double[length]; 

    double *t = new double[length]; 
    double *y = new double[length]; 

    // Using vector here for the argmin function.
    std::vector<double> a(length);  
    
    for (int i=0;i<length;i++)
    {
        r[i] = rgamma(2.0,1.0);
        c[i] = rgamma(2.0,1.0);
        b[i] = uniform(0.0,1);
    }

    int i = 0;
    while(vector[i] > 0 && i<length)
    {
        double s = vector[i];
        t[i] = (log(s)/r[i]) + b[i] ;

        y[i] = exp(r[i]*(t[i]-b[i]));
        a[i] = c[i]/(y[i]*exp(r[i])) ;

        i++;
    }
    std::pair <int,double> sample;   

    sample.second = *std::min_element(a.begin(), a.end());
    sample.first = std::distance(a.begin(), std::min_element(a.begin(), a.end()));

    return sample;
}

// Generate a random number from a gamma distribution
double WeightedMinHash::rgamma(double a,double b) 
{
    double d,c,x,v,u;
    if(a>=1)
    {
        d = a-1./3.; 
        c = 1./sqrt(9.*d);
        while(1)
        {
            do 
            {
                x=gauss(0,1.0); 
                v=1.+c*x;
            } 
            while(v<=0.);

            v = v * v * v; 
            u = uniform(0,1);
            if( u < 1.0-0.0331*(x*x)*(x*x) )
            {
                return d*v*b;
            }
            if( log(u) < 0.5*x*x+d*(1.0-v+log(v)) )
            {
                return d*v*b;
            }
        }
    } else 
    {
        x = rgamma(a+1,b);
        x = x * pow(uniform(0,1), 1.0/a); 
        return x;
    }
}

// generate a random number from a uniform distribution
double WeightedMinHash::uniform(double a, double b)
{  
    return rand() / (RAND_MAX + 1.0) * (b - a) + a;
}

// Generate a random number from a gaussion distribution
double WeightedMinHash::gauss(double mu,double sigma)
{
    double x1, x2, w, y1, y2;

    do {
        x1 = 2.0 * uniform(0,1) - 1.0;
        x2 = 2.0 * uniform(0,1) - 1.0;
        w = x1 * x1 + x2 * x2;
    } while ( w >= 1.0 );

    w = sqrt( (-2.0 * log( w ) ) / w );
    y1 = x1 * w;
    y2 = x2 * w;
    return mu+sigma*y1; 
}

WeightedMinHash::~WeightedMinHash()
{
	for (size_t i = 0; i < _numhashes; i++)
	{
		delete[] _rand_vec[i];
	}
	delete[] _rand_vec;
}