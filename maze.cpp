#include <iostream>
#include<stack>
#include <time.h>
#include <Windows.h>
#include <stdlib.h>
#include <fstream>
#include <istream>
#include <ostream>
#include <stdio.h>
#include <conio.h>
using namespace std;
#define M 200
int COUNT = 0;
int maze[2*M][2*M];
void maze_Init(int m, int n);
void game2(int h, int l);
void game8(int h, int l);
void print(int a, int b,int rux,int ruy,int chux,int chuy);
void find8road(int h, int l);
void print8(int a, int b);
bool fullstacki(int stacki[][7], int top);
bool stackidir(int stacki[][7], int dirstacki, int top);


void randdir(int *dir,int *dirx, int *diry) {
	*dir = rand() % 4 + 1;//从入口开始随机产生一个方向进行
	//cout << *dir << "  ";
	if (*dir == 1) {
		*dirx = -1; *diry = 0;
	}
	else if (*dir == 2) {
		*dirx = 0; *diry = -1;
	}
	else if (*dir == 3) {
		*dirx = 1; *diry = 0;
	}
	else if (*dir == 4) {
		*dirx = 0; *diry = 1;
	}
}

/*  定义方向
    (x,y)  (0,0)  0
	(x-1,y) (-1,0)1
	(x,y-1) (0,-1)2
	(x+1,y) (1,0) 3
	(x,y+1) (0,1) 4
	
	-1 边界
	1  墙
	0  通路
	坐标原点maze[2][2]
	*/

void maze_Init(int m,int n)/*传入参数m为迷宫地图行数
						             n为迷宫地图列数
							 将地图初始化为指定形式*/
{
	for (int i = 0; i < n + 4; i++)
		maze[0][i] = -1;
	for (int i = 0; i < n + 4; i++)
		maze[1][i] = -1;
	for (int i = 0; i < n + 4; i++)
		maze[m + 2][i] = -1;
	for (int i = 0; i < n + 4; i++)
		maze[m + 3][i] = -1;

	for (int i = 2; i < m + 2; i++)
		maze[i][0] = -1;
	for (int i = 2; i < m + 2; i++)
		maze[i][1] = -1;
	for (int i = 2; i < m + 2; i++)
		maze[i][n + 2] = -1;
	for (int i = 2; i < m + 2; i++)
		maze[i][n + 3] = -1;

	for(int i=2;i<2+m;i++)
		for (int j = 2; j < 2 + n; j++)
		{
			if (i % 2 == 0 && j % 2 == 0)
				maze[i][j] = 0;
			else if (i % 2 != 0 && j % 2 != 0)
				maze[i][j] = -1;
			else maze[i][j] = 1;
		}
}



void maze_Create(int m, int n) {//m行数  n列数
	maze_Init(m, n);
	//print8(m, n); system("pause");
	int xin = 2, yin = 2;//入口坐标
	int xout = n + 1, yout = m + 1;//出口坐标
	//int xout = 2, yout = 2;
	maze[xout][yout] = -3;
	int  dirxxout, diryyout;//记录移动方向
	srand(time(NULL));
	int  directnowout;//方向   
	/*随机产生1到4的自然数
	  分别代表四个方向*/
	  /*for (int i = 0; i < 20; i++) {
		  directnow = rand() % 4 + 1;
		  cout << directnow << endl;
	  }*/
	  /*从入口开始挖，经过的通路标志为-2
		从出口开始挖，经过的通路标志为-3
		设置局部标志，记录是否联通
		*/
	bool in_out = false;//初始标志为false 表示未联通
	int outstackshuzu[20 * M][7];
	int rodefromout[20 * M][2];
	int outstacktop = 1;
	outstackshuzu[1][1] = 0;
	outstackshuzu[1][2] = 0;
	outstackshuzu[1][3] = 1;
	outstackshuzu[1][4] = 1;
	outstackshuzu[1][5] = xout;
	outstackshuzu[1][6] = yout;



	
	
		/*abc++; cout << "a" << COUNT << "  ";
		x2:if (!stack_full(inflag)) {
		x1:if (!stack_full(in)) {//如果四个方向没有探索一遍
			randdir(&directnowin, &dirxxin, &diryyin);
			//cout << directnowin << "  ";
			//判断这个方向探索过没
			if (!stack_dir(in, directnowin))//如果没有探索过
			{
				//cout << directnowin << "  ";
				stack_pushdir(in, directnowin);
				//cout << directnowin << "  ";
				if (maze[xin + dirxxin][yin + diryyin] == 1)//如果是墙
				{
					//判断墙的另一边
					if (maze[xin + 2 * dirxxin][yin + 2 * diryyin] == 0) {
						//如果未挖通
						maze[xin + dirxxin][yin + diryyin] = -2;//从入口挖通标记未-2
						xin = xin + 2 * dirxxin; yin = yin + 2 * diryyin;
						stack_push(in, xin, yin);
						stack_pushdir(in, (directnowin + 2) % 4);//将该方向入栈
					}
					else if (maze[xin + 2 * dirxxin][yin + 2 * diryyin] == -2) {
						//如果已从入口挖通
						goto x1;
					}
					else if (maze[xin + 2 * dirxxin][yin + 2 * diryyin] == -3) {//从出口挖通标记-3
						if (!in_out) {//如果未联通
							maze[xin + dirxxin][yin + diryyin] = -2;
							in_out = true;
							goto x1;
						}
					}
				}
				else if (maze[xin + dirxxin][yin + diryyin] == -1) {
					goto x1;
				}
			}
			else if (stack_dir(in, directnowin)) { goto x1; }
		}//cout << "a" << COUNT << "  ";
		else if(stack_full(in)){
			//in->top = in->top->next;
			//cout << in->top->xy[0] << "a  ";
			stack_pop(in);
			//if (in->top == NULL)goto x2;
			//else goto x1;
			//cout << in->top->xy[0] << "b  ";
		}
	}*/


out1:	while (outstackshuzu[1][1] == 0 || outstackshuzu[1][2] == 0) {//如果四个方向没有探索一遍
	//print(m, n); system("cls");
	randdir(&directnowout, &dirxxout, &diryyout);// print(m, n);
	//判断这个方向探索过没
	//如果没有探索过
	
	if (!stackidir(outstackshuzu, directnowout, outstacktop))//(outstackshuzu[outstacktop][directnowout] == 0)
	{
		outstackshuzu[outstacktop][directnowout] = 1;

		if (maze[xout + dirxxout][yout + diryyout] == 1)//如果是墙
		{
			//判断墙的另一边
			if (maze[xout + 2 * dirxxout][yout + 2 * diryyout] == 0) {
				//如果未挖通
				maze[xout + dirxxout][yout + diryyout] = -3;//从出口挖通标记未-3

				xout = xout + 2 * dirxxout; yout = yout + 2 * diryyout;
				if (xout == xin&&yout == yin)
				{
					for (int i = 1; i <= outstacktop; i++)
					{
						rodefromout[i][0] = outstackshuzu[i][5];
						rodefromout[i][1] = outstackshuzu[i][6];
						
					}
					rodefromout[0][0] = outstacktop;
				}
				maze[xout][yout] = -3;
				//stack_push_xy(out, &xout, &yout);
				outstacktop++;
				for (int i = 0; i < 7; i++)outstackshuzu[outstacktop][i] = 0;
				outstackshuzu[outstacktop][5] = xout;
				outstackshuzu[outstacktop][6] = yout;
				//int directnowoutstemp = (directnowout + 2) % 4;
				outstackshuzu[outstacktop][(directnowout + 2) % 4] = 1;

			}
			if (maze[xout + 2 * dirxxout][yout + 2 * diryyout] == -2) {//从出口挖通标记-3
				if (in_out == false) {//如果未联通
					maze[xout + dirxxout][yout + diryyout] = -3;
					in_out = true;
				}
			}
		}
	}
	/*if (outstackshuzu[outstacktop][1] == 1 && outstackshuzu[outstacktop][2] == 1 && outstackshuzu[outstacktop][3] == 1 && outstackshuzu[outstacktop][4] == 1) {
		outstacktop--;
		xout = outstackshuzu[outstacktop][5];
		yout = outstackshuzu[outstacktop][6];
		if (outstacktop == 1)
			goto out1;
	}*/
	if (maze[xout + 2 * dirxxout][yout + 2 * diryyout] == -3)
	{
		outstackshuzu[outstacktop][directnowout] = 1;
	}
	if (fullstacki(outstackshuzu, outstacktop))
	{
		if (outstacktop == 1)
		{

			goto out1;
		}
		else
		{
			outstacktop--;
			xout = outstackshuzu[outstacktop][5];
			yout = outstackshuzu[outstacktop][6];
		}
	}
}
		for(int i=0;i<m+4;i++)
			for (int j = 0; j < n + 4; j++)
			{
				if (maze[i][j] != 1 && maze[i][j] != -1)
					maze[i][j] = 0;
			}
		for (int i = 1; i <= rodefromout[0][0]; i++)
		{
			
			maze[rodefromout[i][0]][rodefromout[i][1]] = 10;
		}
		maze[xin][yin] = 10;
		for(int i=2;i<m+2;i++)
			for (int j = 2; j < n + 2; j++)
			{
				if (maze[i][j] == 0)
				{
					if (maze[i - 1][j] == 10 && maze[i + 1][j] == 10)
						maze[i][j] = 10;
					else if (maze[i][j - 1] == 10 && maze[i][j + 1] == 10)
						maze[i][j] = 10;
				}
			}
}
void print(int a,int b,int rux,int ruy,int chux,int chuy) {
	for (int i = 0; i < a+4; i++) {
		for (int j = 0; j <b+4; j++)
		{
			if( (i == rux&&j == ruy - 1)||(i==chux&&j==chuy))
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
				cout << "  ";
			}
			else {
				if (maze[i][j] == -1 || maze[i][j] == 1) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
					printf("※");
				}
				else
					printf("  ");//两个空格
			}
		}
		cout << endl;
	}
}

