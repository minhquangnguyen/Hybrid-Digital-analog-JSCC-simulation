#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <string.h>
using namespace std;

unsigned short convert(unsigned short s)
{	
	short r = 0, st;
	unsigned short t[] = {0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 
							0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000};

	for(int i = 0; i < 16; i++)
	{
		if(s % 2 == 1)
			r = r ^ t[15 - i];
		s = s >> 1;
	}	
	return r;
}




//! Byte swap unsigned short
uint16_t swap_uint16( uint16_t val ) 
{
    return (val << 8) | (val >> 8 );
}

//! Byte swap short
int16_t swap_int16( int16_t val ) 
{
    return (val << 8) | ((val >> 8) & 0xFF);
}

int main(int argc, char* argv[])
{
	std::ifstream or_file, rev_file, dec_file;
	std::ofstream n_file;
	or_file.open(argv[1], std::ios::in | std::ios::binary);
	rev_file.open(argv[2], std::ios::in | std::ios::binary);
	dec_file.open(argv[3], std::ios::in | std::ios::binary);
	n_file.open(argv[4], std::ios::out | std::ios::binary);

	cout << "original file: " << argv[1] << " file1: " << argv[2] << " and file2: " << argv[3] << endl;

	unsigned short sh1, sh2, sh3, t2, t3, _sh1, _sh2, _sh3;
	int c1 = 0, c2 = 0;
	int cc;
	do
	{
		or_file.read((char*)&sh1, sizeof(short));
		rev_file.read((char*)&sh2, sizeof(short));
		dec_file.read((char*)&sh3, sizeof(short));

		if(!or_file || !rev_file || !dec_file)
			break;

		if(strcmp(argv[5], "-1") == 0)
		{
			_sh1 = swap_uint16(sh1);
			_sh2 = swap_uint16(sh2);
			_sh3 = swap_uint16(sh3);
		}
		else if(strcmp(argv[5], "-2") == 0)
		{
			_sh1 = convert(sh1);
			_sh2 = convert(sh2);
			_sh3 = convert(sh3);
		}
		else if(strcmp(argv[5], "-3") == 0)
		{
			_sh1 = sh1;
			_sh2 = sh2;
			_sh3 = sh3;
		}
		else
		{
			cout << "Please identify mode..." << endl;
			return 0;
		}

		t2 = _sh2 - _sh1;
		t3 = _sh3 - _sh1;
		
		if(t2 < 0) t2 = 0 - t2;
		if(t3 < 0) t3 = 0 - t3;
		
		cc = t2 * t3;
		if(cc < 0 || t2 < 0 || t3 < 0)
			cout << t2 << " " << t3 << " "; 

		if(t2 >= t3)
		{
			c1++;
			n_file.write(reinterpret_cast<const char*>(&sh3), sizeof(short));
		}
		else
		{
			c2++;
			n_file.write(reinterpret_cast<const char*>(&sh2), sizeof(short));
		}
	}
	while(or_file || rev_file || dec_file);
	
	or_file.close();
	rev_file.close();
	dec_file.close();
	n_file.close();

	cout << "\nc1 = " << c1 << endl;
	cout << "c2 = " << c2 << endl;
	return 0;
}

