//PURPOSE: determine which signed binary number is bigger
//Aug 22, 2023

//This program can easily be extended to working on shorts, ints, longs, long longs.
//Beware of floats and doubles, where the sign bit part will still (mostly) work (after casting the float or double to a void pointer
//  to allow bit shifting), but care needs to be taken to ensure that the mantissa bits and exponent bits are identified correctly
//  (check the IEEE754 standard)

#define DEBUG 0

void printBinary(const char input)
{
	if(DEBUG){printf("printBinary(%d): ",input);}
	char iterations = 0;
	char c = input;
	while(iterations!=8)
	{
		if(c & (unsigned char)128){printf("1");}	//if(MSB is1){print1} otherwise{print0}
		else{printf("0");}
		
		c <<= 1;
		iterations++;
	}
	printf("\n");
}

//&  means 'bitwise AND', which applies the AND operation to all bits of a specific place value, then does that for every place value.
//<< means 'bitshift left', which shifts each bit in the binary representation of a decimal # to the left by the specified amount,
//     overwriting and discarding the MSB and filling in the LSB with a 0.
//   128<<1 means (1000 0000 << 1) ==  (1_discard 0000 000 0_fill) == (0000 0000) == 0
//   20<<2  means (0001 0100 << 2) == (00_discard 0101 00 00_fill) == (0101 0000) == 16+64==80
//   someVar <<= 3 means Shift 3 bits to the left, then store the result into the same variable

//Uses bitwise AND (&), logical OR (||), logical AND (&&), equivalence (==), negated equivalence (!=), bitshift left (<<), if statements
//Returns the bigger of the input chars (with the original input address, not newly allocated #s).
//If inputs are the same value, the 1st input is returned.
const char biggerNumIs(const char in1, const char in2)
{
	//0000 0000 ==    0
	//1000 0000 == -128
	//0111 1111 ==  127
	//1111 1111 ==   -1
	//if the MSB (of a signed char) is 1, then the overall # is negative.
	const unsigned char MSBplaceVal = 128;
	unsigned char input1MSB, input2MSB, shiftedInput1, shiftedInput2;
	input1MSB = (MSBplaceVal & in1);	//x000 0000	 MSB==mostSignificantBit  input1's MSB
	if(DEBUG){printf("input1MSB: %d\n", input1MSB);}
	input2MSB = (MSBplaceVal & in2);	//x000 0000	 MSB==mostSignificantBit  input2's MSB
	if(DEBUG){printf("input2MSB: %d\n", input2MSB);}
	
	//if exactly one number is positive (including zero)
		 if(input1MSB==0 && input2MSB==MSBplaceVal){return in1;}	//if(input1_IsPositive && input2_IsNegative){biggerNumIs1;}
	else if(input2MSB==0 && input1MSB==MSBplaceVal){return in2;}	//if(input2_IsPositive && input1_IsNegative){biggerNumIs2;}

	//else: both inputs are negative, or both are positive (positive includes zero here)
	if(DEBUG){printf("Both inputs are negative, or both are positive (positive includes zero here)\n");}
	shiftedInput1 = in1;
	shiftedInput2 = in2;
	while(shiftedInput1!=0 || shiftedInput2!=0)
	{
		//x1wyz... is bigger than x0wyz...  It doesn't matter if x (the MSB) is 1 or 0 (signifying a negative overall value),
		//  as long as x has the same bit value on both numbers. If x is 1 (meaning both inputs are negative), it's like
		//  adding -128 to both inputs, which doesn't change the relative difference between the two inputs

		//Go to next lowest place value
		//(more scalable by bitshifting instead of manually selecting decreasing binary place values to bitwise AND with the inputs)
		if(DEBUG){printf("BEFORE SHIFT:  shiftedInput1: %d, shiftedInput2: %d\n",shiftedInput1,shiftedInput2);}
		shiftedInput1 <<= 1;
		shiftedInput2 <<= 1;
		if(DEBUG){printf("  AFTER SHIFT:  shiftedInput1: %d, shiftedInput2: %d\n",shiftedInput1,shiftedInput2);}
		input1MSB = MSBplaceVal & shiftedInput1;	//Gets the bit value at place value 128 (then 64, then 32, then ...) from input1
		input2MSB = MSBplaceVal & shiftedInput2;	//Gets the bit value at place value 128 (then 64, then 32, then ...) from input2

		//As soon as there are different bit values for a given place value, return the # that has bit value 1 for that given place value
		if(input1MSB==0 && input2MSB==MSBplaceVal){return in2;}	//if(input1AtPlaceValueX==0... and input2AtPlaceValueX==1...){input2IsMorePositive}
		if(input2MSB==0 && input1MSB==MSBplaceVal){return in1;}	//if(input2AtPlaceValueX==0... and input1AtPlaceValueX==1...){input1IsMorePositive}
	}

	//both shifted inputs are now 0 (or were already 0), meaning the two inputs were equivalently valued. E.g. input1==15 and input2==15
	if(DEBUG){printf("Equal input values\n");}
	return in1;		//Arbitrarily in1 instead of in2
}

