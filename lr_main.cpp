
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linear_regression.h"

int main( int argc, char **argv )
{
	FILE* fin = NULL;
	FILE* fout= NULL;
	size_t   line_len = 1024;
	char  *line = new char[line_len];

	bool show_input   = false;
	bool show_slope   = false;
	bool show_headers = false;
	int samples;

	if( argc == 3 )
	{
		fin = stdin;
		fout= stdout;
	}
	else if( argc == 4 )
	{
		fin = fopen( argv[1], "r" );
		fout= stdout;
	}
	else if( argc > 5 )
	{
		fin = fopen( argv[1], "r" );
		fout= fopen( argv[2], "w" );
	}
	else
	{
		printf( "%s : -[i = show input data too | s = show slope of curve | h = print headers ] -[number of samples]\n", argv[0] ); 
		exit(-1);
	}

	if( argv[1][0] == '-' )
	{
		int len = 0;
		if( (len = strlen( argv[1] ) ) > 1 )
		{
			for( int i = len; i > 0; i-- )
			{
				switch( argv[1][i] )
				{
					case 'i':
						show_input = true;
						break;
					case 's':
						show_slope = true;
						break;
					case 'h':
						show_headers = true;
						break;
				}
			}
		}
	}

	if( argv[2][0] == '-' )
	{
		int len = 0;
		if( (len = strlen( argv[1] ) ) > 1 )
		{
			samples = atoi( &(argv[2][1]) );
		}
	}

	linear_regression lr(samples);

	if( fin == NULL || fout == NULL )
		exit(1);


	for( double x = 1.0; getline( &line, &line_len, fin ) >= 0; x += 1.0 )
	{
		//fprintf( fout, "%f\n", atof(line) );
		lr.log_entry( x, atof(line) );
		if( show_input )
			fprintf( fout, "%f,%f\n", atof(line), lr.calc() );
		else
			fprintf( fout, "%f\n", lr.calc() );
	}


	exit(0);
}

