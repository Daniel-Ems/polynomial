#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "polynomial.h"

struct term *
term_create (int coeff, int exp)
{
  struct term *node = malloc (sizeof (*node));

  if (node)
    {
      node->coeff = coeff;
      node->exp = exp;
      node->next = NULL;
    }
  return node;
}


polynomial *
poly_add (polynomial * a, polynomial * b)
{
  struct term *cursorA = a;
  struct term *cursorB = b;
  polynomial *add, *front;

  if (cursorA->exp > cursorB->exp)
    {
      add = term_create (cursorA->coeff, cursorA->exp);
      cursorA = cursorA->next;
    }
  else if (cursorA->exp < cursorB->exp)
    {
      add = term_create (cursorB->coeff, cursorB->exp);
      cursorB = cursorB->next;
    }
  else
    {
      add = term_create (cursorA->coeff + cursorB->coeff, cursorA->exp);
      cursorA = cursorA->next;
      cursorB = cursorB->next;

    }
  front = add;
  while (cursorA && cursorB)
    {

      if (cursorA->exp > cursorB->exp)
	{
	  add->next = term_create (cursorA->coeff, cursorA->exp);
	  cursorA = cursorA->next;

	}
      else if (cursorA->exp < cursorB->exp)
	{
	  add->next = term_create (cursorB->coeff, cursorB->exp);
	  cursorB = cursorB->next;
	}
      else
	{
	  add->next =
	    term_create (cursorA->coeff + cursorB->coeff, cursorA->exp);
	  cursorA = cursorA->next;
	  cursorB = cursorB->next;

	}
      add = add->next;
    }

  if (cursorA != NULL)
    {
      while (cursorA)
	{
	  add->next = term_create (cursorA->coeff, cursorA->exp);
	  cursorA = cursorA->next;
	}
    }
  else
    {
      while (cursorB)
	{
	  add->next = term_create (cursorB->coeff, cursorB->exp);
	  cursorB = cursorB->next;
	}
    }
  remove_zeros (&front);
  return front;
}

polynomial *
poly_sub (polynomial * a, polynomial * b)
{
  struct term *cursorA = a;
  struct term *cursorB = b;
  polynomial *sub, *front;

  if (cursorA->exp > cursorB->exp)
    {
      sub = term_create (cursorA->coeff, cursorA->exp);
      cursorA = cursorA->next;
    }
  else if (cursorA->exp < cursorB->exp)
    {
      sub = term_create (cursorB->coeff, cursorB->exp);
      cursorB = cursorB->next;
    }
  else
    {
      sub = term_create (cursorA->coeff - cursorB->coeff, cursorA->exp);
      cursorA = cursorA->next;
      cursorB = cursorB->next;

    }
  front = sub;
  while (cursorA && cursorB)
    {

      if (cursorA->exp > cursorB->exp)
	{
	  sub->next = term_create (cursorA->coeff, cursorA->exp);
	  cursorA = cursorA->next;

	}
      else if (cursorA->exp < cursorB->exp)
	{
	  sub->next = term_create (cursorB->coeff, cursorB->exp);
	  cursorB = cursorB->next;
	}
      else
	{
	  sub->next =
	    term_create (cursorA->coeff - cursorB->coeff, cursorA->exp);
	  cursorA = cursorA->next;
	  cursorB = cursorB->next;

	}
      sub = sub->next;
    }

  if (cursorA != NULL)
    {
      while (cursorA)
	{
	  sub->next = term_create (cursorA->coeff, cursorA->exp);
	  cursorA = cursorA->next;
	}
    }
  else
    {
      while (cursorB)
	{
	  sub->next = term_create (cursorB->coeff, cursorB->exp);
	  cursorB = cursorB->next;
	}
    }
  remove_zeros (&front);
  return front;
}

void
poly_destroy (polynomial * eqn)
{
  while (eqn)
    {
      struct term *tmp = eqn->next;
      free (eqn);
      eqn = tmp;
    }
}

void
poly_print (const polynomial * eqn)
{
  if (!eqn)
    {
      return;
    }
  if (eqn->coeff)
    {
      printf ("%c%d", eqn->coeff > 0 ? '+' : '\0', eqn->coeff);
      if (eqn->exp > 1)
	{
	  printf ("X^%d", eqn->exp);
	}
      else if (eqn->exp == 1)
	{
	  printf ("x");
	}
      printf (" ");
    }
  poly_print (eqn->next);
}


char *
poly_to_string (polynomial * p)
//Returns a newely-malloced string that displays the given polynomial
{
  //two buffers: one to hold each snprintf return and then the other to 
  //return. They are split to better control the amount of data written 
  //to the buffer and prevent any buffer overflows.
  char *tmp = malloc (64);
  char *string = calloc (256, 1);

  struct term *cursor = p;	//Set equal to p to start at the beginnign of the 
  //Polynomial.

  int count = 0;
  while (cursor)
    {

      //c and e are used as variables that hold the coeff and exp of each node
      int c = cursor->coeff;
      int e = cursor->exp;

      //the values for the current cursor position have already been stored
      //which is why the cursor cursorAn move before the if statement begins.
      /* Think about possibly moving this to the bottom? */
      cursor = cursor->next;

      //There are only four possibilities for C, which cursorAn be simplified
      //in two if statements. >= && <=
      if (c >= 1)
	{
	  if (e == 1)
	    {
	      //count holds the number of bites written and is set up to 
	      //add to itself. This gives the programmer the ability to use
	      //count in combination with the return buffer to check the 
	      //buffer size 
	      count += snprintf (tmp, 64, c == 1 ? "+%dX " : "+%dX ", c);
	    }
	  else if (e > 1)
	    {
	      count +=
		snprintf (tmp, 64, c == 1 ? "+%dX^%d " : "+%dX^%d ", c, e);
	    }
	  else
	    {
	      count += snprintf (tmp, 64, c == 1 ? "+%d " : "+%d ", c);
	    }
	}
      if (c <= -1)
	{
	  if (e == 1)
	    {
	      count += snprintf (tmp, 64, c == -1 ? "%dX " : "%dX ", c);
	    }
	  else if (e > 1)
	    {
	      count +=
		snprintf (tmp, 64, c == -1 ? "%dX^%d " : "%dX^%d ", c, e);
	      //printf("%d, %d\n", c, count);
	    }
	  else
	    {
	      count += snprintf (tmp, 64, c == -1 ? "%d " : "%d ", c);
	    }
	}
      strncat (string, tmp, 256 - count);
      //cursor = cursor->next;
    }
  free (tmp);
  return string;
}

