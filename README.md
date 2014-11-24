mini-shell

A C based command interpreter to get familiar with UNIX system calls.

The purpose of this project is to perform the functions specified below:

1.    Like the dos environment the prompt shows the pathe to the current directory. (Unix system call - getcwd - find the name of the current working directory).
2.    Program supports the following DOS commands. With Switches Dos Unix dir ls del rm copy cp move/rename mv type cat cd cd more < file more file md mkdir rd rmdir cls clear
3.    When executing these commands the program forks a process and uses execvp to execute the commands.
4.    Redirection (< and >) works as intended on the Unix system.
5.    Unlike dos, the program supports running the process in the background using the ampersand sign (&). This means that the prompt will printed and the command-interpreter will wait for the next command before the previous commands is completely executed when & is used following the command.
6.    implemented pipes in the program. A pipe (|) enables you to pass the output of the command on the left of the pipe as the input to the command on the right of the pipe. For example: $ ls -l | wc will produce a number that will tell you the number of files (including directories, links etc) in your current directory.
