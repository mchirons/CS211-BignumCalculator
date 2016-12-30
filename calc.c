
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "calc.h"


static BigInt* remainder;

Calculator* CalcCreate(int argc, char ** argv){

	if (!isValidInput(argc, argv)){
		return NULL;
	}

	Calculator *calculator = (Calculator*)malloc(sizeof(Calculator));
	
	calculator->op = argv[1];
	calculator->num1 = argv[2];
	calculator->num2 = argv[3];
	calculator->base = argv[4];

	if (calculator->num1[0] == '-'){
		calculator->num1isNegative = 1;
		memmove(calculator->num1, calculator->num1 + 1, strlen(calculator->num1));
	}
	else{
		calculator->num1isNegative = 0;
	}
	if (calculator->num2[0] == '-'){
		calculator->num2isNegative = 1;
		memmove(calculator->num2, calculator->num2 + 1, strlen(calculator->num2));
	}
	else{
		calculator->num2isNegative = 0;
	}

	calculator->result = calculate(calculator);


	return calculator;
}


int isValidInput(int argc, char ** argv){
	
	if(argv == NULL){
		printf("ERROR: No arguments\n");
		return 0;
	}
	if(argc != 5){
		printf("ERROR: Incorrect amount of arguments\n");
		return 0;
	}

	char *operator = argv[1];
	char *operand1 = argv[2];
	char *operand2 = argv[3];
	char *base = argv[4];

	//check operator
	if (strcmp(operator, "+") != 0 && strcmp(operator, "-") != 0 && 
		strcmp(operator, "*") != 0 && strcmp(operator, "/") != 0 ){
		printf("ERROR: Invalid symbol for operator\n");
		return 0;
	}
	//check first operand
	if (operand1[0] == '-'){
		if (strlen(operand1) < 3){
			printf("ERROR: Invalid first operand\n");
			return 0;
		}
		if (operand1[1] != 'b' && operand1[1] != 'd' 
		&& operand1[1] != 'o' && operand1[1] != 'x' ){
			printf("ERROR: Invalid first operand\n");
			return 0;
		}
	}
	else {	
		if (strlen(operand1) < 2){
			printf("ERROR: Invalid first operator\n");
			return 0;
		}
		if (operand1[0] != 'b' && operand1[0] != 'd' 
		&& operand1[0] != 'o' && operand1[0] != 'x' ){
			printf("ERROR: Invalid first operand\n");
			return 0;
		}
	}
	//check second operand
	if (operand2[0] == '-'){
		if (strlen(operand2) < 3){
			printf("ERROR: Invalid second operand\n");
			return 0;
		}
		if (operand2[1] != 'b' && operand2[1] != 'd' 
		&& operand2[1] != 'o' && operand2[1] != 'x' ){
			printf("ERROR: Invalid second operand\n");
			return 0;
		}
	}
	else {
		if (strlen(operand2) < 2){
			printf("ERROR: Invalid second operand\n");
			return 0;
		}
		if (operand2[0] != 'b' && operand2[0] != 'd' 
		&& operand2[0] != 'o' && operand2[0] != 'x' ){
			printf("ERROR: Invalid second operand\n");
			return 0;
		}
	}
	//check base
	if (strcmp(base, "b") != 0 && strcmp(base, "d") != 0 &&
		strcmp(base, "o") != 0 && strcmp(base, "x") != 0 ){
		printf("ERROR: Invalid base\n");
		return 0;
	}
	return 1;
}	


