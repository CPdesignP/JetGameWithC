#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<Windows.h>
#include<string.h>
#include<conio.h>

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80

//윈도우 시스템의 콘솔창을 조절하는 함수가 있는 console API사용
typedef struct jet {
	int start;
	int bottom;
}Jet;
Jet jet;//전투기의 top, bottom위치 저장하는 구조체를 전역변수로 지정해주기
void setColor(int num);
void getCursor(int* x, int* y);
void getWindowSize(int* width, int* height);
void moveCursor(int x, int y);
void screenClear();
void main_screen();
void printFile(char* filename, int color_num);
void printJet(int x, int y);//인자값을 전달 받기도 하고, 거꾸로 외부로 전달을 할 것이다. 
void moveJet();

//제트기는 파일 입출력을 통해 출력을 한다...?
int main() {
	system("mode con cols=110 lines=35");//콘솔창 크기 고정시키기
	//make jetGame
	//main 화면 만들기
	while (1) {
		main_screen();
	}
	return 0;
}

void setColor(int num) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), num);
}
//*제트기 움직이는 방법*
//커서를 움직이고 그린다.
//windowswindows.h 헤더에 존재하는 SetConsoleCursorPosition 으로 좌표를 이동
//-------▶
//|
//|
//▼

void getCursor(int* x, int* y) {//현재 커서의 위치, 창 크기를 받아옴
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	*x = info.dwCursorPosition.X;
	*y = info.dwCursorPosition.Y;
}
void getWindowSize(int* width, int* height) {
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	*width = info.dwSize.X;
	*height = info.srWindow.Bottom - info.srWindow.Top + 1;
}
void moveCursor(int x, int y) {
	COORD position = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}
void screenClear() {//화면 전환 시 화면을 지우는 함수
	system("cls");
	moveCursor(10, 2);
	int width, height;
	getWindowSize(&width, &height);

	for (int i = 0; i < width - 20; i++)
		printf("*");
	moveCursor(10, 3);
	for (int i = 4; i < height - 5; i++) {
		moveCursor(10, i - 1);
		printf("*");
		moveCursor(width - 11, i - 1);
		printf("*");
	}
	moveCursor(10, height - 6);
	for (int i = 0; i < width - 20; i++)
		printf("*");
	moveCursor(0, 0);
}
void moveJet() {
	char c;
	while (1) {
		if (_kbhit()) {
			c = _getch();
			if (c == -32) {
				c = _getch();
				switch (c) {
				case LEFT:
					screenClear();
					printJet(jet.start - 1, jet.bottom);
					moveCursor(0, 0);
					printf("왼쪽으로 1만큼 이동:(%d, %d)",jet.start, jet.bottom );
					break;
				case RIGHT:
					screenClear();
					printJet(jet.start + 1, jet.bottom);
					moveCursor(0, 0);
					printf("오른쪽으로 1만큼 이동:(%d, %d)", jet.start, jet.bottom);
					break;
				case UP:
					screenClear();
					printJet(jet.start, jet.bottom - 1);
					moveCursor(0, 0);
					printf("위로 1만큼 이동:(%d, %d)", jet.start, jet.bottom);
					break;
				case DOWN:
					screenClear();
					printJet(jet.start, jet.bottom + 1);
					moveCursor(0, 0);
					printf("아래로 1만큼 이동:(%d, %d)", jet.start, jet.bottom);
					break;
				default:
					moveCursor(0, 0);
					printf("잘못된 키를 누름");
					return;
				}
			}
		}
	}
}
void main_screen() {
	screenClear();
	printFile("main_title.txt", 14);
	Sleep(1000);
	int x, y, width, height;
	getCursor(&x, &y);
	getWindowSize(&width, &height);
	
	//printf("현재 커서의 위치:(%d,  %d), 콘솔창 크기:(%d, %d)\n", x, y, width, height);
	char answer[100];
	moveCursor(45, 20);
	printf("\"시작\"을 입력하세요");

	moveCursor(45, 22);
	printf("Input>>");
	scanf("%s", answer);
	if (answer == NULL)
		return;
	if (strcmp(answer, "시작") == 0) {
		screenClear();
		moveCursor(30, 10);
		printJet(30, 10);
		moveJet();
		Sleep(10000);//멈추기
	}
}
void printJet(int x, int y) {
	FILE* file;
	setColor(11);
	int c;
	char line[1000];
	char* copyline;
	file = fopen("jet.txt", "r");
	
	jet.start = x;
	jet.bottom = y;//함수를 빠져나가도 제대로 데이터가 유지

	if (file != NULL) {
		int i = y;
		while (!feof(file)) {
			moveCursor(x, i);
			copyline = fgets(line, sizeof(line), file);
			printf("%s", copyline);
			++i;
		}
		setColor(15);
		fclose(file);
	}
	else {
		printf("Failed");
		return -1;
	}
}
void printFile(char* filename, int color_num)//커서 위치도 인자로 받기, 움직이면 커서 위치를 갱신해줘야 함으로
{
	FILE* file;
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
			printf("%s", copyline);
			++i;
		}
		setColor(15);
		fclose(file);
	}
	else {
		printf("Failed");
		return -1;
	}
}
