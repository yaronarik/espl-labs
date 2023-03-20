
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct fun_desc {
char *name;
void (*fun)(state);
};

typedef struct {
  char debug_mode;
  char file_name[128];
  int unit_size;
  unsigned char mem_buf[10000];
  size_t mem_count;
  /*
   .
   .
   Any additional fields you deem necessary
  */
} state;

// debug: f- false , t-true.
bool isDebug(state *s){
    if(s->debug_mode == 'f'){return false;}
    else{return true;}
}


void ToggleDebugMode(state* s)
// Getting the state as a pointer allows the functions to change it.
{   
   if(isDebug(s)){s->debug_mode='f'; printf("Debug flag now off\n");}
   else{s->debug_mode='t'; printf("Debug flag now on\n");}
}

void Set_File_Name(state* s){
 // Getting the state as a pointer allows the functions to change it.
    char str [128];
    printf("please enter new name\n");
        gets(str);
        // scanf(,s->file_name);
        strcpy(s->file_name,str);
        if(isDebug){
            // printf("you enterd : %s\n", str);
            printf("Debug: file name set to  %s\n", s->file_name);
        }
}

void SetUnitSize(state* s){
    char str [10];
    printf("please enter new [user for a number]\n");
    gets(str);
    int input = atoi(str);
    if(input ==1 || input ==2 || input ==4){
        s->unit_size = input;
        if(isDebug(s)){
             printf("Debug: set size to %d\n", input);
        }
    }
    else{
        if(isDebug(s)){
            printf("error unit should be 1 or 2 or 4 \n");
        }
    }
}
// void LoadIntoMemory(){printf("Not implemented yet");}
void ToggleDisplayMode(state* s){printf("Not implemented yet");}
void MemoryDisplay(state* s){printf("Not implemented yet");}
void SaveIntoFile(state* s){printf("Not implemented yet");}
void MemoryModify(state* s){printf("Not implemented yet");}
void Quit(){printf("quitting\n"); exit(0);}

void LoadIntoMemory (char *message){
    char * i;
    printf("Please enter <location> <length>");
    fgets(i,128,stdin);
    // sscanf();
    


    FILE *fileInput;
    fileInput = fopen(message, "r");
    if(fileInput){
        char trash[4];
        fread(trash, sizeof(char), 4, fileInput);
        while(!feof(fileInput)){
            // virus *nextVirus = readVirus(fileInput);
            // list_append(list, nextVirus);
        }
    }
    fclose(fileInput);
 }


void Do(state* s,void (*f) ()){f(s);}

int main (int argc ,char **argv){
    struct fun_desc menu[] = { {"Toggle Debug Mode",ToggleDebugMode} ,      {"Set File Name", Set_File_Name},
    {"Set Unit Size", SetUnitSize},  
    {"Load Into Memory", LoadIntoMemory} ,
    {"Toggle Display Mode", ToggleDisplayMode} ,
    {"Memory Display" , MemoryDisplay} ,
    {"Save Into File" , SaveIntoFile} ,
    {"Memory Modify" , MemoryModify} ,
    {"Quit", Quit}, 
    {NULL,NULL}};
    // char* carray = calloc( 5, sizeof(char));
    state *mystate = malloc(sizeof(state));
    mystate->debug_mode = 'f';
    // putc(mystate->debug_mode,stdout);

    while(1){
        if(isDebug(mystate)){
            printf("debug info:\n \tunit_size:%d \n\tfile_name: %s \n\tmem_count: %zu\n" ,
            mystate->unit_size, mystate->file_name, mystate->mem_count);
        }
        puts("Please choose a function: \n");
        int i =0;
        while(menu[i].name != NULL){
            fprintf(stdout, "%d) %s\n",i,menu[i].name);
            i++;
        }
        int input;
        scanf("%d",&input);
        // printf("i is: %d\n",i);
        fgetc(stdin);
        // printf("input is %d\n",input);
        
        if(input >= 0 &&input<i ){
            puts("Within bounds\n");
            }
        else{puts("Not within bounds\n");exit(0);}

        printf("Option : %d\n" , input);

        //  carray = map(carray,5,menu[input].fun);
        Do(mystate,menu[input].fun);
        puts ("Done\n");
    }
    free(mystate);

    return 0;
}