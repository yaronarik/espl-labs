#include <stdbool.h>
#include <stdio.h>
 #include <limits.h>
#include <unistd.h>
#include "utils/LineParser.c"
#include <sys/wait.h>



void cdHandler(cmdLine *pCmdLine){
    int res = -1;
    if (pCmdLine->argCount == 2) {
        res = chdir(pCmdLine->arguments[1]);} 

   
    if (res == -1) {perror("ERROR changing directory");}
}

void execute(cmdLine *pCmdLine){
    int if_returned_error;
    if(strcmp(pCmdLine-> arguments[0] , "quit") == 0) { exit(0); }

    int pid = fork();
    if(pid == 0){
        if_returned_error = execvp(pCmdLine->arguments[0] ,pCmdLine->arguments);
        if(if_returned_error == -1 ){
            perror("execv error");
            _exit(if_returned_error);
        }
    }
    else if (pid > 0){
        // printf("%d\n" ,pid);
        // printf("%d\n",pCmdLine->blocking);
        if (pCmdLine->blocking){
            waitpid(pid , &if_returned_error ,0);
        }
    }
}


void debug_printer( int pid, char* executing_command){
    fprintf(stderr,  "PID: %d\n" , pid);
    fprintf(stderr,  "Executing command: %s\n" , executing_command);
}





int main(int argc , char* argv[]){
    FILE * in = stdin;

    char buffer_path_holder [PATH_MAX];

    char buffer_user_command_holder [PATH_MAX];
    int size_of_user_command = 2048;


    bool debug_mode = false;
    for(int i = 0; i < argc; i++){
        if(strncmp(argv[i] ,"-d", 2) == 0) debug_mode = true; 
    }


    
    while (true){
    getcwd(buffer_path_holder,PATH_MAX);
    printf("%s$" , buffer_path_holder);
    // sleep(1);
    fgets(buffer_user_command_holder, size_of_user_command, in);
    // if (debug_mode){debug_printer(pid,buffer_user_command_holder);}
    cmdLine * line = parseCmdLines(buffer_user_command_holder);
    if (strcmp(line->arguments[0], "cd") == 0) { cdHandler(line); }
    else if(strcmp(line-> arguments[0] , "quit") == 0) { exit(0); }
    else{
    execute(line);
    }
    freeCmdLines(line);
        
    }

    return 0;
}