void printroad(int a, int b) {
	for (int i = 0; i < a + 4; i++) {
		for (int j = 0; j <b + 4; j++)
		{
			if (maze[i][j] == -1 || maze[i][j] == 1) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				cout << "※";
			}
			else if (maze[i][j]==10)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
				cout << "※";
			}
			else
				cout << "  ";//两个空格
		}
		cout << endl;
	}
}
void print8road(int a, int b) {
	for (int i = 0; i < a + 4; i++) {
		for (int j = 0; j <b + 4; j++)
		{
			if ( (i == 2 && j == 1) || (i == 2 && j == b + 2) )
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
				cout << "  ";
			}
			else if ( (i == a + 1 && j == 1) || (i == a + 1 && j == b + 2))
			{
				cout << "  ";
			}else

			{if (maze[i][j] == -1 || maze[i][j] == 1) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				cout << "※";
			}
			else if (maze[i][j] == 11)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
				cout << "※";
			}
			else if (maze[i][j] == 12)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				cout << "※";
			}
			else
				cout << "  ";//两个空格
			}
		}
		cout << endl;
	}
}
void print8(int a, int b)
{
	for (int i = 0; i < a + 4; i++) {
		for (int j = 0; j <b + 4; j++)
		{
			if ((i == 2&&j == 1) || (i == 2&&j == b+2) )
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
				cout << "  ";
			}
			else if ( (i == a+1&&j == 1) || (i == a+1&&j == b+2) )
			{
				cout << "  ";
			}
			else {
				if (maze[i][j] == -1 || maze[i][j] == 1) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
					printf("※");
				}
				else
					printf("  ");//两个空格
			}
		}
		cout << endl;
	}
}
void maze_rode(int xin1, int yin1, int xout1, int yout1) {
	int mazefindrode[20 * M][7];
	int rodeflag = 1;
	int findroaddir = 0;
	int findroaddirx = 0, findroaddiry = 0;
	//增加一个判断入口类型
	//初始化入口
	for (int i = 0; i < 7; i++)mazefindrode[1][i] = 0;
	mazefindrode[1][5] = xin1;
	mazefindrode[1][6] = yin1;
	maze[xin1][yin1] = 10;
	while (xin1 != xout1||yin1 != yout1) {
		printroad(xout1-1, yout1-1);
		randdir(&findroaddir, &findroaddirx, &findroaddiry);
		if (maze[xin1 + findroaddirx][yin1 + findroaddiry] != 1 && maze[xin1 + findroaddirx][yin1 + findroaddiry] != -1)//首先判断这个方向是不是通路
		{
			if (maze[xin1 + findroaddirx][yin1 + findroaddiry]!=10&& maze[xin1 + findroaddirx][yin1 + findroaddiry]!=-10)
			{
				maze[xin1 + findroaddirx][yin1 + findroaddiry] = 10;
				xin1 = xin1 + 2 * findroaddirx;
				yin1 = yin1 + 2 * findroaddiry;
				rodeflag++;
				for (int i = 0; i < 7; i++)mazefindrode[rodeflag][i] = 0;
				
				mazefindrode[rodeflag][5] = xin1;
				mazefindrode[rodeflag][6] = yin1;
				int finddirectstemp = (findroaddir + 2) % 4;
				mazefindrode[rodeflag][finddirectstemp] = 1;
				maze[xin1][yin1] = 10;
			}
		}
		else
		{
			mazefindrode[rodeflag][findroaddir] = 1;
		}
		if (mazefindrode[rodeflag][1] == 1 && mazefindrode[rodeflag][2] == 1 && mazefindrode[rodeflag][3] == 1 && mazefindrode[rodeflag][4] == 1)
		{
			if (rodeflag == 1)
			{
				cout << "没有通路" << endl;
			}
			else
			{
				maze[xin1][yin1] = -10;
				rodeflag--;
				xin1 = mazefindrode[rodeflag][5];
				yin1 = mazefindrode[rodeflag][6];
				
				
					if (maze[xin1 - 1][yin1] != 1 && maze[xin1 - 1][yin1] != -1)
					{
						if (maze[xin1 - 2][yin1] == -10)
							maze[xin1 - 1][yin1] = -10;
					}
					else if (maze[xin1][yin1 - 1] != 1 && maze[xin1][yin1-1]!=-1) {
						if (maze[xin1][yin1 - 2] == -10)
							maze[xin1][yin1 - 1] = -10;
					}
					else if (maze[xin1 + 1][yin1] != 1 && maze[xin1 + 1][yin1] != -1) {
						if (maze[xin1 + 2][yin1] == -10)
							maze[xin1 + 1][yin1] = -10;
					}
					else if (maze[xin1][yin1 + 1] != 1 && maze[xin1][yin1 + 1] != -1) {
						if (maze[xin1][yin1 + 2] == -10)
							maze[xin1][yin1 + 1] = -10;
					}
					
				

			}
		}
	}
}
/*void FullScreen() {
	HWND hwnd = GetForegroundWindow();
	int x = GetSystemMetrics(SM_CXSCREEN) + 3000;
	int y = GetSystemMetrics(SM_CYSCREEN) + 3000;
	char setting[30];

	sprintf_s(setting, "mode con:cols=%d lines=%d", x, y);
	system(setting);
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, x + 3000, y + 3000, NULL);
	MoveWindow(hwnd, -10, -40, x + 3000, y + 3000, 1);
}*/

