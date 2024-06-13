#ifndef NUMREPR_NUMREPR_H
#define NUMREPR_NUMREPR_H

#endif //NUMREPR_NUMREPR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

static double Log(int, double);
void itohex(int, char*, int);
void itobin(int, char*, int);
int btoint(char*, int);
int htobin(char*, int);
int btohex(char *, int);

static double
Log(int base, double num) {
	// Compute log `base` of `num`.
	double exp = log10(num) / log10(base);
	return exp;
}

void transparr(void *arr, int arrsize, int tsize) {
	// Reverse the array by swapping bit patterns of indicies.
	//
	// Parameters
	// ----------
	// arr : The array to be transposed.
	// arrsize : The size of the array.
	// tsize : The size of the data type of the array.
	//
	int nbytes = arrsize * tsize;
	int left = 0;
	int right = nbytes - tsize;
	char buffer[nbytes];
	// Temporary buffer for storing the contents of the array.
	memcpy(buffer, arr, nbytes);
	while (left < right) {
		char lval[tsize], rval[tsize];
		int lidx = left, ridx = right;
		// Retrieve from the buffer the left and right values to be swapped
		// for the current iteration of the while loop.
		for (int i = 0; i < tsize; i++, lidx++, ridx++) {
			lval[i] = buffer[lidx];
			rval[i] = buffer[ridx];
		}
		memcpy(&buffer[left], rval, tsize); // Replace left value with right.
		memcpy(&buffer[right], lval, tsize); // Replace right value with left.
		// Move the left and right pointers inward by one.
		left += tsize;
		right -= tsize;
	}
	memcpy(arr, buffer, nbytes);
}

int
htobin(char *hexarr, int arrsize) {
	// Convert a character array representing a hexidecimal value to its
	// binary equivalent.
	//
	// Parameters
	// ----------
	// hexarr : The array representing some hex value.
	// arrsize : The size of the array.
	//
	const int scale_by = 4; // For each hex digit we need four binary digits.
	// Temporary buffer for storing the contents of the original array.
	char *buffer = (char *) malloc(sizeof(char) * arrsize);
	if (buffer == NULL) {
		fprintf(stderr, "Unable to allocate sufficient memory for array!\n");
		return 1;
	}
	char bin_lookup[16][5] = {
		"0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111",
		"1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"
	};
	memcpy(buffer, hexarr, sizeof(char) * arrsize);
	// Resize the original array; make it large enough to store binary
	// characters needed to represent the same value.
	hexarr = (char *) realloc(hexarr, sizeof(char) * scale_by);
	if (hexarr == NULL) {
		fprintf(stderr, "Unable to allocate sufficient memory for array!\n");
		return 1;
	} else {
		for (int i = 0; i < arrsize; i++) {
			int digit = buffer[i]; // Retrieve the ith hex value.
			// Make the ASCII values align with the true value of the digit.
			if (digit >= 48 && digit <= 57) {
				digit -= 48;
			} else if (digit >= 65 && digit <= 90) {
				digit -= 55;
			} else if (digit >= 97 && digit <= 122) {
                digit -= 87;
            }
			// Map the hex value to its corresponding binary value.
			char *bin = (char *) &bin_lookup[digit][0];
			memcpy(&hexarr[i * scale_by], bin, sizeof(bin));
		}
	}
	free(buffer);
    return arrsize * scale_by;
}

void
itobin(int num, char *arr, int arrsize) {
	// Convert a base-10 integer to a character array which represents the
	// binary equivalent.
	//
	// Parameters
	// ----------
	// num : The integer number to be converted.
	// arr : The array for storing the binary characters.
	// arrsize : The size of the array.
	//
	int i = arrsize - 1, quotient, digit;
	float rem;
	for (; num > 0; i--) {
		quotient = num / 2; // Produces an integer value.
		rem = num / 2.0 - quotient; // Remainder as a decimal.
		digit = rem * 2; // The next digit to be added to the array.
		arr[i] = (digit + '0'); // Convert `digit` from `int` to `char`.
		num = quotient; // The new value of `num`.
	}
}

void
itohex(int num, char *arr, int arrsize) {
	// Convert a character array representing a base-10 integer to its
	// hex equivalent.
	//
	// Parameters
	// ----------
	// num : The integer number to be converted.
	// arr : The array for storing the hex values.
	// arrsize : The size of the array.
	//
	char hexchars[16] = "0123456789ABCDEF";
	int max_idx = arrsize - 1;
	if (num >= 0) {
		arr[0] = '0';
	}
	for (int i = max_idx; num > 0; num /= 16, i--) {
		arr[i] = hexchars[num % 16];
	}
}

int
btoint(char *binarr, int arrsize) {
	// Convert a binary character array to its corresponding base-10 integer.
	//
	// Parameters
	// ----------
	// binarr : The character array representing a binary number.
	// arrsize : The size of the binary array in bytes.
	//
	int num = 0;
	int max_idx = arrsize - 1; // Represents the last index position.
	int i = max_idx;
	for (; i > 0; i--) {
		if (binarr[i] == '1') {
			int delta = max_idx - i;
			num += pow(2, delta);
		}
	}
	// Don't compute value for the first index position until after the loop.
	// A leading value of 1 means that the integer value is negative. This
	// saves us from needing to perform an additional comparison on every
	// iteration of the loop (i.e. we don't need to determine if we've
	// reached the first index position of the array).
	if (binarr[i] == '1') {
		int delta = max_idx - i;
		num += pow(2, delta) * -1;
	}
	return num;
}

int
htoint(char *hexarr, int arrsize) {
	// Convert a character array representing a hexidecimal integer to its
	// corresponding base-10 integer.
	//
	// Parameters
	// ----------
	// hexarr : The character array representing some hexidecimal value.
	// arrsize : The size of the array.
	//
	const int scale_by = 4;
	int scaled_size = arrsize * scale_by;
	htobin(hexarr, arrsize);
	int num = btoint(hexarr, scaled_size);
	return num;
}
