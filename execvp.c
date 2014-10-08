#include  <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
void  parse(char *cmd, char **argv)
{
	int i = 0;
	while (*cmd != '\0') {      
        while (*cmd == ' ' || *cmd == '\t' || *cmd == '\n')
			*cmd++ = '\0';     
        *argv++ = cmd;
        while (*cmd != '\0' && *cmd != ' ' && 
				*cmd != '\t' && *cmd != '\n') 
            cmd++;             
    }
    *argv = '\0';         
}
     
void print_cmd_line (char **argv) {
	int i = 0;
	printf ("The whole command line is: ");
	for (i = 0; argv[i]; i++) {
		printf ("%s ", argv[i]);
	}
	printf("\n");
}

void  execute(char **argv)
{
    pid_t pid;
    int status;
	int ampersand_found = 0;
	int pipe_found = 0;
	int in_found = 0;
	int out_found = 0;
	char **p_ptr;
	char **p_ptr2;
	
	if (!*argv) return;

	if(strcmp(*argv,"dir") == 0){
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
	for (i = 0; argv[i]; i++){
		if (strcmp (argv[i], "&") == 0) {
			fprintf (stderr, "Ampersand Found!!\n");
			ampersand_found = 1;
			argv[i] = (char *)NULL;
		}
		
		else if (strcmp (argv[i], "|") == 0) {
			fprintf (stderr, "Pipe Found!!\n");
			pipe_found = 1;
			if(pipe_found == 1){
				p_ptr = &argv[0];
				argv[i] = NULL;
				p_ptr2 = &argv[i+1]; 
				//print_cmd_line(p_ptr, p_ptr2);
			}
		}
		
		else if (strcmp (argv[i], "<") == 0) {
			fprintf (stderr, "in Found!!\n");
			in_found = 1;
			if(in_found == 1){
				p_ptr = &argv[i+1];
				argv[i] = NULL;
				print_cmd_line(p_ptr);
			}
		}
		
		else if (strcmp (argv[i], ">") == 0) {
			fprintf (stderr, "out Found!!\n");
			out_found = 1;
			if(out_found == 1){
				p_ptr = &argv[i+1];
				argv[i] = NULL;
				print_cmd_line(p_ptr);
			}
		}
		
	}
	
	
	if (strcmp(*argv, "exit") == 0){
			exit(0);
	}
	else if ((strcmp(*argv, "cd")) == 0) {
		if ((chdir(argv[1])) != 0)
			fprintf (stderr, "ERR: Cannot change directory!!\n");
    }
	else{
		if(pipe_found == 0 && in_found == 0 && out_found == 0){
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
		else if(pipe_found == 1){
			int p[2];
			if(pipe(p) == -1) {
			  perror("Pipe failed");
			  exit(1);
			}

			if(fork() == 0)     
			{
				close(1);       
				dup(p[1]);  
				close(p[0]);
				close(p[1]);
				execvp(*p_ptr, p_ptr);
				perror("execvp of first fork failed");
				exit(1);
			}

			if(fork() == 0)        
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
		else if(in_found == 1){
			int fdes;
			int cur_in;
			int in;
			fdes = open(*p_ptr, O_RDONLY, 0);
			dup2(fdes, STDIN_FILENO);
			in = 0;
			cur_in = dup(0);
			
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
		else if(out_found == 1){
			int fdes;
			int cur_out;
			int out;
			fdes = open(*p_ptr, O_CREAT|O_TRUNC|O_WRONLY, 0644);
			dup2(fdes, STDOUT_FILENO);
			out = 0;
			cur_out = dup(1);
			
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

void  prompt()
{
    char* pwd;
    char buf[PATH_MAX];

    pwd = getcwd(buf, PATH_MAX);
    printf( "%s : ", pwd );
}

     
void  main(void)
{
    char  cmd[1024]; 
    char  *argv[128];  
    while (1) {
		prompt();
		gets(cmd);             
		printf("\n");
		parse(cmd, argv);       
		execute(argv);
    }
}