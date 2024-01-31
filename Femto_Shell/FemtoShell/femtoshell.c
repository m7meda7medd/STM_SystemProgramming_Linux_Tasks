#include <stdio.h>
#include <string.h>
#include "femtoshell.h"
ReturnStatus GetShellMessage(void)
{
 ReturnStatus status = STATUS_TRUE ; 	
 char msg[MAX_MSG_SIZE] ;
 char ch ;
 unsigned char counter = 0 ;
 printf("I can't hear you Say something > ") ;
 while ((ch = getchar()) != '\n')
 {
 	msg[counter] = ch ;
	counter++ ;
 } 
 msg[counter] = '\0' ;
 if (0 == strcmp(msg,"exit"))
 	{
		printf("I got it, Bye Dear\n") ;
		status = STATUS_FALSE ;
	}
 else
 	{
		 printf("you Said: %s? \n",msg) ;
	}
 return status ;
}
