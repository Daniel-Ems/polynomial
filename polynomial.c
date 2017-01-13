#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	char *string = calloc(256,1);
	struct term *cursor = p;
	//struct term *previous = cursor;
	int count=0;
	while(cursor)
	{
	
		//printf("second loop iteration%s\n", tmp);
		int c = cursor->coeff;
		int e = cursor->exp;
		//previous = cursor;
		cursor = cursor->next;
		if(c >= 1)
		{
			if(e == 1)
			{
			count += snprintf(tmp, 64, c == 1 ? "+%dX " : "+%dX ",c);
			//printf("%s\n", tmp);
			}
			else if(e > 1)
			{
			count += snprintf(tmp, 64, c == 1 ? "+%dX^%d " : "+%dX^%d ",c,e);
			}
			else
			{ 
			count += snprintf(tmp, 64, c == 1 ? "+%d " : "%d ",c); 
			}
		}
		if(c <= -1)
		{
			if(e == 1)
			{
			count += snprintf(tmp, 64, c == -1 ? "%dX " : "%dX ", c);
			}
			else if(e > 1)
			{
			count += snprintf(tmp, 64, c == -1 ? "%dX^%d " : "%dX^%d ",c, e);
			//printf("%d, %d\n", c, count);
			}
			else
			{ 
			count += snprintf(tmp, 64, c == -1 ? "%d ": "%d ",c); 
			}
		}
	strncat(string, tmp, 256-count);
	//cursor = cursor->next;
	}
	free(tmp);
	return string;
}

void Simplify_poly(polynomial **a)
{
	struct term *cursor, *previous;
	cursor = *a;
	previous = cursor;
	cursor = cursor -> next;
	while(cursor)
	{
		if(previous->exp == cursor->exp)
		{
			previous->coeff += cursor -> coeff;
			//printf("%d", previous->coeff);
			previous -> next = cursor->next;
			free(cursor);
			cursor = previous->next;
		}
		else
		{
		previous = cursor;
		cursor = cursor->next;
		}
	}
	cursor = *a;
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

struct term *front= term_create(-1, 2);
front->next = term_create(5, 2);
front->next->next = term_create(3, 2);
//front->next = second;
//second->next = third;
polynomial *poly = front;
poly_print(poly);
char *string = poly_to_string(poly);
puts("test");
printf("%s\n", string);
Simplify_poly(&poly);
poly_print(poly);
poly_destroy(poly);
//free(poly);
free(string);
}


			
		
		
			
