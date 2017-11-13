class MCMC
{
private:
    double ** _data;
    int * _label;
    int _num; 
    int _dim;
    int _clusnum;
    double Likelihood(int * label, int clusnum);
    double L2Distance(double * a, double * b);
    int * Operation(int * label);
    double ** GetCenters(int * label, int clusnum);
    int * OperationSM(int * label, int * clusnum);
    int * OperationSDDSSM(int * label, int * clusnum);
public:
    //MCMC();
    MCMC(double ** data, int * label, int num, int dim, int clusnum);
    int * EM_GMM();
    int * SM_GMM();
    int * SDDSSM_GMM();
    //int * LSHSM_GMM();

    //double NMI (int * label); 
    ~MCMC();
};

