#include "predefine.h"
map::map(char* filename)
{
	ifstream f(filename);
	
	int n, m;
	
	for(int i = 0; i < 256; i++)
	{
		f >> n;
		
		f >> m;
		
		mt[n] = m;
		
		de_mt[m] = n;
	}
	f.close();
	return;
}

unsigned char map::en_map(unsigned char c)
{
	return mt[c];
}

unsigned char map::de_map(unsigned char c)
{
	return de_mt[c];
}
