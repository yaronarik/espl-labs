#include <stdio.h>
#include <stdbool.h>
#include <string.h>


int main (int argc, char* argv[])
{
    bool debug = false;
    bool encoderKey = false;
    bool input = false;
    bool output = false;
    int argvIndex;
    FILE *fpIn =stdin;
    FILE *fpOut = stdout;
    
    for (int i =1; i<argc ; ++i)
    {
        if (strcmp(argv[i], "-D")==0)
        {
            debug =true;
            fprintf(stderr,"-D\n");
        }
        if (strncmp(argv[i] ,"+e",2)==0 || strncmp(argv[i],"-e",2)==0){
            encoderKey=true;
            argvIndex=i;  
        }

        if (strncmp(argv[i], "-i", 2)==0)
        {
            fpIn = fopen(argv[i]+2 , "r");
            input =true;
        }

        if (strncmp(argv[i], "-o", 2)==0)
        {
            fpOut = fopen(argv[i]+2 , "w");
            output =true;
        }
            
    }

    int c = getc(fpIn);
    if(encoderKey==true){
        bool signPlus=false;
        if(strncmp(argv[argvIndex],"+",1)==0){signPlus=true;}
        int keylen=strlen(argv[argvIndex]);
        int counter=2;
        
        while (c!= EOF)
        {

            if(signPlus){
                putc(c+(argv[argvIndex][counter]-'0') ,fpOut);
                if(debug){
                    fprintf(stderr, "%x %x\n" ,c , c+(argv[argvIndex][counter])-'0');
                }
            }
            else{
                putc(c-(argv[argvIndex][counter]-'0') ,fpOut);
                if(debug){
                    fprintf(stderr, "%x %x\n" ,c , c-(argv[argvIndex][counter]-'0'));
                }
            }
            ++counter;
            if(counter == keylen || c=='\n'){
                counter=2;
            }
            
            c= getc(fpIn);        
        }

    }
    
    else{
        while (c!= EOF)
        {

            if ('a' <=c && c<= 'z'){
                putc(c-('a'-'A') ,fpOut);
                if(debug){
                    fprintf(stderr, "%x %x\n" ,c , c-('a'-'A') );
            }
        }
            else{
                putc(c,fpOut);
                if(debug){
                    fprintf(stderr, "%x %x\n" ,c , c );
            }
        }
            c = getc(fpIn);
        }
    }
    if(input == true) fclose(fpIn);
    if(output == true) fclose(fpOut);

    return 0;
}
