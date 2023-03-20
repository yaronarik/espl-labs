#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct fun_desc{
    char *name;
    char(*fun)(char);
};

char* map (char *array, int array_length, char (*f)(char)){
    char * mapped_array = (char*)(malloc(array_length*sizeof(char)));
    for(int i=0 ; i <array_length; ++i){
        mapped_array[i] = f(array[i]);
    }
    free(array);
    
    return mapped_array;
}

char censor(char c) {
  if(c == '!')
    return '.';
  else
    return c;
}

char my_get(char c){
/* Ignores c, reads and returns a character from stdin using fgetc. */
    
    return fgetc(stdin);
}
char cprt(char c){
/* If c is a number between 0x20 and 0x7E, cprt prints the character of ASCII value c followed by a new line.
 Otherwise, cprt prints the dot ('.') character. After printing, cprt returns the value of c unchanged. */
  if(0x20 <= c && c <= 0x7E){
    printf("%c\n",c); ///////////
    // printf("/");////////////
    }
  else{
    printf(".\n");
  }
  return c;
}

char encrypt(char c){
/* Gets a char c and returns its encrypted form by adding 3 to its value. If c is not between 0x20 and 0x7E it is returned unchanged */
  if(0x20 <= c && c <= 0x7E){
    // printf("%c\n holla",c+3); //////////////
    return c+3;
  }
  else
  {
    return c;  
  }
}

char decrypt(char c){
/* Gets a char c and returns its decrypted form by reducing 3 to its value. If c is not between 0x20 and 0x7E it is returned unchanged */
if(0x20 <= c && c <= 0x7E){
    // printf("%c\n holla",c-3); //////////////
    // printf("hi");/////////////////////
    return c-3;
  }
  else
  {
    return c;  
  }
}

char xprt(char c){
/* xprt prints the value of c in a hexadecimal representation followed by a new line, and returns c unchanged. */
if(0x20 <= c && c <= 0x7E){
  printf("%x\n", c);
  return c;
}
}
 

char quit(char c){
/* Gets a char c, and if the char is 'q' , ends the program with exit code 0. Otherwise returns c. */
  if(c =='q'){exit(0);}
}

int main (int argc ,char **argv){
    struct fun_desc menu[] = { {"Get string",my_get} , {"print string", cprt},{"Print hex", xprt} ,  {"Censor", censor} , {"Encrypt", encrypt} , {"Decrypt" , decrypt} , {"Quit" , quit} , {NULL,NULL}};
    char* carray = calloc(5 , sizeof(char));
    int i=(sizeof(menu)/sizeof(menu[0]))-1;///becuse of null.

    while(1){
        puts("Please choose a function: \n");
        int j =0;
        while(menu[j].name != NULL){
            fprintf(stdout, "%d) %s\n",j,menu[j].name);
            j++;
        }
        // i=(sizeof(menu)/sizeof(menu[0]))-1;///becuse of null.
        int input;
        scanf("%d",&input);
        //  printf("i is: %d\n",i);
        fgetc(stdin);
        // printf("input is %d\n",input);
        
        if(input >= 0 &&input<j ){
            puts("Within bounds\n");
            }
        else{puts("Not within bounds\n");exit(0);}

        printf("Option : %d\n" , input);
 
        carray = map(carray,5,menu[input].fun);
        puts ("Done\n");
    }
    free(carray);

    return 0;
}
