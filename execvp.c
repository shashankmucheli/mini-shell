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
	strcpy(temp, *(argv-1));
	if(i = 0){
		printf("temp value is: %s\n",*(argv-1));
		i=1;
	}
	  /*if(strcmp(temp,"dir") == 0){
		//*(argv-1) = "ls";
		strcpy(*(argv-1),"ls");
		printf("The command entered is %s\n",*(argv-1));
	  }
	  else if(strcmp(temp,"del") == 0){
		//(argv-1) = "rm";
		strcpy(*(argv-1),"rm");
		printf("The command entered is %s\n",*(argv-1));
	  }
	  else if(strcmp(temp,"copy") == 0){
		//*(argv-1) = "cp";
		strcpy(*(argv-1),"cp");
		printf("The command entered is %s\n",*(argv-1));
	  }
	  else if(strcmp(temp,"move") == 0){
		//*(argv-1) = "mv";
		strcpy(*(argv-1),"mv");
		printf("The command entered is %s\n",*(argv-1));
	  }
	  else if(strcmp(temp,"rename") == 0){
		//*(argv-1) = "mv";
		strcpy(*(argv-1),"mv");
		printf("The command entered is %s\n",*(argv-1));
	  }
	  else if(strcmp(temp,"type") == 0){
		//*(argv-1) = "cat";
		strcpy(*(argv-1),"cat");
		printf("The command entered is %s\n",*(argv-1));
	  }
	  else if(strcmp(temp,"more<file") == 0){
		//*(argv-1) = "more file";
		strcpy(*(argv-1),"more file");
		printf("The command entered is %s\n",*(argv-1));
	  }
	  else if(strcmp(temp,"md") == 0){
		//*(argv-1) = "mkdir";
		strcpy(*(argv-1),"mkdir");
	  }
	  else if(strcmp(temp,"rd") == 0){
		//*(argv-1) = "rmdir";
		strcpy(*(argv-1),"rmdir");
	  }
	  else if(strcmp(temp,"cls") == 0){
		//*(argv-1) = "clear";
		strcpy(*(argv-1),"clear");
	  }*/
	  //else {printf("Dude, I couldn't recognize it!\n");}
	  //printf("temp value is: %s\n",&temp);
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