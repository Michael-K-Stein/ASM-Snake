.data
	randStm  SYSTEMTIME<>

	headX WORD -1
	headY WORD -1

.code

GenerateFruit proc ; Generate a new red fruit
	invoke GetSystemTime, ADDR randStm
	movzx eax, randStm.wMilliseconds
	movzx ebx, randStm.wSecond
	add eax, ebx
	mov ecx, 13;snake.Head.x
	add ecx, 1
	mul ecx
	mov edx, 0
	movzx ebx, gameParams.bW
	div ebx
	;add dx, gameParams.reservedSpace
	mov field.Fruit.x, dx
	invoke GetSystemTime, ADDR randStm
	movzx eax, randStm.wMilliseconds
	movzx ebx, randStm.wSecond
	add eax, ebx
	mov ecx, 17;snake.Head.y
	add ecx, 1
	mul ecx
	mov edx, 0
	movzx ebx, gameParams.bHe
	div ebx
	add dx, 1
	mov field.Fruit.y, dx
	ret	
GenerateFruit endp

MoveSnake proc ; Move all the parts of the snake to their new position
	mov field.snakeLenTemp, 0
		;invoke drd_imageDraw, offset field.greenI, snake.Head.x, snake.Head.y


		movzx eax, field.snakeLen
		sub eax, 1
		mov ebx, 4
		mul ebx
		lea edx, offset snake.Head
		add edx, eax
		mov field.snakeAdr, edx

		MovePartOfSnake:
			add field.snakeLenTemp, 1
			mov ax, field.snakeLen
			cmp field.snakeLenTemp, ax
			je FinishedMove
			;lea edx, offset snake.Head
			mov edx, field.snakeAdr
			sub edx, 4
			mov field.snakeAdr, edx
			mov ebx, offset snake.Head
			ASSUME edx: ptr SnakeBody
			mov bx, [edx + 0].x
			mov [edx + 4].x, bx
			mov bx, [edx + 0].y
			mov [edx + 4].y, bx
			;invoke drd_imageDraw, offset field.greenI, [edx + 0].x, [edx + 0].y
			jmp MovePartOfSnake
		FinishedMove:
			ret
MoveSnake endp

amIDead proc adrHead:dword  ; Check if the snake ate itself
	mov edx, adrHead
	ASSUME edx: ptr SnakeBody
	mov cx, [edx].x
	mov headX, cx
	mov cx, [edx].y
	mov headY, cx
	mov bx, field.snakeLen
	mov field.snakeLenTemp, bx
	mov edx, adrHead
	mov field.snakeAdr, edx
	add field.snakeAdr, 4
	LoopParts:
		sub field.snakeLenTemp, 1
		cmp field.snakeLenTemp, 0
		je DoneLoop
		mov edx, field.snakeAdr
		ASSUME edx: ptr SnakeBody
		mov cx, [edx].x
		cmp headX, cx
		jne NotDeadYet
		mov cx, [edx].y
		cmp headY, cx
		je YouDied
		jne NotDeadYet
		NotDeadYet:
			add field.snakeAdr, 4
			jmp LoopParts
	DoneLoop:
		ret
	ret
	YouDied:
		mov field.isDead, 1
		ret

amIDead endp

didEatFruit proc adrFruit:dword ; Check if the snake ate a fruit
	jmp CheckFruitEaten1
	CheckFruitEaten1:
			mov edx, adrFruit
			ASSUME edx: ptr SnakeBody
			mov bx, [edx].x
			movzx eax, gameParams.bFact
			mul bx
			movzx ebx, snake.Head.x
			cmp ebx, eax
			je CheckFruitEaten2
			ret
		CheckFruitEaten2:
			mov edx, adrFruit
			ASSUME edx: ptr SnakeBody
			mov bx, [edx].y
			movzx eax, gameParams.bFact
			mul bx
			movzx ebx, snake.Head.y
			cmp ebx, eax
			je FruitEaten
			ret
		FruitEaten:
			add field.snakeLen, 1

			mov ax, field.snakeLen
			cmp ax, header.highScore
			jg newHighScore
			jmp notNewHighScore

			newHighScore:
				mov bx, field.snakeLen
				mov header.highScore, bx
				jmp notNewHighScore
			notNewHighScore:

			; Move the new part out of the screen (temporarily)
			movzx eax, field.snakeLen
			sub eax, 1
			mov ebx, 4
			mul ebx
			lea edx, offset snake.Head
			add edx, eax
			mov field.snakeAdr, edx

			mov [edx], -1
			mov [edx - 2], -1

			invoke GenerateFruit
	ret
didEatFruit endp

WaitForSpace proc ; Wait for space to be pressed (for loading screen)
	invoke GetAsyncKeyState, VK_SPACE
	cmp eax,0
	jne CHANGEDRC
	mov eax, 0
	ret

	CHANGEDRC:
		mov eax,1
		ret
WaitForSpace endp
