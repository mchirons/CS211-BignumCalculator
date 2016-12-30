#ifndef FORMAT_H_
#define FORMAT_H_

struct Formatter_ {

	char *bitString;
	char *type;
	char *output;
	int integer;
	float floater;
	float mantissa;
	int exponent;
	int sign;

};

typedef struct Formatter_ Formatter;

Formatter* FMCreate(int argc, char **argv);

int isValidInput(int argc, char ** argv);

int buildInt(char *num);

float buildFloat(int sign, int exponent, float mantissa);

char* intToDecASCII(int x);

char* floatToDecASCII(Formatter *fm);

int getSign(Formatter *fm);

int getExponent(Formatter *fm);

float getMantissa(Formatter *fm);

#endif //FORMAT_H_