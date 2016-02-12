#!/bin/awk -f  
BEGIN{
    ts = 0;
}
{
	lr = linearRegression( $1, ts, 10, 0 );
	ts += 10;

	printf( "%f\n", lr );

}


function linearRegression( newAngle, looptime, samples, prediction )
{

	lreg[counter%samples] = newAngle;
	tss[counter%samples] = looptime;

	counter++;

	lxy = 0;
	lxx = 0;
	lxs = 0;
	lys = 0;

	len = length(lreg);

	for( i = 0; i < len; i++ )
	{
		lxy += lreg[i]*tss[i];
		lxx += tss[i]*tss[i];
		lxs += tss[i];
		lys += lreg[i];
	}

	if( ((len*lxx)-(lxs*lxs)) == 0 )
		return newAngle;

	m = ((len*lxy)-(lxs*lys))/((len*lxx)-(lxs*lxs));
	b = (lys-(m*lxs))/len;

	return m*(looptime+prediction)+b;
}



