	;Objectives: 	To encrypt strings such that integers are replaced with (value+5)%10
	;Inputs	   :	String(the string to be encrypted) and response(whether user wants to encrypt another string)
	;Outputs   :	Encrypted version of final string entered

%include "io.mac"
.DATA
	user_query		db					"Do you wish to terminate? ",0
	user_input		db					"Please enter another string: ",0
.UDATA
	string			resb				20
	response		resb				1
.CODE
	.STARTUP
loop:
	GetStr			string,20
	mov				EBX,string
	xor				ECX,ECX				
repeat_encrypt:
	mov				AL,[EBX+ECX]	
	cmp				AL,0
	je				done_encrypt
	mov				AH,[EBX+ECX]
	and				AH,0hf0				;mask off the last 4 bits 
	cmp				AH,0h40				;not an integer if ascii value is greater than 64
	jge				non_int
	and				AL,0h0f				;mask off the first 4 bits
	cmp				AL,0h05
	jge				greater_than4
	add				AL,0h05				;if less than 5, add 5 to the value
	add				AL,AH				;equivalent to adding 48 since AH will be 0h30
	mov				[EBX+ECX],AL
	jmp				non_int				;increments ECX and jumps to repeat_encrypt
greater_than4:
	sub				AL,0h05				;if greater than 4, subtract 5 from the value
	add 			AL,AH
	mov				[EBX+ECX],AL
non_int:
	inc				ECX
	jmp				repeat_encrypt
done_encrypt:
	PutStr			user_query
	GetStr 			response,1
	cmp				byte[response],'y'
	je				done
	cmp				byte[response],'Y'
	je				done
	PutStr			user_input
	jmp				loop
done:
	PutStr			EBX
	nwln
	.EXIT