// needs to handle removing a zero at the beginning of the linked list
void
remove_zeros (polynomial ** poly)
{
  struct term *cursor, *previous;
  cursor = *poly;
  previous = cursor;
  while (cursor->coeff == 0)
    {
      cursor = cursor->next;
      *poly = cursor;
      free (previous);
      previous = cursor;

    }
  while (cursor)
    {
      if (cursor->coeff == 0)
	{
	  previous->next = cursor->next;
	  free (cursor);
	  cursor = previous->next;
	}
      else
	{
	  previous = cursor;
	  cursor = cursor->next;
	}
    }
  //cursor = poly;
}

void
Simplify_poly (polynomial * a)
{
  struct term *cursor, *previous;
  cursor = a;
  previous = cursor;
  cursor = cursor->next;

  while (cursor)
    {
      if (previous->exp == cursor->exp)
	{
	  previous->coeff += cursor->coeff;
	  previous->next = cursor->next;
	  free (cursor);
	  cursor = previous->next;
	}
      else
	{
	  previous = cursor;
	  cursor = cursor->next;
	}
    }
  remove_zeros (&a);
}

bool
poly_equal (polynomial * a, polynomial * b)
{
  struct term *cursorA = a;
  struct term *cursorB = b;
  if ((cursorA->coeff == cursorB->coeff) && (cursorA->exp == cursorB->exp))
    {
      while (cursorA != NULL && cursorB != NULL)
	{
	  if ((cursorA->coeff == cursorB->coeff)
	      && (cursorA->exp == cursorB->exp))
	    {
	      cursorA = cursorA->next;
	      cursorB = cursorB->next;
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

void
transform (struct term *b)
{
  double sum;
  sum = pow (4.2, b->exp);
  sum *= b->coeff;
  printf ("coeff %d\n", b->coeff);
  printf ("exp %d\n", b->exp);
  printf ("sum = %lf\n", sum);
}

double
poly_eval (polynomial * p, double x)
{
  double sum = 0;
  double tmp = 0;
  struct term *cursor = p;
  while (cursor != NULL)
    {
      tmp = pow (x, cursor->exp);
      tmp *= cursor->coeff;
      cursor = cursor->next;
      sum += tmp;
    }
  return sum;
}


void
poly_iterate (polynomial * p, void (*transform) (struct term *))
//cursorAlls the function transform on each term of the polynomial
{
  struct term *cursor = p;
  while (cursor != NULL)
    {
      transform (cursor);
      cursor = cursor->next;
    }
}

//double poly_eval(const polynomial *p, double x);
//Evaluates the polynomial by substituting x in the variable of the polynomial

int
main ()
{
//make polynomial named poly
  struct term *front = term_create (2, 5);
  front->next = term_create (2, 4);
  front->next->next = term_create (2, 3);
  front->next->next->next = term_create (-5, 2);
  polynomial *poly = front;

//print poly
  poly_print (poly);
  puts (" ");

//simplify poly
  Simplify_poly (poly);

//turn poly to string
  char *string = poly_to_string (poly);
  printf ("%s\n", string);

//make polynomial named polyB
  struct term *second = term_create (2, 5);
  second->next = term_create (2, 4);
  second->next->next = term_create (2, 0);
  polynomial *polyB = second;

//print polyB
  poly_print (polyB);
  puts (" ");

//simpligy polyB
  Simplify_poly (polyB);

//make polyB a string
  char *stringB = poly_to_string (polyB);
  printf ("%s\n", stringB);
  puts (" ");

//make new polynomial from the addition of polynomial a and b
  polynomial *Addition = poly_add (poly, polyB);

//make a new polynomial from the subtraction of a and b
  polynomial *Subtraction = poly_add (poly, polyB);

//turn subtraction into string
  char *substring = poly_to_string (Subtraction);
  printf ("%s", substring);

//turn the addition polynomial into a string
  char *addstring = poly_to_string (Addition);
  printf ("%s\n", addstring);


  poly_print (Addition);


//check and see if they are equal
  if (poly_equal (poly, polyB))
    {
      printf ("they are equal it works\n");
    }
  else
    {
      printf ("This doesn't work\n");
    }

//send transform to poly_iterate;
  poly_iterate (polyB, transform);

//evaluate the polynomial based on substituting x for a double
  double x = poly_eval (front, 2.3);
  printf ("%lf", x);

  free (addstring);
  poly_destroy (Addition);
  free (stringB);
  poly_destroy (polyB);
  free (string);
  poly_destroy (poly);

}
