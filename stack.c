/*
 * stack.c
 */

#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX 50

struct Stack
{
  int top;

  char arr[MAX];
};

struct Stack *
stack_new (void)
{
  struct Stack *stack;

  stack = malloc (sizeof (struct Stack));

  stack->top = -1;

  return stack;
}

void
stack_push (struct Stack *stack,
            char          c)
{
  /* We ran out of storage :( */
  if (stack->top >= MAX - 1)
    return;

  stack->arr[++stack->top] = c;
}

char
stack_pop (struct Stack *stack)
{
  if (stack->top == -1)
    return '\0';

  return stack->arr[stack->top--];
}

char
stack_top (struct Stack *stack)
{
  if (stack->top == -1)
    return '\0';

  return stack->arr[stack->top];
}

bool
stack_is_empty (struct Stack *stack)
{
  return stack->top == -1;
}

void
stack_dump (struct Stack *stack)
{
  int i = stack->top;

  printf ("> ");
  while (i != -1)
    {
      printf ("%c ", stack->arr[i]);
      i--;
    }
  printf ("\n");
}
