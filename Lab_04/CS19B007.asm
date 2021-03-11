    ;Objectives:        To search for an element in a given array, using binary search
    ;Inputs    :        The size of the array(size), the number to be searched for(num), and the response for termination(query)
    ;Outputs   :        1 if the element is present and 0 if it is not

%include                "io.mac"
.DATA
    terminate_query     db          "Do you wish to terminate? ",0
    next_query          db          "Please enter the next number ",0
.UDATA
    array               resd        100                     ;Setting the maximum size of the array as 100
    size                resd        1                       ;Size of the array
    num                 resd        1                       ;Number to be searched
    query               resb        1                       ;Response for termination of the program
.CODE   
    .STARTUP
    GetLInt             [size]
    mov                 ECX,[size]                          ;ECX temporarily store the size of the array
    xor                 ESI,ESI                             ;ESI represents the index of the array
GetArray:
    GetLInt             [array + ESI*4]
    inc                 ESI
    dec                 ECX
    cmp                 ECX,0
    jg                  GetArray
GetQuery:
    mov                 ECX, [size]                         ;Storing size into ECX again for the binary search
    dec                 ECX                                 ;Decreasing ECX so as to start indexing from 0
    GetLInt             [num]
    mov                 EAX,[num]                           ;EAX stores the number to be searched
    call                Binary_search
    PutStr              terminate_query
    GetStr              query
    cmp                 byte[query],'y'
    je                  end
    cmp                 byte[query],'Y'
    je                  end
    jmp                 GetQuery
end:
    .EXIT


;START_IND: The starting index of the subarray which will be searched
;END_INT  : The ending index of the subarray which will be searched
;KEY      ; The element to be searched for

%define                 START_IND   dword       [EBP-4]
%define                 END_IND     dword       [EBP-8]
%define                 KEY         dword       [EBP-12]
Binary_search:
    enter               12,0
    mov                 START_IND,0                         ;Initialising the start index to 0
    mov                 END_IND,ECX                         ;Initialising the end index to size-1
    mov                 KEY,EAX                             ;Moving the element to be searched into key
loop:
    mov                 EAX,END_IND                         
    mov                 EBX,START_IND
    cmp                 EAX,EBX                             ;If equal, only one element remains, so we directly check that with key
    jle                 final_check
    add                 EAX,EBX                             ;Sum of lower index + higher index, used to calculate middle index
    mov                 EDX,0                               ;Initialising EDX to 0, since it will be involved in the div instruction
    mov                 ECX,2                               ;Used for the div instruction
    div                 ECX                                 ;Divides EDX:EAX by ECX and returns quotient into EAX
    mov                 ESI,EAX                             
    mov                 ECX,[array + ESI*4]                 ;Stores the middle element in ECX
    cmp                 KEY,ECX
    je                  found
    cmp                 KEY,ECX
    jl                  lower_half                          ;Prunes the upper half of the array
    jmp                 upper_half                          ;Prunes the lower half of the array
lower_half:
    sub                 ESI,1                               ;So as to not involve the middle element in the subarray
    mov                 END_IND,ESI                         ;Subarray = array[1..middle-1]
    jmp                 loop
upper_half:
    inc                 ESI                                 ;So as to not involve the middle element in the subarray
    mov                 START_IND,ESI                       ;Subarray = array[middle+1..n]
    jmp                 loop                 
final_check:
    mov                 ESI,END_IND
    mov                 ECX,[array + ESI*4]
    cmp                 KEY,ECX
    je                  found
    PutInt              0
    nwln
    jmp                 terminate                
found:
    PutInt              1
    nwln
terminate:
    leave
    ret
