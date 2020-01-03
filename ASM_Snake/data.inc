.const
ScreenParams STRUCT	
	bW WORD ?
	bHe WORD ?
	bFact WORD ?
	swidth WORD ?
	sheight WORD ?
	reservedSpace WORD ?
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
	Fruit SnakeBody < >
	isDead Byte 0
	gray BYTE "graphics/gray.png", 0
	grayI Img <>
	green BYTE "graphics/green.png", 0
	greenI Img <>
	red BYTE "graphics/red.png", 0
	redI Img <>
Field ends

Header STRUCT
	highScore WORD ?

	highscore BYTE "graphics/header/highscore.png", 0
	highscoreI Img <>

	Hd1 BYTE "graphics/header/digits/1.png", 0
	Hd1I Img <>
	Hd2 BYTE "graphics/header/digits/2.png", 0
	Hd2I Img <>
	Hd3 BYTE "graphics/header/digits/3.png", 0
	Hd3I Img <>
	Hd4 BYTE "graphics/header/digits/4.png", 0
	Hd4I Img <>
	Hd5 BYTE "graphics/header/digits/5.png", 0
	Hd5I Img <>
	Hd6 BYTE "graphics/header/digits/6.png", 0
	Hd6I Img <>
	Hd7 BYTE "graphics/header/digits/7.png", 0
	Hd7I Img <>
	Hd8 BYTE "graphics/header/digits/8.png", 0
	Hd8I Img <>
	Hd9 BYTE "graphics/header/digits/9.png", 0
	Hd9I Img <>
	Hd0 BYTE "graphics/header/digits/0.png", 0
	Hd0I Img <>
Header ends
.data
gameParams ScreenParams< 30, 20, 25, 0, 0, 25 >
;snakeHead SnakeBody< 0, 0 >
snake Snake< <0, 25> >
field Field<>
header Header<>