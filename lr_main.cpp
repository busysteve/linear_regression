#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linear_regression.h"
#include <chrono>

#include "rowparser.h"



int main( int argc, char **argv )
{
	FILE* fin = NULL;
	FILE* fout= NULL;
	size_t   line_len = 1024;
	char  *line = new char[line_len];

	bool show_input   = false;
	bool show_slope   = false;
	bool show_headers = false;
	bool show_se = false;
	bool show_rs = false;
	bool show_m = false;
	bool show_b = false;
	bool show_t = false;
	int samples = 0;


		fin = stdin;
		fout= stdout;


	auto usage = [argv]()
	{
		printf("%s : -[i = show input data too | s = show slope of curve | h = print headers ] -[number of samples]\n", argv[0]);
		exit(-1);
	};


	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] != '-')
			usage();

		int o = 1;
		switch (argv[i][1])
		{
		case 'c':
			o = 0;
			i++;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			samples = atoi(&(argv[i][o]));
			break;

		case 'i':
			i++;
			fin = fopen(argv[i], "r");
			break;

		case 'o':
			i++;
			fout = fopen(argv[i], "w");
			break;

		case 'l':
			show_input = true;
			break;
		case 's':
			show_slope = true;
			break;
		case 'e':
			show_se = true;
			break;
		case 'r':
			show_rs = true;
			break;
		case 'm':
			show_m = true;
			break;
		case 'b':
			show_b = true;
			break;
		case 't':
			show_t = true;
			break;
		}
	}


	linear_regression lr(samples);

	if( fin == NULL || fout == NULL )
		exit(1);


	auto rp = RowParser(fin, "\t", "\n", false, false );

	while( rp.ParseNextRow() )
	{
		std::string sx = rp[0];
		std::string sy = rp[1];

		double x = atof(sx.c_str() );
		double y = atof(sy.c_str() );


		//fprintf( fout, "%f\n", atof(line) );
		lr.log_entry( x, y );
		//lr.calc();
		if( show_input && show_slope )
			fprintf( fout, "%s,%f,%f,%f\n", (sx+","+sy).c_str(), lr.calc(), lr.slope(), lr.slope_point(30.6895) );
		else if( show_input )
			fprintf( fout, "%s,%f\n", (sx + "," + sy).c_str(), lr.calc() );
		else if( show_slope )
			fprintf( fout, "%f,%f\n", lr.calc(), lr.slope() );
		//else
		//	fprintf( fout, "%f\n", lr.calc() );

	}

	

	if (show_rs)
	{
		auto t1 = std::chrono::high_resolution_clock::now();
	
		auto R2 = lr.r_squared();
	
		auto t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> ms = t2 - t1;

		printf("R2 = %f", R2 );
		if( show_t )
			printf(" : ms = %f", ms.count() );
		printf("\n");

	}


	if (show_se)
	{
		auto t1 = std::chrono::high_resolution_clock::now();
	
		auto SE = lr.standard_error();
	
		auto t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> ms = t2 - t1;

		printf("SE = %f", SE );
		if( show_t )
			printf(" : ms = %f", ms.count() );
		printf("\n");

	}


	if (show_m)
		printf("m = %f\n", lr.m());

	if (show_b)
		printf("b = %f\n", lr.b());


	exit(0);
}
