#pragma once
class Gaussian
{
	private:
		double *_data;
		double _mean = 0.0;
        double _variance = 0.0;
        int _size = 0;
	public:
		Gaussian();
        Gaussian(double mean, double variance, int size);
        double * getData ();
		~Gaussian();
};

