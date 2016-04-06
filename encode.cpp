#include "predefine.h"
extern bool is_uep;
encode::encode()
{
	_mp = new map((char *)"map.txt");
	return;
}


char encode::bvec2char(bvec &bv, int pos)
{
	char ret = 0;
	int a[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
	if(pos > bv.length() - 8)
	{
		cout << "Can sua loi nay";
		return 0;
	}
	for(int i = pos; i < pos + 8; i++)
	{
		if(bv[i] == 1)
			ret = ret | a[7 - i + pos];
	}
	return ret;
}


void encode::char2vec(char t, bvec &v)
{
	//can xem lai
	int i = (int) t;
	v = itpp::dec2bin(8, i);
	return;
}

// doc tu byte thu i, dua ra vector bv. So luong bit doc: BS 
int encode::read_frame(bvec &bv, FILE* f, int i)
{
	bvec a;
	
	int total = 0, i_read, pos = 0;
	
	char c;

	int temp = i/8;
	
	fseek(f, temp, SEEK_SET);
	
	i_read = fread(&c, 1, 1, f);

	if(is_uep)
		c = _mp->en_map(c); // for unequal error protection
	
	while(i_read)
	{
		total += i_read * 8;
		
		a = itpp::dec2bin(i_read * 8, (int) c);
		
		bv.replace_mid(pos, a);
		
		pos = pos + a.length();
		
		i_read = fread(&c, 1, 1, f);

		if(is_uep)
			c = _mp->en_map(c); // for unequal error protection
		
		if(total == BS)
			break;		
	};
	return total;
}

void encode::write_file(bvec &bv, FILE* f, int t)
{
	char b;
	
	for(int j = 0; j < t/8; j++)
	{
		b = bvec2char(bv, j * 8);
		fwrite(&b, 1, 1, f);
	}
	
	return;
}

// encode size byte of n-th frame in file fin, result in c_symbols, 
void encode::encode_frame(FILE *fin, vec& c_symbols, int n, int size)
{
	int constraint_length, error = 0;
	double Ec, Eb, error_rate, _BER;
	
	ivec generators;
	vec trans_symbols;
	c_symbols.set_size(size * WL); //the number here is flexible 
	bvec coded_bits;
	BPSK bpsk;
	
	generators.set_size(2, false); // ban dau la generators.set_size(4, false)
	generators(0) = 0133;
	generators(1) = 0145;
	generators(2) = 0175;
	generators(3) = 0123;
	constraint_length = 2; // ban dau = 2
	Convolutional_Code code;
	code.set_generator_polynomials(generators, constraint_length);
	
	
	bvec bv;
	bv.set_size(BS);
	
	//FILE* fin = fopen (s1, "rb");
	
	int i = 48 + n * size * BS, t = 0, pos = 0;
	
	bv.clear();

	t = read_frame(bv, fin, i);

	//cout << size << " Encode: " << bv << endl;
	
	coded_bits = code.encode(bv); //The convolutional encoder function.
	
	bpsk.modulate_bits(coded_bits, trans_symbols); //The BPSK modulator.

	c_symbols.replace_mid(pos, trans_symbols);

	pos = pos + WL;

	i = i + t;
	
	for(int _i = 0; _i < size - 1; _i++)//while(t > 0 && i < 104)
	{
		t = read_frame(bv, fin, i);

		//cout << "Encode: " << bv << endl;
		
		i += t;
		
		coded_bits = code.encode(bv); //The convolutional encoder function.
	
		bpsk.modulate_bits(coded_bits, trans_symbols); //The BPSK modulator.

		//cout << " trans_symbols size: " << trans_symbols.size() << endl;
	
		c_symbols.replace_mid(pos, trans_symbols);

		pos = pos + WL;
	}
	//cout << "size: " << c_symbols.size() << endl;
	
	return;
}

/*int main(int argc, char* argv[])
{
	cout << "Sending " << argv[1];
	
	transfer(argv[1]);
	
	cout << "Finished." << endl;
	
	return 0;
}*/
