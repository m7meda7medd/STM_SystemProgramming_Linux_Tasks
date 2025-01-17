#include "picoshell.h"
#include<signal.h>
/* static functions Declarations */
static char **Parser (unsigned long *);
static int echo (int argc, char **argv);
static int pwd (void);
static int has_char(char* str , char ch) ;
/*****************************************/
void SigintHandler(int sig_num)
{
fflush(stdout) ;
printf("\n") ;
printf ("%s@stm-linux:$ ", getlogin());
fflush(stdout) ;
}	
ReturnStatus
GetShellMessage (void)
{
  char *env_user = NULL, **tokens = NULL;
  unsigned long argc;
  int err;
  char* key ; 
  char* value ;
  pid_t pid = -1;
  ReturnStatus status = STATUS_TRUE ;
  env_user = getlogin ();
  signal(SIGINT ,SigintHandler) ;
  printf ("%s@stm-linux:$ ", env_user);
  tokens = Parser (&argc);
  if (argc != 0)
    {
      if (0 == strcmp (tokens[0], "exit"))
	{
	  status = STATUS_FALSE;
	}
      else if (0 == strcmp (tokens[0], "echo"))
	{
	  err = echo (argc, tokens);
	  if (err != 0)
	    {
	      printf ("errno = %d :Error occurred with echo", errno);
	    }
	}
      else if (0 == strcmp (tokens[0], "pwd"))
	{
	  err = pwd ();
	  if (err != 0)
	    {
	      printf ("errno = %d :Error occurred with pwd", errno);
	    }
	}
      else if (0 == strcmp (tokens[0], "cd"))
	{
	  if (argc != 2)
	    {
	      printf ("Usage of cd is: cd path\n");
	    }
	  else
	    {
	      if (chdir (tokens[1]) == 0)
		{
		  // done 
		}
	      else
		{
		  printf ("errno = %d:Error occurred with cd\n", errno);
		}
	    }

	}
      else if ( 0 == strcmp(tokens[0],"unset"))
      {
	err = unsetenv(tokens[1]) ;
	if (err)
	{
	printf("Error to unset env %s",tokens[1]) ;
	}

      }
      else if (has_char(tokens[0],'=')) 
 	{ 
	   key  = strtok(tokens[0] , "=") ;
	   value = strtok( NULL , "=" ) ;

	   err = setenv (key, value, 1);
	 
          if (err)
            {
              printf ("Error to Set %s", key);
            }

	
	}
      else
	{
	  pid = fork ();
	  if (pid > 0)		// Parent 
	    {
	      pid_t returned_child;
	      int wstatus;
	      unsigned char res_wstatus = 0;
	      returned_child = wait (&wstatus);

	      /*  // For Debugging Purposes 
	         res_wstatus = ((unsigned char) (WIFEXITED(wstatus) << 0) |
	         (unsigned char) (WEXITSTATUS(wstatus) << 1) |
	         (unsigned char) (WIFSIGNALED(wstatus) << 2) | (unsigned char) (WTERMSIG(wstatus) << 3) | (unsigned char) (WCOREDUMP(wstatus) << 4 ) | (unsigned char) (WIFSTOPPED(wstatus)<<5) | (unsigned char) (WSTOPSIG(wstatus) << 6) | (unsigned char) (WIFCONTINUED(wstatus) << 7)) ;
	         switch (res_wstatus){ // for futute edit 
	         case 1  :
	         printf("res_wstatus = %d\n",res_wstatus) ;break ;

	         case 2 :
	         printf("res_wstatus = %d\n",res_wstatus) ;
	         break ;

	         case 4 : printf("res_wstatus = %d\n",res_wstatus)  ;break ;

	         case 8 : printf("res_wstatus = %d\n",res_wstatus)  ;break ;

	         case 16 : printf("res_wstatus = %d\n",res_wstatus)  ;break ;

	         case 32 : printf("res_wstatus = %d\n",res_wstatus)  ;break ;

	         case 64 : printf("res_wstatus = %d\n",res_wstatus) ; break ;

	         case 128 :
	         printf("res_wstatus = %d\n",res_wstatus) ; 
	         break ; 
	         default : 
	         printf("Error with wait result \n") ;        
	         }
	       */

	    }
	  else if (pid == 0)	// child 
	    {
	      if (execvp (tokens[0], tokens) == -1);
	      {
		// printf("%s : Can't execute with errno = %d\n",tokens[0],errno) ;
		printf ("%s :command not found\n", tokens[0]);
	      }

	    }
	  else
	    {

	      printf ("Failed to fork\n");
	    }
	}
    }
  else
    {

    }
  for (unsigned long i = 0; i < argc; i++)
    free (tokens[i]);
  free (tokens);
  return status;
}

