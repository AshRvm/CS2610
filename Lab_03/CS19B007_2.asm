    ;Objectives:    To find and print the nth fibonacci number using a recursive function
    ;Inputs    :    The position of the number to be found in the fibonacci series
    ;Oututs    :    The nth fibonacci number

%include            "io.mac"
.DATA
    num             dd                  0
    program_time    db                  "Difference in lower 32 bits of MSR before and after execution :",0          
.UDATA
    n               resd                1
    temp            resd                1
.CODE
    .STARTUP
    GetLInt         [n]                 
    mov             EBX,[n]             ;stores the index of the fibonacci number to be found in EBX
    RDTSCP
    mov             [temp],EAX
    call            fibonacci
    mov             [num],EAX           ;stores the nth fibonacci number into [num]
    PutLInt         EAX
    nwln
    RDTSCP
    sub             EAX,[temp]
    PutStr          program_time
    PutLInt         EAX
    nwln
    .EXIT

fibonacci:
    cmp             EBX,2               ;moves to base case if n<3
    jle             base
    push            EDX                 ;used to store fibonacci(n-1)
    dec             EBX                 ;used to compute fibonacci(n-1) in the next call
    call            fibonacci
    mov             EDX,EAX             ;stores fibonacci(n-1) into EDX
    dec             EBX                 ;used to compute fibonacci(n-2) in the next call
    call            fibonacci
    add             EAX,EDX             ;add fibonacci(n-2) and fibonacci(n-1) and stores it into EAX
    add             EBX,2               ;stores original value of EBX into EBX
    pop             EDX                 ;stores original value of EDX into EDX
    jmp             terminate
base:
    mov             EAX,1               ;fibonacci(1) = fibonacci(2) = 1
terminate:
    ret

