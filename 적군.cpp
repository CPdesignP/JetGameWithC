#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "console.h"

#define MAX 20

struct ST_ENEMY
{
	int x = 0;
	int y = 0;
	bool bActive = false;
};

int main()
{
	ST_ENEMY stEnemy[MAX];

	srand(time(NULL));

	int i;
	int num = 0;
	while (num<3)
	{
		Clear();

		for (i = 0; i < MAX; i++)
		{
			if (!stEnemy[i].bActive)
			{
				stEnemy[i].x = (rand() % 20 * 2);
				stEnemy[i].y = 0;
				stEnemy[i].bActive = true;
				break;
			}
		}

		for (i = 0; i < MAX; i++)
		{
			if (stEnemy[i].bActive)
			{
				GotoXY(stEnemy[i].x, stEnemy[i].y);
				printf("=.=");
				stEnemy[i].y++;
				if (stEnemy[i].y > 28)
				{
					stEnemy[i].bActive = false;
				}
			}
		}
		++num;
	}

	return 0;

}