BigInt* calculate(Calculator *calculator){

	int num1isNegative = calculator->num1isNegative;
	int num2isNegative = calculator->num2isNegative;

	BigInt *num1 = getBigInt(calculator->num1);
	num1 = trim(num1);

	BigInt *num2 = getBigInt(calculator->num2);
	num2 = trim(num2);

	BigInt *result = NULL;

	if (calculator->op[0] == '+'){
		if (!num1isNegative && !num2isNegative){
			result = add(num1, num2);
			calculator->sign = '+';
		}
		else if (!num1isNegative && num2isNegative){
			result = subtract(num1, num2);
			if (isLessThanOrEqual(num1, num2)){
				calculator->sign = '-';
			}
			else{
				calculator->sign = '+';
			}
		}
		else if (num1isNegative && !num2isNegative){
			result = subtract(num2, num1);
			if (isLessThanOrEqual(num2, num1)){
				calculator->sign = '-';
			}
			else {
				calculator->sign = '+';
			}
		}
		else if (num1isNegative && num2isNegative){
			result = add(num1, num2);
			calculator->sign = '-';
		}
		else{}
		
	}
	else if (calculator->op[0] == '-'){
		if (!num1isNegative && !num2isNegative){
			result = subtract(num1, num2);
			if (isLessThanOrEqual(num1, num2)){
				calculator->sign = '-';
			}
			else {
				calculator->sign = '+';
			}
		}
		else if (!num1isNegative && num2isNegative){
			result = add(num1, num2);
			calculator->sign = '+';
		}
		else if (num1isNegative && !num2isNegative){
			result = add(num1, num2);
			calculator->sign = '-';
		}
		else if (num1isNegative && num2isNegative){
			result = subtract(num2, num1);
			if(isLessThanOrEqual(num1, num2)){
				calculator->sign = '+';
			}
			else{
				calculator->sign = '-';
			}
		}
		else{}
	}
	else if (calculator->op[0] == '*'){
		result = multiply(num1, num2);
		if (!num1isNegative && !num2isNegative){
			calculator->sign = '+';
		}
		else if (!num1isNegative && num2isNegative){
			calculator->sign = '-';
		}
		else if (num1isNegative && !num2isNegative){
			calculator->sign = '-';
		}
		else if (num1isNegative && num2isNegative){
			calculator->sign = '+';
		}
		else{}
	}
	else if (calculator->op[0] == '/'){
		result = divide(num1, num2);
		if (!num1isNegative && !num2isNegative){
			calculator->sign = '+';
		}
		else if (!num1isNegative && num2isNegative){
			calculator->sign = '-';
		}
		else if (num1isNegative && !num2isNegative){
			calculator->sign = '-';
		}
		else if (num1isNegative && num2isNegative){
			calculator->sign = '+';
		}
		else{}
	}
	result = trim(result);
	freeBigInt(num1);
	freeBigInt(num2);
	return result;
}


BigInt* getBigInt(char *num){

	BigInt *newInt;
	if (num[0] == 'b'){
		memmove(num, num + 1, strlen(num));
		newInt = binASCIIToBigInt(num);
	}
	if (num[0] == 'd'){
		memmove(num, num + 1, strlen(num));
		newInt = decASCIIToBigInt(num);
	}
	if (num[0] == 'o'){
		memmove(num, num + 1, strlen(num));
		newInt = octASCIIToBigInt(num);
	}
	if (num[0] == 'x'){
		memmove(num, num + 1, strlen(num));
		newInt = hexASCIIToBigInt(num);
	}

	return newInt;
}

BigInt* BigIntCreate(int size){
	BigInt *bigint = (BigInt*)malloc(sizeof(BigInt));
	bigint->array = (int*)calloc(size, sizeof(int));
	bigint->k = size;

	return bigint;

}

//need to make this work for numbers of different sizes
BigInt* add(BigInt *a, BigInt *b){
	int i;
	int carry = 0;;
	int sum = 0;
	int size = (a->k > b->k) ? a->k + 1 : b->k + 1;

	BigInt* result = BigIntCreate(size);
	int j = 1;	

	for (i = 0; i < size - 1; i++, j++){
		if (a->k - j < 0){
			sum = carry + b->array[b->k - j];
		}
		else if (b->k - j < 0){
			sum = carry + a->array[a->k - j];
		}
		else{
			sum = a->array[a->k - j] + carry + b->array[b->k - j];
		}
		if (sum > 9){
			sum = sum % 10;
			carry = 1;
		}
		else {
			carry = 0;
		}
		result->array[size - j] = sum;
	}
	if (carry){
		result->array[size - j] = carry;
	}
	result = trim(result);
	
	return result;
}

