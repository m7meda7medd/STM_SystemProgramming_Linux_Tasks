#include <stdio.h>
#include <unistd.h>
#include <string.h>

int
main (int argc, char *argv[])
{
  unsigned char count = 1;
  int num_write = 0;
  while (count != argc)
    {
      num_write = write (1, argv[count], strlen (argv[count]));	// echo the arg

      num_write = write (1, " ", 2);	// echo space between each arg 
      if (num_write == -1)	// check if there is an error happened
	{
	  return -1;
	}
      count++;
    }

  num_write = write (1, "\n", 2);	// echo space between each argument

  return 0;
}
