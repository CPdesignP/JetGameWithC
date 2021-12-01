#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include<stdio.h>
#include<string.h>
#include<conio.h>
#include <stdbool.h>//c언어 bool자료형 사용 

//더블 버퍼링
static int ScreenIndex;
static HANDLE Screen[2];//화면전환 버퍼2개
char* c;
void ScreenInit();//버퍼 초기화
void ScreenRelease();//버퍼 메모리 해제
void ScreenFlip();//화면 버퍼를 전환
//void ScreenClear();//화면 초기화


//키보드 입력, 소문자
#define W 119//상
#define S 115//하
#define SPACE 32//공격

//게임 요소 구조체
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

static int score = 0;//점수
char score_s[100];
int MAX = 100;//난이도 조절
int sign = 0;//시작, 종료 시그널

//게임함수
void moveCursor(int, int);
void printJet();
void printEnemy();
void initBullet();
void printBullet();
void crashCheck();//충돌 검사
void keyboard(char);
void Render();
void inGame();
void printFile(char*, int);
void main_screen();
void initObject();//적비행기,나, 점수 초기화
void printCloud(int);
void printrealCloud();

//커스텀
void moveCursor(int x, int y);
void setColor(int num);
void printf_b(char*);

//딜레이를 주기위한(날아가는 속도 등
int time = 0;


int main() {

	//system("mode con cols=80 lines=25");

	//SetWindowSize(50, 30);
	ScreenInit();
	Render();
	ScreenRelease();
}

void Render() {//게임
	//한 화면에 나타내야 할것
	//제트기(사용자), 적 비행기, 총알, 점수확인
	while (1) {
		initObject();//구조체 초기화
		inGame();//게임실행
		if (sign == -1)// 프로그램 종료
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
		//1000점 이상이 되면 끝내기

		if (time + 100 < GetTickCount64()) {//100틱 지나면, 이만큼 시간이 흐르면 코드 진행, 여기서 게임 속도 조절 가능!
			time = GetTickCount64();//다시 업뎃
			COORD Coor = { 0, 0 };
			DWORD dw;

			FillConsoleOutputCharacter(Screen[ScreenIndex], ' ', 80 * 45, Coor, &dw);
			if (_kbhit()) {
				keyboard(_getch());
			}
			printJet();//제트기 출력
			printBullet();
			printrealCloud();
			printEnemy();
			crashCheck();
			moveCursor(60, 25);
			sprintf(score_s, "%d", score);
			printf_b(score_s);
			ScreenFlip();
		}
		//적 비행기 출력
		//printBullet();//쏘기
		//crashCheck();//충돌검사
		//moveCursor(45, 20);

		//printf("점수: %d", score);//점수출력
		//Sleep(1);//게임 속도 조절, 깜박이 임시방편
	}
}

void initObject() {
	jet.top = 10;
	score = 0;
	for (int i = 0; i < 2; i++) {
		cloud[i].x = 110;//램덤한 위치
		cloud[i].y = 5 * i;//랜덤한 위치
		cloud[i].cActivate = true;//첫화면은 보이게
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
	printf("시작하려면 아무키나 누르세요");
	while (1) {
		if (_kbhit())//입력들어오면 끝
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
			//밖으로 나가면 bActivate 비활성화
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
			printf_b("<ㅔ(");
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
			cloud[i].x = 100;//악 귀찬스
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
void printFile(char* filename, int color_num)//커서 위치도 인자로 받기, 움직이면 커서 위치를 갱신해줘야 함으로
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
		//총알 발사하는 이벤트 추가
		initBullet();
		break;
	default:
		moveCursor(0, 0);
		break;
		//printf("잘못된 키를 누름, %d", c);

	}
}
//BUFFER
void ScreenInit() // 화면 버퍼 2개 생성
{
	CONSOLE_CURSOR_INFO cci;

	Screen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	Screen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	// 커서를 숨긴다.
	cci.dwSize = 1;
	cci.bVisible = FALSE;

	SetConsoleCursorInfo(Screen[0], &cci);
	SetConsoleCursorInfo(Screen[1], &cci);//아마 첫번째 인자가 화면 버퍼를 의미하는 것!

}
void ScreenRelease()//버퍼 메모리 해제
{
	CloseHandle(Screen[0]);
	CloseHandle(Screen[1]);
}
void ScreenFlip()//화면 버퍼를 전환하는 함수
{
	SetConsoleActiveScreenBuffer(Screen[ScreenIndex]);
	ScreenIndex = !ScreenIndex;//인덱스 변환(버퍼 변환)
}