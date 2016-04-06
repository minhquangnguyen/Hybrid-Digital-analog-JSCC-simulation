#include "predefine.h"
extern ofstream debug_file2;
extern int intd1, intd2;
// read length float from file f, from pos-th bit, add to vec fr
void signal_composer::frame_composer(vec &fr, int pos, int length, FILE *f)
{
	fseek(f, pos, SEEK_SET);
	short e;
	int d = CE;
	double x, s1 = 0, s2 = 0;
	intd2++;
	for(int i = 0; i < length; i++)
	{
		size_t st = fread(&e, sizeof(e), 1, f);

		//fr(2 * i) = fr(2 * i) + (double)((double) e / (double)d);
		//fr(2 * i + 1) = fr(2 * i + 1) + (double)((double) e / (double)d);
		
		//x = quantifier(e);

		digitalize(e, s1, s2);		

		fr(2 * i) = fr(2 * i) + s1;
		fr(2 * i + 1) = fr(2 * i + 1) + s2;
		

		//fr(2 * i) = fr(2 * i) + x;
		//fr(2 * i + 1) = fr(2 * i + 1) + x;
		
		//sum_d += e;
		//double_d += (double)((double) e / (double)d);
		//if(e <= -150)
		//	cout << e << " ";
		
		double_d += x;
		if(e < 0) e = -e;
		sum_d += e;
	}
	
	return;
}

void signal_composer::digitalize(short& m, double& s1, double& s2)
{
	short n;
	int id;

	if(m > 0)
	{	
		if(m >= 2800)
			n = 3500;
		else if(m >= 125)
			n = (m / 100) * 100 + 50;
		else if(m >= 100)
			n = 100;
		else
			n = (m / 25) * 25 + 25;
	}
	else 
	{
		if(m <= -2800)
			n = -3500;
		else if(m <= -125)
			n = (m / 100) * 100 - 50;
		else if(m <= -100)
			n = -100;
		else
			n = (m / 25) * 25 - 25 ;
	}
	
	int i, j;
	double pa[4] = {0.1, 0.3, 0.5, 0.7};
	double pna[8] = {-0.7, -0.5, -0.3, -0.1, 0.1, 0.3, 0.5, 0.7};
	bool isP = true;

	if(n < 0)
	{
		n = -n;
		isP = false;
	}
	if(n > 0)
	{
		if(n < 150)
			id = n / 25 - 1;
		else if(n < 2800)
			id = n / 100 + 3;
		else
			id = 31;
		
		i = id % 4;
		j = id / 4;
		s1 = pa[i];
		s2 = pna[j];
	}
	if(!isP)
		s1 = -s1;
	return;
}

double signal_composer::quantifier(short m)
{
	double ret;
	int n;

	if(m > 0)
	{	
		if(m >= 2000)
			n = 2225;
		else if(m >= 100)
			n = (m / 100) * 100;
		else 
			n = (m / 50) * 50;
	}
	else 
	{
		if(m <= -2000)
			n = -2225;
		else if(m <= -100)
			n = (m / 100) * 100;
		else 
			n = (m / 50) * 50;
	}

	if(n >= 0)
	{
		if(n < 100)
			ret = 0.1 * (n / 50);
		else if(n < 2000)
			ret = ((n / 100) + 1) * 0.1;
		else
			ret = 2.1;
	}
	else 
	{
		if(n > -100)
			ret = 0.1 * (n / 50);
		else if(n > -2000)
			ret = ((n / 100) - 1) * 0.1;
		else if(n < 0)
			ret = -2.1;

	}

	return ret;
}
