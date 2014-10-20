/* Mini - Shell 
 * Name: Tarun Naidu
 * ID: 
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char *history[64];
int count = 0;

/*void hist(char *cmd){
	*history = strdup(cmd);
	*history+1;
	count++;
}*/

void  parser(char *cmd, char **argv)
{
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
     
void print_cmd_line (char **history) {			//Print function used for testing
	int i = 0;
	int j = 0;
	printf ("The whole command line is: %d\n", count);
	for (j = 0; j < count; j++) {
			printf ("%s ", history[j]);
		}
	printf("\n");
}

void  syscall(char **argv)				
{
    pid_t pid;					
    int status;
	int ampersand_found = 0;
	
	
	if (!*argv) return;
	int i = 0;
	for (i = 0; argv[i]; i++){
		if (strcmp (argv[i], "&") == 0) {
			fprintf (stderr, "Ampersand Found!!\n");
			ampersand_found = 1;
			argv[i] = (char *)NULL;
		}
	}
	if (strcmp(*argv, "exit") == 0){
			exit(0);
	}
	else if (strcmp(*argv, "history") == 0){
			print_cmd_line(history);
	}
	else if ((strcmp(*argv, "cd")) == 0) {
		if ((chdir(argv[1])) != 0)
			fprintf (stderr, "ERR: Cannot change directory!!\n");
    }
	else{
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
void  main(void)
{
    char  cmd[1024];																					
    char  *argv[128];	
    while (1) {																		
		printf("Shell> ");
		gets(cmd);
		//hist(cmd);
		printf("\n");
		parser(cmd, argv);													
		syscall(argv);																	
    }
}