void maze_8_dir(int hang, int lie) {
	int un[9][9];
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			un[i][j] = 0;//八个方向的关系矩阵
	//从八个方向生成迷宫
	/*八个方向依次挖过的依次标记为11，12，13，，，，，18
	   -1为边界
	   1 为墙壁*/
	int stack1[20 * M][7], stack2[20 * M][7],  stack6[20 * M][7], stack8[20 * M][7];

	maze_Init(hang, lie); //print(hang, lie); system("pause");
	int x0 = 2, y0 = 2;//左上角坐标
	int xend = lie + 1, yend = hang + 1;//右下角坐标
	//maze[x0][y0] = 10; maze[yend][xend] = 10;
	//printroad(hang, lie); system("pause");
	//定义探索的八个坐标位置
	int x1 = x0, y1 = y0;
	int x2 = x0, y2 = yend;
	int x6 = xend, y6 = y0;
	int x8 = xend, y8 = yend;
	//起点入栈并初始化
	int top1 = 1, top2 = 1,  top6 = 1, top8 = 1;
	stack1[1][5] = x1; stack1[1][6] = y1;
	stack2[1][5] = x2; stack2[1][6] = y2;
	stack6[1][5] = x6; stack6[1][6] = y6;
	stack8[1][5] = x8; stack8[1][6] = y8;
	for (int i = 0; i < 7; i++)
	{
		stack1[0][i] = 0;
		stack2[0][i] = 0;
		stack6[0][i] = 0;
		stack8[0][i] = 0;
	}
	for (int i = 1; i < 5; i++)
	{
		stack1[1][i] = 0;
		stack2[1][i] = 0;
		stack6[1][i] = 0;
		stack8[1][i] = 0;
	}
	int rand8dir = 0, rand8dirx = 0, rand8diry = 0;
	maze[x1][y1] = 11;
	maze[x2][y2] = 12;
	maze[x6][y6] = 16; 
	maze[x8][y8] = 18;
	int temptemp; 
	int tempcount1 = 0, tempcount2 = 0,  tempcount6 = 0,  tempcount8 = 0;
	while (!(tempcount1==1&&tempcount2==1&&tempcount6==1&&tempcount8==1))//|| !fullstacki(stack4, 1) || !fullstacki(stack5, 1) || !fullstacki(stack6, 1) || !fullstacki(stack7, 1) || !fullstacki(stack8, 1))
	{ 
	 //成功
		if (!fullstacki(stack1, 1))
		{
			randdir(&rand8dir, &rand8dirx, &rand8diry);
			if (!stackidir(stack1, rand8dir, top1))//判断给方向是否探索过
			{
				stack1[top1][rand8dir] = 1;//yeah
				if (maze[x1+rand8dirx][y1+rand8diry]==1)//判断该方向是否为墙
				{
					if(maze[x1+2*rand8dirx][y1+2*rand8diry]==0)
					{
						maze[x1 + rand8dirx][y1 + rand8diry] = 11;
						maze[x1+ 2 * rand8dirx][y1 + 2 * rand8diry] = 11;
						
						x1 = x1 + 2 * rand8dirx;
						y1 = y1+ 2 * rand8diry;
						top1++;
						for (int i = 0; i < 7; i++)
							stack1[top1][i] = 0;
						stack1[top1][5] = x1;
						stack1[top1][6] = y1;
						stack1[top1][(rand8dir + 2) % 4] = 1;
					}
					else
					{
						temptemp = maze[x1+ 2 * rand8dirx][y1 + 2 * rand8diry];
						if (temptemp != 11 && un[1][temptemp-10] == 0 )
						{
							maze[x1 + rand8dirx][y1 + rand8diry] = 11;
							un[1][temptemp-10] = 1;
							un[temptemp - 10][1] = 1;
						}
					}
				}
				if (maze[x1 + 2 * rand8dirx][y1 + 2 * rand8diry] == 11 || maze[x1 + 2 * rand8dirx][y1 + 2 * rand8diry] == 12 || maze[x1 + 2 * rand8dirx][y1 + 2 * rand8diry] == 13 || maze[x1 + 2 * rand8dirx][y1 + 2 * rand8diry] == 14 || maze[x1 + 2 * rand8dirx][y1 + 2 * rand8diry] == 15 || maze[x1 + 2 * rand8dirx][y1 + 2 * rand8diry] == 16 || maze[x1 + 2 * rand8dirx][y1 + 2 * rand8diry] == 17 || maze[x1 + 2 * rand8dirx][y1 + 2 * rand8diry] == 18)
				{
					stack1[top1][rand8dir] = 1;
				}
			}			
		}
		if (fullstacki(stack1, top1))
		{
			if (top1 == 1)tempcount1 = 1;
			else
			{
				top1--;
				x1 = stack1[top1][5];
				y1 = stack1[top1][6];
			}

		}
		if (fullstacki(stack1, 1)) { tempcount1 = 1; }
		//成功
		if (!fullstacki(stack2, 1))
		{
			randdir(&rand8dir, &rand8dirx, &rand8diry);
			if (!stackidir(stack2, rand8dir, top2))//判断给方向是否探索过
			{
				stack2[top2][rand8dir] = 1;
				if (maze[x2 + rand8dirx][y2 + rand8diry] == 1)//判断该方向是否为墙
				{
					if (maze[x2 + 2 * rand8dirx][y2 + 2 * rand8diry] == 0)
					{
						maze[x2 + rand8dirx][y2 + rand8diry] = 12;
						maze[x2 + 2 * rand8dirx][y2 + 2 * rand8diry] = 12;
						
						x2 = x2 + 2 * rand8dirx;
						y2 = y2 + 2 * rand8diry;
						top2++;
						for (int i = 0; i < 7; i++)
							stack2[top2][i] = 0;
						stack2[top2][5] = x2;
						stack2[top2][6] = y2;
						stack2[top2][(rand8dir + 2) % 4] = 1;
					}
					else
					{
						temptemp = maze[x2 + 2 * rand8dirx][y2 + 2 * rand8diry];
						if (temptemp != 12)
							// (un[2][temptemp-10] == 0 && un[temptemp-10][2] == 0))
							if(un[2][temptemp-10]==0)
						{
							maze[x2 + rand8dirx][y2 + rand8diry] = 12;
							un[2][temptemp-10] = 1;
							un[temptemp-10][2] = 1;
						}
					}
				}
			}
			if (maze[x2 + 2 * rand8dirx][y2 + 2 * rand8diry] == 11 || maze[x2 + 2 * rand8dirx][y2 + 2 * rand8diry] == 12 || maze[x2 + 2 * rand8dirx][y2 + 2 * rand8diry] == 13 || maze[x2 + 2 * rand8dirx][y2 + 2 * rand8diry] == 14 || maze[x2 + 2 * rand8dirx][y2 + 2 * rand8diry] == 15 || maze[x2 + 2 * rand8dirx][y2 + 2 * rand8diry] == 16 || maze[x2 + 2 * rand8dirx][y2 + 2 * rand8diry] == 17 || maze[x2 + 2 * rand8dirx][y2 + 2 * rand8diry] == 18)
			{
				stack2[top2][rand8dir] = 1;
			}
		}
		if (fullstacki(stack2, top2))
		{
			if (top2 == 1)
				tempcount2 = 1;
			else
			{
				top2--;
				x2 = stack2[top2][5];
				y2 = stack2[top2][6];
			}

		}
		if (fullstacki(stack2, 1))tempcount2 = 1;
		//成功
		if (!fullstacki(stack8, 1))
		{
			randdir(&rand8dir, &rand8dirx, &rand8diry);
			if (!stackidir(stack8, rand8dir, top8))//判断给方向是否探索过
			{
				stack8[top8][rand8dir] = 1;
				if (maze[x8 + rand8dirx][y8 + rand8diry] == 1)//判断该方向是否为墙
				{
					if (maze[x8 + 2 * rand8dirx][y8 + 2 * rand8diry] == 0)
					{
						maze[x8 + rand8dirx][y8 + rand8diry] = 18;
						maze[x8 + 2 * rand8dirx][y8 + 2 * rand8diry] = 18;

						x8 = x8 + 2 * rand8dirx;
						y8 = y8 + 2 * rand8diry;
						top8++;
						for (int i = 0; i < 7; i++)stack8[top8][i] = 0;
						stack8[top8][5] = x8;
						stack8[top8][6] = y8;
						stack8[top8][(rand8dir + 2) % 4] = 1;
					}
					else
					{
						temptemp = maze[x8 + 2 * rand8dirx][y8 + 2 * rand8diry];
						if (temptemp != 18 && (un[8][temptemp - 10] == 0 ))
						{
							maze[x8 + rand8dirx][y8 + rand8diry] = 18;
							un[8][temptemp - 10] = 1;
							un[temptemp - 10][8] = 1;
						}
					}
				}
			}
			if (maze[x8 + 2 * rand8dirx][y8 + 2 * rand8diry] == 11 || maze[x8 + 2 * rand8dirx][y8 + 2 * rand8diry] == 12 || maze[x8 + 2 * rand8dirx][y8 + 2 * rand8diry] == 13 || maze[x8 + 2 * rand8dirx][y8 + 2 * rand8diry] == 14 || maze[x8 + 2 * rand8dirx][y8 + 2 * rand8diry] == 15 || maze[x8 + 2 * rand8dirx][y8 + 2 * rand8diry] == 16 || maze[x8 + 2 * rand8dirx][y8 + 2 * rand8diry] == 17 || maze[x8 + 2 * rand8dirx][y8 + 2 * rand8diry] == 18)
			{
				stack8[top8][rand8dir] = 1;
			}

		}
		if (fullstacki(stack8, top8))
		{
			if (top8 == 1)tempcount8 = 1;
			else
			{
				top8--;
				x8 = stack8[top8][5];
				y8 = stack8[top8][6];
			}

		}
		if (fullstacki(stack8, 1))tempcount8 = 1;
		//成功
		if (!fullstacki(stack6, 1))
		{
		randdir(&rand8dir, &rand8dirx, &rand8diry);
		if (!stackidir(stack6, rand8dir, top6))//判断给方向是否探索过
		{
		stack6[top6][rand8dir] = 1;
		if (maze[x6 + rand8dirx][y6 + rand8diry] == 1)//判断该方向是否为墙
		{
		if (maze[x6 + 2 * rand8dirx][y6 + 2 * rand8diry] == 0)
		{
		maze[x6 + rand8dirx][y6 + rand8diry] = 16;
		maze[x6 + 2 * rand8dirx][y6 + 2 * rand8diry] = 16;

		x6 = x6 + 2 * rand8dirx;
		y6 = y6 + 2 * rand8diry;
		top6++;
		for (int i = 0; i < 7; i++)stack6[top6][i] = 0;
		stack6[top6][5] = x6;
		stack6[top6][6] = y6;
		stack6[top6][(rand8dir + 2) % 4] = 1;
		}
		else if(maze[x6 + 2 * rand8dirx][y6 + 2 * rand8diry] == 11 || maze[x6 + 2 * rand8dirx][y6 + 2 * rand8diry] == 12 || maze[x6 + 2 * rand8dirx][y6 + 2 * rand8diry] == 13 || maze[x6 + 2 * rand8dirx][y6 + 2 * rand8diry] == 14 || maze[x6 + 2 * rand8dirx][y6 + 2 * rand8diry] == 15 || maze[x6 + 2 * rand8dirx][y6 + 2 * rand8diry] == 16 || maze[x6 + 2 * rand8dirx][y6 + 2 * rand8diry] == 17 || maze[x6 + 2 * rand8dirx][y6 + 2 * rand8diry] == 18)
		{
		temptemp = maze[x6 + 2 * rand8dirx][y6 + 2 * rand8diry];
		if (temptemp != 16 && (un[6][temptemp-10] == 0))
		{
		maze[x6 + rand8dirx][y6 + rand8diry] = 16;
		un[6][temptemp-10] = 1;
		un[temptemp-10][6] = 1;
		}
		}
		}
		}
		if (maze[x6 + 2 * rand8dirx][y6 + 2 * rand8diry] == 11 || maze[x6 + 2 * rand8dirx][y6 + 2 * rand8diry] == 12 || maze[x6 + 2 * rand8dirx][y6 + 2 * rand8diry] == 13 || maze[x6 + 2 * rand8dirx][y6 + 2 * rand8diry] == 14 || maze[x6 + 2 * rand8dirx][y6 + 2 * rand8diry] == 15 || maze[x6 + 2 * rand8dirx][y6 + 2 * rand8diry] == 16 || maze[x6 + 2 * rand8dirx][y6 + 2 * rand8diry] == 17 || maze[x6 + 2 * rand8dirx][y6 + 2 * rand8diry] == 18)
		{
			stack6[top6][rand8dir] = 1;
		}
		}
		if (fullstacki(stack6, top6))
		{
			if (top6 == 1)tempcount6 = 1;
		else
		{
		top6--;
		x6 = stack6[top6][5];
		y6 = stack6[top6][6];
		}

		}
		if (fullstacki(stack6, 1))tempcount6 = 1;
	
//print(hang, lie); //system("pause");
	}
	//cout << "d" << endl;
	//print(hang, lie);
	//cout << tempcount1 << "  " << tempcount2 << "  " << tempcount3 << "  " << tempcount4 << "  " << tempcount5 << "  " << tempcount6 << "  " << tempcount7 << "  " << tempcount8 << endl;
	//system("pause");
	//该处重新整理地图
	for (int i = 2; i < hang + 1; i++)
		for (int j = 2; j < lie + 1; j++)
			if (maze[i][j] != 1 && maze[i][j] != -1)
				maze[i][j] = 0;
	//print(hang, lie);
	//system("pause");
}
bool stackidir(int stacki[][7],int dirstacki,int top)
{
	if (stacki[top][dirstacki] == 1)return true;
	else return false;
}

