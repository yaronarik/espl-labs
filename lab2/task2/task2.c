
 #include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
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
  if(32 <= c && c <= 126){
    printf("%c\n",c);
    }
  else{
    printf(".\n");
  }
  return c;
}


char encrypt(char c){
/* Gets a char c and returns its encrypted form by adding 3 to its value. If c is not between 0x20 and 0x7E it is returned unchanged */
  if(32 <= c && (char)c <= 126){
    // printf("%c\n",c+3);
    return c+3;
  }
  else
  {
    return c;  
  }
}

char decrypt(char c){
/* Gets a char c and returns its decrypted form by reducing 3 to its value. If c is not between 0x20 and 0x7E it is returned unchanged */
if(32 <= c && c <= 126){
    // printf("%c\n holla",c-3); //////////////
    // printf("hihih");///////
    return c-3;
  }
  else
  {
    return c;  
  }
}

char xprt(char c){
/* xprt prints the value of c in a hexadecimal representation followed by a new line, and returns c unchanged. */
  printf("%x\n", c);
  return c;
}
 
 /*
Implement the map function that receives a pointer to a char (a pointer to a char array), 
an integer, and a pointer to a function. Map returns a new array (after allocating space for it),
 such that each value in the new array is the result of applying the function f on the corresponding character in the input array.
 */
char* map(char *array, int array_length, char (*f) (char)){
  char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
  for (int i=0; i<array_length; ++i)
  {
    mapped_array[i]= f(array[i]);
  }
  return mapped_array;
}

char quit(char c){
/* Gets a char c, and if the char is 'q' , ends the program with exit code 0. Otherwise returns c. */
  if(c =='q'){exit(0);}
}
 
int main(int argc, char **argv){
    char arr_1[] = {'H','E','Y','!'};
    char* arr_2 = map(arr_1, 4, censor);
    //  printf("%s\n", arr_2);

    int base_len = 5;
    char arr1[base_len];

    char* arr2 = map(arr1, base_len, my_get);
    // printf("%s\n", arr2);

    char* arr3 = map(arr2, base_len, cprt);
    // printf("%s\n", arr3);
     
    char* arr4 = map(arr3, base_len, xprt);
    // printf("%s\n", arr4);

    char* arr5 = map(arr4, base_len, encrypt);
    // printf("%s\n", arr5);

    char* arr6 = map(arr5, base_len, decrypt);
    // printf("%s\n", arr6);

    free(arr2);
    free(arr3);
    free(arr4);
    free(arr5);
    free(arr6);
}


