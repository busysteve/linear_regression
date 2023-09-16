#!/bin/awk -f 
BEGIN{

	for( i=0; i < 100000; i++ )
	{
		messy1 =  i + rand() * 30 ;
		messy2 =  1.5 * i + rand() * 50 ;
		print messy1"\t"messy2;
	}

}
