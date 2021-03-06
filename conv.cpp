#include "predefine.h"
#define L 50

int get_number_of_frame(FILE* f)
{
	fseek(f, 0, SEEK_END);
	int size = ftell(f) - 6;
	//cout << size << endl;
	uint8_t c;
	fseek (f, 6, SEEK_SET);
	size_t st = fread(&c, 1, 1, f);
	if(st == -1)
		return st;
	if(c == 0x04)
		size = size / 13;
	else if(c == 0x0C)
		size = size / 14;
	else if(c == 0x14)
		size = size / 16;
	else if(c == 0x1C)
		size = size / 18;
	else if(c == 0x24)
		size = size / 20;
	else if(c == 0x2C)
		size = size / 21;
	else if(c == 0x34)
		size = size / 27;
	else if(c == 0x3c)
		size = size / 32;

	cout << " " << (int) c << endl;
	return size;
}

void char2vec(char t, bvec &v)
{
	//can xem lai
	int i = (int) t;
	v = itpp::dec2bin(8, i);
	return;
}

void chararay2vec(char* t, bvec &v, int n)
{
	int temp;
	bvec b;
	for(int i = 0; i < n; i++)
	{
		temp = (int) t[i];
		//cout << temp << endl;
		b = itpp::dec2bin(8, temp);
		//cout << b << endl;
		v.replace_mid(i * 8, b);
	}
	return;
}

void init_bvec(bvec &v, int n, char t)
{
	int temp = (int) t;
	bvec b;
	for(int i = 0; i < n; i++)
	{
		b = itpp::dec2bin(8, temp);
		v.replace_mid(i * 8, b);
	}
	return;
}

int main(int argc, char* argv[])
{
//Scalars
	int MaxNrofErrors, Nobits, MaxIterations, p, i;
	double Ec, Eb;

//Vectors
	ivec generators;
	vec EbN0dB, EbN0, N0, ber, trans_symbols, rec_symbols;
	bvec uncoded_bits, coded_bits, decoded_bits;
	
//Classes
	BPSK bpsk;
	BERC berc;
	Convolutional_Code conv_code;
	double f1, f2;
	sscanf(argv[1], "%lf", &f1);
	sscanf(argv[2], "%lf", &f2);
	channel _channel(f1, f2);
	
	generators.set_size(2, false);
	generators(0) = 0133;
	generators(1) = 0145;
	//generators(2) = 0175;
	//generators(3) = 0123;
	int constraint_length = 2;
	conv_code.set_generator_polynomials(generators, constraint_length);


	Ec = 1.0;
	EbN0dB = linspace(-2, 6, 5);
	EbN0 = inv_dB(EbN0dB);
	Eb = Ec / conv_code.get_rate();
	N0 = Eb * pow(EbN0, -1);
	MaxNrofErrors = 100;
	Nobits = 8;
	MaxIterations = 100;
	char t[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	char t1[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00};
	RNG_randomize();
	uncoded_bits.set_size(L * 8);
	int error_sum0 = 0, error_sum1 = 0;
	for (i = 0; i < MaxIterations; i++) 
	{
		berc.clear();
		//uncoded_bits = (0, 0, 0, 0, 0, 0, 0, 0);//randb(Nobits); 
		//chararay2vec(t, uncoded_bits, 10);
		init_bvec(uncoded_bits, L, 0x00);
		//cout << uncoded_bits << endl;
		coded_bits = conv_code.encode(uncoded_bits);
		//cout << coded_bits << " ";
		bpsk.modulate_bits(coded_bits, trans_symbols);
		//cout << trans_symbols << endl;

		_channel.transfer(trans_symbols, trans_symbols.size());
		//cout << trans_symbols << endl;
		decoded_bits = conv_code.decode(trans_symbols);
		berc.count(uncoded_bits, decoded_bits);
		//cout << decoded_bits << " Error: " << berc.get_errorrate() * L * 8 << endl;
		error_sum0 += berc.get_errorrate() * L * 8;
	}


	for (i = 0; i < MaxIterations; i++) 
	{
		berc.clear();
		//uncoded_bits = (0, 0, 0, 0, 0, 0, 0, 0);//randb(Nobits); 
		//chararay2vec(t, uncoded_bits, 10);
		init_bvec(uncoded_bits, L, 0xff);
		//cout << uncoded_bits << endl;
		coded_bits = conv_code.encode(uncoded_bits);
		//cout << coded_bits << " ";
		bpsk.modulate_bits(coded_bits, trans_symbols);
		//cout << trans_symbols << endl;

		_channel.transfer(trans_symbols, trans_symbols.size());
		//cout << trans_symbols << endl;
		decoded_bits = conv_code.decode(trans_symbols);
		berc.count(uncoded_bits, decoded_bits);
		//cout << decoded_bits << " Error: " << berc.get_errorrate() * L * 8 << endl;
		error_sum1 += berc.get_errorrate() * L * 8;
	}
	cout << "Error sum 0: " << error_sum0 << " with rate: " << ((double)error_sum0/(double)(L * 8 * MaxIterations)) << endl;
	cout << "Error sum 1: " << error_sum1 << " with rate: " << ((double)error_sum1/(double)(L * 8 * MaxIterations)) << endl;
	return 0;
}

