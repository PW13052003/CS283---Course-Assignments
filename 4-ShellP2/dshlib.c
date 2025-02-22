#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */


/* *********************************************************** 
 * FUNCTION NAME: build_cmd_buff()
 * PARAMETER(S): char *cmd_line, cmd_buff_t *cmd_buff
 * RETURN VALUE(S): STANDARD RETURN CODES
 * PURPOSE: To handle quoted arguments
 * ******************************************************** */

int build_cmd_buff (char *cmd_line, cmd_buff_t *cmd_buff) {
	while (*cmd_line == SPACE_CHAR) {
		cmd_line++;
	}

	if (*cmd_line == '\0') {
		return WARN_NO_CMDS;
	}

	// Duplicate command line for processing
	cmd_buff->_cmd_buffer = strdup(cmd_line);
	if (cmd_buff->_cmd_buffer == NULL) {
		return ERR_MEMORY;
	}

	cmd_buff->argc = 0;
	char *arg_token = cmd_buff->_cmd_buffer;

	while (*arg_token) {
		// Skip leading spaces before a new argument
		while (isspace (*arg_token)) {
			arg_token++;
		}

		if (*arg_token == '\0') {
			break;
		}

		// Handle quotes arguments
		if (*arg_token == '"' || *arg_token == '\'') {
			char quote_char = *arg_token++;
			cmd_buff->argv[cmd_buff->argc++] = arg_token;

			// Find the matching closing quote
			while (*arg_token && *arg_token != quote_char) {
				arg_token++;
			}

			if (*arg_token) {
				*arg_token = '\0';
				arg_token++;
			}
		} 

		// Handle normal arguments
		else {
			cmd_buff->argv[cmd_buff->argc++] = arg_token;

			while (*arg_token && !isspace (*arg_token)) {
				arg_token++;
			}

			if (*arg_token) {
				*arg_token = '\0';
				arg_token++;
			}
		}

		// Avoid overflow
		if (cmd_buff->argc >= CMD_ARGV_MAX - 1) {
			break;
		}
	}

	// Null terminate argv list
	cmd_buff->argv[cmd_buff->argc] = NULL;

	return OK;
}


/* *********************************************************** 
 * FUNCTION NAME: Built_In_Cmds exec_built_in_cmd()
 * PARAMETER(S): cmd_buff_t *cmd
 * RETURN VALUE(S): STANDARD RETURN CODES
 * PURPOSE: Handles built-in commands
 * ******************************************************** */

Built_In_Cmds exec_built_in_cmd (cmd_buff_t *cmd) {
	if (cmd->argc == 0) {
		return BI_NOT_BI;  // No command entered
	}

	// Handle exit command
	if (strcmp (cmd->argv[0], "exit") == 0) {
		exit(0);
	}

	// Handle cd command
	if (strcmp (cmd->argv[0], "cd") == 0) {
		if (cmd->argc < 2) {
			return BI_EXECUTED;  // Do nothing if no argument is given
		}
		if (chdir (cmd->argv[1]) != 0) {
			perror ("cd failed");
		}
		return BI_EXECUTED;
	}

	return BI_NOT_BI;
}


/* ***********************************************************
 * FUNCTION NAME: exec_cmd()
 * PARAMETER(S): cmd_buff_t *cmd
 * RETURN VALUE(S): STANDARD RETURN CODES
 * PURPOSE: Executes an external command using fork-exec 
 * ******************************************************** */

int exec_cmd (cmd_buff_t *cmd) {
	if (cmd->argc == 0) {
		return WARN_NO_CMDS;  // No command entered
	}

	// Create a new child process using fork()
	pid_t pid = fork();

	// Check if fork() failed
	if (pid < 0) {
		perror ("fork failed");
			return ERR_EXEC_CMD;
	}

	// Child process execution
	if (pid == 0) {
		execvp (cmd->argv[0], cmd->argv);
		perror("execvp failed");
		exit (ERR_EXEC_CMD);
	}

	// Parent process execution
	else {
		int status;
		waitpid (pid, &status, 0);
		return WEXITSTATUS(status);
	}
}


/* ***********************************************************
 * FUNCTION NAME: exec_local_cmd_loop()
 * PARAMETER(S): NONE
 * RETURN VALUE(S): STANDARD RETURN CODES
 * PURPOSE: Implements the main shell loop
 * ******************************************************** */

int exec_local_cmd_loop() {
	char cmd_line [SH_CMD_MAX];
	cmd_buff_t cmd;

	while (1) {
		printf ("%s", SH_PROMPT);

		if (fgets (cmd_line, SH_CMD_MAX, stdin) == NULL) {
			printf("\n");
			break;
		}

		// Remove trailing newline
		cmd_line[strcspn (cmd_line, "\n")] = '\0';

		// Parse the command into cmd_buff_t
		if (build_cmd_buff (cmd_line, &cmd) != OK) {
			printf ("%s", CMD_WARN_NO_CMD);
			continue;
		}

		// Check if command is built-in (cd, exit)
		Built_In_Cmds bi_cmd = exec_built_in_cmd(&cmd);
		if (bi_cmd == BI_EXECUTED) {
			continue; // Built-in command executed, go to next loop iteration
		}

		// Execute external command
		exec_cmd (&cmd);
	}

	return OK;
}

