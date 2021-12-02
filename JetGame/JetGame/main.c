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
char score_s[100];//char
int MAX = 100;//난이도 조절
int sign = 0;//시작, 종료 시그널
int energy = 10;//생명
char energy_s[10];

//게임함수
void moveCursor(int, int);
void printJet();
void printEnemy();
void initBullet();
void printBullet();
int gameover(void);
void crashCheck();
void crashCheck2();
void crashCheckCloud();
void keyboard(char);
void Render();
void inGame();
void printFile(char*, int);
void main_screen();
void initObject();
void printCloud(int);
void printrealCloud();

//커스텀
void moveCursor(int x, int y);
void setColor(int num);
void printf_b(char*);

//딜레이를 주기위한(날아가는 속도 등
int time = 0;


int main() {

	ScreenInit();
	Render();
	ScreenRelease();
}

void Render() {
	//한 화면에 나타내야 할것
	//제트기(사용자), 적 비행기, 총알, 점수확인 + 구름
	main_screen();
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
		if (score >= MAX || energy == 0) {
			sign = gameover();
			return;
		}
		if (time + 60 < GetTickCount64()) {//100틱 지나면, 이만큼 시간이 흐르면 코드 진행, 여기서 게임 속도 조절 가능!
			time = GetTickCount64();//다시 업뎃
			COORD Coor = { 0, 0 };
			DWORD dw;
			FillConsoleOutputCharacter(Screen[ScreenIndex], ' ', 80 * 45, Coor, &dw);//화면 지우기

			if (_kbhit()) {
				keyboard(_getch());//키보드 입력
			}
			printJet();//사용자 전투기 출력
			printBullet();//총알 출력
			printrealCloud();//구름 출력
			printEnemy();//적 출력
			crashCheck();//총알 적 충돌검사
			crashCheck2();//나 적 충돌검사
			moveCursor(60, 25);
			setColor(15);
			printf_b("점수: ");
			moveCursor(65, 25);
			sprintf(score_s, "%d", score);//int->string 변환
			printf_b(score_s);//점수 출력
			moveCursor(60, 27);
			printf_b("생명: ");
			moveCursor(65, 27);
			sprintf(energy_s, "%d", energy);
			printf_b(energy_s);
			ScreenFlip();//화면 버퍼 전환
		}
	}
}
int gameover(void)//게임종료화면
{
	COORD Coor = { 0, 0 };
	DWORD dw;
	//back buffer의 화면 지우기
	FillConsoleOutputCharacter(Screen[ScreenIndex], ' ', 80 * 45, Coor, &dw);
	setColor(14);
	FILE* file;
	int c;
	char line[1000];
	char* copyline;
	file = fopen("end_title.txt", "r");

	if (file != NULL) {
		int i = 5;
		while (!feof(file)) {
			moveCursor(30, i);
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
	moveCursor(45, 20);
	printf_b("다시하기:y");
	moveCursor(45, 23);
	printf_b("나가기:n");
	ScreenFlip();//지금 까지 back buffer에 그린것을 front buffer와 전환
	while (1)
	{
		if (_kbhit())
		{
			char c = _getch();
			if (c == 'Y' || c == 'y')
				return 1;//게임으로
			else if (c == 'N' || c == 'n')
				return -1;//종료

		}
	}

}
void initObject() {
	jet.top = 10;
	score = 0;
	energy = 10;
	for (int i = 0; i < 1; i++) {
		cloud[i].x = 110;//램덤한 위치
		cloud[i].y = (rand() % 10 * 3);//랜덤한 위치
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
	//COORD Coor = { 0, 0 };
	//DWORD dw;
	//FillConsoleOutputCharacter(Screen[ScreenIndex], ' ', 80 * 45, Coor, &dw);
	//printFile("main_title.txt", 1);
	//moveCursor(0, 0);
	////printf("시작하려면 아무키나 누르세요");
	setColor(14);
	FILE* file;
	int c;
	char line[1000];
	char* copyline;
	file = fopen("main_title.txt", "r");

	if (file != NULL) {
		int i = 5;
		while (!feof(file)) {
			moveCursor(30, i);
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
	moveCursor(40, 20);
	printf_b("시작하려면 아무키나 누르세요");
	ScreenFlip();//다 그리면 front buffer에 오게 버퍼 전환을 해준다.
	while (1) {
		if (_kbhit())//입력들어오면 끝
			return;
	}
}

//JET
void printJet() {
	setColor(11);
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
	setColor(14);
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
void crashCheck2() {//나와 적 충돌 검사
	for (int i = 0; i < 20; i++) {
		if (stEnemy[i].bActive && (stEnemy[i].x <= jet.start + 7) && ((stEnemy[i].y >= jet.top) && (stEnemy[i].y <= jet.top + 6))) {
			stEnemy[i].bActive = false;
			energy -= 1;
		}
	}

}
//ENEMY
void printEnemy() {
	setColor(12);
	for (int i = 0; i < 10; i++)
	{
		if (!stEnemy[i].bActive)
		{
			stEnemy[i].y = (rand() % 10 * 3);
			stEnemy[i].x = 100;
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
			if (stEnemy[i].x < 3)
			{
				stEnemy[i].bActive = false;
			}
		}
	}
}
void printCloud(int i) {
	setColor(15);
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
			cloud[i].x = 100;
			cloud[i].y = (rand() % 10 * 2) + 5;
			continue;
		}
		if (cloud[i].cActivate) {
			printCloud(i);
			cloud[i].x -= 3;
		}
	}
}
void crashCheckCloud() {
	//if((cloud[0].x <= jet.start + 7) && ((cloud[0].y >= jet.top) && (cloud[0].y <= jet.top + 6))|| ((cloud[0].y >= jet.top) && (cloud[0].y <= jet.top + 6))|| ((cloud[0].y >= jet.top) && (cloud[0].y <= jet.top + 6))) {//3가지 경우
	//	energy = -3;
	//	cloud[0].x = 100;
	//	}
	//}

}
//SYSTEM
void printFile(char* filename, int color_num)//커서 위치도 인자로 받기, 움직이면 커서 위치를 갱신해줘야 함으로
{
	FILE* file;

	//setColor(color_num);
	int c;
	char line[1000];
	char* copyline;
	file = fopen("cloud.txt", "r");
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
		while (1) {
			printf("Failed");
		}

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
	SetConsoleCursorInfo(Screen[1], &cci);//첫번째 인자가 화면 버퍼를 의미

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