/*Brando Morley */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sysexits.h>
#include "split.h"
#define INPUT_LINE 10000

extern int errno;

/*This program takes lines from the command line or the standard input and
  executes the program with the arguments from both the standard input and
  command line. If the first argument is -n the programs are not executed but
  instead they are printed*/

int main(int argc, char *argv[]) {
  char buffer[INPUT_LINE], *arguments[INPUT_LINE], **input_arguments;
  int i, flag = 0, j = 0, status;
  pid_t child_pid;
  
  /*checks if there are any arguments or programs to run*/
  if(argc == 1) {
    exit(0);
  }
  
  /*checks if -n was the first argument and sets flag to true*/
  
  if(strcmp(argv[1], "-n") == 0){
    flag = 1;
  }

  /*goes through if loop, when -n was first argument*/
  if(flag == 1) {
    /*Goes line by line and prints what would be printed if -n was not given*/
      while(fgets(buffer, INPUT_LINE, stdin)) {
	/*Goes one line at a time. Split function was given. It splits buffer
	  into an array of strings with no whitespace*/
	      input_arguments = split(buffer);
	      for(i = 2; i < argc; i++) {
	  /*prints strings from command line. Makes sure not to print -n*/ 
	        if(i == argc - 1) {
	          printf("%s", argv[i]);
	        }else {
	          printf("%s ", argv[i]);
	        }
	      }
	      i = 0;
	/*prints strings arguments from standard input*/
	      while(input_arguments[i] != NULL) {
	        printf(" %s", input_arguments[i]);
	        i++;
	      }
	      printf("\n");
      }  
    
  } else { 
    while(fgets(buffer, INPUT_LINE, stdin)) {
      /*This goes line by line and gets arguments from standard input. Uses
        split function mentioned above*/
      input_arguments = split(buffer);
      j = 0;
      for(i = 1; i < argc; i++) {
	/*Makes an array of strings for arguments need in exec. Starts with
	  command line arguments first*/
	      arguments[j] = malloc(strlen(argv[i]) + 1);
	      strcpy(arguments[j], argv[i]);
	      j++;
      }
      i = 0;
      while(input_arguments[i] != NULL) {
	/*Adds in arguments from standard input*/
	      arguments[j] = malloc(strlen(input_arguments[i]) + 1);
	      strcpy(arguments[j], input_arguments[i]);
	      i++;
	      j++;
      }
      /*Adds NULL at end for exec function*/
      arguments[j] = NULL;
      /*Creats child process to execute*/
      child_pid = fork();
      
      /*Checks if fork worked*/
 
      if(child_pid > 0) {
	/*Reaps child process and checks if it failed with any problems. if the
	  child process failed the parent exits with exit status*/
	      wait(&status);
	      if(WIFEXITED(status) && WEXITSTATUS(status)) {
	        exit(WEXITSTATUS(status));
	      }
      }else if (child_pid == 0) {
	/*executes using arguments created above*/
	      execvp(argv[1], arguments);
      }
      i = 0;
      /*Frees all the dynamically created strings used for the arguments*/
      while(arguments[i] != NULL) {
	      free(arguments[i]);
	      i++;
      }
      i = 0;
      /*frees all the dynaically created strings from split function*/
      while(input_arguments[i] != NULL) {
	      free(input_arguments[i]);
	      i++;
      }
      /*frees array list created by split function */
      free(input_arguments);
    }
  }
 
  /*exits with 0 when successful*/
  exit(0);
  return 1;
}
