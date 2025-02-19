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

int build_cmd_buff(char *cmd_line, command_t *cmd_buff)
{
    // Trim trailing spaces
    while (*cmd_line == SPACE_CHAR) {
	    cmd_line++;
    }
    
    if (*cmd_line == '\0') {
	    return WARN_NO_CMDS;
    }
    
    // Initialize the struct
    memset (cmd_buff, 0, sizeof (command_t));

    // Tokenize input into argv[]
    char *save_ptr;
    char *cmd_token = strtok_r (cmd_line, " ", &save_ptr);
    int arg_count = 0;
    bool quotes = false;

    while (cmd_token != NULL && arg_count < CMD_ARGV_MAX - 1) {
	    // Handle quoted strings
            if (*cmd_token == '"') {
		    quotes = true;
                    cmd_token++;  // Skip the first quote
            }
	    
	    cmd_buff->argv[arg_count] = cmd_token;
	    arg_count++;
	    
	    if (quotes && cmd_token[strlen(cmd_token) - 1] == '"') {
		    cmd_token[strlen(cmd_token) - 1] = '\0';
		    quotes = false;
	    }
	    
	    cmd_token = strtok_r (NULL, " ", &save_ptr);
    }

    cmd_buff->argv[arg_count] = NULL;
    cmd_buff->argc = arg_count;

    return OK;
}

Built_In_Cmds exec_built_in_cmd (command_t *cmd) {

	if (cmd->argc == 0) {
		return BI_NOT_BI;
	}

	if (strcmp (cmd->argv[0], EXIT_CMD) == 0) {
		exit(0);
	}

	if (strcmp (cmd->argv[0], "cd") == 0) {
		
		if (cmd->argc == 1) {
			return BI_EXECUTED;
		}

		else if (cmd->argc == 2) {
			if (chdir(cmd->argv[1]) != 0) {
				perror("cd error");  // Print error if directory change fails
				return ERR_EXEC_CMD;
			}
			return ERR_EXEC_CMD;
		}

		else {
			printf ("Too many arguments\n");
			return ERR_CMD_ARGS_BAD;
		}
	}

	return BI_NOT_BI;
}

int exec_cmd (command_t *cmd) {
	
	pid_t pid = fork();

	if (pid < 0) {
		perror ("Error: fork");
		return ERR_EXEC_CMD;
	}

	if (pid == 0) {
		// Execute command
		execvp (cmd->argv[0], cmd->argv);
		perror("Error: exec");
		exit (ERR_EXEC_CMD);
	}

	else {
		int status;
		waitpid (pid, &status, 0);
		return WEXITSTATUS(status);
	}
}

int exec_local_cmd_loop() {

	char input_line[SH_CMD_MAX];
	command_t cmd;

	while (1) {
		printf("%s", SH_PROMPT);

		if (fgets (input_line, SH_CMD_MAX, stdin) == NULL) {
			printf("\n");
			break;
		}

		input_line[strcspn (input_line, "\n")] = '\0';

		// Trim spaces and parse command
		if (build_cmd_buff (input_line, &cmd) == WARN_NO_CMDS) {
			printf("%s", CMD_WARN_NO_CMD);
			continue;
		}

		// Check for built in commands
		Built_In_Cmds bi_cmd = exec_built_in_cmd(&cmd);

		if (bi_cmd == BI_EXECUTED) {
			continue;
		}

		// If not built in, execute it using fork or execvp
		exec_cmd(&cmd);
	}

	return OK;
}

	


