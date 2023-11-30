#include "Collision.h"

void Setup_Block()
{
	for (int i = 0; i < 30; ++i) {
		for (int j = 0; j < 30; j++)
		{
			Block[i][j].x = -29 + j * 2;
			Block[i][j].z = -29 + i * 2;
		}
	}

	// 테두리 벽
	for (int i = 0; i < 30; i++)
	{
		Block[0][i].exist = true;
		Block[i][0].exist = true;

		Block[29][i].exist = true;
		Block[i][29].exist = true;
	}
}

