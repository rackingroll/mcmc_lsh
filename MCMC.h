#include "LSH.h"
#include "L2LSH.h"

class MCMC
{
private:
    double ** _data;
    int * _label;
    int _num; 
    int _dim;
    int _clusnum;
    LSH* lsh;
    L2LSH * l2lsh;

    double Likelihood(int * label, int clusnum);
    double L2Distance(double * a, double * b);
    int * Operation(int * label);
    double ** GetCenters(int * label, int clusnum);
    int * OperationSM(int * label, int * clusnum);
    int * OperationSDDSSM(int * label, int * clusnum);
    int * OperationLSHSM(int * label, int * clusnum);
    double * Inverse (double * a, int dim);
public:
    //MCMC();
    MCMC(double ** data, int * label, int num, int dim, int clusnum);
    MCMC(double ** data, int * label, int num, int dim, int clusnum, int K, int L);
    int * EM_GMM(int clusnum);
    int * SM_GMM();
    int * SDDSSM_GMM();
    int * LSHSM_GMM();

    //double NMI (int * label); 
    ~MCMC();
};

