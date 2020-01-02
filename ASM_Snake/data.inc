.const
ScreenParams STRUCT	
	bW WORD ?
	bHe WORD ?
	bFact WORD ?
	swidth WORD ?
	sheight WORD ?
ScreenParams ends

SnakeBody STRUCT
	x WORD ?
	y WORD ?
SnakeBody ends

SnakeBody4 STRUCT
	BodyPart1 SnakeBody< >
	BodyPart2 SnakeBody< >
	BodyPart3 SnakeBody< >
	BodyPart4 SnakeBody< >
SnakeBody4 ends

SnakeBody16 STRUCT
	BodyPart1 SnakeBody4< >
	BodyPart2 SnakeBody4< >
	BodyPart3 SnakeBody4< >
	BodyPart4 SnakeBody4< >
SnakeBody16 ends

SnakeBody64 STRUCT
	BodyPart1 SnakeBody16< >
	BodyPart2 SnakeBody16< >
	BodyPart3 SnakeBody16< >
	BodyPart4 SnakeBody16< >
SnakeBody64 ends

SnakeBody256 STRUCT
	BodyPart1 SnakeBody64< >
	BodyPart2 SnakeBody64< >
	BodyPart3 SnakeBody64< >
	BodyPart4 SnakeBody64< >
SnakeBody256 ends

Snake STRUCT
	Head SnakeBody< >
	BodyPart1 SnakeBody256< >
Snake ends

Field STRUCT
	drawX WORD ?
	drawY WORD ?
	snakeLen WORD ?
	snakeDir WORD ?
	snakeAdr DWORD ?
	snakeLenTemp WORD ?
	Fruit SnakeBody<>
	isDead Byte 0
	gray BYTE "graphics/gray.png", 0
	grayI Img <>
	green BYTE "graphics/green.png", 0
	greenI Img <>
	red BYTE "graphics/red.png", 0
	redI Img <>
Field ends
.data
gameParams ScreenParams< 30, 20, 25, 0, 0 >
;snakeHead SnakeBody< 0, 0 >
snake Snake< <0, 0> >
field Field<>