bool fullstacki(int stacki[][7],int top)//判断栈顶四个方向是否探索完毕
{
	//cout << stacki[top][1] << "  " << stacki[top][2];
	//system("pause");
	if (stacki[top][1] == 1 && stacki[top][2] == 1 && stacki[top][3] == 1 && stacki[top][4] == 1)
		return true;
	else return false;
}
void maze_more_in_out( ) {//多入口多出口，用户输入入口和出口个数 还有地图尺寸
	//从八个方向开始生成迷宫
	//入口为（2，2） 和（hang+1,2）   出口为 (2,lie+1）和（hang+1,lie+1）
	int maze8flag = 1;
	int hang, lie;
	//int rux, ruy;
	//int chux, chuy;
	while (maze8flag == 1) {
		system("cls");
		cout << " 请输入迷宫行数 和 列数(均不小于11)" << endl;
		cout << "行数："; cin >> hang; cout << endl;
		cout << "列数："; cin >> lie; cout << endl;
		if (hang < 11)hang = 11;
		if (lie < 11)lie = 11;
		if (hang % 2 == 0)hang++;
		if (lie % 2 == 0)lie++;
		maze_8_dir(hang, lie);
		for (int i = 0; i < hang + 4; i++)
			for (int j = 0; j < lie + 4; j++)
			{
				if (maze[i][j] != 1 && maze[i][j] != -1)
					maze[i][j] = 0;
			}
		print8(hang, lie);
		{
			ofstream fout8("maze_more_in_out.txt");
			if (fout8)
			{
				fout8 << hang << "  " << lie << "  " << endl;
				fout8 << 2 << "  " << 2 << "  " << endl;
				fout8 << hang + 1 << "  " << 2 << "  " << endl;
				fout8 << 2 << "  " << lie + 1 << "  " << endl;
				fout8 << hang + 1 << "  " << lie + 1 << "  " << endl;
				for (int i = 0; i < hang + 4; i++) {
					for (int j = 0; j < lie + 4; j++)
						fout8 << maze[i][j] << "   ";
					fout8 << endl;
				}
			}
			fout8.close();
		}
		//system("pause");
		//开始寻路
		//print8road(hang, lie); system("pause");
		cout << "   输入 4 直接打印路径"
			"   输入 3 开始游戏"
			"   输入 2 直接导出到文件"
			"   输入 1 继续"
			"   输入 0 返回" << endl;
		cin >> maze8flag;
		if (maze8flag == 3)
		{
			game8(hang, lie);
		}
		if (maze8flag == 2)
		{
			cout << "导出成功！" << endl;
			cout << "是否继续？继续请输入1   返回请输入0:  ";
			cin >> maze8flag;
		}
		if (maze8flag == 4)
		{
			cout << "打印路径" << endl;

			find8road(hang, lie);
			system("pause");
		}
		//if (maze8flag == 0);
			//goto end8;
		//system("pause");
		//cout << "输入数字 2 将地图导出到文件" << endl;
		//cout << "是否继续？继续请输入1   返回请输入其他数字:  ";
		//cin >> maze8flag;

	}
//end8:
}
void find8road(int h, int l)
{
	int find8road1[20 * M][7];
	int find8road2[20 * M][7];
	int xin1 = 2, yin1 = 2;//入口1
	int xin2 = h + 1, yin2 = 2;//入口2
	int xout1 = h + 1, yout1 = l + 1;//出口1
	int xout2 = 2, yout2 = l + 1;//出口2
	int findtop1 = 1, findtop2 = 1;
	for (int i = 0; i < 7; i++)
	{
		find8road1[1][i] = 0;
		find8road2[1][i] = 0;
	}
	find8road1[1][5] = xin1; find8road1[1][6] = yin1;
	find8road2[1][5] = xin2; find8road2[1][6] = yin2;
	find8road1[1][1] = 1; find8road1[1][2] = 1;
	//find8road2[1][1] = 1; find8road2[1][4] = 1;
	int findflag1 = 0; int findflag2 = 0;
	int finddir = 0, finddirx = 0, finddiry = 0;
	//print8road(h, l); system("pause");
	maze[xin1][yin1] = 11;
	find1:while (findflag1 != 1 )
	{
		//print8road(h, l);// system("pause");
			if (!fullstacki(find8road1, findtop1))
			{
				randdir(&finddir, &finddirx, &finddiry);
				if (!stackidir(find8road1, finddir, findtop1))
				{
					find8road1[findtop1][finddir] = 1;
					if (maze[xin1 + finddirx][yin1 + finddiry] != 1 && maze[xin1 + finddirx][yin1 + finddiry] != -1)
					{
						if (maze[xin1 + finddirx][yin1 + finddiry] != 11)
						{
							maze[xin1 + finddirx][yin1 + finddiry] = 11;
							maze[xin1 + 2 * finddirx][yin1 + 2 * finddiry] = 11;
							xin1 = xin1 + 2 * finddirx; yin1 = yin1 + 2 * finddiry;
							if (xin1 == xout1&&yin1 == yout1)
							{
								findflag1 = 1;
								goto find1;
							}
							findtop1++;
							for (int i = 0; i < 7; i++)
								find8road1[findtop1][i] = 0;
							find8road1[findtop1][5] = xin1;
							find8road1[findtop1][6] = yin1;
							find8road1[findtop1][(finddir + 2) % 4] = 1;
						}
					}
					
				}
			}
			if (fullstacki(find8road1, findtop1))
			{
				if (findtop1 == 1)
				{
					findflag1 = 1;
					goto find1;
				}
				else
				{
					findtop1--;
					maze[xin1][yin1] = 0;
					xin1 = find8road1[findtop1][5];
					yin1 = find8road1[findtop1][6];
				}
			}
	}
		  for(int i=0;i<h+4;i++)
			  for (int j = 0; j < l + 4; j++)
			  {
				  if (maze[i][j] == 11)
				  {
					  if (maze[i - 1][j] == 0 && maze[i + 1][j] == 0)
						  maze[i][j] = 0;
					  else if (maze[i][j - 1] == 0 && maze[i][j + 1] == 0)
						  maze[i][j] = 0;
				  }
				  if (maze[i][j] == 0)
				  {
					  if ((maze[i - 1][j] == 1 && maze[i + 1][j] == 11))
						  maze[i + 1][j] = 0;
					  else if ((maze[i - 1][j] == 11 && maze[i + 1][j] == 1))
						  maze[i - 1][j] = 0;
					  else if ((maze[i][j - 1] == 1 && maze[i][j + 1] == 11))
						  maze[i][j + 1] = 0;
					  else if ((maze[i][j - 1] == 11 && maze[i][j + 1] == 1))
						  maze[i][j - 1] = 0;
				  }
			  }
		 // print8road(h, l);
		  int road1[20 * M][2]; int road1count = 0;
		  for(int i=0;i<h+4;i++)
			  for (int j = 0; j < l + 4; j++)
			  {
				  if (maze[i][j] == 11)
				  {
					  road1count++;
					  road1[road1count][0] = i;
					  road1[road1count][1] = j;
					 
				  }
			  }
	maze[xin2][yin2] = 12;
	find2:while (findflag2 != 1)
	{
		//print8road(h, l);
		if (!fullstacki(find8road2, findtop2))
		{
			randdir(&finddir, &finddirx, &finddiry);
			if (!stackidir(find8road2, finddir, findtop2))
			{
				find8road2[findtop2][finddir] = 1;
				if (maze[xin2 + finddirx][yin2 + finddiry] != 1 && maze[xin2 + finddirx][yin2 + finddiry] != -1)
				{
					if (maze[xin2 + finddirx][yin2 + finddiry] != 12)
					{
						maze[xin2 + finddirx][yin2 + finddiry] = 12;
						maze[xin2 + 2 * finddirx][yin2 + 2 * finddiry] = 12;
						xin2 = xin2 + 2 * finddirx; yin2 = yin2 + 2 * finddiry;
						if (xin2 == xout2&&yin2 == yout2)
						{
							findflag2= 1;
							goto find2;
						}
						findtop2++;
						for (int i = 0; i < 7; i++)
							find8road2[findtop2][i] = 0;
						find8road2[findtop2][5] = xin2;
						find8road2[findtop2][6] = yin2;
						find8road2[findtop2][(finddir + 2) % 4] = 1;
					}
				}

			}
		}
		if (fullstacki(find8road2, findtop2))
		{
			if (findtop2 == 1)
			{
				findflag2 = 1;
				goto find2;
			}
			else
			{
				findtop2--;
				maze[xin2][yin2] = 0;
				xin2 = find8road2[findtop2][5];
				yin2 = find8road2[findtop2][6];
			}
		}
	}
		  for (int i = 0; i<h + 4; i++)
			  for (int j = 0; j < l + 4; j++)
			  {
				  if (maze[i][j] == 12)
				  {
					  if (maze[i - 1][j] == 0 && maze[i + 1][j] == 0)
						  maze[i][j] = 0;
					  else if (maze[i][j - 1] == 0 && maze[i][j + 1] == 0)
						  maze[i][j] = 0;
				  }
				  else if (maze[i][j] == 11)
					  maze[i][j] = 0;
				  if (maze[i][j] == 0)
				  {
					  if (maze[i - 1][j] == 1 && maze[i + 1][j] == 12)
						  maze[i + 1][j] = 0;
					  else if (maze[i - 1][j] == 12 && maze[i + 1][j] == 1)
						  maze[i - 1][j] = 0;
					  else if (maze[i][j - 1] == 1 && maze[i][j + 1] == 12)
						  maze[i][j + 1] = 0;
					  else if (maze[i][j - 1] == 12 && maze[i][j + 1] == 1)
						  maze[i][j - 1] = 0;
				  }
			  }
		  for (int i = 1; i <= road1count; i++)
			  maze[road1[i][0]][road1[i][1]] = 11;
	print8road(h, l);
	//system("pause");
}
void maze_2_out()
{
	int mazeflag = 1;
	int size;//长宽
	int rux, chux;
	int ruy, chuy;
	while (mazeflag == 1)
	{
		system("cls");
		cout << "请输入地图大小（不小于5） ："; cin >> size; cout << endl;
		if (size < 5)size = 5;
		if (size % 2 == 0)size++;
		rux = 2; ruy = 2;
		chux = size + 1; chuy = size + 2;
		maze_Create(size, size);
		print(size, size,rux,ruy,chux,chuy);
		cout << "输入数字 3 开始游戏" << endl;
		cout << "输入数字 4 直接打印路径" << endl;
		
			
			//cout << "打印路径 " << endl;
		//printroad(size, size);
		cout << "输入数字 2 将地图导出到文件" << endl;
		cout << "输入数字 1 继续" << endl;
		cin >> mazeflag;
		if (mazeflag == 4)
			printroad(size, size);
		if (mazeflag == 3) {
			game2(size, size);
			cout << endl;
			cout << "是否继续？继续请输入1   返回请输入其他数字:  ";
			cin >> mazeflag;
			cout << endl;
		}
		
		if (mazeflag == 2)
		{
			ofstream fout("maze_2_out.txt");
			if (fout)
			{
				fout << size << "  " << size << "  " << endl;
				fout << rux <<"  "<< ruy << "  " << endl;
				fout << chux << "  " << chuy << "  " << endl;
				for (int i = 0; i < size + 4; i++) {
					for (int j = 0; j < size + 4; j++)
						fout << maze[i][j] << "   ";
					fout << endl;
				}
			}
			fout.close();
			cout << "导出成功！" << endl;
			cout << "是否继续？继续请输入1   返回请输入0:  ";
			cin >> mazeflag;
		}
	}
}

