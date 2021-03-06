#include "predefine.h"
#include <random>
#include <ctime>
#include <cstdlib>

#define N 40

channel::channel(double _m, double _n)
{
	vt0 = new int[N*32];
	vt1 = new int[N*32];
	error = 0;
	m = _m; 
	n = _n;
}

// count number of 1 and -1 in vector
void channel::count_a(vec &dt, int total)
{
	memset(vt0, 0xff, N*128);
	memset(vt1, 0xff, N*128);
	int p0, p1;
	number_of_1 = p0 = p1 = 0;
	for(int i = 0; i < total; i++)
	{
		if(dt[i] == -1)
		{
			vt1[p1] = i;
			p1++;
		}
		else
		{
			vt0[p0] = i;
			p0++;
		}
	}
	number_of_0 = p0;
	number_of_1 = p1;
	return;
}


//transfer vector dt over channel
void channel::transfer(double zee, double one, vec &dt, int total)
{
	count_a(dt, total);
	int n1 = zee * number_of_0;
	int n2 = one * number_of_1;
	
	error = error + n1 + n2;
	
	//cout << zee << endl;
	//cout << one << endl;
	
	//cout << endl << "n00: " << number_of_0 << " n1: " << n1 << " n01: ";
	//cout << number_of_1 << " n2: " << n2 << endl;
	
	int t, t1, t2;
	int bt;
	
	//cout << "Error: " << n1 + n2 << endl;
	for(int i = 0; i < n1; i++)
	{
		t = rand() % number_of_0;
		t = vt0[t];
		dt[t] = -1;
	}
	
	for(int i = 0; i < n2; i++)
	{
		t = rand() % number_of_1;
		t = vt1[t];
		dt[t] = 1;
	}
	return;
}

// Gaussian noise
void channel::transfer(vec &dt, int l)
{
	srand (time(NULL));
	std::default_random_engine generator;
	std::normal_distribution<double> distribution(m, n);
	double d;
	for(int i = 0; i < l; i++)
	{
		d = distribution(generator);
		dt(i) = dt(i) + d;
	}
	return;
}
