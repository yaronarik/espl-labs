#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <elf.h>
#include <fcntl.h>

struct fun_desc{
	char *name;
	void(*fun)();
};

void* start;
int currentfd = -1;
off_t fileSize;
int debugFlag = 0;

void Examine_ELF_File()
{
    char fileName[1000];
    puts("please enter file name:");
    scanf("%s", fileName);

    if (currentfd > -1) // close any open file 
    {
        close(currentfd);
    }

    if(start)
    {
        munmap(start, fileSize);
    }

    currentfd = open(fileName, O_RDONLY);
    if (currentfd < 0) // file cannot be opened
    {
        printf("open failed");
        return;
    }

    fileSize = lseek(currentfd, 0, SEEK_END);
    lseek(currentfd, 0, SEEK_SET);
    start = mmap(0,fileSize, PROT_READ, MAP_SHARED, currentfd, 0);

    if (start == MAP_FAILED || start == -1)
    {
        printf("mmap failed");
        close(currentfd);
        currentfd = -1;
        return;
    }

    Elf32_Ehdr* header = (Elf32_Ehdr*) start;

    if (fileSize < 5 || !(header->e_ident[0] == 0x7f && header->e_ident[1] == 'E' && header->e_ident[2] == 'L' && header->e_ident[3] == 'F'))
    {
        printf("File is not in Elf format");
        close(currentfd);
        currentfd = -1;
        munmap(start, fileSize);
        start = NULL;
        return;
    }


    printf("1.Bytes 1,2,3 of the magic number (in ASCII): ");
    for (int i = 1; i < 4; i++)
        printf("%c ", header->e_ident[i]);
    printf("\n");
    // ----------------------------------------
    if(header->e_ident[EI_DATA] == 1)
        printf("2. Data encoded scheme of object file: 2's complement, little endian\n");
    else
        printf("2. Data encoded scheme of object file: 2's complement, big endian\n");

    //Elf32_Addr e_entry;     // Address to jump to in order to start program
    printf("3. Entry point (hexadecimal address): ");
    printf("%x\n", (header->e_entry));

    //Elf32_Off e_shoff;      // Section header table's file offset, in bytes
    //This member hilds the section hrader table's file offset in bytes. If the file has no section header table this member holds zero.
    printf("4. The file offset in which the section header table resides: ");
    printf("%d\n", (header->e_shoff));
    
    //Elf32_Half e_shnum;     // Number of entries in the section header table
    //This member holds the number of entries in the section header table.
    printf("5. The number of section header entries: ");
    printf("%d\n", (header->e_shnum));
    // ----------------------------------------
    int sectionsNum = header->e_shnum;
    Elf32_Shdr* section = (Elf32_Shdr*)(start + header->e_shoff);
    // Elf32_Shdr* sh_strtab = &section[map_start->e_shstrndx];
    // const char* const names = map_start+sh_strtab->sh_offset;
    printf("6. The size of each section header entry: \n");
    for (int i=0; i<sectionsNum; i++)
    {
        printf("Section Num: %d Size: %d\n" , i, section->sh_size);
    	section++;
    }
    //e_phnum This member holds the number of entries in the program header table.
    // Thus the product of e_phentsize and e_phnum gives the table’s size in bytes. If a file has no program header table, e_phnum holds the value zero.
    printf("7. The file offset in which the program header table resides: ");
    printf("%x\n", (header->e_phoff));
    // ----------------------------------------
    printf("8. The number of program header entries: ");
    printf("%d\n", (header->e_phnum));
    // ----------------------------------------
     int ProgramsNum = header->e_phnum;
    Elf32_Phdr* program = (Elf32_Phdr*)(start + header->e_phoff);
    printf("9. The size of each program header entry:\n");
     for (int i=0; i<ProgramsNum; i++)
    {
        printf("program number: %d Size: %d\n" , i, program->p_filesz);
    	program++;
    }
    // ----------------------------------------
}

void ToggleDebugMode()
{
    // printf("before D is :%d\n" , debugFlag);
    if(debugFlag){debugFlag=0;printf("Debug Flag is now OFF\n");}
    else{
        debugFlag =1 ;
        printf("Debug Flag is now ON\n");
    }
    // printf("after D is :%d\n" , debugFlag);
}

void printSectionName(){
    if (start == NULL)
    {
        printf("file is not defined"); 
        return;
    }

    Elf32_Ehdr *ehdr = (Elf32_Ehdr*) start;
    Elf32_Shdr *shdr = (Elf32_Shdr *)(start + ehdr->e_shoff);
    unsigned int sectionsNum = ehdr->e_shnum; /* Section header table entry count */
    Elf32_Shdr *sh_strtab = &shdr[ehdr->e_shstrndx]; /* Section header string table index */
    const char *const sh_strtab_p = start + sh_strtab->sh_offset;

    printf("Section Headers:\n");
    printf("[Nr] Name                 Type     Addr    Off   Size    ");

    if(debugFlag)
        printf("      Name offset");
    printf("\n");

    for(int i = 0 ; i < sectionsNum ; i++)
    {
        Elf32_Addr addr = shdr->sh_addr;
        Elf32_Off offset = shdr->sh_offset;
        Elf32_Word size = shdr->sh_size;
        Elf32_Half type = shdr->sh_type;
        printf("[%2d] %-20s %06x  %08x %06x  %06x",
        i, 
        sh_strtab_p + shdr->sh_name,
        type,
        addr,
        offset,
        size
        );
        if(debugFlag)
            printf("%d %06x", i, shdr->sh_name);
        printf("\n");
        shdr++;
    }
    printf("\n");

}

void printSymbol(){
    printf("not implemented yet"); 
}

void relocationTables(){
    printf("not implemented yet"); 
}

void Quit()
{
    printf("quitting\n");
    if (currentfd > -1)
        close(currentfd);
    if (start)
        munmap(start, fileSize);
    exit(0);
}


int main(int argc, char *argv[])
{
    struct fun_desc menu[] = { 
    {"Toggle Debug Mode",ToggleDebugMode}, 
    {"Examine ELF File",Examine_ELF_File},
    {"Print Section Names",printSectionName},  
    {"Print Symbols",printSymbol},
    {"Relocation Tables",relocationTables},
    {"Quit", Quit},
    {NULL,NULL}};
	
	while(1)
    {

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
        menu[input].fun();
        puts ("Done\n");
    }
	
    return 0; 
}
