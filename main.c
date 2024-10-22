/*
 * main.c
 */

#include "grammar.h"
#include "parser.h"
#include "utils.h"

int
main (int   argc,
      char *argv[])
{
  struct Grammar *grammar = NULL;

  if (argc < 2)
    {
      utils_throw_error ("Not enough arguments");
    }

  grammar = parser_get_grammar (argv[1]);

  grammar_optimize (grammar);

  grammar_print (grammar);

  /* cleanup */
  grammar_free (grammar);

  return 0;
}
