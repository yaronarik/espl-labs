#include <stdbool.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include "LineParser.c"
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#define TERMINATED -1
#define RUNNING 1
#define SUSPENDED 0
// #define STDIN 0
// #define STDOUT 1

typedef struct process{
    cmdLine* cmd;                     /* the parsed command line*/
    pid_t pid; 		                  /* the process id that is running the command*/
    int status;                       /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;             /* next process in chain */
} process;

process *process_list = NULL;
FILE* input_file_descriptor;
FILE* output_file_descriptor;

void addProcess(process** process_list, cmdLine* cmd, pid_t pid){
    process *new_process =  malloc (sizeof(struct process));
    new_process ->cmd = cmd;
    new_process->pid = pid;
    new_process->status = RUNNING;
    new_process->next = *process_list;
    *process_list = new_process;
     //point to new head that is the new process.
}

static char *printCommand(cmdLine *cmdLine) {
    char *command;
    command = cmdLine->arguments[0];
    for (int i = 1; i < cmdLine->argCount; i++) {
        strcat(command, " ");
        strcat(command, cmdLine->arguments[i]);
    }
    return command;
}

void freeProcessList(process *process_list) {
    process *current_process;
    while (process_list != NULL) {
        current_process = process_list;
        process_list = process_list->next;
        freeCmdLines(current_process->cmd);
        free(current_process);
    }
}


void updateProcessStatus(process *process_list, int pid, int status) {
    process *temp = process_list;
    while (temp != NULL) {
        if (temp->pid == pid) {
            temp->status = status;
            break;
        }
        temp = temp->next;
    }
}

void updateProcessList (process *proccss_list){
    process *temp = process_list;
    int status;
    int result_waitpid;
    while (temp != NULL) {
        result_waitpid = waitpid(temp->pid, &status, WNOHANG );
        if (result_waitpid > 0) {
            if (WIFEXITED(status)) {
                updateProcessStatus(temp, temp->pid, TERMINATED);
            }else if (WIFCONTINUED(status)) {
                updateProcessStatus(temp, temp->pid, RUNNING);
            

            } else if (WIFSTOPPED(status)) {
                updateProcessStatus(temp, temp->pid, SUSPENDED);
            }
        } else if (result_waitpid == -1) {
            updateProcessStatus(temp, temp->pid, TERMINATED);
        }
        temp = temp->next;
    }

    temp = NULL; 
    process *current_process = process_list;
    process *prev_process = NULL;
    while (current_process != NULL){
         
        if(current_process->status == -1){
            temp = current_process->next;

            if(prev_process != NULL){
                    prev_process->next = temp;
            }
            else{
                process_list = temp;
            }
            free(current_process);
            current_process = temp;
            
        }
        
        else{
            prev_process = current_process; 
            current_process = current_process->next; 
        } 
    }
}
char * statDef(int i){
    if(i == -1)return "TERMINATED";
    else if(i == 1) return "RUNNING";
    else{return "SUSPENDED";}
}

void printProcessList(process** process_list){
    char*command;
    int list_index = 0;
    process *pointer = *process_list;
    printf("list_index\tPID\tCommand\tSTATUS\n");

    while (pointer != NULL) {
        command = printCommand(pointer->cmd);
        printf("%d\t\t%d\t%s\t%s\n", list_index ,pointer->pid, command, statDef(pointer->status));
        list_index++;
        pointer = pointer->next;
        }
}

void deleteTerminatedProcess(process *proccss_list){
    process *temp = NULL; 
    process *current_process = process_list;
    process *prev_process = NULL;
    while (current_process != NULL){
         
        if(current_process->status == -1){
            temp = current_process->next;

            if(prev_process != NULL){
                    prev_process ->next = temp;
            }
            // free(current_process);
            current_process = temp;
            
        }else{
            prev_process = current_process; 
            current_process = current_process->next; 
        }
    }   
 }

// 2c
void suspend (int pid){

        int kil = kill(pid, SIGTSTP);
        if (kil == -1) {
            perror("suspemd failed\n");
        } else {
            printf("suspend succeeded\n");
            updateProcessStatus(process_list,pid, 0);
    }

}
void kill_process(int pid){

        int kil = kill(pid, SIGINT );
        if (kil == -1) {
            perror("kill_process Failed\n");
        } else {
            printf("kill_process Succeeded\n");
            updateProcessStatus(process_list,pid, -1);
            }
}

void wake (int pid){

        int kil = kill(pid, SIGCONT);
        if (kil == -1) {
            perror("wake Failed\n");
        } else {
            printf("wake Succeeded\n");
            updateProcessStatus(process_list,pid, 1);
            }
}