BigInt *subtract(BigInt* a, BigInt *b){

	BigInt* result;
	if (isZero(a)){
		return b;
	}
	if (isZero(b)){
		return a;
	}

	
	int size = (a->k > b->k) ? a->k : b->k;
	int stop = (a->k > b->k) ? b->k : a->k;
	int aIsLonger = (a->k > b->k);
	int bIsLonger = (b->k > a->k);

	result = BigIntCreate(size);

	int i; 		//for loop iteration
	int j = 1;	//tracks significant digit
	int sum;	//holds digit to be stored in result
	int s; 		//for searching 


	
	//a > b
	if (!(isLessThanOrEqual(a, b))){
		for (i = 0; i < stop; i++, j++){
			if (b->array[b->k - j] > a->array[a->k - j]){
			//search for digit to borrow from
				s = j + 1;
				while ((a->k - s) >= 0){
					if(a->array[a->k - s] != 0){
						a->array[a->k - s]--;
						break;
					}
					else{
						a->array[a->k - s] = 9;
					}
					s++;
				}
				sum = a->array[a->k - j] + 10 - b->array[b->k - j];
			}
			else{
				sum = a->array[a->k - j] - b->array[b->k - j];
			}
		result->array[result->k - j] = sum;
		}
	}
	//a <= b so I flip a and b
	else{
		for (i = 0; i < stop; i++, j++){

		//b digit is greater than a digit
		if (a->array[a->k - j] > b->array[b->k - j]){
			//search for digit to borrow from
			s = j + 1;
			while ((b->k - s) >= 0){
				if(b->array[b->k - s] != 0){
					b->array[b->k - s]--;
					break;
				}
				else{
					b->array[b->k - s] = 9;
				}
				s++;
			}
			sum = b->array[b->k - j] + 10 - a->array[a->k - j];
		}
		else{
			sum = b->array[b->k - j] - a->array[a->k - j];
		}
		result->array[result->k - j] = sum;
		}
	}
	if (aIsLonger){
		while ((a->k - j) >= 0){
			sum = a->array[a->k - j];
			result->array[result->k - j] = sum;
			j++;
		}
	}
	else if (bIsLonger){
		while ((b->k - j) >= 0){
			sum = b->array[b->k - j];
			result->array[result->k - j] = sum;
			j++;
		}
	}
	else{
		//nothing
	}

	return result;
}

BigInt* multiply(BigInt *a, BigInt *b){


	int i; 				//for looping through b
	int j;				//for looping through a
	int carry;			
	int product = 0;	//stores digits to be places in result

	//Holds the sum of addition at the end of a multiplication step
	BigInt* temp = BigIntCreate(a->k + b->k);

	//Holds the result of adding all the temps together
	BigInt* result = BigIntCreate(a->k + b->k);
				
	int index = temp->k - 1;	//starting index of each temp
	int counter = 0;			//for increasing magnitude of temp
	BigInt* copy;

	for (i = b->k - 1; i >= 0; i--){
		carry = 0;
		for (j = a->k - 1; j >= 0; j--){
			product = b->array[i] * a->array[j] + carry;
			if (product > 9){
				carry = product / 10;
				product = product % 10;
			}
			else{
				carry = 0;
			}
			temp->array[index--] = product;
		}
		if (carry){
			temp->array[index] = carry;
		}
		copy = result;
		result = add(result, temp);
		freeBigInt(copy);				
		temp = clearBigInt(temp);
		counter++;
		index = (temp->k -1) - counter;
	}
	freeBigInt(temp);
	result = trim(result);
	return result;
}
//for use in long division algorithm
BigInt* smallDivide(BigInt* a, BigInt *b){

	BigInt* result = BigIntCreate(a->k);
	BigInt* counter = BigIntCreate(1);
	counter->array[0] = 1;
	BigInt* temp;
	BigInt* copy;

	temp = BigIntCreate(a->k);
	copy = temp;
	temp = add(temp, b);
	freeBigInt(copy);
	while (isLessThanOrEqual(temp, a)){
		copy = temp;
		temp = add(temp, b);
		freeBigInt(copy);
		copy = result;
		result = add(result, counter);
		freeBigInt(copy);
	}
	freeBigInt(temp);
	freeBigInt(counter);
	result = trim(result);
	return result;
}

