    ;Objectives:    To store the given number into the file "input.txt"
    ;               To find out whether the number is prime or not (child)
    ;               To print the number in reversed order (parent)
    ;Inputs    :    32-bit integer
    ;Outputs   ;    0 or 1 stating whether the number is prime or not (child)
    ;               The number in reversed order (parent)

%include            "io.mac"
.DATA
    out_file_name    db          "input.txt",0   ;name of file to write number into
.UDATA
    num             resb        11              ;string storing the number
    num_int         resd        1               ;integer form of the number
    fd_out          resd        1               ;file descriptor
    num_size        resd        1               ;number of digits in the number 
.CODE
    .STARTUP
    GetStr          num
    xor             ECX,ECX
    mov             EBX,num
find_length_loop:
    mov             AL,[EBX + ECX]              ;checks each byte for a digit
    cmp             AL,0
    je              create_file                 ;reached end of the string
    inc             ECX
    jmp             find_length_loop
create_file:
    mov             [num_size],ECX
    mov             EAX,8                       ;syscall for create and open file
    mov             EBX,out_file_name
    mov             ECX,0777
    int             0x80
    mov             [fd_out],EAX                ;EAX stores descriptor after interupt
    cmp             EAX,0
    jge             write_to_file
    jmp             done                        ;error in creating and opening file 
write_to_file:
    mov             EAX,4                       ;syscall for write into file
    mov             EBX,[fd_out]
    mov             ECX,num
    mov             EDX,[num_size]
    int             0x80
write_done:
    mov             EAX,6                       ;syscall for close file 
    mov             EBX,[fd_out]
mmap2:
    mov             EAX,192                     ;syscall for mmap2
    mov             EBX,0
    mov             ECX,4096
    mov             EDX,0x7
    mov             ESI,0x01
    mov             EDi,[fd_out]
    mov             EBP,0
    int             0x80
fork:
    mov             EAX,2                       ;syscall for fork
    int             0x80
    cmp             EAX,0
    jz              child

;---------------------------------Parent Process-----------------------
parent:
    mov             EBX,num
    xor             ECX,ECX
    mov             EDX,[num_size]
stack_push:
    mov             EAX,[EBX + ECX]
    and             EAX,0h0f                    ;taking first digit of EAX
    push            EAX
    inc             ECX
    cmp             ECX,EDX
    jl              stack_push
    mov             ECX,0
    mov             EAX,0
stack_pop:
    mov             EBX,10
    mul             EBX                         ;EAX = EAX*10 + popped digit
    pop             EBX
    add             EAX,EBX
    inc             ECX
    cmp             ECX,[num_size]
    jl              stack_pop
    PutLInt         EAX
    nwln
    jmp             done

;---------------------------------Child Process------------------------
child:
    mov             EBX,num
    xor             EAX,EAX
    xor             ECX,ECX
string_to_int:
    mov             EDX,10
    mul             EDX
    mov             EDX,[EBX + ECX]
    and             EDX,0h0f                    ;first digit of EDX
    add             EAX,EDX                     ;EAX = EAX*10 + EDX
    inc             ECX
    cmp             ECX,[num_size]
    jl              string_to_int
    mov             [num_int],EAX
prime_check:
    cmp             EAX,1                       ;1 is not prime
    je              not_prime
    cmp             EAX,2                       ;2 is prime
    je              is_prime
    cmp             EAX,3                       ;3 is prime
    je              is_prime
    mov             EDX,0
    mov             ECX,2
    div             ECX                         ;checks for multiples of 2
    cmp             EDX,0                       ;if remainder is 0, then number is not prime
    je              not_prime
    inc             ECX
prime_loop:
    mov             EDX,0
    mov             EAX,[num_int]
    div             ECX
    cmp             EDX,0                       ;checks for multiples of odd numbers
    je              not_prime
    add             ECX,2                       
    cmp             ECX,[num_int]
    jge             is_prime
    jmp             prime_loop
is_prime:
    mov             EAX,0
    PutLInt         EAX
    nwln
   jmp             done
not_prime:
    mov             EAX,1
    PutLInt         EAX
    nwln
done:
    .EXIT
