
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>



class linear_regression
{
	double *_tss;   // timestamp or 'x'
	double *_lreg;  // data samples or 'y' 

	long _counter;
	long _samples;

	double _m;
	double _b;

	double _x1, _y1, _x2, _y2;

	bool _recalc;

	public:

	linear_regression( int samples )
		: _samples(samples), _counter(0), _lreg(NULL), _tss(NULL)
	{
		_tss  = new double[_samples];
		_lreg = new double[_samples];
		_recalc = true;
	}

	void log_entry( double x, double y )
	{
		_lreg[_counter%_samples] = y;
		_tss[_counter%_samples]  = x;
		_counter++;
		_recalc = true;
	}

	int samples() { return _samples; }



	double r_squared()
	{
		double* line_y = new double[_samples];
		double* result_y = new double[_samples];
		double* avgs_y = new double[_samples];
		
		double  avg_y = std::reduce(_lreg, _lreg + _samples, 0.0, [](double a, double b) {return a + b;}) 
			/ (double)_samples;

		std::fill(avgs_y, avgs_y + _samples, avg_y);
		
		double sst = std::transform_reduce(_lreg, _lreg + _samples, avgs_y, 0.0,
			[&](double a, double b) {return a + b;},
			[&](double a, double b) {return std::pow(a - b, 2.0);}
		);

		double ssr = std::transform_reduce(_lreg, _lreg + _samples, _tss, 0.0,
			[&](double a, double b) {return a + b;},
			[&](double a, double b) {return std::pow(a - slope_point(b), 2.0);}
		);


		std::cout << "ssr = " << ssr << std::endl;
		std::cout << "sst = " << sst << std::endl;

		double rs = 1.0 - (ssr / sst);

		delete[] line_y;
		delete[] result_y;
		delete[] avgs_y;

		return rs;
	}

	// If you need to calculate the standard error of the slope (SE) by hand, use the following formula:
	// 
	// SE = sb1 = sqrt [ sum(yi - slope_yi)^2 / (n - 2) ] / sqrt [ sum(xi - avg(x) )^2 ]
	// or
	// SE = sb1 = sqrt [ Σ(yi - ŷi)^2 / (n - 2) ] / sqrt [ Σ(xi - avg(x) )^2 ]
	//
	// where yi is the value of the dependent variable for observation i, ŷi is estimated value of the dependent 
	// variable for observation i, xi is the observed value of the independent variable for observation i, avg(x) 
	// is the mean of the independent variable, and n is the number of observations.
	//
	//
	double standard_error()
	{
		double* line_y = new double[_samples];
		double* result_y = new double[_samples];
		double* result_x = new double[_samples];
		double* avgs_x = new double[_samples];
		double  avg_x = std::reduce(_lreg, _lreg + _samples) / (double)_samples;

		std::fill(avgs_x, avgs_x + _samples, avg_x);

		double sy = std::sqrt( 
			std::transform_reduce(_lreg, _lreg + _samples, _tss, 0.0,
				[&](double a, double b) {return a + b;},
				[&](double a, double b) {return std::pow(a - slope_point(b), 2.0);}
		) / ((double)_samples - 2.0) );

		double sx = std::sqrt( 
			std::transform_reduce(_tss, _tss + _samples, 0.0,
				[&](double a, double b) {return a + b;},
				[&](double a) {return std::pow(a - avg_x, 2.0);}
		) );


		std::cout << "sy = " << sy << std::endl;
		std::cout << "sx = " << sx << std::endl;


		double se = sy / sx;

		delete[] line_y;
		delete[] result_y;
		delete[] result_x;
		delete[] avgs_x;

		return se;
	}

	double calc()
	{
		double lxy = 0.0;
		double lxx = 0.0;
		double lxs = 0.0;
		double lys = 0.0;

		int len = _counter < _samples ? _counter : _samples;

		for( int i = 0; i < len; i++ )
		{
			lxy += _lreg[i]*_tss[i];
			lxx += _tss[i]*_tss[i];
			lxs += _tss[i];
			lys += _lreg[i];
		}

		if( (((double)len*lxx)-(lxs*lxs)) == 0.0 )
			return _lreg[(_counter-1)%_samples];

		_m = (((double)len*lxy)-(lxs*lys))/(((double)len*lxx)-(lxs*lxs));
		_b = (lys-(_m*lxs))/len;

		_recalc = false;

		return _m*(_tss[(_counter-1)%_samples])+_b;
	}

	double slope_point(double x)
	{
		if (_recalc)
			calc();

		return _m * x + _b;
	}

	double slope()
	{
		if (_recalc)
			calc();

		return _m;
	}

	double x1() { return _x1; }
	double x2() { return _x2; }
	double y1() { return _y1; }
	double y2() { return _y2; }

	double m() { return _m; }
	double b() { return _b; }



	~linear_regression()
	{
		delete [] _tss;
		delete [] _lreg;
	}

};