BigInt* divide(BigInt *a, BigInt *b){

	int i;
	BigInt* quotient = BigIntCreate(a->k);
	BigInt* divisor = b;
	BigInt* dividend = a;
	BigInt* temp;
	BigInt* times;
	BigInt* product;
	BigInt* ten = BigIntCreate(2);
	ten->array[0] = 1;
	ten->array[1] = 0;

	temp = BigIntCreate(1);
	temp->array[0] = dividend->array[0];
	

	for (i = 0; i < dividend->k; i++){
		times = smallDivide(temp, divisor);
		quotient->array[i] = times->array[0];
		product = multiply(divisor, times);
		temp = subtract(temp, product);
		remainder = temp;

		temp = multiply(temp, ten);
		temp->array[temp->k - 1] = dividend->array[i + 1];
	}
	freeBigInt(times);
	freeBigInt(ten);

	return quotient;


}

int isLessThanOrEqual(BigInt *a, BigInt *b){
	int i;

	if (a->k > b->k){
		return 0;
	}
	else if (a->k < b->k){
		return 1;
	}
	else{
		for (i = 0; i < a->k; i++){
			if(a->array[i] < b->array[i]){
				return 1;
			}
			if (a->array[i] > b->array[i]){
				return 0;
			}
			else{
				//continue;
			}
		}
		return 1;
	}
}

BigInt* decASCIIToBigInt(char *num){
	int l = strlen(num);
	int i;

	BigInt* bigint = BigIntCreate(l);

	BigInt* mag = BigIntCreate(l);
	mag->array[l - 1] = 1;

	BigInt* digit = BigIntCreate(1);

	BigInt* factor = BigIntCreate(2);
	factor->array[0] = 1;
	factor->array[1] = 0;

	
	int counter = 2;

	for (i = l - 1; i >= 0; i--, counter++){
		if (!isdigit(num[i])){
			printf("ERROR: Invalid character\n");
			exit(0);
		}
		digit->array[0] = num[i] - '0';
		digit = multiply(digit, mag);
		bigint = add(bigint, digit);
		mag = multiply(mag, factor);
		free(digit);
		digit = BigIntCreate(1);
	}


	bigint = trim(bigint);

	return bigint;
}

BigInt* binASCIIToBigInt(char *num){

	int l = strlen(num);
	int i;

	BigInt* bigint = BigIntCreate(l);
	
	BigInt* mag = BigIntCreate(l);
	mag->array[l - 1] = 1;
	
	BigInt* digit = BigIntCreate(1);

	BigInt* factor = BigIntCreate(1);
	factor->array[0] = 2;
	
	for (i = l - 1; i >= 0; i--){
		if (num[i] != '0' && num[i] != '1'){
			printf("Error: Invalid character\n");
			exit(0);
		}
		digit->array[0] = num[i] - '0';
		digit = multiply(digit, mag);
		bigint = add(bigint, digit);
		mag = multiply(mag, factor);
		free(digit);
		digit = BigIntCreate(1);
	}

	bigint = trim(bigint);

	return bigint;
}


BigInt* octASCIIToBigInt(char *num){
	int l = strlen(num);
	int i;

	BigInt* bigint = BigIntCreate(l);
	
	BigInt* mag = BigIntCreate(l);
	mag->array[l - 1] = 1;
	
	BigInt* digit = BigIntCreate(1);

	BigInt* factor = BigIntCreate(1);
	factor->array[0] = 8;

	for (i = l - 1; i >= 0; i--){
		if (!isoctaldigit(num[i])){
			printf("ERROR: Invalid character\n");
			exit(0);
		}
		digit->array[0] = num[i] - '0';
		digit = multiply(digit, mag);
		bigint = add(bigint, digit);
		mag = multiply(mag, factor);
		free(digit);
		digit = BigIntCreate(1);
	}

	bigint = trim(bigint);
	return bigint;
}

