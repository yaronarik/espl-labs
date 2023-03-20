
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct fun_desc {
char *name;
void (*fun)();
};

typedef struct virus {
unsigned short SigSize;
char virusName[16];
unsigned char* sig;
} virus;


typedef struct link {
struct link *nextVirus;
virus *vir;
}link;

int defBuffersize=10000;

void hexPrint (unsigned char buffer[], int length, FILE* output){
    unsigned char ch;
    int i = 0;
    while(i < length){
        ch = buffer[i];
        fprintf(output, "%02X ", ch);
        i++;
        if(i%20 == 0){printf("\n");}

    }
    fprintf(output,"%c",'\n');
}

 /*this function receives a file pointer and returns a virus* that represents the next virus in the file.
 To read from a file, use fread(). See man fread(3) for assistance.*/
virus* readVirus(FILE* fileInput){
    virus *nextVirus=(virus*)malloc(sizeof(struct virus));
//size
    unsigned short size;
    fread(&size,sizeof(unsigned short),1,fileInput);
    nextVirus->SigSize=size;
//name
    
    fread(nextVirus->virusName ,sizeof(char),16,fileInput);
    
//sig
    unsigned char *sig=(unsigned char*)malloc(sizeof(unsigned char)*size);
    fread(sig,sizeof(unsigned char),size,fileInput);
    nextVirus->sig = sig;
    return nextVirus;
}
  
 /* this function receives a virus and a pointer to an output file.
  The function prints the virus to the given output. It prints the virus name (in ASCII),
   the virus sig length (in decimal), and the virus sig (in hexadecimal representation).*/
 void printVirus(virus* virus, FILE* output){
    	fprintf(output,"%s %s\n", "Virus name:", virus->virusName);
	    fprintf(output,"%s %d\n", "Virus size:", virus->SigSize);
	    fprintf(output,"%s\n", "sig:");
	    hexPrint(virus->sig, virus->SigSize, output);
        fprintf(output,"%c", '\n');   
 }
 
 /*Open the sigs file, use readVirus in order to read the viruses one-by-one,
  and use printVirus in order to print the virus (to a file or to the standard output, 
  up to your choice).*/
 
//1.b

// /* Print the data of every link in list to the given stream. Each item followed by a newline character. */
void list_print(struct link *virusLink,FILE* output){
    while(virusLink != NULL){
        printVirus(virusLink->vir,output);
        virusLink = virusLink->nextVirus;
        }
}

// /* Add a new link with the given data to the list (either at the end or the beginning, depending on what your TA tells you), and return a pointer to the list 
// (i.e., the first link in the list). If the list is null - create a new entry and return a pointer to the entry. */
struct link* list_append(struct link* virusList, virus* data){
    if(virusList->vir == NULL){
        virusList->vir = data;
        virusList->nextVirus = NULL;
        return virusList;
    }
    struct link *temp=virusList;
    while(temp->nextVirus != NULL){
        temp=temp->nextVirus;
    }
    struct link *tempLink = malloc(sizeof (struct link));
    tempLink->vir = data;
    tempLink->nextVirus = NULL;
    temp->nextVirus = tempLink;

    return virusList;
}

// /* Free the memory allocated by the list. */
void list_free(struct link *virusList){
    while(virusList != NULL){
        struct link *nextLink = virusList->nextVirus;
        free(virusList->vir->sig);
        free(virusList->vir);
        free(virusList);
        virusList = nextLink;
    }

}

void load (char *message, struct link* list){
    FILE *fileInput;
    fileInput = fopen(message, "r");
    if(fileInput){
        char rm[4]; //told in group in whatsapp.
        fread(rm, sizeof(char), 4, fileInput);
        while(!feof(fileInput)){
            virus *nextVirus = readVirus(fileInput);
            list_append(list, nextVirus);
        }
    }
    fclose(fileInput);
 }

void quit(){exit(0);}

//1.c
void detectVirus(char *buffer, unsigned int size, link *virusList){

    if(virusList != NULL){
		int sigSize = virusList->vir->SigSize;
		unsigned char sig[sigSize];
		int i = 0; 
		while(i < size){
             // void *memcpy(void *dest, const void *src, size_t n) 
            //The memcpy() function copies n bytes from memory area src to memory area dest.
			int comper = memcmp(virusList->vir->sig,buffer+i, sigSize);
			if(comper == 0){
				fprintf(stdout, "%s\n", "Virus detected");
				fprintf(stdout, "%s %d\n" , "Location:", i);
				fprintf(stdout, "%s %s\n" , "Virus name:", virusList->vir->virusName);
				fprintf(stdout, "%s %d\n" , "Virus size:", sigSize);
			}
			i++;
		}
		detectVirus(buffer, size, virusList->nextVirus);
	}
}


int main(int argc, char **argv){
    struct link* list = malloc(sizeof (struct link));
    struct fun_desc menu[] = {  { "Load signatures ", load}, { "Print signatures ", list_print },
    { "Detect viruses",  detectVirus },{ "Fix file",  list_print },{ "Quit ", quit } , {NULL,NULL}};

      while(1){
        puts("Please choose a function: \n");
        int i = 1;
        while(menu[i-1].name != NULL){
            fprintf(stdout, "%d) %s\n",i,menu[i-1].name);
            i++;
        }
        int Input;
        scanf("%d",&Input);
        // printf("i is: %d\n",i);
        fgetc(stdin);
        // printf("Input is %d\n",Input);
        
        if(Input >= 0 && Input<i ){
            puts("Within bounds\n");
            }
        else{puts("Not within bounds\n"); exit(0);}

        printf("Option : %d\n" , Input);
    if(Input == 1){
        printf("Please enter file name: \n");
        char *message = (char*)malloc(sizeof(char)*100);
        fgets(message,100,stdin);
        printf("\n");
        message[strlen(message)-1] = '\0';
        load(message,list);
        printf("succesfull load\n");
        printf("\n");
        free(message);
    }
    if(Input == 2){
        list_print(list,stdout);
    }
    if(Input == 3){
        char buffer[defBuffersize];
        char *suspect = (char*)malloc(sizeof(char)*100);
        puts("Enter suspect file name: ");
        fgets(suspect,100,stdin);
        suspect[strlen(suspect)-1] = '\0';
        int j = 0;
        FILE *input;
        input = fopen(suspect,"r");
        while(fgetc(input) != EOF){
            j++;
        }
        fread(buffer, 1,j,input);
        fseek(input,0,SEEK_SET);
        printf("\n");
        
        detectVirus(buffer,j, list);
        fclose(input);
        // printf("not implemented yet");
    }
    if(Input == 4){
        printf("not implemented yet");
    }
    if(Input == 5){
        list_free(list);
        quit();
    }
        puts ("Done\n");
    }
    return 0;
}