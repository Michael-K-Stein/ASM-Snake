
.386                            ; Tells MASM to use Intel 80386 instruction set.

.model flat,stdcall             ; Flat memory model

option casemap:none             ; Treat labels as case-sensitive

include \masm32\include\masm32rt.inc

include game.inc

.data
	MBC		DB		"Snake",	0
    MBT		DD		0001b
	stm  SYSTEMTIME<>
	delayMil	DWORD	250
.code

main proc
	
	invoke Load	

	game:
		
		invoke	Update	

		invoke Draw

		mov ebx, delayMil
		loopyloop:
			sub ebx, 1
			cmp ebx, 0
			jne loopyloop
			je delay

		jmp delay

	jmp game
	mov eax, 0 ; Reload
	
	delay:
		invoke GetSystemTime, ADDR stm

		mov ebx, 0
		checkLoop:
			movzx ecx, stm.wMilliseconds
			cmp ecx, ebx
			je game
			add ebx, delayMil
			cmp ebx, 1000
			jle checkLoop
			jg delay




main endp
end main
