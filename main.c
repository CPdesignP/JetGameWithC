#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>




void Clear(void) // ȭ���� �����ְ� Ŀ���� ��ġ�� 0,0 ���� �̵���Ű�� �Լ�
{
	system("cls");
}



void GotoXY(int _x, int _y) // Ŀ���� ��ġ�� _x, _y �� �����ϴ� �Լ�
{
	COORD pos = { _x, _y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


#define MAX 30//�� ����� ����



void setColor(int num) 
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), num);
}

void printFile(const char* filename, int color_num)//���� �б�, ���ֱ�
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
	typedef struct st_enemy //����ü ���� �� �ʱ�ȭ
	{
		int x;//���� x��ǥ
		int y;//���� y��ǥ
		bool bActive; //���� ���̰�,�Ⱥ��̰� ��Ÿ���� �ñ� ����
	} ST_ENEMY;

	ST_ENEMY enemy[MAX] = { 0,0,false};

	

	srand(time(NULL));//�������� ������� ������ 

	int i;
	while (1)
	{
		Clear();

		for (i = 0; i < MAX; i++)
		{
			if (!enemy[i].bActive)//������� ��Ÿ���� ����
			{
				enemy[i].y = (rand() %15 *2);//������� ��ġ ����(ũ�⿡ �°� ������ ����)
				enemy[i].x = 100;
				enemy[i].bActive = true;
				break;//������� 1���� ������ ���� Ż��
			}
		}

		for (i = 0; i < MAX; i++)
		{
			if (enemy[i].bActive)//������� ����
			{
				GotoXY(enemy[i].x, enemy[i].y);//������� ��ġ �̵�
				printf("<��");
				enemy[i].x--;//�������� 
				if (enemy[i].x < 5)//������ ������
				{
					enemy[i].bActive = false;
				}
			}
		}

		GotoXY(45, 35);
		printf("qŰ�� �Է��ϸ� ������ ����˴ϴ�.");

		if (_kbhit())//q�� �Է��� ������ �����Ų��.
		{
			if (_getch() == 'q')
			{
				Clear();
				printFile("C:\\Users\\kebkb\\Desktop\\�� ����\\JetGame\\JetGame\\end_title.txt", 14);
				Sleep(1000);//1�� ����
				break;
			}
		}




	}

	return 0;

	}