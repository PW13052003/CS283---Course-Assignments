#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
void print_buff(char *, int);
void usage(char *);
int count_words(char *, int, int);
void word_print(char *, int);
void reversed_string(char *, int);
void replace_word(char *, char *, char *, int);


/*
 * **************************************************
 * FUNCTION NAME: int setup_buff() 
 * PARAMETER(S): char *buff, char *user_str, int len
 * RETURN VALUE(S): str_len 
 * PURPOSE: To process a user provided string and storing it in the buffer
 * **************************************************
 */
int setup_buff(char *buff, char *user_str, int len){
    char *buff_pointer = buff;
    char *string_pointer = user_str;
    int str_len = 0;
    bool last_char = false;

    // Loop through the input string
    while (*string_pointer != '\0') {
	// Check if buffer is breached
	if (str_len >= len) {
		fprintf(stderr, "String length is very long!");
		return -1;
	}
	
	// Check if current character is a whitespace
	if (*string_pointer == ' ' || *string_pointer == '\t') {
		if (!last_char) { 
			*buff_pointer++ = ' ';
			str_len++;
			last_char = true;
		}
	} else {
		*buff_pointer++ = *string_pointer;
		str_len++;
		last_char = false;
	}
	
	string_pointer++;

    }

    if (str_len < len) {
        *(buff + str_len) = '\0';
    }

    // Fill remaining buffer with dots (.....)
    while (str_len < len) {
    	*buff_pointer++ = '.';
    	str_len++;
    }

    return str_len;

}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}


/*
 * **************************************************
 * FUNCTION NAME: int count_words()
 * PARAMETER(S): char *buff, int len, int str_len
 * RETURN VALUE(S): word_count
 * PURPOSE: To calculate the total number of words in a given buffer 
 * **************************************************
 */
int count_words(char *buff, int len, int str_len){
	int word_count = 0;
	bool in_word = false;

	// Traverse through the length of the buffer
	// If the string length is exceeding the buffer, we ignore all characters after the buffer limit
	for (int i = 0; i < str_len; i++) {
		if (buff[i] == ' ') {
			in_word = false;
		} else if (!in_word) {
			word_count++;
			in_word = true;
		}
	}

	return word_count;
}


/*
 * **************************************************
 * FUNCTION NAME: void reversed_string()
 * PARAMETER(S): char *buff, int str_len
 * RETURN VALUE(S): None
 * PURPOSE: To reverse a given string
 * **************************************************
 */
void reversed_string(char *buff, int str_len) {
	int start_of_string = 0;
	int end_of_string = str_len - 1;
	char swap_var;

	// Reverse the string in place
	while (start_of_string < end_of_string) {
		// Swap characters
		swap_var = buff[start_of_string];
		buff[start_of_string] = buff[end_of_string];
		buff[end_of_string] = swap_var;

		// Update pointers
		start_of_string++;
		end_of_string--;
	}
}


/*
 * **************************************************
 * FUNCTION NAME: void word_print()
 * PARAMETER(S): char *buff, int str_len
 * RETURN VALUE(S): None
 * PURPOSE: To print each word from the given buffer along with its length
 * **************************************************
 */
void word_print(char *buff, int str_len) {
	int word_count = 0;
	int current_word_length = 0;
	bool in_word = false;

	printf("Word Print \n----\n");

	// Traverse through the length of the buffer
        // If the string length is exceeding the buffer, we ignore all characters after the buffer limit
	for (int i = 0; i < str_len; i++) {
		if (buff[i] == ' ' || buff[i] == '.') {
			if (in_word) {
				printf(" (%d)\n", current_word_length);
				in_word = false;
			}
		} else {
			if (!in_word) {
				// Start of a new word
				word_count++;
				printf("%d. ", word_count);
				current_word_length = 0;
				in_word = true;
			}

			printf("%c", buff[i]);
			current_word_length++;
		}
	}

	if (in_word) {
		printf(" (%d)\n", current_word_length);
	}
}


/*
 * **************************************************
 * FUNCTION NAME: void replace_word()
 * PARAMETER(S): char *buff, char *actual_word, char *replaced_word, int str_len
 * RETURN VALUE(S): None 
 * PURPOSE: To replace all occurences of a specified word 
 * **************************************************
 */
void replace_word(char *buff, char *actual_word, char *replaced_word, int str_len) {
	char temp_buff[BUFFER_SZ];
	char *temp_pointer = temp_buff;
	char *buff_pointer = buff;

	int actual_len = 0;
	int replaced_len = 0;

	while (actual_word[actual_len] != '\0') actual_len++;
	while (replaced_word[replaced_len] != '\0') replaced_len++;

	// Traverse through the buffer
	while (*buff_pointer != '\0') {
		// Check if the current position matches the actual word
		if (*buff_pointer == *actual_word && strncmp(buff_pointer, actual_word, actual_len) == 0) {
			// Copy replaced word into temp_buff
			for (int i = 0; i < replaced_len; i++) {
				*temp_pointer++ = replaced_word[i];
			}
			buff_pointer += actual_len; // Skip the actual word in the buffer
		}
		else {
			// Copy current character into temp_buff
			*temp_pointer++ = *buff_pointer++;
		}
	}

	*temp_pointer = '\0';
	strncpy(buff, temp_buff, str_len);
}


/*
 * **************************************************
 * FUNCTION NAME: int main()
 * PARAMETER(S): int argc, char *argv[]
 * RETURN VALUE(S): STANDARD EXIT CODES
 * PURPOSE: Main driver
 * **************************************************
 */
int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    /* ******************* THEORY QUESTION ******************* 
    TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    
     --> If argv[1] does not exist, dereferencing it would cause undefined behavior
     
     * ******************* THEORY QUESTION ******************* */
    
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }
	
    /* ******************* THEORY QUESTION *******************
    TODO:  #2 Document the purpose of the if statement below
    
     --> The if statement ensures that the user has provided at least one input string
     
     * ******************* THEORY QUESTION ******************* */
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99

    buff = malloc(BUFFER_SZ);

    if (buff == NULL) {
    	fprintf(stderr, "Error: Failed to allocate memory for buffer.\n");
    	exit(99); 
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

	case 'r':
	    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);
	    if (user_str_len < 0) {
		    exit(3);
	    }
	
	    reversed_string(buff, user_str_len);
	    printf("The string is reversed as follows: %.*s\n", user_str_len, buff);
	    break;

	case 'w': 
	    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);
            if (user_str_len < 0) {
                    exit(3);
            }

            word_print(buff, user_str_len);
            break;	    

    	case 'x':
	    if (argc != 5) {
		fprintf(stderr, "Error: -x requires 3 arguments!");
    		exit(1);
	    }
	    
	    user_str_len = setup_buff(buff, argv[2], BUFFER_SZ);
	    
	    if (user_str_len < 0) {
        	exit(3); 
    	    }
	
    	    replace_word(buff, argv[3], argv[4], BUFFER_SZ);
    	    printf("Modified String: %.*s\n", user_str_len, buff);
    	    break;	    

	default:
            usage(argv[0]);
            exit(1);
    }

    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

/* ******************* THEORY QUESTION *******************
TODO:  #7  Notice all of the helper functions provided in the 
          starter take both the buffer as well as the length.  Why
          do you think providing both the pointer and the length
          is a good practice, after all we know from main() that 
          the buff variable will have exactly 50 bytes?
  
       --> Providing both pointer and length is a good practice because it improves readability.
       --> It also makes the code reusable because we do not need to hardcode the size of the buffer if it changes.
       --> Functions can verify that they don't exceed the buffer's limits.
* ******************* THEORY QUESTION ******************* */


