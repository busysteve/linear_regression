BEGIN{

	for( i=0; i < 360; i++ )
	{
		wave  =  sin((((i)%360)*3.14159265/180));
		wavy  =  wave+sin(wave*30);
		messy =  10 + wavy + 1;
		print messy;
	}

}
