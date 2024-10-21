/*
 * grammar.c
 */

#include "grammar.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Grammar *
grammar_new (void)
{
  struct Grammar *grammar;

  grammar = malloc (sizeof (struct Grammar));

  grammar->start = 'S';
  grammar->n_terminals = 0;
  grammar->terminals = NULL;
  grammar->n_rules = 0;
  grammar->rules = NULL;

  return grammar;
}

static struct GrammarRule *
grammar_new_single_production (char symbol,
                               char *production)
{
  struct GrammarRule *rule;

  rule = malloc (sizeof (struct GrammarRule));

  rule->symbol = symbol;
  rule->n_productions = 0;

  rule->productions = malloc (sizeof (char *));

  rule->productions[rule->n_productions++] =
    strdup (production);      /* Always a good idea to own a string */

  return rule;
}

void
grammar_create_rule (struct Grammar *grammar,
                     char            symbol,
                     char           *production)
{
  grammar->rules = realloc (grammar->rules,
                            sizeof (struct GrammarRule)
                            * (grammar->n_rules + 1));

  grammar->rules[grammar->n_rules++] =
    grammar_new_single_production (symbol, production);
}

static char
grammar_get_unit_non_terminal (struct Grammar *grammar,
                               char            terminal)
{
  char prod[2] = { '\0' };

  prod[0] = terminal;

  for (int i = 0; i < grammar->n_rules; i++)
    {
      struct GrammarRule *rule;

      rule = grammar->rules[i];

      if (rule->n_productions > 0
          && (strncmp (rule->productions[0], prod, 2) == 0))
        return rule->symbol;
    }

  utils_throw_error ("grammar_get_unit_non_terminal: unreachable code");

  return '\0';
}

char
grammar_add_terminal (struct Grammar *grammar,
                      char            c)
{
  char symbol;
  char prod[2] = { '\0' };

  for (int i = 0; i < grammar->n_terminals; i++)
    {
      /* check if terminal already exists */
      if (grammar->terminals[i] == c)
        return grammar_get_unit_non_terminal (grammar, c);
    }

  /* we don't have the terminal */
  grammar->terminals = realloc (grammar->terminals,
                            sizeof (char) * (grammar->n_terminals + 1));

  grammar->terminals[grammar->n_terminals++] = c;

  symbol = utils_get_next_symbol ();
  prod[0] = c;
  grammar_create_rule (grammar, symbol, prod);

  return symbol;
}

void
grammar_rule_print (struct GrammarRule *rule)
{
  printf ("%c -> ", rule->symbol);

  for (int i = 0; i < rule->n_productions; i++)
    {
      if (i == rule->n_productions - 1)
        printf ("%s\n", rule->productions[i]);
      else
        printf ("%s | ", rule->productions[i]);
    }
}

void
grammar_print (struct Grammar *grammar)
{
  int i = grammar->n_rules;

  for (; --i >= 0; )
    {
      grammar_rule_print (grammar->rules[i]);
    }
}

static void
grammar_rule_free (struct GrammarRule *rule)
{
  for (; --rule->n_productions >= 0; )
    {
      if (rule->productions[rule->n_productions] != NULL)
        free (rule->productions[rule->n_productions]);
    }
}

void
grammar_free (struct Grammar *grammar)
{
  if (grammar->terminals)
    free (grammar->terminals);

  for (; --grammar->n_rules >= 0; )
    {
      if (grammar->rules[grammar->n_rules] != NULL)
        grammar_rule_free (grammar->rules[grammar->n_rules]);
    }
}
