.code
impactOfkey proc  adrobj:dword, key:dword, adrdrc:dword, value:dword
	invoke GetAsyncKeyState, key
	cmp eax,0
	jne CHANGEDRC
	RETCHANGEDRC:
		ret

	CHANGEDRC:
		mov ecx,adrdrc
		mov edx,value
		mov [ecx],edx
		jmp RETCHANGEDRC
impactOfkey endp

keyboard proc adrobj:dword
	
	invoke impactOfkey, adrobj, VK_UP, offset field.snakeDir, 0
	invoke impactOfkey, adrobj, VK_RIGHT, offset field.snakeDir, 1
	invoke impactOfkey, adrobj, VK_DOWN, offset field.snakeDir, 2
	invoke impactOfkey, adrobj, VK_LEFT, offset field.snakeDir, 3

	ret
keyboard endp

moveObject proc adrobj:dword
	cmp field.snakeDir, 0
	je moveUp
	cmp field.snakeDir, 1
	je moveRight
	cmp field.snakeDir, 2
	je moveDown
	cmp field.snakeDir, 3
	je moveLeft

	moveRight:
		mov bx, gameParams.bFact
		add snake.Head.x, bx
		mov bx, gameParams.swidth
		cmp snake.Head.x, bx
		je fixMoveRight
		ret

	fixMoveRight:
		mov snake.Head.x, 0
		ret

	moveLeft:
		mov bx, gameParams.bFact
		sub snake.Head.x, bx
		mov bx, gameParams.swidth
		cmp snake.Head.x, 0
		jl fixMoveLeft
		ret

	fixMoveLeft:
		mov bx, gameParams.bFact
		add snake.Head.x, bx
		mov bx, gameParams.swidth
		mov snake.Head.x, bx
		ret

	moveUp:
		mov bx, gameParams.bFact
		sub snake.Head.y, bx
		mov ax, gameParams.reservedSpace
		cmp snake.Head.y, ax
		jl fixMoveUp
		ret

	fixMoveUp:
		mov bx, gameParams.sheight
		sub bx, gameParams.bFact
		mov snake.Head.y, bx
		ret

	moveDown:
		mov bx, gameParams.bFact
		add snake.Head.y, bx
		mov bx, gameParams.sheight
		cmp snake.Head.y, bx
		jg fixMoveDown
		ret

	fixMoveDown:
		mov ax, gameParams.reservedSpace
		mov snake.Head.y, ax
		ret

moveObject endp