int isoctaldigit(char c){
	return (isdigit(c) && ((c -'0') < 8));
}


BigInt* hexASCIIToBigInt(char *num){
	int l = strlen(num);
	int i;

	BigInt* bigint = BigIntCreate(l);
	
	BigInt* mag = BigIntCreate(l);
	mag->array[l - 1] = 1;
	
	BigInt* digit = BigIntCreate(2);

	BigInt* factor = BigIntCreate(2);
	factor->array[0] = 1;
	factor->array[1] = 6;

	int temp;

	for (i = l - 1; i >= 0; i--){
		if (isalpha(num[i])){
			if(num[i] > 96 && num[i] < 103){
				temp = num[i] - 87;
				digit->array[1] = temp % 10;
				digit->array[0] = 1;
			}
			else if(num[i] > 64 && num[i] < 71){
				temp = num[i] - 55;
				digit->array[1] = temp % 10;
				digit->array[0] = 1;
			}
			else{
				printf("ERROR: Invalid character\n");
				exit(0);
			}
		}
		else{
			if (!isdigit(num[i])){
				printf("ERROR: Invalid character\n");
				exit(0);
			}

			digit->array[1] = num[i] - '0';
		}
		
		digit = multiply(digit, mag);
		bigint = add(bigint, digit);
		mag = multiply(mag, factor);
		free(digit);
		digit = BigIntCreate(2);
	}

	bigint = trim(bigint);

	return bigint;
}


BigInt* clearBigInt(BigInt* bigint){
	int i;
	for (i = 0; i < bigint->k; i++){
		bigint->array[i] = 0;
	}
	return bigint;
}

BigInt* trim(BigInt* bigint){
	int count = 0;
	int i;
	i = 0;
	BigInt *trimmed;
	while (bigint->array[i] == 0 && i < bigint->k){
		count++;
		i++;
	}
	if (count == bigint->k){
		freeBigInt(bigint);
		trimmed = BigIntCreate(1);
		return trimmed;
	}

	trimmed = BigIntCreate(bigint->k - count);
	for (i = 0; i < trimmed->k; i++){
		trimmed->array[i] = bigint->array[count++];
	}

	freeBigInt(bigint);

	return trimmed;
}

char* resultToDecASCII(Calculator *calculator){

	BigInt* bigint = calculator->result;
	int size = bigint->k + 2;
	int i;
	char *string;

	if (calculator->sign == '-'){
		size++;
		string = (char*)malloc(sizeof(char) * size);
		string[0] = '-';
		string[1] = calculator->base[0];
		for (i = 2; i < size; i++){
			string[i] = bigint->array[i - 2] + '0';
		}
	}
	else{
		string = (char*)malloc(sizeof(char) * size);
		string[0] = calculator->base[0];
		for (i = 1; i < size; i++){
			string[i] = bigint->array[i - 1] + '0';
		}
	}
	string[size - 1] = '\0';

	return string;
}

char* resultToBinASCII(Calculator *calculator){

	BigInt* bigint = calculator->result;
	BigInt* binBigInt = BigIntCreate((bigint->k * 4));

	BigInt *quotient = bigint;
	BigInt *two;

	int i = binBigInt->k - 1;

	while (!isZero(quotient)){
		two = BigIntCreate(1);
		two->array[0] = 2;
		quotient = divide(quotient, two);
		binBigInt->array[i] = remainder->array[remainder->k - 1];
		i--;
	}
	binBigInt = trim(binBigInt);
	int size = binBigInt->k + 2;
	char *string;

	if (calculator->sign == '-'){
		size++;
		string = (char*)malloc(sizeof(char) * size);
		string[0] = '-';
		string[1] = calculator->base[0];
		for (i = 2; i < size; i++){
			string[i] = binBigInt->array[i - 2] + '0';
		}
	}
	else{
		string = (char*)malloc(sizeof(char) * size);
		string[0] = calculator->base[0];
		for (i = 1; i < size; i++){
			string[i] = binBigInt->array[i - 1] + '0';
		}
	}
	string[size - 1] = '\0';

	return string;

}

