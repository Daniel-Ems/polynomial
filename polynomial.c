#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "polynomial.h"

//Only creates a single node, and then points to null
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

//This will add polynomial a, and polynomial b
polynomial *
poly_add (polynomial * a, polynomial * b)
{
//CursorA and cursorB act as handles on the nodes so you can traverse
//the polynomial
  struct term *cursorA = a;
  struct term *cursorB = b;

//The polynomial's add and front will be used to have a handle on the new
//polynomial
  polynomial *add, *front;

//This inital if/if else/else statement is used to identify and initiate
//the first node of the new polynomial so that other nodes can be added
//to it to create a polynomial

//The statements will find the larger of the two nodes, or if they are equal
  if (cursorA->exp > cursorB->exp)
    {
//A new node will then be created and the appropriate handle will be moved
//accordingly
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

//the polynomial handle is then set to the new node to identify and get a handle
//on the beginning of the newly linked list
  front = add;

//The while statement will perform the same operations as the above if statement
//However, this can be looped now that there is a handle on the first node
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

//The following if/else statments are in place to handle the case that the two 
//polynomials are not of equal length. If this is the case, then the longer of 
//the two lists will be identified and will undergo the same operations 
//accordingly
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

//the remove zero's function will remove any zero's in the case that a coeff
//is 0'd out due to the addition. i.e. -3 +3.
  remove_zeros (&front);

//finally the new polynomial is returned.
  return front;
}


//This function will act similarly to poly_add. However, it will subtract.
polynomial *
poly_sub (polynomial * a, polynomial * b)
{

//CursorA and cursorB are again being used to maintain a handle on the 
//polynomials.
  struct term *cursorA = a;
  struct term *cursorB = b;

//polynomials sub and front will be used to maintain a handle on the new 
//polynomial
  polynomial *sub, *front;

//The following if/if else/else will be used to identify the first node
//of the polynomial in order to get a handle on the new Polynomial
  if (cursorA->exp > cursorB->exp)
    {
//A new node will be created from the appropriate polynomial, and the
//appropriate handle will be moved accordingly.
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

//the first node of the new polynomial will be assigned to it's front
  front = sub;

//the while loop will check for either cursor to = null, and while they do not
//it will perform the same functions as perform, looping through the list.
//this is only possible because we already have a handle on the new polynomial
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

//The follwoing if/else statments are used in the case the polynomials being 
//subtracted are not of equal length. This will assign the remainder of the 
//longer to the rest of the new polynomial. 
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

//this function will remove any nodes with a coeff of 0
  remove_zeros (&front);
  return front;
}

//This will destroy and free all nodes of a polynomial
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

//this prints the polynomial
void
poly_print (polynomial * eqn)
{
  if (!eqn)
    {
      return;
    }
  if (eqn->coeff)
    {
//The print statement uses a ternary to check for + or negative coeff.
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

      //There are only four possibilities for C, which can be simplified
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

	    }
	  else
	    {
	      count += snprintf (tmp, 64, c == -1 ? "%d " : "%d ", c);
	    }
	}

//The results of tmp are strncatted to string which will ultimately be 
//returned. 
      strncat (string, tmp, 256 - count);

    }
  free (tmp);
  return string;
}

//This removes any node that contains a 0 coefficient
void
remove_zeros (polynomial ** poly)
{
  struct term *cursor, *previous;
  cursor = *poly;
  previous = cursor;
//The first while loop is used to check if the first node, and any 
//immediately consecutive nodes have coeffs that are 0
  while (cursor->coeff == 0)
    {
      cursor = cursor->next;
      *poly = cursor;
      free (previous);
      previous = cursor;

    }
//The second while loop is used to remove any coeff of 0 not at the start of the 
//polynomial
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

//Simplify poly is used to add any coefficient's with the same exponent
void
Simplify_poly (polynomial * a)
{
  struct term *cursor, *previous;
  cursor = a;
  previous = cursor;
  cursor = cursor->next;

//The while loop will loop until it equals null, checking if any consecutive 
//nodes have the same exponent.
  while (cursor)
    {
      if (previous->exp == cursor->exp)
	{
//if consecutive nodes share exponents, the coeff are added to the first node
//then the second is free;
	  previous->coeff += cursor->coeff;
	  previous->next = cursor->next;
	  free (cursor);
	  cursor = previous->next;
	}
//If consecutives nodes do not have matching exponents, then the cursor is 
//moved forward one node.
      else
	{
	  previous = cursor;
	  cursor = cursor->next;
	}
//after the polynomial has been fully traversed, it is passed to the remove_zero
//function in order to remove any nodes that have a coeff of 0 due to the 
//simplify function
    }
  remove_zeros (&a);
}

//poly_equal will check if two polynomials have identical terms
bool
poly_equal (polynomial * a, polynomial * b)
{
  struct term *cursorA = a;
  struct term *cursorB = b;
//This if statement will check to make sure that neither of the polynomials
//are null. It also allows you to enter the while loop and set up the return
  if (cursorA && cursorB)
    {
      //inside this while loop it will check to see if they are equal
      //if so, it will go unti it reaches the end, or it fails
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
  return false;

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


}
