include drd.inc
includelib drd.lib

include data.inc
include funcs.inc

.data
	randStm  SYSTEMTIME<>

.code
	Load proc
		
		mov field.snakeDir, 1
		mov field.snakeLen, 4

		mov ax, gameParams.bW
		;mov ebx, gameParams.bFact
		mul gameParams.bFact
		mov gameParams.swidth, ax

		mov ax, gameParams.bHe
		mul gameParams.bFact
		mov gameParams.sheight, ax

		invoke drd_init, gameParams.swidth, gameParams.sheight, 0
	
		mov field.drawX, 0 ; x
		mov field.drawY, 0 ; y

		invoke drd_imageLoadFile, offset field.gray, offset field.grayI
		invoke drd_imageLoadFile, offset field.green, offset field.greenI
		invoke drd_imageLoadFile, offset field.red, offset field.redI

		fillWindow:
			invoke drd_imageDraw, offset field.grayI, field.drawX, field.drawY
			mov bx, gameParams.bFact
			add field.drawX, bx
			mov bx, field.drawX
			CMP bx, gameParams.swidth
			je addToY
			jne fillWindow
		doneDrawingWindow:
			invoke drd_flip
		ret

		addToY:
			mov bx, gameParams.bFact
			add field.drawY, bx
			mov field.drawX, 0
			mov bx, field.drawY
			cmp bx, gameParams.sheight
			je doneDrawingWindow
			jne fillWindow

	Load endp

	Update proc
		invoke drd_processMessages
		invoke keyboard, offset snake.Head

		;lea edx, offset snake.Head
		;ASSUME edx: ptr SnakeBody
		;mov ebx, snake.Head.x
		;mov [edx + 8].x, ebx
		;mov ebx, snake.Head.y
		;mov [edx + 8].y, ebx

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
			invoke moveObject, offset snake.Head
			invoke amIDead, offset snake.Head
			jmp CheckFruitEaten1
			ret
		ret
		CheckFruitEaten1:
			mov bx, field.Fruit.x
			movzx eax, gameParams.bFact
			mul bx
			movzx ebx, snake.Head.x
			cmp ebx, eax
			je CheckFruitEaten2
			ret
		CheckFruitEaten2:
			mov bx, field.Fruit.y
			movzx eax, gameParams.bFact
			mul bx
			movzx ebx, snake.Head.y
			cmp ebx, eax
			je FruitEaten
			ret
		FruitEaten:
			add field.snakeLen, 1

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

			jmp GenFruit
			ret

		GenFruit:
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
			mov field.Fruit.y, dx

	Update endp

	Draw proc
		invoke drd_pixelsClear,0
		
		mov field.drawX, 0 ; x
		mov field.drawY, 0 ; y

		cmp field.isDead, 1
		je DeathScreen

		fillWindow:
			invoke drd_imageDraw, offset field.grayI, field.drawX, field.drawY
			mov bx, gameParams.bFact
			add field.drawX, bx
			mov bx, field.drawX
			CMP bx, gameParams.swidth
			je addToY
			jne fillWindow
		doneDrawingWindow:
			jmp DrawSnake

		DrawSnake:
			; Draw Snake
			mov field.snakeLenTemp, 0
			invoke drd_imageDraw, offset field.greenI, snake.Head.x, snake.Head.y

			lea edx, offset snake.Head
			mov field.snakeAdr, edx

			DrawPartOfSnake:
				add field.snakeLenTemp, 1
				mov ax, field.snakeLen
				cmp field.snakeLenTemp, ax
				je FinishedDawingParts
				;lea edx, offset snake.Head
				mov edx, field.snakeAdr
				add edx, 4
				mov field.snakeAdr, edx
				ASSUME edx: ptr SnakeBody
				invoke drd_imageDraw, offset field.greenI, [edx + 0].x, [edx + 0].y
				jmp DrawPartOfSnake
			
			FinishedDawingParts:
				; Draw Fruit
				movzx eax, field.Fruit.x
				mul gameParams.bFact
				mov ebx, eax
				movzx eax, field.Fruit.y
				mul gameParams.bFact
				mov ecx, eax
				invoke drd_imageDraw, offset field.redI, ebx, ecx
				invoke drd_flip
				ret
		ret

		addToY:
			mov bx, gameParams.bFact
			add field.drawY, bx
			mov field.drawX, 0
			mov bx, field.drawY
			cmp bx, gameParams.sheight
			je doneDrawingWindow
			jne fillWindow

		addToYDeath:
			mov bx, gameParams.bFact
			add field.drawY, bx
			mov field.drawX, 0
			mov bx, field.drawY
			cmp bx, gameParams.sheight
			je doneDrawingWindowDeath
			jne fillWindowDeath

		DeathScreen:
			fillWindowDeath:
				invoke drd_imageDraw, offset field.redI, field.drawX, field.drawY
				mov bx, gameParams.bFact
				add field.drawX, bx
				mov bx, field.drawX
				CMP bx, gameParams.swidth
				je addToYDeath
				jne fillWindowDeath
			doneDrawingWindowDeath:
				mov field.isDead, 0
				mov field.snakeLen, 4
				invoke drd_flip
				ret

	Draw endp