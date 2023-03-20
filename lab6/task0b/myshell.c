#include <stdbool.h>
#include <stdio.h>
 #include <limits.h>
#include <unistd.h>
#include "utils/LineParser.c"


void execute(cmdLine *pCmdLine){
    if(strcmp(pCmdLine-> arguments[0] , "quit") == 0){exit(0);}
    int if_returned_error = execvp(pCmdLine->arguments[0] ,pCmdLine->arguments);
    if(if_returned_error == -1 ){
        perror("execv error");
        exit(-1);
    }
}


int main(){
    FILE * in = stdin;

    char buffer_path_holder [PATH_MAX];

    char buffer_user_command_holder [PATH_MAX];
    int size_of_user_command = 2048;

    
    while (true){
    printf("%s\n" , getcwd(buffer_path_holder,PATH_MAX));
    fgets(buffer_user_command_holder, size_of_user_command, in);

    execute(parseCmdLines(buffer_user_command_holder));
        
    }

    return 0;
}