#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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


polynomial *poly_add(polynomial *a, polynomial *b)
{
	struct term *ca = a;
	struct term *cb = b;
	polynomial *add, *front; 

	if(ca->exp > cb->exp)
	{
		add = term_create(ca->coeff, ca->exp);
		ca = ca->next;
	}
	else if(ca->exp < cb->exp)
	{
		add = term_create(cb->coeff, cb->exp);
		cb = cb->next;
	}
	else
	{	
		add = term_create(ca->coeff+cb->coeff, ca->exp);
		ca=ca->next;
		cb =cb->next;
			
	}
	front = add;
	while(ca && cb)
	{

		if(ca->exp > cb->exp)
		{
			add->next = term_create(ca->coeff, ca->exp);
			ca = ca->next;
		
		}
		else if(ca->exp < cb->exp)
		{
			add->next = term_create(cb->coeff, cb->exp);
			cb = cb->next;
		}
		else
		{	
			add->next = term_create(ca->coeff+cb->coeff, ca->exp);
			ca=ca->next;
			cb=cb->next;	
		
		}
		add = add->next;
	}
	
	if(ca != NULL)
	{
		while(ca)
		{
			add->next =term_create(ca->coeff, ca->exp);
			ca =ca->next;
		}
	}
	else
	{
		while(cb)
		{
			add->next =term_create(cb->coeff, cb->exp);
			cb =cb->next;
		}
	}
	remove_zeros(&front);
	return front;
}

polynomial *poly_sub(polynomial *a, polynomial *b)
{
	struct term *ca = a;
	struct term *cb = b;
	polynomial *sub, *front; 

	if(ca->exp > cb->exp)
	{
		sub = term_create(ca->coeff, ca->exp);
		ca = ca->next;
	}
	else if(ca->exp < cb->exp)
	{
		sub = term_create(cb->coeff, cb->exp);
		cb = cb->next;
	}
	else
	{	
		sub = term_create(ca->coeff-cb->coeff, ca->exp);
		ca=ca->next;
		cb =cb->next;
			
	}
	front = sub;
	while(ca && cb)
	{

		if(ca->exp > cb->exp)
		{
			sub->next = term_create(ca->coeff, ca->exp);
			ca = ca->next;
		
		}
		else if(ca->exp < cb->exp)
		{
			sub->next = term_create(cb->coeff, cb->exp);
			cb = cb->next;
		}
		else
		{	
			sub->next = term_create(ca->coeff-cb->coeff, ca->exp);
			ca=ca->next;
			cb=cb->next;	
		
		}
		sub = sub->next;
	}
	
	if(ca != NULL)
	{
		while(ca)
		{
			sub->next =term_create(ca->coeff, ca->exp);
			ca =ca->next;
		}
	}
	else
	{
		while(cb)
		{
			sub->next =term_create(cb->coeff, cb->exp);
			cb =cb->next;
		}
	}
	remove_zeros(&front);
	return front;
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
	//two buffers: one to hold each snprintf return and then the other to 
	//return. They are split to better control the amount of data written 
	//to the buffer and prevent any buffer overflows.
	char *tmp = malloc(64);	
	char *string = calloc(256,1);

	struct term *cursor = p; //Set equal to p to start at the beginnign of the 
							 //Polynomial.

	int count=0;
	while(cursor)
	{
	
		//c and e are used as variables that hold the coeff and exp of each node
		int c = cursor->coeff;
		int e = cursor->exp;
		
		//the values for the current cursor position have already been stored
		//which is why the cursor can move before the if statement begins.
		/* Think about possibly moving this to the bottom? */
		cursor = cursor->next;
		
		//There are only four possibilities for C, which can be simplified
		//in two if statements. >= && <=
		if(c >= 1)
		{
			if(e == 1)
			{
			//count holds the number of bites written and is set up to 
			//add to itself. This gives the programmer the ability to use
			//count in combination with the return buffer to check the 
			//buffer size 
			count += snprintf(tmp, 64, c == 1 ? "+%dX " : "+%dX ",c);
			}
			else if(e > 1)
			{
			count += snprintf(tmp, 64, c == 1 ? "+%dX^%d " : "+%dX^%d ",c,e);
			}
			else
			{ 
			count += snprintf(tmp, 64, c == 1 ? "+%d " : "+%d ",c); 
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

// needs to handle removing a zero at the beginning of the linked list
void remove_zeros(polynomial **poly)
{
	struct term *cursor, *previous;
	cursor = *poly;
	previous = cursor;
	while(cursor->coeff == 0)
	{
		cursor = cursor->next;
		*poly = cursor;
		free(previous);
		previous = cursor;
		
	}
	while(cursor)
	{
		if(cursor->coeff == 0)
		{
			previous->next = cursor->next;
			free(cursor);
			cursor = previous->next;
		}
		else{
			previous = cursor;
			cursor = cursor->next;
		}
	}
	//cursor = poly;
}

void Simplify_poly(polynomial *a)
{
	struct term *cursor, *previous;
	cursor = a;
	previous = cursor;
	cursor = cursor -> next;

	while(cursor)
	{
		if(previous->exp == cursor->exp)
		{
			previous->coeff += cursor->coeff;
			previous -> next = cursor->next;
			free(cursor);
			cursor = previous -> next;					
		}
		else
		{
			previous = cursor;
			cursor = cursor->next;
		}
	}
	remove_zeros(&a);
}
			
bool poly_equal(polynomial *a, polynomial *b)
{
	struct term *ca = a;
	struct term *cb = b;
	if((ca->coeff == cb->coeff) && (ca->exp==cb->exp))
	{
		while(ca != NULL && cb != NULL)
		{
			if((ca->coeff == cb->coeff) && (ca->exp==cb->exp))
			{
				ca=ca->next;
				cb=cb->next;
			}
			else
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}
	
	
		


//void poly_iterate(polynomial *p, void (*transform)(struct term *));
//Calls the function transform on each term of the polynomial

//double poly_eval(const polynomial *p, double x);
//Evaluates the polynomial by substituting x in the variable of the polynomial

int main()
{

struct term *front= term_create(2, 5);
front->next = term_create(2, 4);
front->next->next = term_create(2, 3);
//front->next->next->next=term_create(-5,2);
polynomial *poly = front;
poly_print(poly);
puts(" ");
Simplify_poly(poly);
char *string = poly_to_string(poly);
printf("%s\n", string);
poly_print(poly);
puts(" ");

struct term *second = term_create(2,5);
second->next = term_create(2,4);
second->next->next=term_create(2,0);
polynomial *polyB = second;
poly_print(polyB);
puts(" ");
Simplify_poly(polyB);
char *stringB = poly_to_string(polyB);
printf("%s\n",stringB);
puts(" ");

polynomial *Addition = poly_add(poly, polyB);
char *addstring = poly_to_string(Addition);
printf("%s\n", addstring);
poly_print(Addition);

if(poly_equal(poly, polyB))
{
	printf("they are equal it works\n");
}
else
{
	printf("This doesn't work\n");
}
free(addstring);
poly_destroy(Addition);
free(stringB);
poly_destroy(polyB);
free(string);
poly_destroy(poly);

}


			
		
		
			