void test_printBinary()
{
	printf("printBinary(0):    ");
	printBinary(0);

	printf("printBinary(1):    ");
	printBinary(1);

	printf("printBinary(2):    ");
	printBinary(2);

	printf("printBinary(3):    ");
	printBinary(3);

	printf("printBinary(4):    ");
	printBinary(4);

	printf("printBinary(8):    ");
	printBinary(8);

	printf("printBinary(-1):   ");
	printBinary(-1);

	printf("printBinary(127):  ");
	printBinary(127);

	printf("printBinary(-128): ");
	printBinary(-128);

	printf("printBinary(255):  ");
	printBinary(255);

	printf("printBinary(256):  ");
	printBinary(256);
}
void test_biggerNumIs()
{
	char num1=0, num2=0;
	printf("biggerNumIs(%d,%d): %d\n\n", num1,num2,biggerNumIs(num1,num2));

	num1=10, num2=0;
	printf("biggerNumIs(%d,%d): %d\n\n", num1,num2,biggerNumIs(num1,num2));

	num1=0, num2=10;
	printf("biggerNumIs(%d,%d): %d\n\n", num1,num2,biggerNumIs(num1,num2));

	num1=-10, num2=0;
	printf("biggerNumIs(%d,%d): %d\n\n", num1,num2,biggerNumIs(num1,num2));

	num1=0, num2=-10;
	printf("biggerNumIs(%d,%d): %d\n\n", num1,num2,biggerNumIs(num1,num2));

	num1=-5, num2=-10;
	printf("biggerNumIs(%d,%d): %d\n\n", num1,num2,biggerNumIs(num1,num2));

	num1=-10, num2=-5;
	printf("biggerNumIs(%d,%d): %d\n\n", num1,num2,biggerNumIs(num1,num2));

	num1=-10, num2=-9;
	printf("biggerNumIs(%d,%d): %d\n\n", num1,num2,biggerNumIs(num1,num2));

	num1=-8, num2=-9;
	printf("biggerNumIs(%d,%d): %d\n\n", num1,num2,biggerNumIs(num1,num2));

	num1=127, num2=0;
	printf("biggerNumIs(%d,%d): %d\n\n", num1,num2,biggerNumIs(num1,num2));

	num1=0, num2=127;
	printf("biggerNumIs(%d,%d): %d\n\n", num1,num2,biggerNumIs(num1,num2));

	num1=-128, num2=0;
	printf("biggerNumIs(%d,%d): %d\n\n", num1,num2,biggerNumIs(num1,num2));

	num1=0, num2=-128;
	printf("biggerNumIs(%d,%d): %d\n\n", num1,num2,biggerNumIs(num1,num2));

	num1=256, num2=0;
	printf("biggerNumIs(256,%d): %d\n\n", num2,biggerNumIs(num1,num2));

	num1=255, num2=0;
	printf("biggerNumIs(255,%d): %d\n\n", num2,biggerNumIs(num1,num2));

	return;
}

int main()
{
	test_biggerNumIs();
	test_printBinary();
	return 0;
}