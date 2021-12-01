#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include<stdio.h>
#include<string.h>
#include<conio.h>
#include <stdbool.h>//c��� bool�ڷ��� ��� 

//���� ���۸�
static int ScreenIndex;
static HANDLE Screen[2];//ȭ����ȯ ����2��
char* c;
void ScreenInit();//���� �ʱ�ȭ
void ScreenRelease();//���� �޸� ����
void ScreenFlip();//ȭ�� ���۸� ��ȯ
//void ScreenClear();//ȭ�� �ʱ�ȭ


//Ű���� �Է�, �ҹ���
#define W 119//��
#define S 115//��
#define SPACE 32//����

//���� ��� ����ü
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
typedef struct Cloud {
	int x;
	int y;
	bool cActivate;
}Cloud;
Jet jet;
ST_ENEMY stEnemy[20];
Bullet bullet[20];
Cloud cloud[7];

static int score = 0;//����
char score_s[100];
int MAX = 100;//���̵� ����
int sign = 0;//����, ���� �ñ׳�

//�����Լ�
void moveCursor(int, int);
void printJet();
void printEnemy();
void initBullet();
void printBullet();
void crashCheck();//�浹 �˻�
void keyboard(char);
void Render();
void inGame();
void printFile(char*, int);
void main_screen();
void initObject();//�������,��, ���� �ʱ�ȭ
void printCloud(int);
void printrealCloud();

//Ŀ����
void moveCursor(int x, int y);
void setColor(int num);
void printf_b(char*);

//�����̸� �ֱ�����(���ư��� �ӵ� ��
int time = 0;


int main() {

	//system("mode con cols=80 lines=25");

	//SetWindowSize(50, 30);
	ScreenInit();
	Render();
	ScreenRelease();
}

void Render() {//����
	//�� ȭ�鿡 ��Ÿ���� �Ұ�
	//��Ʈ��(�����), �� �����, �Ѿ�, ����Ȯ��
	while (1) {
		initObject();//����ü �ʱ�ȭ
		inGame();//���ӽ���
		if (sign == -1)// ���α׷� ����
			return;
	}
}
void inGame() {
	time = GetTickCount64();
	while (1) {
		/*if (score >= 10) {
			sign = gameover();
			return;
		}*/
		//1000�� �̻��� �Ǹ� ������

		if (time + 100 < GetTickCount64()) {//100ƽ ������, �̸�ŭ �ð��� �帣�� �ڵ� ����, ���⼭ ���� �ӵ� ���� ����!
			time = GetTickCount64();//�ٽ� ����
			COORD Coor = { 0, 0 };
			DWORD dw;

			FillConsoleOutputCharacter(Screen[ScreenIndex], ' ', 80 * 45, Coor, &dw);
			if (_kbhit()) {
				keyboard(_getch());
			}
			printJet();//��Ʈ�� ���
			printBullet();
			printrealCloud();
			printEnemy();
			crashCheck();
			moveCursor(60, 25);
			sprintf(score_s, "%d", score);
			printf_b(score_s);
			ScreenFlip();
		}
		//�� ����� ���
		//printBullet();//���
		//crashCheck();//�浹�˻�
		//moveCursor(45, 20);

		//printf("����: %d", score);//�������
		//Sleep(1);//���� �ӵ� ����, ������ �ӽù���
	}
}

void initObject() {
	jet.top = 10;
	score = 0;
	for (int i = 0; i < 2; i++) {
		cloud[i].x = 110;//������ ��ġ
		cloud[i].y = 5 * i;//������ ��ġ
		cloud[i].cActivate = true;//ùȭ���� ���̰�
	}
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
}
void main_screen() {
	COORD Coor = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(Screen[ScreenIndex], ' ', 80 * 45, Coor, &dw);
	printFile("main_title.txt", 14);
	moveCursor(40, 20);
	printf("�����Ϸ��� �ƹ�Ű�� ��������");
	while (1) {
		if (_kbhit())//�Էµ����� ��
			return;
	}
}
//JET

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
			printf_b(copyline);
			++i;
		}
		fclose(file);
	}
	else {
		//printf("Failed");
		return -1;
	}
}

