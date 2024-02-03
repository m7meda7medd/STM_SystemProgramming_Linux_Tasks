#ifndef FEMTO_SHELL_H
#define FEMTO_SHELL_H
/* includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

/* Configurations */
#define INITIAL_BUF_SIZE 20 
#define BUF_SIZE_INC 10
#define ENV_VAR_SIZE 128

/* typedefs */
typedef enum {
STATUS_TRUE ,
STATUS_FALSE 
}ReturnStatus;
/* functions Declarations */
ReturnStatus GetShellMessage(void) ;


#endif
