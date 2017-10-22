#include <iostream>
#include <unordered_map>
#include "HashFunction.h"
#include "LSH.h"
#include <random>
#include <algorithm>
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

    LSH* lsh = new LSH(10,10);

    SignedRandomProjection* srp = new SignedRandomProjection(3,10) ;

    for (int i=0;i<4;i++)
    {
        lsh->add(srp->getHash(data[i], 3), i);
    }

    int* sample = lsh->sample(srp->getHash(data[2], 3)) ;

    printf("%d, %d, %d\n", sample[0], sample[1], sample[2]);

    cout << "Hello World!" << "\n" ;
}