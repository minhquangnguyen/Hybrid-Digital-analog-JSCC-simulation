#include <itpp/itcomm.h>
#include <iostream>
#include <bitset>

#include <iostream>
#include <fstream>
#include <stdio.h>
using namespace std;


int main(int argc, char* argv[])
{
	std::ifstream orf, rawf;
	std::ofstream out;
	orf.open(argv[1], std::ios::in | std::ios::binary);
	rawf.open(argv[2], std::ios::in | std::ios::binary);
	
	short f1, f2, f3;
	int i = 0, sum = 0;
	do
	{
		orf.read((char*)&f1, sizeof(short));
		rawf.read((char*)&f2, sizeof(short));
		if(!orf || !rawf)
			break;
		f3 = f1 - f2;
		if(f3 < 0)
			f3 = -f3;
		cout << f3 << " ";		
		sum += f3;
		i++;
	}
	while(!orf.eof() && !rawf.eof());
	double tb = (double)sum/(double)i;
	cout << "\ntb = " << tb << endl;
	orf.close();
	rawf.close();
	out.close();
	return 0;
}
