#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */

int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    // Trim trailing spaces
    while (*cmd_line == SPACE_CHAR) {
	    cmd_line++;
    }
    
    if (*cmd_line == '\0') {
	    return WARN_NO_CMDS;
    }

    memset (clist, 0, sizeof (command_list_t));
    char *cmd_token;
    int cmd_count = 0;

    char *save_ptr;

    // Split the input command line by pipes to extract individual commands
    cmd_token = strtok_r (cmd_line, PIPE_STRING, &save_ptr);
    
    while (cmd_token != NULL) {

	   // Trim leading spaces in each command 
	   while (*cmd_token == SPACE_CHAR) {
		  cmd_token++;
	   }

	   // Check if there are too many commands
	   if (cmd_count >= CMD_MAX) {
		   return ERR_TOO_MANY_COMMANDS;
	   } 

	   // Initialize the current command's executable and argument buffers
	   memset(clist->commands[cmd_count].exe, 0, EXE_MAX);
           memset(clist->commands[cmd_count].args, 0, ARG_MAX);

	   char *arg_save_ptr;
           char *arg_token = strtok_r(cmd_token, " ", &arg_save_ptr);
	   
	   if (arg_token == NULL) {
		   // Move to next command
		   cmd_token = strtok_r(NULL, PIPE_STRING, &save_ptr);
		   continue;
	   }

	   // Store the executable name in the command list
	   strncpy (clist->commands[cmd_count].exe, arg_token, EXE_MAX - 1);
	   clist->commands[cmd_count].exe[EXE_MAX - 1] = '\0';

	   // Extract arguments
	   char args_buffer[ARG_MAX] = "";

	   arg_token = strtok_r(NULL, "", &arg_save_ptr); // Preserve spaces in arguments

	   // Store the arguments in the command list
	   if (arg_token != NULL) {
		   strncpy (args_buffer, arg_token, ARG_MAX - 1);
		   args_buffer[ARG_MAX - 1] = '\0';
	   }
	   
	   // Store parsed values in clist
	   strncpy (clist->commands[cmd_count].args, args_buffer, ARG_MAX - 1);
	   clist->commands[cmd_count].args[ARG_MAX - 1] = '\0';

	   cmd_count++;

	   // Get next command
	   cmd_token = strtok_r (NULL, PIPE_STRING, &save_ptr);

    }

    // Store the number of commands parsed
    clist->num = cmd_count;
    return OK;
}


