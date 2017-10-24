#include <iostream>
#include <unordered_map>
#include "HashFunction.h"
#include "LSH.h"
#include <random>
#include <algorithm>
#include <math.h>
#include "SignedRandomProjection.h"
//#pragma once
/* Author: Chen Luo
*  COPYRIGHT PROTECTION
*  Free for research use. 
*  For commercial use, contact:  RICE UNIVERSITY INVENTION & PATENT or the Author.
*/

using namespace std;
//using namespace concurrency;

int main (int argc, char *argv[])
{
    // Generate a sample data here to do experiment.
    // With 0,1 one cluster, 3,4 one cluster
    
    double data[8][3] =
    {
        {1.0,2.0,3.0}, {0.0,2.0,4.0}, {9.0,10.0,11.0}, {11.0,12.0,16.0}, {1.0,2.0,3.0}, {0.0,2.0,4.0}, {9.0,10.0,11.0}, {11.0,12.0,16.0}
    };

    int K = 10;
    int L = 10;

    LSH* lsh = new LSH(K,L);

    SignedRandomProjection* srp = new SignedRandomProjection(3,K) ;

    for (int i=0;i<4;i++)
    {
        lsh->add(srp->getHash(data[i], 3), i);
    }

    int* sample = lsh->sample(srp->getHash(data[2], 3)) ;

    printf("%d, %d, %d\n", sample[0], sample[1], sample[2]);

    int range = 1<<lsh->_rangePow;
    
    cout << "range" << range << endl;
    cout << "collision" << srp->getProb(data[2],data[sample[0]], 3) << endl;
    double cp = (1.0 - 1.0/(range))*srp->getProb(data[2],data[sample[0]], 3) + 1.0/(range);

    cout << "cp:" << (1.0 - pow((1.0 - pow(cp,K)),sample[2])) << endl;
    
    double p = (1.0 - pow((1.0 - pow(cp,K)),sample[2]+1))*(1.0/sample[1]);

    cout << "Hello World!, p=" <<p<< "\n" ;
}