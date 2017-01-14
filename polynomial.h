
#include <stdio.h>
#include <stdlib.h>

struct term
{
  int coeff;
  unsigned int exp;
  struct term *next;
};

typedef struct term polynomial;

struct term *term_create (int coeff, int exp);
void Simplify_poly (polynomial * a);
void remove_zeros (polynomial ** poly);
char *poly_to_string (polynomial * p);
void poly_print (polynomial * eqn);
void poly_destroy (polynomial * eqn);
polynomial *poly_add (polynomial * a, polynomial * b);
polynomial *poly_sub (polynomial * a, polynomial * b);
bool poly_equal (polynomial * a, polynomial * b);
void poly_iterate (polynomial * p, void (*transform) (struct term *));
void transform (struct term *b);
double poly_eval (polynomial * p, double x);