void game2(int h, int l)
{
	//只打印局部地图数据5*5
	int userx=2, usery=2;//初始用户位置为（2，2）游戏起点//用20biaoji
	int endx=l+1, endy=h+1;//定义终点位置
	int mx = 0, my = 0; int dm = 13;//局部地图的起点坐标 和 步长
	//局部地图区域  maze[mx][my]          maze[mx][my+dm]
	//              maze[mx+dm][my]       maze[mx+dm][my+dm]
	maze[userx][usery] = 20; int biaoji[5 * M][2]; int biaojinum = 0;
	int key = 0;
	int ii, jj;
	while ( key==0||key!=101)
	{
		system("cls");
		//if(userx>=4&&userx<=h&&usery>=4&&usery<=l)
		for (ii = mx; ii <= mx + dm; ii++)
		{
			for (jj = my; jj <= my + dm; jj++)
			{
				if (maze[ii][jj] == 1 || maze[ii][jj] == -1) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
					//cout << "※";
					printf("※");
				}
				else if (maze[ii][jj] == 20 || maze[ii][jj] == 21 || maze[ii][jj] == -100)
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
					//cout << "※";
					printf("※");
				}
				else
				{
					//cout << "  ";//两个空格
					printf("  ");
				}
			}
			//cout << endl;
			printf("\n");
		}
		if (userx == endx&&usery == endy)
		{
			system("cls");
			for (ii = mx; ii <= mx + dm; ii++)
			{
				
				for (jj = my; jj <= my + dm; jj++)
				{
					
					//if(ii!=7)
					{
					if (maze[ii][jj] == 1 || maze[ii][jj] == -1) {
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
						//cout << "※";
						printf("※");
					}
					else if (maze[ii][jj] == 20 || maze[ii][jj] == 21||maze[ii][jj]==-100)
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
						//cout << "※";
						printf("※");
					}
					
					else
					{
						//cout << "  ";//两个空格
						printf("  ");
					}
				}
				}
				//cout << endl;
				if (ii == mx + dm)
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
					printf(" yon  win ");
				}
				printf("\n");
			}
		}
		cout << "请切换到英文输入法" << endl;
		cout << "键盘控制移动："
			"向上  w 键，向下 s 键，向左 a 键，向右 d 键"
			"显示路径  q 键，  退出游戏  e 键" ;
		//cout << endl; cout << " 空格 键 标记该位置     再次按 空格 键取消标记    x 键 取消所有标记";
		//_getch();
		key = _getch();
		int xyidong = 6;
		int yyidong = h - 10;
		int y2yidong = l - 10;
		if (key == 32)
		{
			if (maze[userx][usery] == -100)
				maze[userx][usery] = 0;
			else if (maze[userx][usery] == 0) {
				maze[userx][usery] = -100;
				biaoji[biaojinum][0] = userx;
				biaoji[biaojinum][1] = usery;
				biaojinum++;
			}
		}
		if (key == 120)
		{
			for (int k = 0; k <= biaojinum; k++)
				maze[biaoji[k][0]][biaoji[k][1]] = 0;
		}
		if (key == 101)break;
		if (key == 119)//up
		{
			if (maze[userx - 1][usery] == 0|| maze[userx - 1][usery] == 10 || maze[userx-1][usery] == -100)
			{
				if (userx <= xyidong)mx = 0;
				else if (userx >= yyidong+4)mx = yyidong;
				else
				mx--;
				if (maze[userx][usery] == 20)
					maze[userx][usery] = 0;
				//else if()
				else maze[userx][usery] = 10;
				userx--;
				if (maze[userx][usery] == 0)
					maze[userx][usery] = 20;
				else maze[userx][usery] = 21;
			}
		}
		else if (key == 115)//down
		{
			if (maze[userx + 1][usery] == 0|| maze[userx + 1][usery] == 10)
			{
				if (userx <= xyidong)mx = 0;
				else if (userx >= yyidong+4)mx = yyidong;
				else
				mx++;
				if(maze[userx][usery]==20)
				maze[userx][usery] = 0;
				else maze[userx][usery] = 10;
				userx++;
				if(maze[userx][usery]==0)
				maze[userx][usery] = 20;
				else maze[userx][usery] = 21;
			}
		}
		else if (key == 97)//left
		{
			if (maze[userx][usery - 1] == 0|| maze[userx][usery - 1] == 10)
			{
				if (usery <= xyidong)my = 0;
				else if (usery >= y2yidong+4)my = y2yidong;
				//if(usery>= xyidong&&usery<= y2yidong)
				else
				my--;
				if (maze[userx][usery] == 20)
					maze[userx][usery] = 0;
				else maze[userx][usery] = 10;
				usery--;
				if (maze[userx][usery] == 0)
					maze[userx][usery] = 20;
				else maze[userx][usery] = 21;
			}
		}
		else if (key == 100)//right
		{
			if (maze[userx][usery + 1] == 0|| maze[userx][usery + 1] == 10)
			{
				if (usery <= xyidong)my = 0;
				else if (usery >= y2yidong+4)my = y2yidong;
				//if (usery >= xyidong && usery <= y2yidong)
				else
				my++;
				if (maze[userx][usery] == 20)
					maze[userx][usery] = 0;
				else maze[userx][usery] = 10;
				usery++;
				if (maze[userx][usery] == 0)
					maze[userx][usery] = 20;
				else maze[userx][usery] = 21;
			}
		}
		else if (key == 113)//显示路径
		{
			cout << endl;
			printroad(h, l);
			system("pause");
			break;
		}

		
	}
}
void game8(int h, int l)
{
	//定义入口(2,2)    (2,l+1)
	//        (h+1,2)  (h+1,l+1)
	int userx = 2, usery = 2;//初始用户位置为（2，2）游戏起点//用20biaoji
	int endx = l + 1, endy = h + 1;//定义终点位置
	int mx = 0, my = 0; int dm = 13;//局部地图的起点坐标 和 步长
									//局部地图区域  maze[mx][my]          maze[mx][my+dm]
									//              maze[mx+dm][my]       maze[mx+dm][my+dm]
	bool endpoint1 = false, endpoint2 = false, endpoint3 = false; int endcount = 0;
	maze[userx][usery] = 20; int biaoji[5 * M][2]; int biaojinum = 0;
	int key = 0;
	int ii, jj;
	while (key == 0 || key != 101)
	{
		system("cls");
		//if(userx>=4&&userx<=h&&usery>=4&&usery<=l)
		for (ii = mx; ii <= mx + dm; ii++)
		{
			for (jj = my; jj <= my + dm; jj++)
			{
				if (maze[ii][jj] == 1 || maze[ii][jj] == -1) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
					//cout << "※";
					printf("※");
				}
				else if (maze[ii][jj] == 20 || maze[ii][jj] == 21||maze[ii][jj]==-100)
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
					//cout << "※";
					printf("※");
				}
				else
				{
					//cout << "  ";//两个空格
					printf("  ");
				}
			}
			//cout << endl;
			printf("\n");
		}
		printf("you have find %d exit\n", endcount);
		if ((userx == h + 1 && usery == 2) && endpoint1 == false)
		{
			endcount++;
			endpoint1 = true;
			printf("you have find %d exit\n",endcount);
		}
		if ((userx == 2 && usery == l + 1) && endpoint2 == false)
		{
			endcount++;
			endpoint2 = true;
			printf("you have find %d exit\n", endcount);
		}
		if ((userx == h+1 && usery == l + 1) && endpoint3 == false)
		{
			endcount++;
			endpoint3 = true;
			printf("you have find %d exit\n", endcount);
		}
		//if (userx == endx&&usery == endy)
		if(endpoint1==true&&endpoint2==true&&endpoint3==true)
		{
			system("cls");
			for (ii = mx; ii <= mx + dm; ii++)
			{

				for (jj = my; jj <= my + dm; jj++)
				{

					//if(ii!=7)
					{
						if (maze[ii][jj] == 1 || maze[ii][jj] == -1) {
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
							//cout << "※";
							printf("※");
						}
						else if (maze[ii][jj] == 20 || maze[ii][jj] == -100)
						{
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
							//cout << "※";
							printf("※");
						}
						else
						{
							//cout << "  ";//两个空格
							printf("  ");
						}
					}
				}
				//cout << endl;
				if (ii == mx + dm)
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
					printf(" yon  have find all(3)exits ");
				}
				printf("\n");
			}
		}
		cout << "请切换到英文输入法" << endl;
		cout << "键盘控制移动："
			"向上  w 键，向下 s 键，向左 a 键，向右 d 键"
			"显示路径  q 键，  退出游戏  e 键";
		cout << endl; cout << " c 键 标记该位置     再次按 c 键取消标记    x 键 取消所有标记";
		//_getch();
		key = _getch();
		int xyidong = 6;
		int yyidong = h - 10;
		int y2yidong = l - 10;
		if (key == 101)break;
		if (key == 99)
		{
			if (maze[userx][usery] == -100)
				maze[userx][usery] = 20;
			else if (maze[userx][usery] == 0||maze[userx][usery]==20) {
				maze[userx][usery] = -100;
				biaoji[biaojinum][0] = userx;
				biaoji[biaojinum][1] = usery;
				biaojinum++;
			}
		}
		if (key == 120)
		{
			for (int k = 0; k < biaojinum; k++)
				maze[biaoji[k][0]][biaoji[k][1]] = 0;
		}
		if (key == 119)//up
		{
			if (maze[userx - 1][usery] == 0 ||maze[userx-1][usery]==-100)
			{
				if (userx <= xyidong)mx = 0;
				else if (userx >= yyidong + 4)mx = yyidong;
				else
					mx--;
				if (maze[userx][usery] == -100)
					maze[userx][usery] = -100;
				else
					maze[userx][usery] = 0;
				userx--;
				if (maze[userx][usery] == -100)
					maze[userx][usery] = -100;
				else 
					maze[userx][usery] = 20;
				//else maze[userx][usery] = 21;
			}
		}
		else if (key == 115)//down
		{
			if (maze[userx + 1][usery] == 0 || maze[userx + 1][usery] == -100)
			{
				if (userx <= xyidong)mx = 0;
				else if (userx >= yyidong + 4)mx = yyidong;
				else
					mx++;
				//if (maze[userx][usery] == 20)
				if (maze[userx][usery] == -100)
					maze[userx][usery] = -100;
				else
					maze[userx][usery] = 0;
				//else maze[userx][usery] = 10;
				userx++;
				//if (maze[userx][usery] == 0)
				if (maze[userx][usery] == -100)
					maze[userx][usery] = -100;
				else
					maze[userx][usery] = 20;
				//else maze[userx][usery] = 21;
			}
		}
		else if (key == 97)//left
		{
			if (maze[userx][usery - 1] == 0 || maze[userx ][usery-1] == -100)
			{
				if (usery <= xyidong)my = 0;
				else if (usery >= y2yidong + 4)my = y2yidong;
				//if(usery>= xyidong&&usery<= y2yidong)
				else
					my--;
				//if (maze[userx][usery] == 20)
				if (maze[userx][usery] == -100)
					maze[userx][usery] = -100;
				else
					maze[userx][usery] = 0;
				//else maze[userx][usery] = 10;
				usery--;
				//if (maze[userx][usery] == 0)
				if (maze[userx][usery] == -100)
					maze[userx][usery] = -100;
				else
					maze[userx][usery] = 20;
				//else maze[userx][usery] = 21;
			}
		}
		else if (key == 100)//right
		{
			if (maze[userx][usery + 1] == 0 || maze[userx][usery +1] == -100)
			{
				if (usery <= xyidong)my = 0;
				else if (usery >= y2yidong + 4)my = y2yidong;
				//if (usery >= xyidong && usery <= y2yidong)
				else
					my++;
				//if (maze[userx][usery] == 20)
				if (maze[userx][usery] == -100)
					maze[userx][usery] = -100;
				else
					maze[userx][usery] = 0;
				//else maze[userx][usery] = 10;
				usery++;
				//if (maze[userx][usery] == 0)
					maze[userx][usery] = 20;
					if (maze[userx][usery] == -100)
						maze[userx][usery] = -100;
					else
						maze[userx][usery] = 20;
				//else maze[userx][usery] = 21;
			}
		}
		else if (key == 113)//显示路径
		{
			cout << endl;
			find8road(h, l);
			//print8road(h, l);
			system("pause");
			break;
		}


	}
}

