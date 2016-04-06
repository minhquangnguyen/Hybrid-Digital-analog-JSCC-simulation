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
	out.open(argv[3], std::ios::out | std::ios::binary);
	short f1, f2, f3;
	int i = 0, sum = 0;
	do
	{
		orf.read((char*)&f1, sizeof(short));
		rawf.read((char*)&f2, sizeof(short));
		if(!orf || !rawf)
			break;
		f3 = f1 + f2;
		sum += f2;
		i++;
		out.write(reinterpret_cast<const char*>(&f3), sizeof(short));
		//cout << f3 << endl;
	}
	while(!orf.eof() && !rawf.eof());
	cout << "Sum = " << sum << " and i = " << i << endl;
	orf.close();
	rawf.close();
	out.close();
	return 0;
}
