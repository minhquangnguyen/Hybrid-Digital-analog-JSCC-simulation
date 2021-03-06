#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <iomanip>
using namespace std;

void digitalize(short& m, double& s1, double& s2)
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

void de_digitalize(short& m, double& s1, double& s2)
{
	int i, j, id;
	short a[] = {25, 50, 75, 100, 150, 250, 350, 450, 550, 650, 750, 850, 950, 
				1050, 1150, 1250, 1350, 1450, 1550, 1650, 1750, 1850, 1950, 
				2050, 2150, 2250, 2350, 2450, 2550, 2650, 2750, 3500};
	bool isP = true;
	if(s1 < 0)
	{
		s1 = -s1;
		isP = false;
	}
	i = (s1 - 0.099) / 0.2;
	j = (s2 + 0.701) / 0.2;
	id = j * 4 + i;
	m = a[id];
	if(!isP)
		m = -m;
	return;	
}

void debug1(char* s1, char* s2, char* s3, char* s4)
{
	std::ifstream original;
	std::ofstream of, of1, of2;
	original.open(s1, std::ios::in | std::ios::binary);
	of.open(s2, std::ios::out | std::ios::binary);
	of1.open(s3, std::ios::out | std::ios::binary);
	of2.open(s4, std::ios::out | std::ios::binary);
	short f, f1, f2;
	
    std::srand(std::time(0));
    int random_variable, r;
		
	for(int i = 0; i < 44480; i++)
	{
		random_variable = std::rand() % 3;
		random_variable = random_variable - 1;
		r = std::rand() % 250;
		original.read((char*)&f, sizeof(short));
		f1 = f + random_variable * r;
		f2 = f + random_variable * (r + 1);
		of.write(reinterpret_cast<const char*>(&f), sizeof(short));
		of1.write(reinterpret_cast<const char*>(&f1), sizeof(short));
		of2.write(reinterpret_cast<const char*>(&f2), sizeof(short));
	}
	original.close();
	of.close();
	of1.close();
	of2.close();	
	return;
}

void debug3(char* s1, char* s2)
{
	std::ifstream f1;
	std::ofstream f2;
	f1.open(s1, std::ios::in | std::ios::binary);
	f2.open(s2, std::ios::out | std::ios::binary);	
	short m, n;
	int i = 0, d1 = 0, d2 = 0, d3 = 0;
	do
	{
		f1.read((char*)&m, sizeof(short));
		if(!f1)
			break;
		i++;
		if(m > 0)
			n = m - 1;
		f2.write(reinterpret_cast<const char*>(&n), sizeof(short));
	}
	while(!f1.eof());
	
	f1.close();
	f2.close();
}

void debug2(char* s1, char* s2)
{
	std::cout << std::setw(5);
	std::ifstream f1, f2;
	f1.open(s1, std::ios::in | std::ios::binary);
	f2.open(s2, std::ios::in | std::ios::binary);	
	short m, n;
	int i = 0, d1 = 0, d2 = 0, d3 = 0, d4 = 0, d5 = 0;
	do
	{
		f1.read((char*)&m, sizeof(short));
		f2.read((char*)&n, sizeof(short));
		cout << m << " ";
		if(!f1 || !f2)
			break;
		i++;
		if(m * n == 0)
			d5++;
		if((m < 0 && n > 0) || (m > 0 && n < 0))
			d1++;
		if(n > 0 && m > n)
			d2++;
		if(n < 0 && n > m)
			d3++;
		if(m == n)
			d4++;			
	}
	while(!f1.eof() && !f2.eof());
	cout << endl << "Total: " << i << endl;
	cout << "d1: " << d1 << endl;
	cout << "d2: " << d2 << endl;
	cout << "d3: " << d3 << endl;
	cout << "Equal: " << d4 << endl;
	cout << "One of m, n is 0: " << d5 << endl;
	f1.close();
	f2.close();
}

void debug6(char* s1, char* s2)
{
	std::ifstream f1, f2;
	f1.open(s1, std::ios::in | std::ios::binary);
	f2.open(s2, std::ios::in | std::ios::binary);	
	short m, n;
	int i = 0, d1 = 0, d2 = 0, d3 = 0, d4 = 0, d5 = 0;
	do
	{
		
		f1.read((char*)&m, sizeof(short));
		f2.read((char*)&n, sizeof(short));
		if(!f1 || !f2)
			break;
		i++;
		if(m != n)
		{
			cout << m << " " << n << " ";
			cout << i << endl;
		}		
	}
	while(!f1.eof() && !f2.eof());
	f1.close();
	f2.close();
}


short quantifier(double s)
{
	short ret;
	bool b = true;
	int t, t1;
	if(s < 0)
	{
		b = false;
		s = -s;
	}
	t = s / 0.1;
	t1 = (s + 0.05)/0.1;
	if(t == t1 - 1)
		t = t1;
	if(t == 0)
		ret = 0;
	else if(t == 1)
		ret = 50;
	else if(t == 21)
		ret = 2225;
	else 
		ret = (t - 1) * 100;

	if(!b)
		ret = -ret;
	return ret;		
}

double quantifier(short m)
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

void debug4(char* s1, char* s2)
{
	std::ifstream f1;
	std::ofstream f2;
	f1.open(s1, std::ios::in | std::ios::binary);
	f2.open(s2, std::ios::out | std::ios::binary);	
	short m, n, n1;
	double s, _s1 = 0, _s2 = 0, _s;
	int i = 0, d1 = 0, d2 = 0, d3 = 0;
	do
	{
		f1.read((char*)&m, sizeof(short));
		if(!f1)
			break;
		i++;
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

		digitalize(m, _s1, _s2);
		de_digitalize(n1, _s1, _s2);
		if(n != n1)
		{
			cout << "\ns1 = " << _s1 << " s2 = " << _s2 <<  " _s =" << _s << endl;
			cout << "n = " << n << " n1 = " << n1 << " m = " << m << " s = " << s << endl;
		}
		
		f2.write(reinterpret_cast<const char*>(&n), sizeof(short));
	}
	while(!f1.eof());
	
	f1.close();
	f2.close();
}



int main(int argc, char* argv[])
{
	debug4(argv[1], argv[2]);
	/*short m = 168, n;
	double s1, s2;
	digitalize(m, s1, s2);
	cout << s1 << " v " << s2 << endl;
	de_digitalize(n, s1, s2);
	cout << "n " << n << endl;*/
	//cout << "s: " << s << "\ts1: " << s1 << "\ts2: " << s2 << endl;

	/*for(int i = -4; i <= 4; i++)
	{
		for(int j = -4; j <= 4; j++)
		{
			s1 = 0.2 * i;
			s2 = 0.2 * j;
			de_digitalize(s, s1, s2);
			cout << "s: " << s << "\ts1: " << s1 << "\ts2: " << s2 << endl;
		}
	}*/
	//de_digitalize(s, s1, s2);

	//cout << "\ns1 = " << s1 << " s2 = " << s2 << endl;
	return 0;
}
