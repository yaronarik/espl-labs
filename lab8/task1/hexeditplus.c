
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
  bool display_mode;
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
void LoadIntoMemory (state *s){
    if(strcmp(s->file_name, "") == 0){
        printf("file name is empty\n"); 
        return;
    }
    int location; 
    int len;
    char buffer [128];

    printf("Please enter <location> <length>\n");
    fgets(buffer,128,stdin);

    if(s->display_mode){sscanf(buffer,"%x %x",&location,&len);}
    else{sscanf(buffer,"%x %d",&location,&len);}

    if(isDebug(s)){printf("file name : %s  you have entered : %x %d\n" , s->file_name, location, len);}

    FILE *fileInput;
    fileInput = fopen(s->file_name, "r");
    if(fileInput){
        fseek(fileInput,location,SEEK_SET);
        int element_read = fread(s->mem_buf,s->unit_size,len,fileInput);
        fseek(fileInput,0,SEEK_SET); //seek back to beginning of file
        if(element_read == 0){printf("error reading\n");}
        printf("Loaded %d units into memory\n" , element_read);
    }
    else{
        printf("error ocurredat openig file");
        return;
    }

    fclose(fileInput);
 }

void ToggleDisplayMode(state* s){ 
   if(s->display_mode){s->display_mode=false; printf("Display flag now off, decimal representation\n");}
   else{s->display_mode=true; printf("Display flag now on, hexadecimal representation\n");}}

void MemoryDisplay(state* s){
    int sourceAddress;
    int length;
    char buffer [128];
    printf("Enter address(hexa) and length(decimal)\n");

    fgets(buffer,128,stdin);
    sscanf(buffer, "%x %d" , &sourceAddress , &length);
    printf("you enterd : %x , %d\n",sourceAddress , length);
    int buffResult[s->unit_size * length];  
    if(s->display_mode){
        printf("Hexadecimal\n===========\n");
        for(int i=0 ; i<length ; i++){
            printf("%x\n", s->mem_buf[sourceAddress+i*s->unit_size]);
        }


    }
    else{
        printf("Decimal\n===========\n");
            for(int i=0 ; i<length ; i++){
            printf("%d\n", s->mem_buf[sourceAddress+i*s->unit_size]);
        }
    }
        
    
}

 

    // strcpy(s->file_name,str);
 

void SaveIntoFile(state* s){
    int sourceAddress;
    int location;
    int length;
    char buffer[100];
    printf("%s\n" ,s->file_name);
    FILE* myFile = fopen(s->file_name ,"r+");
    if(myFile == NULL){
        if(isDebug(s)){
            printf("cannot open file to write");
        }
        return;
    }
    printf("enter source-address(hexa), location(hexa), length(decimal.\n ");
    fgets(buffer,100,stdin);
    sscanf(buffer,"%x %x %d", &sourceAddress, &location, &length);
    fseek(myFile,0,SEEK_END);// seek to end of file
    int fileSize = ftell(myFile); //get current file pointer
    fseek(myFile,0,SEEK_SET); //seek back to beginning of file
    printf("File size: %d\n", fileSize);
    if(location> fileSize){
        printf("Error: target location is greater then size of file");
    }

fclose(myFile);
}
void MemoryModify(state* s){printf("Not implemented yet");}
void Quit(){printf("quitting\n"); exit(0);}

void Do(state* s,void (*f) ()){f(s);}

int main (int argc ,char **argv){
    struct fun_desc menu[] = { {"Toggle Debug Mode",ToggleDebugMode} , 
    {"Set File Name", Set_File_Name},
    {"Set Unit Size", SetUnitSize},  
    {"Load Into Memory", LoadIntoMemory} ,
    {"Toggle Display Mode", ToggleDisplayMode} ,
    {"Memory Display" , MemoryDisplay} ,
    {"Save Into File" , SaveIntoFile} ,
    {"Memory Modify" , MemoryModify} ,
    {"Quit", Quit}, 
    {NULL,NULL}};

    state *mystate = malloc(sizeof(state));
    mystate->debug_mode = 'f';
    mystate->display_mode = false; //off


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
        fgetc(stdin);
        
        if(input >= 0 &&input<i ){
            puts("Within bounds\n");
            }
        else{puts("Not within bounds\n");exit(0);}

        printf("Option : %d\n" , input);
        Do(mystate,menu[input].fun);
        puts ("Done\n");
    }
    free(mystate);

    return 0;
}