#include "predefine.h"
AWGN_Channel awgn_channel; 
bool is_uep = true;
ofstream debug_file1;
ofstream debug_file2;
int intd1, intd2;
FILE *fde;

int get_number_of_frame(FILE* f, int& number_of_byte)
{
	int nb[] = {13, 14, 16, 18, 20, 21, 27, 32};
	fseek(f, 0, SEEK_END);
	int int_temp, size = ftell(f) - 6;
	uint8_t c;
	fseek (f, 6, SEEK_SET);
	size_t is_r = fread(&c, 1, 1, f);
	if(is_r != 1) return -1; 
	if(c == 0x04)
		int_temp = 0;
	else if(c == 0x0C)
		int_temp = 1;
	else if(c == 0x14)
		int_temp = 2;
	else if(c == 0x1C)
		int_temp = 3;
	else if(c == 0x24)
		int_temp = 4;
	else if(c == 0x2C)
		int_temp = 5;
	else if(c == 0x34)
		int_temp = 6;
	else if(c == 0x3c)
		int_temp = 7;
	number_of_byte = nb[int_temp];
	size = size / number_of_byte;
	fseek (f, 0, SEEK_SET);
	return size;
}

double db(vec& a, vec& b, int n)
{
	double d = 0;
	for(int i = 0; i < n; i++)
	{
		d += a(i) - b(i);
	}
	return d;
}

int main(int argc, char* argv[])
{
	if(argc < 5)
	{
		cout << "Syntax: output transfered_file received_file error_preparation_file transmission_error_file is_uep" << endl;
		return 0;
	}
	
	if(argc == 6)
		is_uep = true;
	else 
		is_uep = false;
	
	debug_file1.open("rec.txt");
	debug_file2.open("tran.txt");
	intd1 = intd2 = 0;
	fde = fopen ("db.txt", "wb");

	encode e;
	decode d;
	channel _channel(0.02, 1);
	signal_composer sc;
	vec temp, received_symbols;
	bvec btemp, berror;

	FILE* fin = fopen (argv[1], "rb");
	FILE* fout = fopen (argv[2], "wb");
	FILE* ferr = fopen (argv[3], "rb");
	FILE* ferr1 = fopen (argv[4], "wb");

	
	char header[] = {0x23, 0x21, 'A', 'M', 'R', 0x0A};
	fwrite(&header, 6, 1, fout);
	fwrite(&header, 6, 1, fde);

	double db1 = 0, db2 = 0;
	vec bv_temp;
	int number_of_frame, number_of_byte, number_of_bit;
	number_of_frame = get_number_of_frame(fin, number_of_byte);
	number_of_bit = number_of_byte * 8;
	btemp.set_size(number_of_bit);

	
	//cout << number_of_frame << endl;
	for(int i = 0; i < number_of_frame; i++)
	{
		e.encode_frame(fin, temp, i, number_of_byte);
		
		bv_temp = temp;

		sc.frame_composer(temp, 320 * i, 160, ferr);
		
		//received_symbols = awgn_channel(temp);

		_channel.transfer(temp, temp.size());

		//_channel.transfer(0.05, 0.2, temp, temp.size());
		
		received_symbols = temp;

		//cout << db(received_symbols, temp, 468) << " ";

		db1 += db(received_symbols, temp, 160);

		d.decode_frame(received_symbols, btemp, number_of_byte);

		d.write_file(btemp, fout, number_of_bit);
		
		d.error_calc(btemp, received_symbols, ferr1, number_of_byte);

		db2 += db(received_symbols, bv_temp, 160);
	}
	cout << "integer error from transmiter: " << sc.sum_d << " and from receiver: " << d.sum_d << endl;
	cout << "double error: " << sc.double_d  << " and from receiver: " << d.double_d << endl;
	cout << "channel error: " << db1 << endl;
	cout << "total error: " << db2 << endl;
	cout << "H";
	cout << sc.pn - sc.nn << endl;
	
	fclose(fin);
	fclose(fout);
	fclose(ferr);
	fclose(ferr1);

	debug_file1.close();
	debug_file2.close();
	fclose(fde);
	
	return 0;
}
