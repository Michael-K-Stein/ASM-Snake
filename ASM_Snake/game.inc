include drd.inc
includelib drd.lib

include data.inc
include funcs.inc

.data
	randStm  SYSTEMTIME<>
	tmpDig100 DWORD 0
	tmpDig10 DWORD 0
	tmpDig1 DWORD 0
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
		add ax, gameParams.reservedSpace
		mov gameParams.sheight, ax

		invoke drd_init, gameParams.swidth, gameParams.sheight, 0
	
		mov field.drawX, 0 ; x
		mov ax, gameParams.reservedSpace
		mov field.drawY, ax ; y

		invoke drd_imageLoadFile, offset field.gray, offset field.grayI
		invoke drd_imageLoadFile, offset field.green, offset field.greenI
		invoke drd_imageLoadFile, offset field.red, offset field.redI
		
		; Load Header Graphics
		invoke drd_imageLoadFile, offset header.highscore, offset header.highscoreI
		invoke drd_imageLoadFile, offset header.Hd1, offset header.Hd1I
		invoke drd_imageLoadFile, offset header.Hd2, offset header.Hd2I
		invoke drd_imageLoadFile, offset header.Hd3, offset header.Hd3I
		invoke drd_imageLoadFile, offset header.Hd4, offset header.Hd4I
		invoke drd_imageLoadFile, offset header.Hd5, offset header.Hd5I
		invoke drd_imageLoadFile, offset header.Hd6, offset header.Hd6I
		invoke drd_imageLoadFile, offset header.Hd7, offset header.Hd7I
		invoke drd_imageLoadFile, offset header.Hd8, offset header.Hd8I
		invoke drd_imageLoadFile, offset header.Hd9, offset header.Hd9I
		invoke drd_imageLoadFile, offset header.Hd0, offset header.Hd0I

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
			jmp GenFruit
		ret

		addToY:
			mov bx, gameParams.bFact
			add field.drawY, bx
			mov field.drawX, 0
			mov bx, field.drawY
			cmp bx, gameParams.sheight
			je doneDrawingWindow
			jne fillWindow

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

	Update endp

	Draw proc
		invoke drd_pixelsClear,0
		
		; Draw Header
		invoke drd_imageDraw, offset header.highscoreI, 0, 0

		; Get highscore digits
		movzx eax, header.highScore
		mov ecx, 100
		div ecx ; EAX: The 100's digit. EDX: The rest
		mov tmpDig100, eax
		mov tmpDig10, edx
		cmp tmpDig100, 0
		je drawHeader100s0
		cmp tmpDig100, 1
		je drawHeader100s1
		cmp tmpDig100, 2
		je drawHeader100s2
		cmp tmpDig100, 3
		je drawHeader100s3
		cmp tmpDig100, 4
		je drawHeader100s4
		cmp tmpDig100, 5
		je drawHeader100s5
		cmp tmpDig100, 6
		je drawHeader100s6
		cmp tmpDig100, 7
		je drawHeader100s7
		cmp tmpDig100, 8
		je drawHeader100s8
		cmp tmpDig100, 9
		je drawHeader100s9

		tensDigit:
		mov eax, tmpDig10
		mov ecx, 10
		div ecx ; EAX: The 10's digit. EDX: The rest
		mov tmpDig10, eax
		mov tmpDig1, edx
		cmp tmpDig10, 0
		je drawHeader10s0
		cmp tmpDig10, 1
		je drawHeader10s1
		cmp tmpDig10, 2
		je drawHeader10s2
		cmp tmpDig10, 3
		je drawHeader10s3
		cmp tmpDig10, 4
		je drawHeader10s4
		cmp tmpDig10, 5
		je drawHeader10s5
		cmp tmpDig10, 6
		je drawHeader10s6
		cmp tmpDig10, 7
		je drawHeader10s7
		cmp tmpDig10, 8
		je drawHeader10s8
		cmp tmpDig10, 9
		je drawHeader10s9

		onesDigit:
		cmp tmpDig1, 0
		je drawHeader1s0
		cmp tmpDig1, 1
		je drawHeader1s1
		cmp tmpDig1, 2
		je drawHeader1s2
		cmp tmpDig1, 3
		je drawHeader1s3
		cmp tmpDig1, 4
		je drawHeader1s4
		cmp tmpDig1, 5
		je drawHeader1s5
		cmp tmpDig1, 6
		je drawHeader1s6
		cmp tmpDig1, 7
		je drawHeader1s7
		cmp tmpDig1, 8
		je drawHeader1s8
		cmp tmpDig1, 9
		je drawHeader1s9

		doneDigits:
		mov field.drawX, 0 ; x
		mov ax, gameParams.reservedSpace
		mov field.drawY, ax ; y

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

		ret
		; Digit Drawings
		drawHeader100s0:
			invoke drd_imageDraw, offset header.Hd0I, 140, 0
			jmp tensDigit
		drawHeader100s1:
			invoke drd_imageDraw, offset header.Hd1I, 140, 0
			jmp tensDigit
		drawHeader100s2:
			invoke drd_imageDraw, offset header.Hd2I, 140, 0
			jmp tensDigit
		drawHeader100s3:
			invoke drd_imageDraw, offset header.Hd3I, 140, 0
			jmp tensDigit
		drawHeader100s4:
			invoke drd_imageDraw, offset header.Hd4I, 140, 0
			jmp tensDigit
		drawHeader100s5:
			invoke drd_imageDraw, offset header.Hd5I, 140, 0
			jmp tensDigit
		drawHeader100s6:
			invoke drd_imageDraw, offset header.Hd6I, 140, 0
			jmp tensDigit
		drawHeader100s7:
			invoke drd_imageDraw, offset header.Hd7I, 140, 0
			jmp tensDigit
		drawHeader100s8:
			invoke drd_imageDraw, offset header.Hd8I, 140, 0
			jmp tensDigit
		drawHeader100s9:
			invoke drd_imageDraw, offset header.Hd9I, 140, 0
			jmp tensDigit

		drawHeader10s0:
			invoke drd_imageDraw, offset header.Hd0I, 165, 0
			jmp onesDigit
		drawHeader10s1:
			invoke drd_imageDraw, offset header.Hd1I, 165, 0
			jmp onesDigit
		drawHeader10s2:
			invoke drd_imageDraw, offset header.Hd2I, 165, 0
			jmp onesDigit
		drawHeader10s3:
			invoke drd_imageDraw, offset header.Hd3I, 165, 0
			jmp onesDigit
		drawHeader10s4:
			invoke drd_imageDraw, offset header.Hd4I, 165, 0
			jmp onesDigit
		drawHeader10s5:
			invoke drd_imageDraw, offset header.Hd5I, 165, 0
			jmp onesDigit
		drawHeader10s6:
			invoke drd_imageDraw, offset header.Hd6I, 165, 0
			jmp onesDigit
		drawHeader10s7:
			invoke drd_imageDraw, offset header.Hd7I, 165, 0
			jmp onesDigit
		drawHeader10s8:
			invoke drd_imageDraw, offset header.Hd8I, 165, 0
			jmp onesDigit
		drawHeader10s9:
			invoke drd_imageDraw, offset header.Hd9I, 165, 0
			jmp onesDigit

		drawHeader1s0:
			invoke drd_imageDraw, offset header.Hd0I, 190, 0
			jmp doneDigits
		drawHeader1s1:
			invoke drd_imageDraw, offset header.Hd1I, 190, 0
			jmp doneDigits
		drawHeader1s2:
			invoke drd_imageDraw, offset header.Hd2I, 190, 0
			jmp doneDigits
		drawHeader1s3:
			invoke drd_imageDraw, offset header.Hd3I, 190, 0
			jmp doneDigits
		drawHeader1s4:
			invoke drd_imageDraw, offset header.Hd4I, 190, 0
			jmp doneDigits
		drawHeader1s5:
			invoke drd_imageDraw, offset header.Hd5I, 190, 0
			jmp doneDigits
		drawHeader1s6:
			invoke drd_imageDraw, offset header.Hd6I, 190, 0
			jmp doneDigits
		drawHeader1s7:
			invoke drd_imageDraw, offset header.Hd7I, 190, 0
			jmp doneDigits
		drawHeader1s8:
			invoke drd_imageDraw, offset header.Hd8I, 190, 0
			jmp doneDigits
		drawHeader1s9:
			invoke drd_imageDraw, offset header.Hd9I, 190, 0
			jmp doneDigits

	Draw endp