#include  <stdio.h>
#include  <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
char temp[100];
void  parse(char *cmd, char **argv)
{
	int i = 0;
	while (*cmd != '\0') {      
        while (*cmd == ' ' || *cmd == '\t' || *cmd == '\n')
			*cmd++ = '\0';     
        *argv++ = cmd;          
	//temp = *(argv-1);
	if(i = 0){
		printf("temp value is: %s\n",*(argv-1));
		i=1;
	}
        while (*cmd != '\0' && *cmd != ' ' && 
				*cmd != '\t' && *cmd != '\n') 
            cmd++;             
    }
    *argv = '\0';         
}
     
void  execute(char **argv)
{
    pid_t pid;
    int status;
	
	//strcpy(temp, *argv);
	
	if(strcmp(*argv,"dir") == 0){
		strcpy(*argv,"ls");
		printf("The command entered is %s\n",*argv);
	}
	else if(strcmp(*argv,"del") == 0){
		strcpy(*argv,"rm");
		printf("The command entered is %s\n",*argv);
	}
	else if(strcmp(*argv,"copy") == 0){
		strcpy(*argv,"cp");
		printf("The command entered is %s\n",*argv);
	}
	else if(strcmp(*argv,"move") == 0){
		strcpy(*argv,"mv");
		printf("The command entered is %s\n",*argv);
	}
	else if(strcmp(*argv,"rename") == 0){
		strcpy(*argv,"mv");
		printf("The command entered is %s\n",*argv);
	}
	else if(strcmp(*argv,"type") == 0){
		strcpy(*argv,"cat");
		printf("The command entered is %s\n",*argv);
	}
	else if(strcmp(*argv,"more<") == 0){
		strcpy(*argv,"more");
		printf("The command entered is %s\n",*argv);
	}
	else if(strcmp(*argv,"md") == 0){
		strcpy(*argv,"mkdir");
		printf("The command entered is %s\n",*argv);
	}
	else if(strcmp(*argv,"rd") == 0){
		strcpy(*argv,"rmdir");
		printf("The command entered is %s\n",*argv);
	}
	else if(strcmp(*argv,"cls") == 0){
		strcpy(*argv,"clear");
		printf("The command entered is %s\n",*argv);
	}
	
	
    if ((strcmp(*argv, "cd")) == 0) {
		if ((chdir(argv[1])) != 0)
			printf("Cannot change directory\n");
    }
	else{
		if ((pid = fork()) < 0) {     
			printf("ERROR: forking child process failed\n");
			exit(1);
		}
		else if (pid == 0) {
			if (execvp(*argv, argv) < 0) {
				printf("ERROR: exec failed\n");
				exit(1);
			}
		}
		else {                                  
			while (wait(&status) != pid)       
				;
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
    char  *argv[64];  
    //int	prompt;
    while (1) {
	//prompt = execl("/bin/pwd","pwd", (char *)0);
	prompt();
	//printf("Shell - Prompt: ");    
		gets(cmd);             
		printf("\n");
		parse(cmd, argv);       
		if (strcmp(argv[0], "exit") == 0)  
			exit(0);
		execute(argv);
    }
}