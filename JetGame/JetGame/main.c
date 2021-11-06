#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<Windows.h>
#include<string.h>
//������ �ý����� �ܼ�â�� �����ϴ� �Լ��� �ִ� console API���
typedef struct jet {
	int x;
	int y;
}Jet;
void setColor(int num);
void printJet();
void printFrame();
void getCursor(int* x, int* y);
void getWindowSize(int* width, int* height);
void moveCursor(int x, int y);
void screenClear();
void main_screen();

void printFile();

//��Ʈ��� ���� ������� ���� ����� �Ѵ�...?
int main() {
	system("mode con cols=110 lines=35");//�ܼ�â ũ�� ������Ű��
	//make jetGame
	//main ȭ�� �����
	while (1) {
		main_screen();
	}
	
	return 0;
}
void setColor(int num) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), num);
}
//*��Ʈ�� �����̴� ���*
//Ŀ���� �����̰� �׸���.
//windowswindows.h ����� �����ϴ� SetConsoleCursorPosition ���� ��ǥ�� �̵�
//-------��
//|
//|
//��
void printFrame() {
	moveCursor(10, 2);
	int width, height;
	getWindowSize(&width, &height);

	for (int i = 0; i < width - 20; i++)
		printf("*");
	moveCursor(10, 3);
	for (int i = 4; i < height - 5; i++) {
		moveCursor(10, i-1);
		printf("*");
		moveCursor(width - 11, i - 1);
		printf("*");
	}
	moveCursor(10, height - 6);
	for (int i = 0; i < width - 20; i++)
		printf("*");
}
void getCursor(int* x, int* y) {//���� Ŀ���� ��ġ, â ũ�⸦ �޾ƿ�
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
void screenClear() {//ȭ�� ��ȯ �� ȭ���� ����� �Լ�
	system("cls");
	moveCursor(0, 0);
}
void main_screen() {
	screenClear();
	printFrame();
	printFile("main_title.txt", 14);
	Sleep(1000);
	int x, y, width, height;
	getCursor(&x, &y);
	getWindowSize(&width, &height);
	
	//printf("���� Ŀ���� ��ġ:(%d,  %d), �ܼ�â ũ��:(%d, %d)\n", x, y, width, height);
	char answer[100];
	moveCursor(45, 20);
	printf("\"����\"�� �Է��ϼ���");

	moveCursor(45, 22);
	printf("Input>>");
	scanf("%s", answer);
	if (answer == NULL)
		return;
	if (strcmp(answer, "����") == 0) {
		screenClear();
		printFrame();
		moveCursor(30, 10);
		printFile("jet.txt", 11);
		Sleep(10000);//���߱�
	}
}

void printFile(char* filename, int color_num)//Ŀ�� ��ġ�� ���ڷ� �ޱ�, �����̸� Ŀ�� ��ġ�� ��������� ������
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
