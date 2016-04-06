#include "predefine.h"
extern bool is_uep;
extern ofstream debug_file1;
extern int intd1, intd2;
extern FILE *fde;

decode::decode()
{
	sum_d = 0;
	double_d = 0;
	_mp = new map((char *)"map.txt");
}

void decode::find_nearest(double& s)
{
	bool b = true;
	if(s < 0)
		b = false;
	int t, t1;
	t = s / 0.1;
	t1 = (s + 0.05) / 0.1;
	if(t == t1)
		s = t * 0.1;
	else
		s = t1 * 0.1;
	if(!b)
		s = -s;
	return;
}

// decode frame, result in size byte
void decode::decode_frame(vec& fr, bvec& ret, int size)
{
	int constraint_length, error = 0;
	ivec generators;
	vec temp_v, trans_symbols, rec_symbols;
	bvec decoded_bits;
	BPSK bpsk;

	generators.set_size(2, false);
	generators(0) = 0133;
	generators(1) = 0145;
	generators(2) = 0175;
	generators(3) = 0123;
	constraint_length = 2;

	code.set_generator_polynomials(generators, constraint_length);
	
	for(int i = 0; i < size; i++)
	{
		temp_v = fr(i * WL, i * WL + WL - 1);
		//cout << "decode size: " << temp_v.size() << endl;
		//temp_v = bpsk.demodulate_bits(fr(i * 36, i * 36 + 35));
		decoded_bits = code.decode(temp_v);
		//cout << decoded_bits << endl;
		ret.replace_mid(i * 8, decoded_bits);
	}
	//cout << "fn: " << ret << endl;
	return;
}

// write bv to file, t is length of bv. The number t must be divisible by 8.
void decode::write_file(bvec &bv, FILE *out, int t)
{
	char b;
	
	for(int j = 0; j < t/8; j++)
	{
		b = bvec2char(bv, j * 8);
		if(is_uep)		
			b = _mp->de_map(b); // for unequal error protection
		fwrite(&b, 1, 1, out);
	}
	
	return;
}

// For debug
void decode::write_file1(bvec &bv, FILE* f, int t)
{
	char b;
	for(int j = 0; j < t/8; j++)
	{
		b = bvec2char(bv, j * 8);
		fwrite(&b, 1, 1, f);
	}
	return;
}

// Convert from bvec to char, start at pos-th bit
char decode::bvec2char(bvec &bv, int pos)
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


void decode::error_calc(bvec &ipt, vec &signal, FILE *fec, int size)
{
	bvec bv_temp, coded_bits;
	BPSK bpsk;
	vec trans_symbols, c_symbols;
	c_symbols.set_size(size * WL);
	bv_temp.set_size(8);
	int n = ipt.size();
	n = n / 8;
	for(int i = 0; i < n; i++)
	{
		bv_temp = ipt(i * 8, i * 8 + 7);

		coded_bits = code.encode(bv_temp);

		bpsk.modulate_bits(coded_bits, trans_symbols);

		c_symbols.replace_mid(i * WL, trans_symbols);
	}
	short itemp;
	
	double e, e1, e2;

	intd1++;
	for(int i = 0; i < 160; i++)
	{
		e1 = signal(2 * i) - c_symbols(2 * i) - 0.02;
		e2 = signal(2 * i + 1) - c_symbols(2 * i + 1) - 0.02;

		de_digitalize(itemp, e1, e2);
		
		fwrite(&itemp, 2, 1, fec);

		if(itemp < 0) itemp = -itemp;
		sum_d += itemp;
		double_d += e;
	}
	return;
}
void decode::de_digitalize(short& m, double& s1, double& s2)
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

short decode::quantifier(double s)
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
