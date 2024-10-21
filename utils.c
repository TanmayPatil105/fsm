/*
 * utils.c
 */

#include "utils.h"

#include <stdlib.h>
#include <errno.h>
#include <string.h>

char
utils_get_next_symbol (void)
{
  /* 'S' is always defined as the start symbol */
  static char label[26] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                            'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                            'T', 'U', 'V', 'W', 'X', 'Y', 'Z', };

  static int index = 0;

  return label[index++];
}


void
utils_throw_error (char *message)
{
  if (message)
    fprintf (stderr, "fsm: %s\n", message);
  else
    fprintf (stderr, "fsm: %s\n", strerror(errno));

  exit (EXIT_FAILURE);
}
