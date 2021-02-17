	;Objectives:	To find the sum of a number of integers
	;Inputs	   :	n(the number of integer), num(each integer)
	;Outputs   :	The sum of all integers
	
%include 			"io.mac"
.DATA
	sum				dd			0	;sum of the numbers
.UDATA
	n				resd		1	;number of numbers to be added
	num				resd		1	;number to be added in the sum
.CODE
    .STARTUP
	GetLInt		 	[n]						
	mov 			ECX,[n]	
repeat_add:
	cmp				ECX,0
	je				done
	GetLInt			[num]		
	mov				EAX,[num]
	add				[sum],EAX
	dec				ECX				
	jmp				repeat_add
done:
	PutLInt			[sum]
	nwln
    .EXIT
    
