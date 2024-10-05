#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include <stdbool.h>

#define WIDTH 60 // �߶�
#define HEIGHT 20 // ���

// �û����°���ʱ������ֵ
int kx, ky = 0;
int lastx, lasty = 0; // �ƶ�ǰ�ߵ�β������
int speed = 300; // ��Ϸ�ٶ�

// һ������Ķ���
struct Body {
	int x;
	int y;
};

// �����߶���
struct Snake {
	struct Body bodys[WIDTH * HEIGHT];
	int size;
} snake;

// ʳ�����
struct Food {
	int x, y;
} food;

int score = 0; // ����

static void initSnake() {
	snake.size = 2;
	// ��ͷ
	snake.bodys[0].x = WIDTH / 2;
	snake.bodys[0].y = HEIGHT / 2;
	// ����
	snake.bodys[1].x = WIDTH / 2 - 1;
	snake.bodys[1].y = HEIGHT / 2;
}

static void initFood() {
	food.x = rand() % WIDTH; // 0 ~ WIDTH - 1
	food.y = rand() % HEIGHT; // 0 ~ HEIGHT - 1
}

static void initUI() {
	// ����
	COORD coord;
	for (size_t i = 0; i < snake.size; i++)
	{
		coord.X = snake.bodys[i].x;
		coord.Y = snake.bodys[i].y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

		if (i == 0) putchar('@');
		else putchar('*');
	}

	// ȥ���ƶ�ǰ����β
	coord.X = lastx;
	coord.Y = lasty;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	putchar(' ');

	// ��ʳ��
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

		// �����û���������
		if (_kbhit()) { // _kbhit������ʱ�����棬���򷵻ؼ�
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

		// �ƶ�ǰ���洢��β����
		lastx = snake.bodys[snake.size - 1].x;
		lasty = snake.bodys[snake.size - 1].y;

		// ��ͷ����ʳ��
		if (snake.bodys[0].x == food.x && snake.bodys[0].y == food.y) {
			// ʳ����ʧ
			initFood();

			// ��������
			snake.size++;

			// �ӷ�
			score += 10;

			// ����
			speed -= 10;
		}

		// ���ƶ���ǰһ���������һ�����帳ֵ
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
	// ���ع��
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = sizeof(cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);

	srand(time(NULL)); // �������������

	initSnake();
	initFood();
	initWall();
	initUI();
	playGame();
	printScore();

	system("pause");
	return 0;
}