#pragma once

struct Map_Block
{
	bool exist = false;
	float height = 0;

	//ÁÂÇ¥
	float x = 0.0;
	float y = 0.0;
	float z = 0.0;

	//Å©±â
	float x_scale = 1.0;
	float y_scale = 2.0;
	float z_scale = 1.0;

};

static Map_Block Block[30][30];

void Setup_Block();
