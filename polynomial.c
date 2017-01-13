#include <stdio.h>
#include <stdlib.h>

#include "polynomial.h"

struct term *term_create(int coeff, int exp)
{
	struct term *node = malloc(sizeof(*node));
	
	if(node) {
	node -> coeff = coeff;
	node -> exp = exp;
	node -> next = NULL;
	}
	return node;
}

void poly_destroy(polynomial *eqn)
{
	while(eqn) {
		struct term *tmp = eqn->next;
		free(eqn);
		eqn = tmp;
	}
}

void poly_print(const polynomial *eqn)
{
	if(!eqn) {
		return;
	}
	if(eqn->coeff) {
		printf("%c%d", eqn->coeff > 0 ? '+' : '\0', eqn->coeff);
		if(eqn->exp > 1) {
			printf("X^%d", eqn->exp);
		} else if(eqn->exp==1) {
			printf("x");
		}
		printf(" ");
	}
	poly_print(eqn->next);
}

char *poly_to_string(polynomial *p) 
//Returns a newely-malloced string that displays the given polynomial
{
	char *tmp = malloc(64);	
	struct term *cursor = p;
	int count=0;
	while(cursor)
	{
		int c = cursor->coeff;
		int e = cursor->exp;
		//previous = cursor;
		cursor = cursor->next;
		count = snprintf(tmp+count, 64, c > 1 ? "+%dx^%d " : "%dx^%d ", c,e);
	}
	return tmp;
}
//polynomial *poly_add(const polynomial *a, const polynomial *b)
//Returns a newly-malloced polynomial that is the sum of the two arguments
 
//polynomial *poly_sub(const polynomial *a, const polynomial *b);
//Returns a newly-malloced polynomial that is the difference of the two arguments

//int poly_equal(const polynomial *a, const polynomial *b);
//Returns 1 if the two arguments have the same terms; 0 otherwise.

//void poly_iterate(polynomial *p, void (*transform)(struct term *));
//Calls the function transform on each term of the polynomial

//double poly_eval(const polynomial *p, double x);
//Evaluates the polynomial by substituting x in the variable of the polynomial

int main()
{

struct term *front= term_create(-3, 5);
struct term *second = term_create(3, 4);
front->next = second;
polynomial *poly = front;
poly_print(poly);
char *string = poly_to_string(poly);
puts(" ");
printf("%s\n", string);

}


			
		
		
			
