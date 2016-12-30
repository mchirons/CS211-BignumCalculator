#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "format.h"




Formatter* FMCreate(int argc, char **argv){

	if (!isValidInput(argc, argv)){
		return NULL;
	}
	//isValidInput()
	Formatter* fm = (Formatter*)malloc(sizeof(Formatter));
	fm->bitString = argv[1];
	fm->sign = fm->bitString[0] - '0';

	fm->type = argv[2];
	
	if(strcmp(fm->type, "int") == 0){
		
		fm->integer = buildInt(fm->bitString);
		fm->output = intToDecASCII(fm->integer);
	}
	else{
		
		int sign;
		fm->exponent = getExponent(fm);
		fm->mantissa = getMantissa(fm);
		sign = getSign(fm);
		fm->floater = buildFloat(sign, fm->exponent, fm->mantissa);
		fm->output = floatToDecASCII(fm);
	}
	

	return fm;

}

int isValidInput(int argc, char ** argv){

	if (argv == NULL){
		printf("ERROR: No arguments\n");
		return 0;
	}

	if (argc != 3){
		printf("ERROR: Incorrect amount of arguments\n");
		return 0;
	}
	if (strlen(argv[1]) != 32){
		printf("ERROR: Bit sequence larger or smaller that 32 bit\n");
		return 0;
	}
	int i; 
	for (i = 0; i < strlen(argv[1]); i++){
		char c = argv[1][i];
		if (c != '0' && c != '1'){
			printf("ERROR: Incorrect characters in bit sequence\n");
			return 0;
		}
	}
	if (strcmp(argv[2], "int") != 0 && strcmp(argv[2], "float") != 0){
		printf("ERROR: Incorrect number type\n");
		return 0;
	}
	return 1;

}

int getSign(Formatter *fm){
	if (fm->bitString[0] == '1'){
		return -1;
	}
	else{
		return 1;
	}
}


int getExponent(Formatter* fm){

	int exponent = 0;
	char e[9];
	memcpy(e, &fm->bitString[1], 8);
	e[8] = '\0';
	


	if (strcmp(e, "11111111") == 0){			//check for Nan or Infinity
		exponent = -666;
		return exponent;
	}
	

	
	int mag = 1;
	int digit;
	int i;
	for (i = strlen(e) - 1; i >= 0; i--){
		digit = e[i] - '0';
		exponent = exponent + digit * mag;
		mag = mag * 2;
	}
	exponent = exponent - 127;
	
	if (strcmp(e, "00000000") == 0){			//check for 0 exponent
		exponent = -127;
	}

	return exponent;
}

float getMantissa(Formatter* fm){

	float mantissa = 1.0;
	char m[24];
	memcpy(m, &fm->bitString[9], 23);
	m[23] = '\0';
	

	if (strcmp(m, "00000000000000000000000") == 0){
		mantissa = 0.0; 
		return mantissa;
	}	

	mantissa = 1.0;
	float bit = 0;
	int i;
	float power = 2;
	int c;

	for (i = 0; i < strlen(m); i++){
		c = m[i] - '0';		
		
		bit = (c * 1.0) / power;
		
		mantissa = mantissa + bit;
		
		power = power * 2;
		
	}
	//printf("mantissa: time 10^7 %f\n", mantissa * 10e7);

	return mantissa;
}


int buildInt(char *num){
	int integer = 0;
	int mag = 1;
	int digit;
	int i;
	
	if (num[0] == '0'){
		for (i = strlen(num) - 1; i >= 0; i--){
		digit = num[i] - '0';
		
		integer = integer + digit * mag;
		
		mag = mag * 2;
		}
	}
	else if (num[0] == '1'){
		int carry = 1;
		//flip bits
		for (i = 0; i < strlen(num); i++){
			if (num[i] == '0'){
				num[i] = '1';
			}
			else{
				num[i] = '0';
			}
		}
		
		
		
		//add 1
		i = strlen(num) - 1;
		
		while (carry){
			if (num[i] == '1'){
				num[i] = '0';
				carry = 1;
			}
			else {
				num[i] = '1';
				carry = 0;
			}
			i--;
		}
		
		for (i = strlen(num) - 1; i >= 0; i--){
			digit = num[i] - '0';
			
			integer = integer + digit * mag;
			
			mag = mag * 2;
		}	
		integer = integer * -1;
	}
	else{}
	return integer;
}


float buildFloat(int sign, int exponent, float mantissa){
	int i;
	float e = 1.0;
	float floater;
	
	//Nan or inf
	if (exponent == -666){
		floater = 0.0;
		return floater;
	}
	//0.0
	if (mantissa == 0.0 && exponent == -420){
		floater = 0.0;
		return floater;
	}

	//if(exponent == 0){
	//	e = 1.0;
	//}
	
	if( exponent > 0){
		for(i = 0; i < exponent; i++){
			e = e * 2;
		}
	}
	else {
		for(i = 0; i < exponent * -1; i++){
			e = e * 2;
		}
		e = 1.0 / e;
	}
	if (exponent == -127){
		e = 1.0;
		mantissa = mantissa - 1.0; 
		for (i = 0; i < 126; i++){
			e = e * 2;
		}
		e = 1.0 / e;
	}
	

	//printf("mantissa: %f\n", mantissa);
	//printf("exponent: %f\n", e);

	
	floater = sign * e * mantissa;
	//floater = floater + 0.000001;
	
	//printf("float after build: %f\n", floater);

	return floater;
}

