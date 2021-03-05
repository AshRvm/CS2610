    ;Objectives:    To find and print the nth fibonacci number using an iterative function
    ;Inputs    :    The position of the number to be found in the fibonacci series
    ;Oututs    :    The nth fibonacci number                 

%include            "io.mac"
.DATA
    num             dd          0
    program_time    db          "Difference in lower 32 bits of MSR before and after execution :",0
.UDATA
    n               resd        1
    temp            resd        1
.CODE
    .STARTUP
    GetLInt         [n] 
    RDTSCP
    mov             [temp],EAX          ;stores the current lower 32 bits of MSR into temp
    mov             ECX,[n]             ;stores the the value of n as a counter     
    call            fibonacci   
    mov             [num],EAX           ;moves the result into [num]
    PutLInt         EAX
    nwln
    RDTSCP
    sub             EAX,[temp]          ;stores the difference of the lower 32 bits 
    PutStr          program_time
    PutLInt         EAX
    nwln
    .EXIT

%define             FIB_low     dword      [EBP-4]
%define             FIB_high    dword      [EBP-8]
fibonacci:
    enter           8,0
    push            EBX
    mov             FIB_low,1           ;starts with fibonacci(1)
    mov             FIB_high,1          ;starts with fibonacci(2)
fib_loop:
    cmp             ECX,0
    jle             end
    mov             EAX,FIB_low
    mov             EBX,FIB_high
    mov             FIB_low,EBX         ;fib_low for the next iteration will be fib_high of the current iteration
    add             EBX,EAX             ;fib_high for the next iteration will be the sum of current fib_low and fib_high
    mov             FIB_high,EBX
    dec             ECX
    jmp             fib_loop
end:
    pop             EBX
    leave
    ret

