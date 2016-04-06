#include <iostream>
#include <fstream>
#include <stdio.h>
using namespace std;
void error_calculate(char* s1, char* s2, char* s3)
{
	std::ifstream orf, rawf;
	std::ofstream out;
	orf.open(s1, std::ios::in | std::ios::binary);
	rawf.open(s2, std::ios::in | std::ios::binary);
	out.open(s3, std::ios::out | std::ios::binary);
	short f1, f2, f3;
	int sum = 0;
	int i = 0;
	size_t rd1, rd2;
	do
	{
		orf.read((char*)&f1, sizeof(short));
		rawf.read((char*)&f2, sizeof(short));
		if(!orf || !rawf)
			break;
		f3 = f1 - f2;
		sum += f3;
		i++;
		out.write(reinterpret_cast<const char*>(&f3), sizeof(short));
		//cout << f3 << " " << endl;
	}
	while(!orf.eof() && !rawf.eof());
	cout << "Sum = " << sum << " and i = " << i << endl;
	orf.close();
	rawf.close();
	out.close();
	return;
}

int main(int argc, char* argv[])
{
	error_calculate(argv[1], argv[2], argv[3]);
	return 0;
}