char* intToDecASCII(int x){
	char *s;
	if(x == 0){
		return "0";
	}
	int y = x;
	int size = 0;
	int isNegative = x < 0;
	while (y /= 10){
		size++;
	}
	size++;
	if (isNegative){
		size++;
		x = x * -1;
	}
	s = (char*)malloc(size + 1);
	s[size] = '\0';
	while (size--){
		s[size] = x % 10 + '0';
		x /= 10;
	}
	if (isNegative){
		s[0] = '-';
	}
	return s;
}


static void
get_float_digit( float x, char * digit, int * pow10, float * rem )
{
	int p10;
	
	if ( x == 0.0 )
	{
		*digit = '0';			// digit is always '0'
		*pow10 = 0;
		*rem = 0.0;
	}
	else
	{
		*pow10 = 0;			// start with 10^0
		while ( x >= 10.0 )		// reduce
		{
			x /= 10.0;		// large values get smaller
			*pow10 += 1;
		}
		while ( x < 1.0 )		// increase
		{
			x *= 10.0;		// small values get larger
			*pow10 -= 1;
		}
		*digit = '0';
		do {				// 10.0 > x >= 1.0
			x -= 1.0;
			*digit += 1;		// digit is always non-'0'zero
		} while ( x >= 1.0 );
		
		p10 = 0;
		while ( p10 > *pow10 )		// leading digit is now removed from x
		{
			x /= 10;
			p10 -= 1;
		}
		while ( p10 < *pow10 )
		{
			x *= 10;
			p10 += 1;
		}
		*rem = x;
	}
}


static void
append( char * s, char c )
{
	char		buf[2];

	buf[0] = c;
	buf[1] = '\0';
	strcat( s, buf );
}



char* floatToDecASCII( Formatter *fm )
{
	float x = 	fm->floater;
	char		c;
	int			pow10, p10, plast;
	int			i;
	float		rem;
	char		exponent[10];
	char* output;

	

	
	if (fm->exponent == -666 && fm->mantissa == 0.0 && fm->sign == 1){
		output = "-inf";
		return output;
	}
	else if (fm->exponent == -666 && fm->mantissa == 0.0 && fm->sign == 0){
		output = "+inf";
		return output;
	}
	else if (fm->exponent == -666 && fm->mantissa != 0.0 && fm->sign == 1){
		output = "-NaN";
		return output;
	}
	else if (fm->exponent == -666 && fm->mantissa != 0.0 && fm->sign == 0){
		output = "+Nan";
		return output;
	}
	else if (fm->mantissa == 0.0 && fm->sign == 1 ){
		output = "-0.0e0";
		return output;
	}
	else if (fm->mantissa == 0.0 && fm->sign == 0) {
		output = "0.0e0";
		return output;
	}
	else{
		//continue
	}

	
	output = (char*)malloc(sizeof(char));
	output[0] ='\0';
	if ( x < 0.0 )
	{
		append( output, '-' );
		x = -x;					// make x positive
	}
	get_float_digit( x, &c, &pow10, &rem );
	append( output, c );
	append( output, '.' );
	x = rem;
	plast = p10 = pow10;			// pow10 set by get_float_digit()
	for ( i = 1 ; i < 7 ; i++ )		// 7 significant digits in 32-bit float
	{
		
		get_float_digit( x, &c, &p10, &rem );
		if ( (plast - p10) > 1 )
		{
			append( output, '0' );	// fill in zero to next nonzero digit
			plast -= 1;
		}
		else
		{
			append( output, c );
			x = rem;
			plast = p10;
		}
	}

	if ( pow10 < 0 )		// negative exponent
	{
		exponent[0] = 'e';
		char *power = intToDecASCII(pow10);
		for (i = 0; i < strlen(power); i++){
			exponent[i + 1] = power[i];
		}
		exponent[i + 1] = '\0';
	}
	else if ( pow10 < 10 )		// positive single-digit exponent
	{
		exponent[0] = 'e';
		//exponent[1] = '+';
		//exponent[2] = '0';
		char *power = intToDecASCII(pow10);
		for (i = 0; i < strlen(power); i++){
			exponent[i + 1] = power[i];
		}
		exponent[i + 1] = '\0';
	}
	else				// positive multi-digit exponent
	{
		exponent[0] = 'e';
		exponent[1] = '+';
		char *power = intToDecASCII(pow10);
		for (i = 0; i < strlen(power); i++){
			exponent[i + 2] = power[i];
		}
		exponent[i + 2] = '\0';
	}
	strcat( output, exponent );
	return output;
}




int main(int argc, char **argv){

	Formatter* fm = FMCreate(argc, argv);
	if (fm != NULL){
		printf("%s\n", fm->output);
	}

	return 0;
}
