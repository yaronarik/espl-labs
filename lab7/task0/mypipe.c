#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(){
    int fd[2];
    if (pipe(fd) == -1){
        printf ("an error ocurred with opening the pipe!\n");
        return 1;
    }
    int id = fork();
    if (id == -1 ){printf ("an error ocurred with fork\n"); return 6;}
    if(id == 0){
        // this is a code for a child process.
        if (close (fd[0]) ==  -1){
            printf("An error ocurred in closing fd[0]\n");
            return 2;
        }
        char* x="hello father!!\n" ;

        if (write (fd[1], x ,strlen(x)) ==  -1){
            printf("An error ocurred in writing\n");
            return 3;
        }
        if(close(fd[1]) == -1){
            printf( "an error ocurred in closing chilef fd[1]\n");
            return 4;
        }
    }
    else{
        //this is a code for a parent. 
        if(close (fd[1]) == -1){
            printf("An error ocurred in closing parent \n");
            return 5;
        }
        char y [20] ;
        if(read(fd[0], y, 20) == -1){
            printf(" An error ocurred in reading fd[0]\n ");
            return 6;
        }
        if(close(fd[0]) == -1){
            printf("An error ocurred in closing fd[0]\n");
            return 7;
        }
        printf("%s" , y);
    }
    return 0;
}