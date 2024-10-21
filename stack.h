/*
 * stack.h
 */

#ifndef STACK_H
#define STACK_H

#include <stdbool.h>


struct Stack *stack_new      (void);
void          stack_push     (struct Stack *stack,
                              char          c);
char          stack_top      (struct Stack *stack);
char          stack_pop      (struct Stack *stack);
bool          stack_is_empty (struct Stack *stack);
void          stack_dump     (struct Stack *stack);

#endif
