class MCMC
{
private:
    double ** _data;
    int * _label;
    double Likelihood(int * label);
public:
    MCMC();
    MCMC(double ** data, int * label);
    int * EM_GMM();
    int * SM_GMM();
    int * SDDSSM_GMM();
    int * LSHSM_GMM();

    double NMI (int * label); 
    static int _size;
    ~Bucket();
};

