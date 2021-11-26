#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<Windows.h>
#include<string.h>
#include<conio.h>
#include <stdbool.h>//c��� bool�ڷ��� ��� 

//Ű���� �Է�, �ҹ���
#define W 119//��
#define S 115//��
#define SPACE 32//����

typedef struct jet {
	int start;
	int top;
}Jet;
typedef struct ST_ENEMY
{
	int x;
	int y;
	bool bActive;
}ST_ENEMY;
typedef struct Bullet {
	int x;
	int y;
	bool bActivate;
}Bullet;

Jet jet;
ST_ENEMY stEnemy[20];
Bullet bullet[20];

//����
static int score = 0;
int MAX = 100;//���̵� ����

void moveCursor(int, int);
void printJet();
void printEnemy();
void initBullet();
void printBullet();
void crashCheck();
void keyboard();
void Render();


int main() {
	jet.start = 10;
	jet.top = 10;
	for (int i = 0; i < 20; i++) {
		stEnemy[i].x = 0;
		stEnemy[i].y = 0;
		stEnemy[i].bActive = false;
	}
	for (int i = 0; i < 20; i++) {
		bullet[i].x = 0;
		bullet[i].y = 0;
		bullet[i].bActivate = false;
	}
	Render();

}

void Render() {
	//�� ȭ�鿡 ��Ÿ���� �Ұ�
	//��Ʈ��(�����), �� �����, �Ѿ�, ����Ȯ��f 

	while (1) {
		if (score >= 100)//100�� �̻��� �Ǹ�
			break;
		if (_kbhit()) {//Ű���� �Է��� ������
			keyboard(_getch());
		}
		system("cls");
		printJet();
		printEnemy();
		printBullet();
		crashCheck();
		moveCursor(45, 20);
		printf("����: %d", score);
		Sleep(10);//���� �ӵ� ����
	}
}
void keyboard(char c) {
	switch (c) {
	case W:
		jet.top -= 1;
		moveCursor(0, 0);
		printf("jet(%d, %d)", jet.start, jet.top);
		break;
	case S:
		jet.top += 1;
		moveCursor(0, 0);
		printf("jet(%d, %d)", jet.start, jet.top);
		break;
	case SPACE:
		//�Ѿ� �߻��ϴ� �̺�Ʈ �߰�
		initBullet();
		break;
	default:
		moveCursor(0, 0);
		printf("�߸��� Ű�� ����, %d", c);

	}
}
void moveCursor(int x, int y) {
	COORD position = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}
void printJet() {
	FILE* file;
	int c;
	char line[1000];
	char* copyline;
	file = fopen("jet.txt", "r");

	if (file != NULL) {
		int i = jet.top;
		while (!feof(file)) {
			moveCursor(jet.start, i);
			copyline = fgets(line, sizeof(line), file);
			printf("%s", copyline);
			++i;
		}
		fclose(file);
	}
	else {
		printf("Failed");
		return -1;
	}
}

void printEnemy() {
	for (int i = 0; i < 10; i++)
	{
		if (!stEnemy[i].bActive)
		{
			stEnemy[i].y = (rand() % 10 * 2);
			stEnemy[i].x = 80;
			stEnemy[i].bActive = true;
			break;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		if (stEnemy[i].bActive)
		{
			moveCursor(stEnemy[i].x, stEnemy[i].y);
			printf("<��(");
			//printf("<��( %d, %d )", stEnemy[i].x, stEnemy[i].y);
			stEnemy[i].x--;
			if (stEnemy[i].x < 10)
			{
				stEnemy[i].bActive = false;
			}
		}
	}
}
void initBullet() {
	for (int i = 0; i < 20; i++) {
		if (!(bullet[i].bActivate)) {
			bullet[i].bActivate = true;
			bullet[i].x = jet.start + 5;
			bullet[i].y = jet.top + 2;
			break;
		}
	}
}
void printBullet() {
	for (int i = 0; i < 20; i++) {
		if (bullet[i].bActivate) {
			//������ ������ bActivate ��Ȱ��ȭ
			if (bullet[i].x > 80) {
				bullet[i].bActivate = false;
				continue;
			}
			moveCursor(bullet[i].x, bullet[i].y);
			printf("->->");
			//printf("->->%d( %d, %d )", i, bullet[i].x, bullet[i].y);
			bullet[i].x += 2;
			//break;
		}
	}
}
void crashCheck() {
	//�浹 �˻�
	for (int i = 0; i < 20; i++) {
		if (bullet[i].bActivate) {
			for (int j = 0; j < 20; j++) {
				//moveCursor(0, 0);
				if (stEnemy[j].bActive && (stEnemy[j].y == bullet[i].y)) {
					moveCursor(0, 0);
					printf("�浹");
					bullet[i].bActivate = false;
					stEnemy[j].bActive = false;
					score += 10;
				}
			}
		}
	}
}