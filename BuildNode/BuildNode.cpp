#include <stdio.h>
#include <conio.h>

#define MAX_CV_NUM	2
#define MAX_MV_NUM	2
#define MAX_DV_NUM  1
#define MAX_GV_NUM  1

#include "DynDBio.h"

void main()
{
	if(!DynInitialize())
	{
		printf("Can not initialize the FIX!\n");
		getch();
		return;
	}

	if(Create_Blocks())
		printf("Blocks created!\n");
	else
		printf("Blocks can not be built!\n");

	
	getch();
}