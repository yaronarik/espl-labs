       #include <stdio.h>
       #include <stdlib.h>

 
int main(int argc, const char *argv[]) {
    int i;
    for (i = 1; i < argc; ++i) {
        FILE* fp = fopen(argv[i], "rb");
        if (fp) { /* ignore if failed to open */
            int c;
            while ((c = fgetc(fp)) != EOF) {
                printf(" %02x", c);
            }
            fclose(fp);

            printf("\n");
        }
    }
    return 0;
}