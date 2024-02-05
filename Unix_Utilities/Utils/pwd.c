#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int
main (void)
{
  size_t buf_size = 20;
  char *buf = NULL;
  if (buf == (char *) NULL)
    {
      errno = ENOMEM;
    }
  buf = getcwd (buf, buf_size);	// it allocates memory with buf_size 
  while (((char *) NULL == buf) && (errno == ERANGE))	// if there is a range error happened and  NULL returned  
    {
      free (buf);
      buf = NULL;
      buf_size += 10;
      buf = getcwd (buf, buf_size);	// allocate more memory ;

    }
  switch (errno)
    {
    case EACCES:
      printf ("errno = %d : Permision Denied\n", errno);
      break;
    case EFAULT:
      printf ("errno = %d : Bad Address \n", errno);
      break;
    case EINVAL:
      printf ("errno = %d : Size argument is zero and buf is not null\n",
	      errno);
      break;
    case ENOENT:
      printf ("errno = %d : Unlinked Dir \n", errno);
      break;
    case ENOMEM:
      printf ("errno = %d : Out of Memory \n", errno);
      break;
    default:
      printf ("%s\n", buf);
    }
  free (buf);
  return 0;
}
