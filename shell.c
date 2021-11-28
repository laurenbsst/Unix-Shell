/*
-------------------------------------------------------------------
 Program name: shell.c
 Author: Lauren Bassett
 Created: 03/11/2021
 Last modified: 28/11/2021
 Description:
 Takes in input from the user, parses the user input
 into tokens using strtok_r, and uses fork() and execvp() to execute the
 command.
--------------------------------------------------------------------
*/
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

void execute(char str[]){
    char *rest = NULL;
    char *args[64];
    char *token;
    char cwd[256];
    int i = 0;

    // &rest maintains context between successive calls that parse the same string
    // New line added so fgets() does not add new line character to args[]
    token = strtok_r(str, " \n", &rest);

    while(token!= NULL){
        // Store next token at next array position
        args[i++] = token;
        // Move onto the next token in the same string
        token = strtok_r(NULL, " \n", &rest);
    }
    // Array must be null-terminated
    args[i] = NULL;

    // If user types 'exit', exit shell
    if(strcmp(args[0], "exit") == 0){
        exit(0);
    }
    // If user types 'cd', change directory (NOT YET WORKING)
    else if (strcmp(args[0], "cd") == 0){
        // Outputs current working directory
        getcwd(cwd, sizeof(cwd));
        chdir(args[1]);
        getcwd(cwd, sizeof(cwd));
        printf("directory changed\n");
        printf("%s\n", cwd);
    }

    // Process ID
    pid_t pid;
    int status;

    // Forking has failed if child process ID is less than 0
    if ((pid = fork()) < 0){
        perror("ERROR: forking child failed\n"); // If negative value returned, execution failed
        exit(1);
    }
    else if (pid == 0){ // For the child process:
        if(execvp(args[0], args) < 0) { // Execute the command
            perror("ERROR: execvp failed\n"); // If negative value returned, execution failed
            exit(1);
        }
    }
    else {
        // Wait for the child process to finish executing
        while (wait(&status) != pid);
    }

}

int main() {
    // Array of chars to store user input
    char str[64];
    // Keep asking user for an input each time a command is executed (until exit)
    while(1){
        printf("Command->");
        // Get input from the user
        fgets(str, 64, stdin);
        execute(str);
    }
}