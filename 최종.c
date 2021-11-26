#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>


void gamemain();
void gameover();
void Score();
void Clear();
void GotoXY(int _x, int _y);
void setColor(int num);
void printFile(const char* filename, int color_num);
#define MAX 30//적 비행기 개수


typedef struct st_enemy //구조체 정의 후 초기화
{
	int x;//적기 x좌표
	int y;//적기 y좌표
	bool bActive; //적기 보이게,안보이게 나타나는 시기 조정
} ST_ENEMY;

ST_ENEMY enemy[MAX] = { 0,0,false };

void Clear(void) // 화면을 지워주고 커서의 위치를 0,0 으로 이동시키는 함수
{
	system("cls");
}



void GotoXY(int _x, int _y) // 커서의 위치를 _x, _y 로 변경하는 함수
{
	COORD pos = { _x, _y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}



void setColor(int num) 
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), num);
}

void printFile(const char* filename, int color_num)//파일 읽기, 색넣기
{
	FILE* file;
	setColor(color_num);
	char line[100];
	char* copyline;
	file = fopen(filename, "r");

	

	if (file != NULL) 
	{
		int i = 4;
		while (!feof(file)) 
		{
			GotoXY(30, i);
			copyline = fgets(line, sizeof(line), file);
		
			printf("%s", copyline);
			++i;
		}
		setColor(15);
		fclose(file);
	}
	
}

void gameover(void)//게임종료화면
{
		Clear();
		printFile("C:\\Users\\kebkb\\Desktop\\새 폴더\\JetGame\\JetGame\\end_title.txt", 14);
		GotoXY(45, 20);
		printf("다시하기:y");
		GotoXY(45, 23);
		printf("나가기:n");
		while (1)
		{

			if (_kbhit())//y키눌러서 게임메인으로
			{
				if (_getch() == 'Y' || _getch() == 'y')
				{
					
					gamemain();
					
				}
			}
			if (_kbhit())//n키누르면 게임종료
			{
				if (_getch() == 'n'||_getch()=='N')
				{

					break;
				}
			}
			
		}
}


int score = 90;//점수 100점 만들기 위해
void Score()
{
	int i;
	for (i = 0; i < MAX; i++)
	{
		if (enemy[i].bActive)
			score++;
	}
	Sleep(50);
	
}


void gamemain()
{
	srand(time(NULL));//랜덤으로 적비행기 나오기 

	int i;
	while (1)
	{
		Clear();

		for (i = 0; i < MAX; i++)
		{
			if (!enemy[i].bActive)//적비행기 나타나는 조건
			{
				enemy[i].y = (rand() % 15 * 2);//적비행기 위치 조절(크기에 맞게 나머지 조절)
				enemy[i].x = 100;
				enemy[i].bActive = true;
				break;//적비행기 1개씩 생성을 위해 탈출
			}
		}

		for (i = 0; i < MAX; i++)
		{
			if (enemy[i].bActive)//적비행기 생성
			{
				GotoXY(enemy[i].x, enemy[i].y);//적비행기 위치 이동
				printf("<ㅔ");
				enemy[i].x--;//왼쪽으로 
				if (enemy[i].x < 5)//어디까지 갈건지
				{
					enemy[i].bActive = false;
				}
			}
		}

		Score();
		

		GotoXY(45, 35);
		printf("q키를 입력하면 게임이 종료됩니다.");

		if (_kbhit())//q를 입력해 게임을 종료시킨다.
		{
			if (_getch() == 'q')
			{
				Clear();
				printFile("C:\\Users\\kebkb\\Desktop\\새 폴더\\JetGame\\JetGame\\end_title.txt", 14);
				
				break;
			}
		}

		if (score == 100)//100점되면 적비행기들 거짓만들어 리셋시키고 게임종료화면으로
		{
			for (int i = 0; i < MAX; i++)
				enemy[i].bActive = false;
			gameover();
			break;
		}
		
		

		
	}
}



int main()
{
	while (1)
	{
		gamemain();
		break;
	}
	
	return 0;


}