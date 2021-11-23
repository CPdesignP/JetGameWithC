#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>




void Clear(void) // 화면을 지워주고 커서의 위치를 0,0 으로 이동시키는 함수
{
	system("cls");
}



void GotoXY(int _x, int _y) // 커서의 위치를 _x, _y 로 변경하는 함수
{
	COORD pos = { _x, _y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


#define MAX 30//적 비행기 개수



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


int main()
{
	typedef struct st_enemy //구조체 정의 후 초기화
	{
		int x;//적기 x좌표
		int y;//적기 y좌표
		bool bActive; //적기 보이게,안보이게 나타나는 시기 조정
	} ST_ENEMY;

	ST_ENEMY enemy[MAX] = { 0,0,false};

	

	srand(time(NULL));//랜덤으로 적비행기 나오기 

	int i;
	while (1)
	{
		Clear();

		for (i = 0; i < MAX; i++)
		{
			if (!enemy[i].bActive)//적비행기 나타나는 조건
			{
				enemy[i].y = (rand() %15 *2);//적비행기 위치 조절(크기에 맞게 나머지 조절)
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

		GotoXY(45, 35);
		printf("q키를 입력하면 게임이 종료됩니다.");

		if (_kbhit())//q를 입력해 게임을 종료시킨다.
		{
			if (_getch() == 'q')
			{
				Clear();
				printFile("C:\\Users\\kebkb\\Desktop\\새 폴더\\JetGame\\JetGame\\end_title.txt", 14);
				Sleep(1000);//1초 쉰다
				break;
			}
		}




	}

	return 0;

	}