char *resultToOctASCII(Calculator *calculator){
	BigInt* bigint = calculator->result;
	BigInt* octBigInt = BigIntCreate((bigint->k * 2));

	BigInt *quotient = bigint;
	BigInt *eight;

	int i = octBigInt->k - 1;

	while (!isZero(quotient)){
		eight = BigIntCreate(1);
		eight->array[0] = 8;
		quotient = divide(quotient, eight);
		octBigInt->array[i] = remainder->array[remainder->k - 1];
		i--;
	}
	octBigInt = trim(octBigInt);
	int size = octBigInt->k + 2;
	char *string;

	if (calculator->sign == '-'){
		size++;
		string = (char*)malloc(sizeof(char) * size);
		string[0] = '-';
		string[1] = calculator->base[0];
		for (i = 2; i < size; i++){
			string[i] = octBigInt->array[i - 2] + '0';
		}
	}
	else{
		string = (char*)malloc(sizeof(char) * size);
		string[0] = calculator->base[0];
		for (i = 1; i < size; i++){
			string[i] = octBigInt->array[i - 1] + '0';
		}
	}
	string[size - 1] = '\0';

	return string;
}

static char hex_digit[] = "0123456789abcdef";

char* resultToHexASCII(Calculator *calculator){
	BigInt* bigint = calculator->result;
	BigInt* hexBigInt = BigIntCreate((bigint->k));

	BigInt *quotient = bigint;
	BigInt *sixteen;

	int i = hexBigInt->k - 1;

	while (!isZero(quotient)){
		sixteen = BigIntCreate(2);
		sixteen->array[0] = 1;
		sixteen->array[1] = 6;
		quotient = divide(quotient, sixteen);
		if (remainder->array[remainder->k - 2] == 1){
			hexBigInt->array[i] = remainder->array[remainder->k - 1] + 10;
		}else{
			hexBigInt->array[i] = remainder->array[remainder->k - 1];
		}
		i--;
	}
	hexBigInt = trim(hexBigInt);
	int size = hexBigInt->k + 2;
	char *string;
	int c;
	if (calculator->sign == '-'){
		size++;
		string = (char*)malloc(sizeof(char) * size);
		string[0] = '-';
		string[1] = calculator->base[0];
		for (i = 2; i < size; i++){
			c = hexBigInt->array[i - 2];
			string[i] = hex_digit[c];
		}
	}
	else{
		string = (char*)malloc(sizeof(char) * size);
		string[0] = calculator->base[0];
		for (i = 1; i < size; i++){
			c = hexBigInt->array[i - 1];
			string[i] = hex_digit[c];
		}
	}
	string[size - 1] = '\0';

	return string;
}

void freeBigInt(BigInt *bigint){
	free(bigint->array);
	free(bigint);
}

void freeCalculator(Calculator *calculator){
	free(calculator->result);
	free(calculator);
}

int isZero(BigInt *bigint){
	int i;
	for(i = 0; i < bigint->k; i++){
		if (bigint->array[i] != 0){
			return 0;
		}
		else{
			
		}
	}
	return 1;
}

char* getResult(Calculator *calculator){
	if (calculator->base[0] == 'b' ){
		return resultToBinASCII(calculator);
	}
	if (calculator->base[0] == 'd' ){
		return resultToDecASCII(calculator);
	}
	if (calculator->base[0] == 'o' ){
		return resultToOctASCII(calculator);
	}
	if (calculator->base[0] == 'x' ){
		return resultToHexASCII(calculator);
	}
	return NULL;

}


int main(int argc, char **argv){
	
	Calculator* calculator = CalcCreate(argc, argv);

	if (calculator != NULL){
		printf("%s\n", getResult(calculator));
		freeCalculator(calculator);
	}
	

	return 0;
}
