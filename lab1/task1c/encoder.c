#include <stdio.h>
#include <stdbool.h>
#include <string.h>


int main (int argc, char* argv[])
{
    bool debug = false;
    bool encoderKey = false;
    int argvIndex;
    FILE *fp =stdin;
    
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
            
    }

    int c = getc(fp);
    if(encoderKey==true){
        bool signPlus=false;
        if(strncmp(argv[argvIndex],"+",1)==0){signPlus=true;}
        int keylen=strlen(argv[argvIndex]);
        int counter=2;
        
        while (c!= EOF)
        {

            if(signPlus){
                putc(c+(argv[argvIndex][counter]-'0') ,stdout);
                if(debug){
                    fprintf(stderr, "%x %x\n" ,c , c+(argv[argvIndex][counter])-'0');
                }
            }
            else{
                putc(c-(argv[argvIndex][counter]-'0') ,stdout);
                if(debug){
                    fprintf(stderr, "%x %x\n" ,c , c-(argv[argvIndex][counter]-'0'));
                }
            }
            ++counter;
            if(counter == keylen || c=='\n'){
                counter=2;
            }
            
            c= getc(fp);        
        }

    }
    
    else{
        while (c!= EOF)
        {

            if ('a' <=c && c<= 'z'){
                putc(c-('a'-'A') ,stdout);
                if(debug){
                    fprintf(stderr, "%x %x\n" ,c , c-('a'-'A') );
            }
        }
            else{
                putc(c,stdout);
                if(debug){
                    fprintf(stderr, "%x %x\n" ,c , c );
            }
        }
        
            
            c = getc(fp);
        }
    }
    return 0;
}
