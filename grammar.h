/*
 * grammar.h
 */

#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <stdbool.h>

struct GrammarRule
{
  char symbol;

  int n_productions;
  char **productions;
};

struct Grammar
{
  /* Terminal symbols */
  char start;
  int n_terminals;
  char *terminals;

  int n_rules;
  struct GrammarRule **rules;
};

struct Grammar *grammar_new               (void);
void            grammar_create_rule       (struct Grammar *grammar,
                                           char            symbol,
                                           char           *production);
char            grammar_add_terminal      (struct Grammar *grammar,
                                           char            c);
void            grammar_print             (struct Grammar *grammar);
void            grammar_free              (struct Grammar *grammar);

#endif
