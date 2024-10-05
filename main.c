#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include <stdbool.h>

#define WIDTH 60 // 高度
#define HEIGHT 20 // 宽度

// 用户按下按键时的坐标值
int kx, ky = 0;
int lastx, lasty = 0; // 移动前蛇的尾部坐标
int speed = 300; // 游戏速度

// 一个身体的对象
struct Body {
	int x;
	int y;
};

// 定义蛇对象
struct Snake {
	struct Body bodys[WIDTH * HEIGHT];
	int size;
} snake;

// 食物对象
struct Food {
	int x, y;
} food;

int score = 0; // 分数

static void initSnake() {
	snake.size = 2;
	// 蛇头
	snake.bodys[0].x = WIDTH / 2;
	snake.bodys[0].y = HEIGHT / 2;
	// 蛇身
	snake.bodys[1].x = WIDTH / 2 - 1;
	snake.bodys[1].y = HEIGHT / 2;
}

static void initFood() {
	food.x = rand() % WIDTH; // 0 ~ WIDTH - 1
	food.y = rand() % HEIGHT; // 0 ~ HEIGHT - 1
}

static void initUI() {
	// 画蛇
	COORD coord;
	for (size_t i = 0; i < snake.size; i++)
	{
		coord.X = snake.bodys[i].x;
		coord.Y = snake.bodys[i].y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

		if (i == 0) putchar('@');
		else putchar('*');
	}

	// 去除移动前的蛇尾
	coord.X = lastx;
	coord.Y = lasty;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	putchar(' ');

	// 画食物
	coord.X = food.x;
	coord.Y = food.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	putchar('#');
}

static void initWall() {
	for (size_t i = 0; i <= HEIGHT; i++)
	{
		for (size_t j = 0; j <= WIDTH; j++)
		{
			if (j == WIDTH) {
				printf("|");
			}
			else if (i == HEIGHT) {
				printf("_");
			}
			else {
				printf(" ");
			}
		}
		printf("\n");
	}
}

static bool snakeHitWall() {
	if (snake.bodys[0].x >= 0 && snake.bodys[0].x < WIDTH && snake.bodys[0].y >= 0 && snake.bodys[0].y < HEIGHT)
		return false;

	return true;
}

static bool snakeHitBody() {
	for (size_t i = 1; i < snake.size; i++)
	{
		if (snake.bodys[0].x == snake.bodys[i].x && snake.bodys[0].y == snake.bodys[i].y)
			return true;
	}
	return false;
}

static void playGame() {
	char key = 'd';
	while (!snakeHitWall() && !snakeHitBody()) {
		// initWall();
		initUI();

		// 接收用户按键输入
		if (_kbhit()) { // _kbhit有输入时返回真，否则返回假
			key = _getch();
		}

		switch (key)
		{
		case 'w':
			kx = 0;
			ky = -1;
			break;
		case 's':
			kx = 0;
			ky = 1;
			break;
		case 'a':
			kx = -1;
			ky = 0;
			break;
		case 'd':
			kx = 1;
			ky = 0;
			break;
		default:
			break;
		}

		// 移动前，存储蛇尾坐标
		lastx = snake.bodys[snake.size - 1].x;
		lasty = snake.bodys[snake.size - 1].y;

		// 蛇头碰到食物
		if (snake.bodys[0].x == food.x && snake.bodys[0].y == food.y) {
			// 食物消失
			initFood();

			// 身体增长
			snake.size++;

			// 加分
			score += 10;

			// 加速
			speed -= 10;
		}

		// 蛇移动后，前一节身体给后一节身体赋值
		for (size_t i = snake.size - 1; i > 0; i--)
		{
			snake.bodys[i].x = snake.bodys[i - 1].x;
			snake.bodys[i].y = snake.bodys[i - 1].y;
		}
		snake.bodys[0].x += kx;
		snake.bodys[0].y += ky;

		Sleep(speed);
		// system("cls");
	}
}

static void printScore() {
	COORD coord;
	coord.X = 0;
	coord.Y = HEIGHT + 2;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

	printf("Game Over!\n");
	printf("Your Score: %d\n\n", score);
}

int main(void) {
	// 隐藏光标
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = sizeof(cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);

	srand(time(NULL)); // 播种随机数种子

	initSnake();
	initFood();
	initWall();
	initUI();
	playGame();
	printScore();

	system("pause");
	return 0;
}