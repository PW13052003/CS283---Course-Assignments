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
 * FUNCTION NAME: build_cmd_list()
 * PARAMETER(S): char *cmd_line, command_list_t *clist
 * RETURN VALUE(S): STANDARD RETURN CODES
 * PURPOSE: To handle quoted arguments
 * ******************************************************** */

int build_cmd_list (char *cmd_line, command_list_t *clist) {

	// Trim leading spaces 
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

		// Initialize the command buffer
		cmd_buff_t *cmd = &clist->commands[cmd_count];
		memset (cmd, 0, sizeof (cmd_buff_t));

		// Duplicate command string for proper parsing
		cmd->_cmd_buffer = strdup (cmd_token);
		if (cmd->_cmd_buffer == NULL) {
			return ERR_MEMORY;
		}

		char *arg_token = cmd->_cmd_buffer;
		int arg_in_quotes = 0;
		char quote_char = '\0';

		cmd->argc = 0;

		while (*arg_token) {
			
			// Skip leading spaces
			while (*arg_token == SPACE_CHAR && !arg_in_quotes) {
				arg_token++;
			}

			if (*arg_token == '\0') {
				break;
			}

			// Handle quoted arguments
			if (*arg_token == '"' || *arg_token == '\'') {
				arg_in_quotes = 1;
				quote_char = *arg_token++;
			        cmd->argv[cmd->argc++] = arg_token;

				// Find closing quote
				while (*arg_token && *arg_token != quote_char) {
					arg_token++;
				}

				if (*arg_token) {
					*arg_token = '\0';
					arg_token++;
				}

			}

			else {
				// Normal argument
				cmd->argv[cmd->argc++] = arg_token;

				while (*arg_token && !isspace (*arg_token)) {
					arg_token++;
				}

				if (*arg_token) {
					*arg_token = '\0';
					arg_token++;
				}
			}

			// Prevent overflow
			if (cmd->argc >= CMD_ARGV_MAX - 1) {
				break;
			}
		}

		cmd->argv[cmd->argc] = NULL;

		cmd_count++;

		cmd_token = strtok_r(NULL, PIPE_STRING, &save_ptr);
	}

	// Store the number of commands parsed
    	clist->num = cmd_count;
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
	command_list_t clist;
	bool is_pipeline = false;

	while (1) {

		if (!is_pipeline) {
			printf ("%s", SH_PROMPT);
		}

		if (fgets (cmd_line, SH_CMD_MAX, stdin) == NULL) {
			printf("\n");
			break;
		}

		// Remove trailing newline
		cmd_line[strcspn (cmd_line, "\n")] = '\0';

		// Parse command line into a list of commands
		int rc = build_cmd_list (cmd_line, &clist);

		if (rc == WARN_NO_CMDS) {
			printf ("%s", CMD_WARN_NO_CMD);
			continue;
		}

		if (rc == ERR_TOO_MANY_COMMANDS) {
			printf (CMD_ERR_PIPE_LIMIT, CMD_MAX);
			continue;
		}

		is_pipeline = (clist.num > 1);

		// Execute commands in a pipeline
		execute_pipeline (&clist);

		fflush(stdout);
		is_pipeline = false;
	}

	return OK;
}



/* ***********************************************************
 * FUNCTION NAME: execute_pipeline
 * PARAMETER(S): command_list_t *clist
 * RETURN VALUE(S): STANDARD RETURN CODES
 * PURPOSE: Executes multiple piped commands by creating child processes
 * ******************************************************** */

int execute_pipeline (command_list_t *clist) {
	int num_of_cmds = clist->num;
	int pipe_fds[CMD_MAX - 1][2];
	pid_t pids[CMD_MAX];
	int status;

	// Create pipes
	for (int i = 0; i < num_of_cmds - 1; i++) {
		if (pipe (pipe_fds[i]) == -1) {
			perror ("pipe failed");
			return ERR_EXEC_CMD;
		}
	}

	// Fork child processes for each command
	for (int i = 0; i < num_of_cmds; i++) {
		pids[i] = fork();

		if (pids[i] < 0) {
			perror ("fork failed");
			return ERR_EXEC_CMD;
		}

		// Child process
		if (pids[i] == 0) {

			// Redirect input from previous pipe if its not the 1st command
			if (i > 0) {
				dup2 (pipe_fds[i-1][0], STDIN_FILENO);
			}

			// Redirect output to next pipe if its not the last command
			if (i < num_of_cmds - 1) {
				dup2 (pipe_fds[i][1], STDOUT_FILENO);
			}

			else {
				dup2 (STDOUT_FILENO, STDOUT_FILENO);
			}
			// Close all pipes in child process
			for (int j = 0; j < num_of_cmds - 1; j++) {
				close (pipe_fds[j][0]);
				close (pipe_fds[j][1]);
			}

			// Execute the command
			execvp (clist->commands[i].argv[0], clist->commands[i].argv);
			perror ("execvp failed");
			exit (ERR_EXEC_CMD);
		}
	}

	// Parent process closes pipes
	for (int i = 0; i < num_of_cmds - 1; i++) {
		close (pipe_fds[i][0]);
                close (pipe_fds[i][1]);
	}

	// Wait for all child processes
	for (int i = 0; i < num_of_cmds; i++) {
		waitpid (pids[i], &status, 0);
	}

	return OK;
}



