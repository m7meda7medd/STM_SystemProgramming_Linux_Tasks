#include<stdio.h>
#include <errno.h>
#define OLD_PATH 1
#define NEW_PATH 2

int
main (int argc, char *argv[])
{
  if ((argc != 3) || (rename (argv[OLD_PATH], argv[NEW_PATH])))	// check wether the arc not equal 3 
    {
      perror("Usage : ./mv Path1 Path2\n");
      return -1;
    }

  return 0;
}
