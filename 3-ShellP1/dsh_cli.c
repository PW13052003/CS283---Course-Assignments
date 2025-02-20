#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dshlib.h"

/*
 * Implement your main function by building a loop that prompts the
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.  Since we want fgets to also handle
 * end of file so we can run this headless for testing we need to check
 * the return code of fgets.  I have provided an example below of how
 * to do this assuming you are storing user input inside of the cmd_buff
 * variable.
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
 *
 *   Expected output:
 *
 *      CMD_OK_HEADER      if the command parses properly. You will
 *                         follow this by the command details
 *
 *      CMD_WARN_NO_CMD    if the user entered a blank command
 *      CMD_ERR_PIPE_LIMIT if the user entered too many commands using
 *                         the pipe feature, e.g., cmd1 | cmd2 | ... |
 *
 *  See the provided test cases for output expectations.
 */
int main()
{
    char cmd_buff[SH_CMD_MAX];
    int rc = 0;
    command_list_t clist;

    while (1) {
	    // Print the shell prompt
	    printf("%s", SH_PROMPT);

	    if (fgets (cmd_buff, SH_CMD_MAX, stdin) == NULL) {
		    printf("\n");
		    break; // Exit on End of File
	    }

	    // Remove trailing newline
	    cmd_buff [strcspn (cmd_buff, "\n")] = '\0';

	    // Check if exit command is entered by the user and exit accordingly
	    if (strcmp (cmd_buff, EXIT_CMD) == 0) {
		    //break;
		    exit(0);
	    }

	    // Parse command using build_cmd_list
	    rc = build_cmd_list (cmd_buff, &clist);

	    // Handle return codes
	    if (rc == WARN_NO_CMDS) {
		    // Print warning that no commands provided
		    printf("%s", CMD_WARN_NO_CMD);
	    }

	    else if (rc == ERR_TOO_MANY_COMMANDS) {
		    // Print error message that too many commands provided
		    printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
	    }

	    else if (rc == OK) {
		    // Print the total number of commands parsed
		    printf(CMD_OK_HEADER, clist.num);
		    
		    // Iterate through each command in the command list
		    for (int i = 0; i < clist.num; i++) {
			    if (strlen (clist.commands[i].args) > 0) {
				    // Print details for each command
				    printf("<%d> %s [%s]\n",  i+1, clist.commands[i].exe, clist.commands[i].args);
			    }

			    else {
				    printf("<%d> %s\n", i+1, clist.commands[i].exe);
			    }
		    }
	    }
    }

    return 0;
}


