

class linear_regression
{
	double *_tss;   // timestamp or 'x'
	double *_lreg;  // data samples or 'y' 

	long _counter;
	long _samples;

	public:

	linear_regression( int samples )
		: _samples(samples), _counter(0), _lreg(NULL), _tss(NULL)
	{
		_tss  = new double[_samples];
		_lreg = new double[_samples];
	}

	void log_entry( double x, double y )
	{
		_lreg[_counter%_samples] = y;
		_tss[_counter%_samples]  = x;
		_counter++;
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

		double m = (((double)len*lxy)-(lxs*lys))/(((double)len*lxx)-(lxs*lxs));
		double b = (lys-(m*lxs))/len;

		return m*(_tss[(_counter-1)%_samples])+b;
	}

	~linear_regression()
	{
		delete [] _tss;
		delete [] _lreg;
	}

};