void cdHandler(cmdLine *pCmdLine){
    int res = -1;
    if (pCmdLine->argCount == 2) {
        res = chdir(pCmdLine->arguments[1]);} 
    if (res == -1) {perror("ERROR changing directory");}
}

void IO_redirection(cmdLine *pCmdLine){
    if(pCmdLine->inputRedirect != NULL){
        if(fclose (stdin) == -1){
            printf("an error ocurred with closing stdin file[io redirection]\n");
        }
        input_file_descriptor = fopen(pCmdLine->inputRedirect,"r");
        if(input_file_descriptor == NULL){
            printf("an error ocurred with opening  input file[io redirection]\n");
        }
       
    }
    if(pCmdLine->outputRedirect !=NULL){
        if(fclose(stdout) == -1){
            printf("an error occured closing stdout [io redirection\n");
        }
        output_file_descriptor = fopen(pCmdLine->outputRedirect, "w");
        if(output_file_descriptor == NULL){
             printf("an error ocurred with opening  output file [io redirection]\n");
        }
        
    }
}

void execute(cmdLine *pCmdLine){
    int if_returned_error;
    int pid = fork();
    if(pid == 0){
        //child process.
        IO_redirection(pCmdLine);
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
    addProcess(&process_list,pCmdLine,pid);
}


void debug_printer( int pid, char* executing_command){
    fprintf(stderr,  "PID: %d\n" , pid);
    fprintf(stderr,  "Executing command: %s\n" , executing_command);
}

/////////////////////////////////////////////////////////////////////////////////////////
void creatPipeline(cmdLine *pCmdLine){
//creat a pipe
    int fd[2];
    if (pipe(fd) == -1){
        printf ("an error ocurred with opening the pipe!\n");
    }

    int childId1 = fork();
    
    if (childId1 == -1 ){
        printf ("an error ocurred with fork\n");
        }

    if(childId1 == 0){
         // this is a code for a child1 process.

        //Close the standard output.
        if(fclose(stdout) == -1){
            printf("an error occured closing stdout [io redirection\n");
        }

        //Duplicate the write-end of the pipe using dup (see man
        int dup_fileD = dup(fd[1]);

        //Close the file descriptor that was duplicated.
        if(close(fd[1]) == -1){
            printf("an error occured in closing fd[1]\n");
        
        }
        // char * line[] = {"ls", "-l", NULL};
        int result_execvp = execvp(pCmdLine->arguments[0], pCmdLine->arguments);
        if(result_execvp < 0 ){printf("error in execvp");}
    } 
    else{
        // //this is a code for a parent. 
        int if_returned_error;

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
            // char *line2 [] ={"tail","-n" ,"2", NULL};
            if_returned_error =  execvp(pCmdLine->next->arguments[0], pCmdLine->next->arguments);
            if (if_returned_error == -1)
            {
                perror("execv error");
                _exit(if_returned_error);
            }

        }
        else{
            //In the parent process: Close the read end of the pipe.
            if(close(fd[0]) == -1){
                printf("an error occured in closing read end of pipe in parent \n");
            }
        //Now wait for the child processes to terminate, in the same order of their execution.

        }
        waitpid(childId1, &if_returned_error, 0);     
        waitpid(childId2, &if_returned_error, 0);
            

    }
}


//////////////////////////////////////////////////////////////////////////////////////////

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
    fgets(buffer_user_command_holder, size_of_user_command, in);
    cmdLine * line = parseCmdLines(buffer_user_command_holder);
    if (strcmp(line->arguments[0], "cd") == 0) { cdHandler(line); }
    else if(strcmp(line-> arguments[0] , "quit") == 0) { freeProcessList(process_list); exit(0); }
    else if(strcmp(line-> arguments[0] , "procs") == 0) { 
 
        puts("\n Now print after update\n");
        updateProcessList(process_list);
        // deleteTerminatedProcess(process_list);
        printProcessList(&process_list);
        }
    else if(strcmp(line-> arguments[0] , "suspend") == 0) { 
        suspend(atoi(line->arguments[1]));
        }   
    else if(strcmp(line-> arguments[0] , "kill") == 0) { 
        kill_process (atoi(line->arguments[1])); 
        } 
    else if(strcmp(line-> arguments[0] , "wake") == 0) { 
        wake(atoi(line->arguments[1])); }  
        
    else if(line->next != NULL){
        creatPipeline(line);
        //execute(line);
    }
    else{
        execute(line);
    }
    // freeCmdLines(line);
        
    }
    return 0;
}