//BULLET
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
	char* bullet_txt = "-->";
	for (int i = 0; i < 20; i++) {
		if (bullet[i].bActivate) {
			//������ ������ bActivate ��Ȱ��ȭ
			if (bullet[i].x > 100) {
				bullet[i].bActivate = false;
				continue;
			}
			moveCursor(bullet[i].x, bullet[i].y);
			printf_b(bullet_txt);
			//printf("->->%d( %d, %d )", i, bullet[i].x, bullet[i].y);
			bullet[i].x += 2;
			//break;
		}
	}
}
void crashCheck() {
	for (int i = 0; i < 20; i++) {
		if (bullet[i].bActivate) {
			for (int j = 0; j < 20; j++) {
				//moveCursor(0, 0);
				if (stEnemy[j].bActive && (stEnemy[j].y == bullet[i].y) && (stEnemy[j].x <= bullet[i].x + 3)) {
					moveCursor(0, 0);
					bullet[i].bActivate = false;
					stEnemy[j].bActive = false;
					score += 1;
				}
			}
		}
	}
}
//ENEMY
void printEnemy() {
	for (int i = 0; i < 10; i++)
	{
		if (!stEnemy[i].bActive)
		{
			stEnemy[i].y = (rand() % 10 * 3);
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
			printf_b("<��(");
			stEnemy[i].x--;
			if (stEnemy[i].x < 10)
			{
				stEnemy[i].bActive = false;
			}
		}
	}
}
void printCloud(int i) {
	FILE* file;
	int c;
	char line[1000];
	char* copyline;
	file = fopen("cloud.txt", "r");

	if (file != NULL) {
		int j = cloud[i].y;
		while (!feof(file)) {
			moveCursor(cloud[i].x, j);
			copyline = fgets(line, sizeof(line), file);
			printf_b(copyline);
			++j;
		}
		fclose(file);
	}
	else {
		//printf("Failed");
		return -1;
	}

}
void printrealCloud() {
	for (int i = 0; i < 1; i++) {
		/*if (cloud[i].x = jet.start + 10)
			continue;*/
		if (cloud[i].x < 10) {
			cloud[i].x = 100;//�� ������
			cloud[i].y = 5;
			continue;
		}
		if (cloud[i].cActivate) {
			printCloud(i);
			cloud[i].x -= 5;
		}
	}
}
//SYSTEM
void printFile(char* filename, int color_num)//Ŀ�� ��ġ�� ���ڷ� �ޱ�, �����̸� Ŀ�� ��ġ�� ��������� ������
{
	FILE* file;
	DWORD dw;

	setColor(color_num);
	int c;
	char line[1000];
	char* copyline;
	file = fopen(filename, "r");
	if (file != NULL) {
		int i = 4;
		while (!feof(file)) {
			moveCursor(30, i);
			copyline = fgets(line, sizeof(line), file);
			printf_b(copyline);
			++i;
		}
		setColor(15);
		fclose(file);
	}
	else {
		//printf("Failed");
		return -1;
	}
}
void moveCursor(int x, int y) {
	COORD position = { x, y };
	SetConsoleCursorPosition(Screen[ScreenIndex], position);
}
void setColor(int num) {
	SetConsoleTextAttribute(Screen[ScreenIndex], num);
}
void printf_b(char* str) {
	DWORD dw;
	WriteFile(Screen[ScreenIndex], str, strlen(str), &dw, NULL);
}
void keyboard(char c) {
	switch (c) {
	case W:
		jet.top -= 1;
		moveCursor(0, 0);
		//printf("jet(%d, %d)", jet.start, jet.top);
		break;
	case S:
		jet.top += 1;
		moveCursor(0, 0);
		//printf("jet(%d, %d)", jet.start, jet.top);
		break;
	case SPACE:
		//�Ѿ� �߻��ϴ� �̺�Ʈ �߰�
		initBullet();
		break;
	default:
		moveCursor(0, 0);
		break;
		//printf("�߸��� Ű�� ����, %d", c);

	}
}
//BUFFER
void ScreenInit() // ȭ�� ���� 2�� ����
{
	CONSOLE_CURSOR_INFO cci;

	Screen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	Screen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	// Ŀ���� �����.
	cci.dwSize = 1;
	cci.bVisible = FALSE;

	SetConsoleCursorInfo(Screen[0], &cci);
	SetConsoleCursorInfo(Screen[1], &cci);//�Ƹ� ù��° ���ڰ� ȭ�� ���۸� �ǹ��ϴ� ��!

}
void ScreenRelease()//���� �޸� ����
{
	CloseHandle(Screen[0]);
	CloseHandle(Screen[1]);
}
void ScreenFlip()//ȭ�� ���۸� ��ȯ�ϴ� �Լ�
{
	SetConsoleActiveScreenBuffer(Screen[ScreenIndex]);
	ScreenIndex = !ScreenIndex;//�ε��� ��ȯ(���� ��ȯ)
}