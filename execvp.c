/* Mini - Shell 
 * Name: Shashank Mucheli Sukumar
 * ID: 01442857
 * 
 * Developed at 101-D, CIS Research Labs, Textile Building, UMass Dartmouth.
 *
 * 
 * Course CIS 570 - Advanced Computing Systems.
 * Instructor: Dr. Paul Gracia.
 * Computer and Information Science Department.
 * University Of Massachusetts Dartmouth.
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
void  parse_string(char *cmd, char **argv)											//Manipulating the argument vector to parse it to the execvp function call
{
	int i = 0;
	while (*cmd != '\0') {      
        while (*cmd == ' ' || *cmd == '\t' || *cmd == '\n')
			*cmd++ = '\0';     
        *argv++ = cmd;														//marking the address location of the first word of the argument.
        while (*cmd != '\0' && *cmd != ' ' && 
				*cmd != '\t' && *cmd != '\n') 
            cmd++;             
    }
    *argv = '\0';         
}
     
void print_cmd_line (char **argv) {											//This function is only for testing
	int i = 0;
	printf ("The whole command line is: ");
	for (i = 0; argv[i]; i++) {
		printf ("%s ", argv[i]);
	}
	printf("\n");
}

void  exec(char **argv)													//Game starts here!
{
    pid_t pid;																//Declaring the processID
    int status;
	int ampersand_found = 0;												//Series of flags
	int pipe_found = 0;
	int in_found = 0;
	int out_found = 0;
	char **p_ptr;															//string pointers used for piping
	char **p_ptr2;															//string pointers used for piping
	
	if (!*argv) return;														//Used to avoid seg faults when no input argument is given

	if(strcmp(*argv,"dir") == 0){											//Manipulation of DOS commands to work in UNIX system
		*argv = strdup("ls");
		printf("The command entered is %s\n",*argv);
	}
	else if(strcmp(*argv,"del") == 0){
		*argv = strdup("rm");
		printf("The command entered is %s\n",*argv);
	}
	else if(strcmp(*argv,"copy") == 0){
		*argv = strdup("cp");
		printf("The command entered is %s\n",*argv);
	}
	else if(strcmp(*argv,"move") == 0){
		*argv = strdup("mv");
		printf("The command entered is %s\n",*argv);
	}
	else if(strcmp(*argv,"rename") == 0){
		*argv = strdup("mv");
		printf("The command entered is %s\n",*argv);
	}
	else if(strcmp(*argv,"type") == 0){
		*argv = strdup("cat");
		printf("The command entered is %s\n",*argv);
	}
	else if(strcmp(*argv,"more") == 0){
		if (strcmp (argv[1],"<") == 0) {
			*argv = strdup("more");
			printf("The command entered is %s\n",*argv);
		}
	}
	else if(strcmp(*argv,"md") == 0){
		*argv = strdup("mkdir");
		/* print_cmd_line(argv); */
		printf("The command entered is %s\n",*argv);
	}
	else if(strcmp(*argv,"rd") == 0){
		*argv = strdup("rmdir");
		printf("The command entered is %s\n",*argv);
	}
	else if(strcmp(*argv,"cls") == 0){
		strcpy(*argv,"clear");
		printf("The command entered is %s\n",*argv);
	}
	int i = 0;
	for (i = 0; argv[i]; i++){														//Scanning the string to for an ampersand symbol
		if (strcmp (argv[i], "&") == 0) {
			fprintf (stderr, "Ampersand Found!!\n");
			ampersand_found = 1;
			argv[i] = (char *)NULL;
		}
		
		else if (strcmp (argv[i], "|") == 0) {										//Scanning the string to for pipe symbol
			fprintf (stderr, "Pipe Found!!\n");
			pipe_found = 1;
			if(pipe_found == 1){
				p_ptr = &argv[0];													//Records the command on the left side of the pipe
				argv[i] = NULL;
				p_ptr2 = &argv[i+1];												//Records the command on the right side of the pipe 
				//print_cmd_line(p_ptr, p_ptr2);									//entirely for testing
			}
		}
		
		else if (strcmp (argv[i], "<") == 0) {										//Scanning for input redirection
			fprintf (stderr, "in Found!!\n");
			in_found = 1;
			if(in_found == 1){
				p_ptr = &argv[i+1];
				argv[i] = NULL;
				//print_cmd_line(p_ptr);											//solely for testing
			}
		}
		
		else if (strcmp (argv[i], ">") == 0) {										//Scanning for output redirection
			fprintf (stderr, "out Found!!\n");
			out_found = 1;
			if(out_found == 1){
				p_ptr = &argv[i+1];
				argv[i] = NULL;
				//print_cmd_line(p_ptr);											//solely for testing
			}
		}
		
	}
	
	
	if (strcmp(*argv, "exit") == 0){												//scanning for the exit keyword
			exit(0);
	}
	else if ((strcmp(*argv, "cd")) == 0) {
		if ((chdir(argv[1])) != 0)													//directory traversal
			fprintf (stderr, "ERR: Cannot change directory!!\n");
    }
	else{
		if(pipe_found == 0 && in_found == 0 && out_found == 0){						//executes this section only if there are no admissible special characters in the string
			pid = fork();															//creating child process
			if (pid < 0) {															//checking if the child process was created succefully     
				fprintf (stderr, "ERR: forking child process failed!!\n");
				exit(1);
			}
			else if (pid == 0) {													//system call is executed only in the child process
				if (execvp(*argv, argv) < 0) {										//execvp call is made to process the system call
					fprintf (stderr, "ERR: exec failed!!\n");
					exit(1);
				}
			}
			else {                                  
				if (ampersand_found == 0)											//exectution of the child is pushed to background when an ampersand symbol is found
					while (wait(&status) != pid);
			}
		}
		else if(pipe_found == 1){													//code for the pipe operations
			int p[2];
			if(pipe(p) == -1) {														//pipe library function is invoked
			  perror("Pipe failed");
			  exit(1);
			}

			if(fork() == 0)															//forking to execute left side of the pipe     
			{
				close(1);       
				dup(p[1]);  
				close(p[0]);
				close(p[1]);
				execvp(*p_ptr, p_ptr);
				perror("execvp of first fork failed");
				exit(1);
			}

			if(fork() == 0)															//forking to execute left side of the pipe
			{
				close(0);          
				dup(p[0]);     
				close(p[1]);   
				close(p[0]);
				execvp(*p_ptr2, p_ptr2);
				perror("execvp of second fork failed");
				exit(1);
			}

			close(p[0]);
			close(p[1]);
			wait(0);
			wait(0);
		}
		else if(in_found == 1){														//input redirection operations
			//int fdes;
			//int cur_in;
			int in;
			/*fdes = open(*p_ptr, O_RDONLY, 0);
			dup2(fdes, 0);
			in = 0;*/
			
			in = open(*p_ptr, O_RDONLY);
			dup2(in, STDIN_FILENO);
			/*if(dup2(in, STDIN_FILENO) < 0){
				fprintf(stderr, "dup not 1\n");
				exit(1);
			}*/
			close(in);
			//cur_in = dup(0);
			pid = fork();
			if (pid < 0) {     
				fprintf (stderr, "ERR: forking child process failed!!\n");
				exit(1);
			}
			else if (pid == 0) {
				if (execvp(*argv, argv) < 0) {
					fprintf (stderr, "ERR: exec failed!!\n");
					exit(1);
				}
			}
			else {                                  
				if (ampersand_found == 0)
					while (wait(&status) != pid);
			}
		}
		else if(out_found == 1){													//output redirection operations
			//int fdes;
			//int cur_out;
			int out;
			
			//out = 0;
			//cur_out = dup(1);
			out = open(*p_ptr, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
			dup2(out, STDOUT_FILENO);
			/*if(dup2(out, STDOUT_FILENO) < 0){
				fprintf(stderr, "dup not 1\n");
				exit(1);
			}*/
			close(out);
			//cur_out = dup(1);
			pid = fork();
			if (pid < 0) {     
				fprintf (stderr, "ERR: forking child process failed!!\n");
				exit(1);
			}
			else if (pid == 0) {
				if (execvp(*argv, argv) < 0) {
					fprintf (stderr, "ERR: exec failed!!\n");
					exit(1);
				}
			}
			else {                                  
				if (ampersand_found == 0)
					while (wait(&status) != pid);
			}
		}
	}
}

void  cmd_prompt()																	//function to display the prompt using getcwd function
{
    char* pwd;
    char buf[PATH_MAX];

    pwd = getcwd(buf, PATH_MAX);
    printf( "%s : ", pwd );
}

     
void  main(void)
{
    char  cmd[1024];																//input string 
    char  *argv[128]; 
    while (1) {																		//infinte loop to keep the shell running
		cmd_prompt();
		gets(cmd);             
		printf("\n");
		parse_string(cmd, argv);													//processing the input string
		exec(argv);																	//exectuting the string
    }
}