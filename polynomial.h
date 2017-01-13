
#include <stdio.h>
#include <stdlib.h>

typedef struct term {
	int coeff;
	unsigned int exp;
	struct term *next;
}polynomial;

//typedef struct term polynomial;


