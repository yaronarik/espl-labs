section .data

        WRITE EQU 4
        STDOUT EQU 1
        OPEN EQU 5
        READ EQU 3
        CLOSE EQU 6

        eFlag: dd 0
        iFlag: dd 0
        oFlag: dd 0

        key dd 0
        len_key dd 0

        len_buf: dd 0
 
        newline: db 10
        space: db 32
        a: db 97

        ;inputfile_Path: db "inputFile.txt" ,0
        ;outputfile_Path: db "outputFile.txt" ,0

        fileOpenSuccses: db "file  opened sucssefully" ,0x0a,0
        fileOpenError: db "file didn't open, exit code with:  ",0x0a,0

        findIflag: db "find i flag " ,0x0a,0
        findOflag: db "find o flag " ,0x0a,0
        findEflag: db "find e flag " ,0x0a,0
        startEnc: db "startEncription " ,0x0a,0

        inputFileDescriptor  dd 0  ;STDIN
        outputFileDescriptor dd 1  ;STDOUT

section .bss
        address_of_e_flag resb 5
        buf resb 100

section .text
        global  main            ; let the linker know about main





main:
        push    ebp             ; prepare stack frame for main
        mov     ebp, esp
        sub     esp, 8

        mov     edi, dword[ebp+8]    ; get argc into edi
        mov     esi, dword[ebp+12]   ; get first argv string into esi  

;args loop

start_args_loop:
        mov ebx, [esi]

        cmp word [ebx], "-i"
        je update_iFlag

        cmp word [ebx], "-o"
        je update_oFlag

        cmp word [ebx], "+e"
        je update_eFlag

        mov ecx ,[esi]     ; must dereference esi; points to argv
        
after_update:
        add     esi, 4          ; advance to the next pointer in argv
        dec     edi             ; decrement edi from argc to 0
        cmp     edi, 0          ; when it hits 0, we're done
        jnz     start_args_loop      ; end with NULL pointer
        je      encription_part







update_iFlag:
        mov byte [iFlag] ,1
        mov edx ,[esi]
        add byte edx , 2        ; move esi e.i argv[i] by 2 bytes [delete(-i)]

        mov ecx ,edx    

        ; opening input file

        mov eax ,OPEN
        mov ebx , edx                   ;path
        mov ecx ,0                      ; set file access bits readOnly
        mov edx ,777                    ; permission
        int 0x80

        mov [inputFileDescriptor] ,eax
        mov ecx, [inputFileDescriptor]
        call fileDescriptorGood



        jmp after_update




update_oFlag:
        mov byte [oFlag] ,1
        mov edx ,[esi]
        add byte edx , 2        ; move esi e.i argv[i] by 2 bytes [delete(-i)]

        mov ecx ,edx          

        ; opening output file

        mov eax ,OPEN
        mov ebx , edx                   ;path
        mov ecx ,1                     ; set file access bits writeonly
        mov edx ,777                    ; permission
        int 0x80

        mov [outputFileDescriptor] ,eax
        mov ecx, outputFileDescriptor
        call fileDescriptorGood


        jmp after_update





update_eFlag:
        mov byte [eFlag] ,1
        mov edx ,[esi]
        add byte edx , 2        ; move esi e.i argv[i] by 2 bytes [delete(-i)]
        mov [key], edx

        mov ecx ,edx
        call my_strlen
        mov [len_key], eax      ; upfate len of key   

        jmp after_update










      
encription_part:

        ;open,read from input

        mov eax ,READ
        mov ebx ,[inputFileDescriptor]    ;file descriptor
        mov ecx ,buf                        ;
        mov edx ,100                      ;buffer size. max count of bytes to receive
        int 0x80

        cmp [eFlag], 0
        je echo

enc_loop:
        



 echo:
        ;writing to output file
        
        mov ecx ,buf                        
        pushad
        call my_strlen
        dec eax
        mov ecx, buf
        mov edx ,eax                      ;buffer size. max count of bytes to receive
        mov eax ,WRITE
        mov ebx ,[outputFileDescriptor]    ;file descriptor
        int 0x80
        popad 




end:
        mov     esp, ebp
        pop     ebp
        ret


close_out_file:
        mov eax ,CLOSE
        mov ebx ,[outputFileDescriptor]    ;file descriptor
        int 0x80
        
    mov eax ,CLOSE
        mov ebx ,[inputFileDescriptor]    ;file descriptor
        int 0x80









my_strlen:      mov eax,1
cont:           cmp byte [ecx], 0
                jz done
                inc ecx
                inc eax
                jmp cont
done:           ret



fileDescriptorGood:
                cmp ecx ,0
                jl bad

                pushad
                mov ecx ,fileOpenSuccses   ; succsesMsg (string)
                call my_strlen
                mov ebx ,STDOUT    ;file descriptor
                mov ecx ,fileOpenSuccses                    ;
                mov edx , eax                     ;buffer size. max count of bytes to receive
                mov eax ,WRITE
                int 0x80
                popad
                ret

        bad:    pushad
                mov ecx ,fileOpenError  ;errormsg (string)
                call my_strlen
                mov ebx ,STDOUT    ;file descriptor
                mov ecx ,fileOpenError      
                mov edx , eax                     ;buffer size. max count of bytes to receive
                mov eax ,WRITE
                int 0x80
                popad

                mov eax, 1
                int 0x80
