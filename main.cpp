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
    
    double data[4][3] =
    {
        {1,2,3}, {0,2,4}, {9,10,11}, {11,12,16}
    };
    printf("Here!1\n");
    LSH* lsh = new LSH(4,1);
    printf("Here!2\n");

    SignedRandomProjection* srp = new SignedRandomProjection(3,4) ;

        for (int i=0;i<4;i++)
    {
        lsh->add(srp->getHash(data[i], 3), i);

    }

    cout << "Hello World!" << "\n" ;
}