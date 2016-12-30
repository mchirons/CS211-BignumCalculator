#ifndef CALC_H_
#define CALC_H_


struct BigInt{

	int k;
	int *array;

};

typedef struct BigInt BigInt;


struct Calculator{

	char *op;
	char *base;
	char *num1;
	char *num2;
	int num1isNegative;
	int num2isNegative;
	char sign;
	BigInt *result;

};

typedef struct Calculator Calculator;


Calculator* CalcCreate(int argc, char **argv);


BigInt* calculate(Calculator* calculator);

BigInt* add(BigInt *a, BigInt *b);

BigInt* subtract(BigInt *a, BigInt *b);

BigInt* multiply(BigInt *a, BigInt *b);

BigInt* divide(BigInt *a, BigInt* b);

BigInt* binASCIIToBigInt(char *num);

BigInt* decASCIIToBigInt(char *num);

BigInt* octASCIIToBigInt(char *num);

BigInt* hexASCIIToBigInt(char *num);

BigInt* getBigInt(char *num);

BigInt* BigIntCreate(int size);

BigInt* clearBigInt(BigInt* bigint);

BigInt* trim(BigInt *bigint);

char * resultToBinASCII(Calculator *calculator);

char * resultToDecASCII(Calculator *calculator);

char * resultToOctASCII(Calculator *calculator);

char * resultToHexASCII(Calculator *calculator);

int isLessThanOrEqual(BigInt *a, BigInt *b);

void freeBigInt(BigInt *bigint);

void freeCalculator(Calculator *calculator);

int isZero(BigInt *bigint);

int isValidInput(int argc, char **argv);

int isoctaldigit(char c);


#endif //CALC_H_
