#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <wait.h>

    int main(int argc, char const *argv[]){

    bool debug_mode = false;
    for(int i = 0; i < argc; i++){
        if(strncmp(argv[i] ,"-d", 2) == 0) debug_mode = true; 
    }

    //creat a pipe
    int fd[2];
    if (pipe(fd) == -1){
        printf ("an error ocurred with opening the pipe!\n");
        return 1;
    }

    //Fork a first child process (child1)
    if(debug_mode){fputs("(parent_process>forking…)\n", stderr);}
    int childId1 = fork();
    
    if(debug_mode){fputs("(parent_process>created process with id: )\n", stderr);}
    if (childId1 == -1 ){
        printf ("an error ocurred with fork\n");
        return 1;
        }

    if(childId1 == 0){
         // this is a code for a child1 process.

        //Close the standard output.
        if(debug_mode){fputs("(child1>redirecting stdout to the write end of the pipe…)\n", stderr);}
        if(fclose(stdout) == -1){
            printf("an error occured closing stdout [io redirection\n");
        }

        //Duplicate the write-end of the pipe using dup (see man
        int dup_fileD = dup(fd[1]);

        //Close the file descriptor that was duplicated.
        if(close(fd[1]) == -1){
            printf("an error occured in closing fd[1]\n");
        
        }
        if(debug_mode){fputs("(child1>going to execute cmd: …)\n", stderr);}
        char * line[] = {"ls", "-l", NULL};
        int result_execvp = execvp(line[0], line);
        if(result_execvp < 0 ){printf("error in execvp");}
    } 
    else{
        // //this is a code for a parent. 
        int if_returned_error;
        if(debug_mode){fputs("(parent_process>closing the write end of the pipe…)\n", stderr);}
        
        //In the parent process: Close the write end of the pipe.
        if(close (fd[1]) == -1){
            printf("An error ocurred in closing parent \n");
        }

        //Fork a second child process (child2).
        int childId2 = fork();

        if(childId2 == 0){
            // this is a code for a child2 process.
            //Close the standard input.
            if(fclose(stdin) == -1){
            printf("an error occured closing stdin in child2\n");
            }

            //Duplicate the read-end of the pipe using dup.
            int dup_read = dup(fd[0]);
            //Close the file descriptor that was duplicated.
            if(close(fd[0]) == -1){
            printf("an error occured in closing fd[0] \n");
            }
            char *line2 [] ={"tail","-n" ,"2", NULL};
            if_returned_error =  execvp(line2[0],line2);
            if (if_returned_error == -1)
            {
                perror("execv error");
                _exit(if_returned_error);
            }

        }
        else{
             if(debug_mode){fputs("(parent_process>closing the read end of the pipe…)\n", stderr);}
            //In the parent process: Close the read end of the pipe.
            if(close(fd[0]) == -1){
                printf("an error occured in closing read end of pipe in parent \n");
            }
        //Now wait for the child processes to terminate, in the same order of their execution.
        if(debug_mode){fputs("(parent_process>waiting for child processes to terminate…)\n", stderr);}

        
        if(debug_mode){
            fputs("(parent_process>waiting for child processes to terminate…)\n", stderr);
        }
        waitpid(childId1, &if_returned_error, 0);     
        waitpid(childId2, &if_returned_error, 0);
            

    }
    }
    return 0;
    }
    


