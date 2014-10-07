#include  <stdio.h>
#include  <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
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
	printf ("\n");
}

void  execute(char **argv)
{
    pid_t pid;
    int status;
	int ampersand_found = 0;
	int pipe_found = 0;
	
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
	/* else { */
		int i = 0;
		for (i = 0; argv[i]; i++){
			if (strcmp (argv[i], "&") == 0) {
				fprintf (stderr, "Ampersand Found!!\n");
				ampersand_found = 1;
				argv[i] = (char *)NULL;
			}
			
			if (strcmp (argv[i], "|") == 0) {
				fprintf (stderr, "Pipe Found!!\n");
				pipe_found = 1;
				if(pipe_found == 1){
					char **p_ptr;
					*p_ptr = argv[1+1];
					printf("%s", &p_ptr);
					/*while(argv[i] != "\0"){
						printf("%c", &argv[i]);
						i++;
					}*/
					exit(0);
				}
				/*argv[i] = (char *)NULL;
				char* p_ptr = argv[1+i];
				printf("%c", *p_ptr);*/
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
				/* wait (NULL); */
				while (wait(&status) != pid);
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