int main()
{
	//maze_Init(25, 25);
	//print8(25, 25);
	//system("pause");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	//cout << "※";
	cout << endl;
	cout << endl;
	cout << "          欢迎进入 迷宫游戏 " << endl;
	cout << endl;
	cout << endl;
	cout << "       注意：当输入地图大小较大时 建议全屏模式查看 ，否则地图显示不全\n"; cout << endl;
	cout << "      游戏时请注意切换到英文输入法"
		"\n\n     游戏控制方向为w a s d"
		"\n\n     多入口多出口地图中可以标记"
		"\n\n     c 键标记   再按 c 键取消标记"
		"\n      x 键取消所有标记"
		"\n  \n 请按 提示 输入相应操作的序号 后按"; 
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);

	cout << "  回车  ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	//cout << "※";
		cout<<"继续"
		"\n";
	cout << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	
	cout << "作者信息：" << endl;
	cout << "          樊克虎" << endl;
	cout << "          2016060102020" << endl;
	cout << "          软件开发综合实践课程" << endl;
	cout << "          任课老师 王丽杰" << endl;
	cout << "          版本 1.2" << endl;
	cout << "          最后更新时间 2017.12.1" << endl;
	cout << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	//cout << "※";
		cout<<"\n   按任意键 继续";
	system("pause");
	//FullScreen();
	//system("pause");
	int choice;
	while (1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		//cout << "※";
		system("cls");
		cout << endl;
		cout << "    高级模式，自定义地图" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);

		cout << "    1 单入口单出口单方向" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		//cout << "※";
		cout << "\n          介绍：入口左上角 出口右下角 只有一条路径  "
			"\n           生成方式 ：单向探测" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << endl;
		cout << "    2  多入口多出口四方向" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		//cout << "※";
		cout << "\n         介绍：四个角为出口或入口，有多条路径"
			" \n         生成方式：四个方向同时探测，通过并查集相联通" << endl;
		cout << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		//cout << "※";
		cout << "    普通模式，固定地图" << endl;
		cout << "    从文件导入地图" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);

		cout << "    3  导入上次的地图（单入口单出口）" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);

		cout << "    4  导入上次的地图（多入口多出口）" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);

		cout << "    5  简单--导入固定地图（单入口单出口 25*25）" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);

		cout << "    6  中等--导入固定地图（多入口多出口 25*25）" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);

		cout << "    7  较难--导入固定地图（多入口多出口 50*50）" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);

		cout << "    0  推出" << endl; cout << endl;

		cout << "    请输入您的选择：";
		cin >> choice;
		switch (choice)
		{
		case 1:maze_2_out();
			system("pause");
			break;
		case 2:maze_more_in_out();
			system("pause");
			break;
		case 3:
		{
			int hang, lie;
			int inx, iny, outx, outy;
			ifstream fin("maze_2_out.txt");
			if (fin)
			{
				fin >> hang >> lie;
				fin >> inx >> iny;
				fin >> outx >> outy;
				for(int i=0;i<hang+4;i++)
					for (int j = 0; j < lie + 4; j++)
					{
						fin >> maze[i][j];
					}
			}
			fin.close();
			cout << "导入成功！" << endl;
			print(hang, lie, inx, iny, outx, outy);
			cout << "开始游戏  输入  1"
				"\n打印路径  输入  2"
				"\n推出      输入  0" << endl;
			int daorucaozuo;
			cin >> daorucaozuo;
			if (daorucaozuo == 1)game2(hang, lie);
			else if (daorucaozuo == 2) {
				cout << "打印路径" << endl;
				printroad(hang, lie);
			}
			else if (daorucaozuo == 0)
				goto ab;
		}
		system("pause");
		ab:break;
		case 4: {
			int hang, lie;
			int inx1, iny1; int inx2, iny2;
			int outx1;// outy1;
			int outx2, outy2;
			ifstream finmore("maze_more_in_out.txt");
			if (finmore)
			{
				finmore >> hang >> lie;
				finmore >> inx1 >> iny1;
				finmore >> inx2 >> iny2;
				finmore >> outx1 >> outx2;
				finmore >> outx2 >> outy2;
				for (int i = 0; i<hang + 4; i++)
					for (int j = 0; j < lie + 4; j++)
					{
						finmore >> maze[i][j];
					}
			}
			finmore.close();
			cout << "导入成功！" << endl;
			print8(hang, lie);
			cout << "开始游戏  输入  1"
				"\n打印路径  输入  2"
				"\n推出      输入  0" << endl;
			int daorucaozuo4;
			cin >> daorucaozuo4;
			if (daorucaozuo4 == 1)game8(hang, lie);
			else if (daorucaozuo4 == 2) {
				cout << "打印路径" << endl;
				find8road(hang, lie);
				//print8road(hang, lie);
			}
			else if (daorucaozuo4 == 0)
				goto ab4;
			///cout << "打印路径" << endl;
			//find8road(hang, lie);
			
		}
				system("pause");
			ab4:	break;
		case 5:
		{
			int hang, lie;
			int inx, iny, outx, outy;
			ifstream fin("maze_2_out25.txt");
			if (fin)
			{
				fin >> hang >> lie;
				fin >> inx >> iny;
				fin >> outx >> outy;
				for (int i = 0; i<hang + 4; i++)
					for (int j = 0; j < lie + 4; j++)
					{
						fin >> maze[i][j];
					}
			}
			fin.close();
			cout << "导入成功！" << endl;
			print(hang, lie, inx, iny, outx, outy);
			cout << "开始游戏  输入  1"
				"\n打印路径  输入  2"
				"\n推出      输入  0" << endl;
			int daorucaozuo1;
			cin >> daorucaozuo1;
			if (daorucaozuo1 == 1)game2(hang, lie);
			else if (daorucaozuo1 == 2) {
				cout << "打印路径" << endl;
				printroad(hang, lie);
			}
			else if (daorucaozuo1 == 0)
				goto abc;
		}
		system("pause");
		abc:break;
		case 6:
		{
			int hang, lie;
			int inx1, iny1; int inx2, iny2;
			int outx1; //outy1; 
			int outx2, outy2;
			ifstream finmore("maze_more_in_out25.txt");
			if (finmore)
			{
				finmore >> hang >> lie;
				finmore >> inx1 >> iny1;
				finmore >> inx2 >> iny2;
				finmore >> outx1 >> outx2;
				finmore >> outx2 >> outy2;
				for (int i = 0; i<hang + 4; i++)
					for (int j = 0; j < lie + 4; j++)
					{
						finmore >> maze[i][j];
					}
			}
			finmore.close();
			cout << "导入成功！" << endl;
			print8(hang, lie);
			cout << "开始游戏  输入  1"
				"\n打印路径  输入  2"
				"\n推出      输入  0" << endl;
			int daorucaozuo6;
			cin >> daorucaozuo6;
			if (daorucaozuo6 == 1)game8(hang, lie);
			else if (daorucaozuo6 == 2) {
				cout << "打印路径" << endl;
				find8road(hang, lie);
				//print8road(hang, lie);
			}
			else if (daorucaozuo6 == 0)
				goto ab6;
			//cout << "打印路径" << endl;
			//find8road(hang, lie);
		}
		system("pause");
		ab6:break;
		case 7:
		{
			int hang, lie;
			int inx1, iny1; int inx2, iny2;
			int outx1; //outy1;
			int outx2, outy2;
			ifstream finmore("maze_more_in_out50.txt");
			if (finmore)
			{
				finmore >> hang >> lie;
				finmore >> inx1 >> iny1;
				finmore >> inx2 >> iny2;
				finmore >> outx1 >> outx2;
				finmore >> outx2 >> outy2;
				for (int i = 0; i<hang + 4; i++)
					for (int j = 0; j < lie + 4; j++)
					{
						finmore >> maze[i][j];
					}
			}
			finmore.close();
			cout << "导入成功！" << endl;
			print8(hang, lie);
			cout << "开始游戏  输入  1"
				"\n打印路径  输入  2"
				"\n推出      输入  0" << endl;
			int daorucaozuo7;
			cin >> daorucaozuo7;
			if (daorucaozuo7 == 1)game8(hang, lie);
			else if (daorucaozuo7 == 2) {
				cout << "打印路径" << endl;
				find8road(hang, lie);
				//print8road(hang, lie);
			}
			else if (daorucaozuo7 == 0)
				goto ab7;
			//cout << "打印路径" << endl;
			//find8road(hang, lie);
		}
		system("pause");
		ab7:break;
		default:goto ex;
		}
	}
	ex:
	return 0;
}