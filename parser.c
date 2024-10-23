/*
 * parser.c
 */

#include "parser.h"
#include "grammar.h"
#include "utils.h"
#include "stack.h"

#include <string.h>
#include <ctype.h>


#define EPSILON "ε"

static bool
isbracket (char c)
{
  return c == '('
         || c == ')';
}

static bool
isoperator (char c)
{
  return c == '*'
         || c == '+'
         || c == '|';
}

static void
learn_grammar (struct Grammar *grammar,
               struct Stack   *op_stack,
               struct Stack   *char_stack)
{
  char top;
  char symbol;
  char prod[3] = { '\0' };

  if (stack_is_empty (op_stack)
      || stack_is_empty (char_stack))
    return;

  top = stack_pop (op_stack);

  switch (top)
    {
      case '(':
        learn_grammar (grammar, op_stack, char_stack);
        break;
      case ')':
        /* consume ')' */
        return;
      case '|':
        symbol = utils_get_next_symbol ();

        prod[0] = stack_pop (char_stack);
        grammar_create_rule (grammar, symbol, prod);

        learn_grammar (grammar, op_stack, char_stack);

        prod[0] = stack_pop (char_stack);
        grammar_create_rule (grammar, symbol, prod);

        stack_push (char_stack, symbol);
        break;
      case '*':
        symbol = utils_get_next_symbol ();

        prod[0]  = stack_pop (char_stack);
        prod[1] = symbol;
        grammar_create_rule (grammar, symbol, prod);

        grammar_create_rule (grammar, symbol, EPSILON);

        stack_push (char_stack, symbol);
        break;
      case '+':
        symbol = utils_get_next_symbol ();

        prod[0]  = stack_pop (char_stack);
        prod[1] = symbol;
        grammar_create_rule (grammar, symbol, prod);

        stack_push (char_stack, symbol);
        break;
      case '-':
        symbol = utils_get_next_symbol ();

        prod[0] = stack_pop (char_stack);
        prod[1] = stack_pop (char_stack);

        grammar_create_rule (grammar, symbol, prod);
        stack_push (char_stack, symbol);

        break;
      default:
        utils_throw_error ("learn_grammar: unreachable code");
    }

  if (!stack_is_empty (char_stack))
    learn_grammar (grammar, op_stack, char_stack);
}

static void
create_start_production (struct Grammar *grammar,
                         char            c)
{
  char prod[2] = { '\0' };

  prod[0] = c;

  grammar_create_rule (grammar, grammar->start, prod);
}

struct Grammar *
parser_get_grammar (char *regex)
{
  struct Grammar *grammar;
  struct Stack *char_stack;
  struct Stack *op_stack;
  int br_count = 0;
  char last = '0';

  grammar = grammar_new ();
  char_stack = stack_new ();
  op_stack = stack_new ();

  stack_push (op_stack, ')');

  for (int i = strlen (regex) - 1; i >= 0; i--)
    {
      if (isspace (regex[i]))
        continue;

      if (isoperator (regex[i]))
        {
          stack_push (op_stack, regex[i]);

          last = '0';
        }
      else if (isbracket (regex[i]))
        {
          if (regex[i] == ')')
            {
              br_count++;
            }
          else
            {
              br_count--;
            }

          stack_push (op_stack, regex[i]);

          last = last == '1' ? '1': '0';
        }
      else if (islower (regex[i]))
        {
          char symbol;

          symbol = grammar_add_terminal (grammar, regex[i]);

          stack_push (char_stack, symbol);
          if (last == '1')
            {
              stack_push (op_stack, '-');
            }

          last = '1';
        }
      else
        {
          utils_throw_error ("grammar: invalid character");
        }

      if (br_count < 0)
        {
          utils_throw_error ("grammar: invalid parenthesis");
        }
    }

  if (br_count != 0)
    {
      utils_throw_error ("grammar: invalid parenthesis");
    }

  stack_push (op_stack, '(');

  //stack_dump (op_stack);
  //stack_dump (char_stack);

  learn_grammar (grammar, op_stack, char_stack);

  if (stack_is_empty(char_stack))
    utils_throw_error ("grammar: error while create grammar");

  create_start_production (grammar, stack_pop (char_stack));

  return grammar;
}