char **
Parser (unsigned long *argc)
{
  size_t index = 0;
  char ch;
  char* l_str = NULL ;
  size_t str_len;
  char ch2 ; 
  char quotes = 0;
  char env  = 0 ;
  char pipe =  0; 
  char redir = 0 ;
  char special_char = 0 ;
  char **argv = NULL;
  unsigned char start = 1;	// start ==  1 when we will start new token
  unsigned long l_argc = 0;	// number of tokens    
  fflush (stdin);	  // fflush stdin to avoid buffer undefined behaviour 
  while ((ch = getchar()) != '\n')
    { 	
      if ((ch != ' '))		// if char is not space 
	{
	  if (start == 1)	// if it's about to start new token
	    {
	      l_argc++;		// increase number of tokens by one
	      argv = (char **) realloc (argv, sizeof (char *) * l_argc);	// allocate space for the new pointer to the token 
	      if (argv == NULL)	// if it can't allocate print and break
		{
		  // printf ("Can't Allocate = 1");
		  break;
		}
	      argv[l_argc - 1] = NULL;	// avoid dangling pointer :D
	      start = 0;
	    }
	   if ((ch == '|')&& (special_char == 0)) 
	   {


	   }
	   if (ch == '\\')
	   {
	    special_char = 1 ;
	    continue ;
	   }
	   if ((ch == '$') && (index == 0) && (special_char == 0))
	   {
	  	env = 1 ;
		continue  ;
	   }
	   if ( (special_char == 0) && ((ch == '\"') || (ch == '\'')))
            {
              quotes = 1;
              while ((ch2 = getchar ()) != ch)
                {
                  if (ch2 == '\n')
                    {
                      printf ("> ");
                    }
	   if (ch2 == '\\')
           {
            special_char = 1 ;
            continue ;
           }
                  if ((ch2 == ch) && (special_char == 0))
                    {

                      quotes = 0;
		      if (special_char)
			      special_char = 0 ;
                    }
                  if (quotes == 1)
                    {
                      argv[l_argc - 1] = (char *) realloc (argv[l_argc - 1], sizeof (char) * (index + 1));      //alocate memory for the character .
                      if (argv[l_argc - 1] == NULL)     // if it can't allocate print and break
                        {
                          // printf ("Can't Allocate = 2");
                          break;
                        }
                      argv[l_argc - 1][index] = ch2;    // store the char
                      index++;
		    }
                }
            }
	else{	
	  argv[l_argc - 1] = (char *) realloc (argv[l_argc - 1], sizeof (char) * (index + 1));	//alocate memory for the character .
	  if (argv[l_argc - 1] == NULL)	// if it can't allocate print and break
	    {
	      // printf ("Can't Allocate = 2"); 
	      break;
	    }
	
	  argv[l_argc - 1][index] = ch;	// store the char
	  index++;
	  if (special_char)
		  special_char = 0 ;
	  
	   }
	}
      else if ((start == 0))	// if start == 0 and ch == space , allocate space for ch and store '\0'
	{
	  start = 1;
	  argv[l_argc - 1] =
	    (char *) realloc (argv[l_argc - 1], sizeof (char) * (index + 1));
	  if (argv[l_argc - 1] == NULL)	// same as above xD 
	    {
	      //  printf ("Can't Allocate = 3");
	      break;
	    }
	  argv[l_argc - 1][index] = '\0';
	  index = 0;		// reset index
       if (env == 1)
            {
              l_str = NULL;
              l_str = getenv (argv[l_argc - 1]);
              if (l_str != NULL)
                {
                  str_len = strlen (l_str);
                  argv[l_argc - 1] =
                    (char *) realloc (argv[l_argc - 1],
                                      sizeof (char) * (str_len+1));
                  strcpy (argv[l_argc - 1], l_str);
                  argv[l_argc - 1][str_len] = '\0';
                }
	      else {
		int i = 0 ;
		while (argv[l_argc-1][i] != 0)
		{
		argv[l_argc-1][i] = 0 ;
		i++ ;
		}
	      }
              env = 0;
	    }

	}
      else			// else  space and start == 1 
	{
	  // do nothing 
	  // don't be like this else :D
	}
    }
  if (start == 0)		// after looping if start == 0 put '\0' 
    {
      argv[l_argc - 1] =
	(char *) realloc (argv[l_argc - 1], sizeof (char) * (index + 1));
      if (argv[l_argc - 1] == NULL)
	{
	  printf ("Can't Allocate = 4");
	}
      argv[l_argc - 1][index] = '\0';
      if (env == 1)
            {
              l_str = NULL;
              l_str = getenv (argv[l_argc - 1]);
              if (l_str != NULL)
                {
                  str_len = strlen (l_str);
                  argv[l_argc - 1] =
                    (char *) realloc (argv[l_argc - 1],
                                      sizeof (char) * (str_len+1));
                  strcpy (argv[l_argc - 1], l_str);
                  argv[l_argc - 1][str_len] = '\0';
                }
	       else {
                int i = 0 ;
                while (argv[l_argc-1][i] != 0)
                {
                argv[l_argc-1][i] = 0 ;
                i++ ;
                } 
	       }
              env = 0;
            }


    }

  argv = (char **) realloc (argv, sizeof (char *) * l_argc + 1);	// allocate space for the new pointer to the token  
  argv[l_argc] = NULL;		// Make The Last Pointer Pointing to NULL argv[argc] == NULL :D

  *argc = l_argc;
  return argv;
}

static int
echo (int argc, char **argv)	// echo implementation 
{
  unsigned char count = 1;
  int num_write = 0;
  while (count != argc)
    {  
      num_write = write (1, argv[count], strlen (argv[count]));	// echo 
      if (count != argc-1)
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


static int
pwd (void)			// built in pwd 
{
  size_t buf_size = INITIAL_BUF_SIZE;
  char *buf = NULL;
  if (buf == (char *) NULL)
    {
      errno = ENOMEM;
    }
  buf = getcwd (buf, buf_size);
  while (((char *) NULL == buf) && (errno == ERANGE))	// if there is a range error happened and  NULL returned
    {
      free (buf);
      buf = NULL;
      buf_size += BUF_SIZE_INC;
      buf = getcwd (buf, buf_size);

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
static int has_char(char* str , char ch) {
int i = 0 ;
int status = 0 ;
while (str[i] != '\0')
{
 if (str[i] == ch)
 {

 status = 1 ;

 }
 i++ ;
}

return status ;
}
