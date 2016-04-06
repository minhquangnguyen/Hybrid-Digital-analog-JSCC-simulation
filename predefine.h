#include <itpp/itcomm.h>
#include <iostream>
#include <bitset>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
using namespace itpp;
using namespace std;
#define BS 8
#define RFS 320 // Frame size in raw file (bytes)
#define CE 16768
#define TH 200
#define WL 18 // Word length after convolutional code
class map
{
public:
	int mt[256];
	int de_mt[256];
	map(char*);
	unsigned char en_map(unsigned char c);
	unsigned char de_map(unsigned char c);
};

class encode
{
private:
	map* _mp;
public:
	encode();
	char bvec2char(bvec &bv, int pos);
	void char2vec(char t, bvec &v);
	int read_frame(bvec &bv, FILE* f, int i);
	void encode_frame(FILE *fin, vec& c_symbols, int n, int size);
	void write_file(bvec &bv, FILE* f, int t);
};

class signal_composer
{
public:
	int sum_d;
	double double_d;
	int nn, pn;
	signal_composer()
	{
		sum_d = 0;
		double_d = 0;
		nn = pn = 0;
	}
	double quantifier(short s);
	void frame_composer(vec &fr, int pos, int length, FILE *f);
	void digitalize(short& i, double& s1, double& s2);
};

class decode
{
	Convolutional_Code code;
	map* _mp;
public:
	int sum_d;
	double double_d;
	decode();
	void decode_frame(vec& fr, bvec& ret, int size);
	void write_file(bvec &bv, FILE *out, int t);
	void write_file1(bvec &bv, FILE *out, int t);
	char bvec2char(bvec &bv, int pos);
	void error_calc(bvec &ipt, vec &signal, FILE *fec, int size);
	short quantifier(double s);
	void de_digitalize(short& i, double& s1, double& s2);
	void find_nearest(double& s);
};

class channel{
	double m, n;
	int error;
	int* vt0;
	int* vt1;
	int number_of_0, number_of_1;
public:
	channel(double _m, double _n);
	void count_a(vec &dt, int total);
	void transfer(double zee, double one, vec &dt, int total);
	void transfer(vec &dt, int n);
};
