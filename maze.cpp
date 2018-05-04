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
	*dir = rand() % 4 + 1;//����ڿ�ʼ�������һ���������
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

/*  ���巽��
    (x,y)  (0,0)  0
	(x-1,y) (-1,0)1
	(x,y-1) (0,-1)2
	(x+1,y) (1,0) 3
	(x,y+1) (0,1) 4
	
	-1 �߽�
	1  ǽ
	0  ͨ·
	����ԭ��maze[2][2]
	*/

void maze_Init(int m,int n)/*�������mΪ�Թ���ͼ����
						             nΪ�Թ���ͼ����
							 ����ͼ��ʼ��Ϊָ����ʽ*/
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



void maze_Create(int m, int n) {//m����  n����
	maze_Init(m, n);
	//print8(m, n); system("pause");
	int xin = 2, yin = 2;//�������
	int xout = n + 1, yout = m + 1;//��������
	//int xout = 2, yout = 2;
	maze[xout][yout] = -3;
	int  dirxxout, diryyout;//��¼�ƶ�����
	srand(time(NULL));
	int  directnowout;//����   
	/*�������1��4����Ȼ��
	  �ֱ�����ĸ�����*/
	  /*for (int i = 0; i < 20; i++) {
		  directnow = rand() % 4 + 1;
		  cout << directnow << endl;
	  }*/
	  /*����ڿ�ʼ�ڣ�������ͨ·��־Ϊ-2
		�ӳ��ڿ�ʼ�ڣ�������ͨ·��־Ϊ-3
		���þֲ���־����¼�Ƿ���ͨ
		*/
	bool in_out = false;//��ʼ��־Ϊfalse ��ʾδ��ͨ
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
		x1:if (!stack_full(in)) {//����ĸ�����û��̽��һ��
			randdir(&directnowin, &dirxxin, &diryyin);
			//cout << directnowin << "  ";
			//�ж��������̽����û
			if (!stack_dir(in, directnowin))//���û��̽����
			{
				//cout << directnowin << "  ";
				stack_pushdir(in, directnowin);
				//cout << directnowin << "  ";
				if (maze[xin + dirxxin][yin + diryyin] == 1)//�����ǽ
				{
					//�ж�ǽ����һ��
					if (maze[xin + 2 * dirxxin][yin + 2 * diryyin] == 0) {
						//���δ��ͨ
						maze[xin + dirxxin][yin + diryyin] = -2;//�������ͨ���δ-2
						xin = xin + 2 * dirxxin; yin = yin + 2 * diryyin;
						stack_push(in, xin, yin);
						stack_pushdir(in, (directnowin + 2) % 4);//���÷�����ջ
					}
					else if (maze[xin + 2 * dirxxin][yin + 2 * diryyin] == -2) {
						//����Ѵ������ͨ
						goto x1;
					}
					else if (maze[xin + 2 * dirxxin][yin + 2 * diryyin] == -3) {//�ӳ�����ͨ���-3
						if (!in_out) {//���δ��ͨ
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


out1:	while (outstackshuzu[1][1] == 0 || outstackshuzu[1][2] == 0) {//����ĸ�����û��̽��һ��
	//print(m, n); system("cls");
	randdir(&directnowout, &dirxxout, &diryyout);// print(m, n);
	//�ж��������̽����û
	//���û��̽����
	
	if (!stackidir(outstackshuzu, directnowout, outstacktop))//(outstackshuzu[outstacktop][directnowout] == 0)
	{
		outstackshuzu[outstacktop][directnowout] = 1;

		if (maze[xout + dirxxout][yout + diryyout] == 1)//�����ǽ
		{
			//�ж�ǽ����һ��
			if (maze[xout + 2 * dirxxout][yout + 2 * diryyout] == 0) {
				//���δ��ͨ
				maze[xout + dirxxout][yout + diryyout] = -3;//�ӳ�����ͨ���δ-3

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
			if (maze[xout + 2 * dirxxout][yout + 2 * diryyout] == -2) {//�ӳ�����ͨ���-3
				if (in_out == false) {//���δ��ͨ
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
					printf("��");
				}
				else
					printf("  ");//�����ո�
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
				cout << "��";
			}
			else if (maze[i][j]==10)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
				cout << "��";
			}
			else
				cout << "  ";//�����ո�
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
				cout << "��";
			}
			else if (maze[i][j] == 11)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
				cout << "��";
			}
			else if (maze[i][j] == 12)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				cout << "��";
			}
			else
				cout << "  ";//�����ո�
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
					printf("��");
				}
				else
					printf("  ");//�����ո�
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
	//����һ���ж��������
	//��ʼ�����
	for (int i = 0; i < 7; i++)mazefindrode[1][i] = 0;
	mazefindrode[1][5] = xin1;
	mazefindrode[1][6] = yin1;
	maze[xin1][yin1] = 10;
	while (xin1 != xout1||yin1 != yout1) {
		printroad(xout1-1, yout1-1);
		randdir(&findroaddir, &findroaddirx, &findroaddiry);
		if (maze[xin1 + findroaddirx][yin1 + findroaddiry] != 1 && maze[xin1 + findroaddirx][yin1 + findroaddiry] != -1)//�����ж���������ǲ���ͨ·
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
				cout << "û��ͨ·" << endl;
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
			un[i][j] = 0;//�˸�����Ĺ�ϵ����
	//�Ӱ˸����������Թ�
	/*�˸����������ڹ������α��Ϊ11��12��13����������18
	   -1Ϊ�߽�
	   1 Ϊǽ��*/
	int stack1[20 * M][7], stack2[20 * M][7],  stack6[20 * M][7], stack8[20 * M][7];

	maze_Init(hang, lie); //print(hang, lie); system("pause");
	int x0 = 2, y0 = 2;//���Ͻ�����
	int xend = lie + 1, yend = hang + 1;//���½�����
	//maze[x0][y0] = 10; maze[yend][xend] = 10;
	//printroad(hang, lie); system("pause");
	//����̽���İ˸�����λ��
	int x1 = x0, y1 = y0;
	int x2 = x0, y2 = yend;
	int x6 = xend, y6 = y0;
	int x8 = xend, y8 = yend;
	//�����ջ����ʼ��
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
	 //�ɹ�
		if (!fullstacki(stack1, 1))
		{
			randdir(&rand8dir, &rand8dirx, &rand8diry);
			if (!stackidir(stack1, rand8dir, top1))//�жϸ������Ƿ�̽����
			{
				stack1[top1][rand8dir] = 1;//yeah
				if (maze[x1+rand8dirx][y1+rand8diry]==1)//�жϸ÷����Ƿ�Ϊǽ
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
		//�ɹ�
		if (!fullstacki(stack2, 1))
		{
			randdir(&rand8dir, &rand8dirx, &rand8diry);
			if (!stackidir(stack2, rand8dir, top2))//�жϸ������Ƿ�̽����
			{
				stack2[top2][rand8dir] = 1;
				if (maze[x2 + rand8dirx][y2 + rand8diry] == 1)//�жϸ÷����Ƿ�Ϊǽ
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
		//�ɹ�
		if (!fullstacki(stack8, 1))
		{
			randdir(&rand8dir, &rand8dirx, &rand8diry);
			if (!stackidir(stack8, rand8dir, top8))//�жϸ������Ƿ�̽����
			{
				stack8[top8][rand8dir] = 1;
				if (maze[x8 + rand8dirx][y8 + rand8diry] == 1)//�жϸ÷����Ƿ�Ϊǽ
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
		//�ɹ�
		if (!fullstacki(stack6, 1))
		{
		randdir(&rand8dir, &rand8dirx, &rand8diry);
		if (!stackidir(stack6, rand8dir, top6))//�жϸ������Ƿ�̽����
		{
		stack6[top6][rand8dir] = 1;
		if (maze[x6 + rand8dirx][y6 + rand8diry] == 1)//�жϸ÷����Ƿ�Ϊǽ
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
	//�ô����������ͼ
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

bool fullstacki(int stacki[][7],int top)//�ж�ջ���ĸ������Ƿ�̽�����
{
	//cout << stacki[top][1] << "  " << stacki[top][2];
	//system("pause");
	if (stacki[top][1] == 1 && stacki[top][2] == 1 && stacki[top][3] == 1 && stacki[top][4] == 1)
		return true;
	else return false;
}
void maze_more_in_out( ) {//����ڶ���ڣ��û�������ںͳ��ڸ��� ���е�ͼ�ߴ�
	//�Ӱ˸�����ʼ�����Թ�
	//���Ϊ��2��2�� �ͣ�hang+1,2��   ����Ϊ (2,lie+1���ͣ�hang+1,lie+1��
	int maze8flag = 1;
	int hang, lie;
	//int rux, ruy;
	//int chux, chuy;
	while (maze8flag == 1) {
		system("cls");
		cout << " �������Թ����� �� ����(����С��11)" << endl;
		cout << "������"; cin >> hang; cout << endl;
		cout << "������"; cin >> lie; cout << endl;
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
		//��ʼѰ·
		//print8road(hang, lie); system("pause");
		cout << "   ���� 4 ֱ�Ӵ�ӡ·��"
			"   ���� 3 ��ʼ��Ϸ"
			"   ���� 2 ֱ�ӵ������ļ�"
			"   ���� 1 ����"
			"   ���� 0 ����" << endl;
		cin >> maze8flag;
		if (maze8flag == 3)
		{
			game8(hang, lie);
		}
		if (maze8flag == 2)
		{
			cout << "�����ɹ���" << endl;
			cout << "�Ƿ����������������1   ����������0:  ";
			cin >> maze8flag;
		}
		if (maze8flag == 4)
		{
			cout << "��ӡ·��" << endl;

			find8road(hang, lie);
			system("pause");
		}
		//if (maze8flag == 0);
			//goto end8;
		//system("pause");
		//cout << "�������� 2 ����ͼ�������ļ�" << endl;
		//cout << "�Ƿ����������������1   ������������������:  ";
		//cin >> maze8flag;

	}
//end8:
}
void find8road(int h, int l)
{
	int find8road1[20 * M][7];
	int find8road2[20 * M][7];
	int xin1 = 2, yin1 = 2;//���1
	int xin2 = h + 1, yin2 = 2;//���2
	int xout1 = h + 1, yout1 = l + 1;//����1
	int xout2 = 2, yout2 = l + 1;//����2
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
	int size;//����
	int rux, chux;
	int ruy, chuy;
	while (mazeflag == 1)
	{
		system("cls");
		cout << "�������ͼ��С����С��5�� ��"; cin >> size; cout << endl;
		if (size < 5)size = 5;
		if (size % 2 == 0)size++;
		rux = 2; ruy = 2;
		chux = size + 1; chuy = size + 2;
		maze_Create(size, size);
		print(size, size,rux,ruy,chux,chuy);
		cout << "�������� 3 ��ʼ��Ϸ" << endl;
		cout << "�������� 4 ֱ�Ӵ�ӡ·��" << endl;
		
			
			//cout << "��ӡ·�� " << endl;
		//printroad(size, size);
		cout << "�������� 2 ����ͼ�������ļ�" << endl;
		cout << "�������� 1 ����" << endl;
		cin >> mazeflag;
		if (mazeflag == 4)
			printroad(size, size);
		if (mazeflag == 3) {
			game2(size, size);
			cout << endl;
			cout << "�Ƿ����������������1   ������������������:  ";
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
			cout << "�����ɹ���" << endl;
			cout << "�Ƿ����������������1   ����������0:  ";
			cin >> mazeflag;
		}
	}
}

void game2(int h, int l)
{
	//ֻ��ӡ�ֲ���ͼ����5*5
	int userx=2, usery=2;//��ʼ�û�λ��Ϊ��2��2����Ϸ���//��20biaoji
	int endx=l+1, endy=h+1;//�����յ�λ��
	int mx = 0, my = 0; int dm = 13;//�ֲ���ͼ��������� �� ����
	//�ֲ���ͼ����  maze[mx][my]          maze[mx][my+dm]
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
					//cout << "��";
					printf("��");
				}
				else if (maze[ii][jj] == 20 || maze[ii][jj] == 21 || maze[ii][jj] == -100)
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
					//cout << "��";
					printf("��");
				}
				else
				{
					//cout << "  ";//�����ո�
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
						//cout << "��";
						printf("��");
					}
					else if (maze[ii][jj] == 20 || maze[ii][jj] == 21||maze[ii][jj]==-100)
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
						//cout << "��";
						printf("��");
					}
					
					else
					{
						//cout << "  ";//�����ո�
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
		cout << "���л���Ӣ�����뷨" << endl;
		cout << "���̿����ƶ���"
			"����  w �������� s �������� a �������� d ��"
			"��ʾ·��  q ����  �˳���Ϸ  e ��" ;
		//cout << endl; cout << " �ո� �� ��Ǹ�λ��     �ٴΰ� �ո� ��ȡ�����    x �� ȡ�����б��";
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
		else if (key == 113)//��ʾ·��
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
	//�������(2,2)    (2,l+1)
	//        (h+1,2)  (h+1,l+1)
	int userx = 2, usery = 2;//��ʼ�û�λ��Ϊ��2��2����Ϸ���//��20biaoji
	int endx = l + 1, endy = h + 1;//�����յ�λ��
	int mx = 0, my = 0; int dm = 13;//�ֲ���ͼ��������� �� ����
									//�ֲ���ͼ����  maze[mx][my]          maze[mx][my+dm]
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
					//cout << "��";
					printf("��");
				}
				else if (maze[ii][jj] == 20 || maze[ii][jj] == 21||maze[ii][jj]==-100)
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
					//cout << "��";
					printf("��");
				}
				else
				{
					//cout << "  ";//�����ո�
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
							//cout << "��";
							printf("��");
						}
						else if (maze[ii][jj] == 20 || maze[ii][jj] == -100)
						{
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
							//cout << "��";
							printf("��");
						}
						else
						{
							//cout << "  ";//�����ո�
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
		cout << "���л���Ӣ�����뷨" << endl;
		cout << "���̿����ƶ���"
			"����  w �������� s �������� a �������� d ��"
			"��ʾ·��  q ����  �˳���Ϸ  e ��";
		cout << endl; cout << " c �� ��Ǹ�λ��     �ٴΰ� c ��ȡ�����    x �� ȡ�����б��";
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
		else if (key == 113)//��ʾ·��
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
	//cout << "��";
	cout << endl;
	cout << endl;
	cout << "          ��ӭ���� �Թ���Ϸ " << endl;
	cout << endl;
	cout << endl;
	cout << "       ע�⣺�������ͼ��С�ϴ�ʱ ����ȫ��ģʽ�鿴 �������ͼ��ʾ��ȫ\n"; cout << endl;
	cout << "      ��Ϸʱ��ע���л���Ӣ�����뷨"
		"\n\n     ��Ϸ���Ʒ���Ϊw a s d"
		"\n\n     ����ڶ���ڵ�ͼ�п��Ա��"
		"\n\n     c �����   �ٰ� c ��ȡ�����"
		"\n      x ��ȡ�����б��"
		"\n  \n �밴 ��ʾ ������Ӧ��������� ��"; 
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);

	cout << "  �س�  ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	//cout << "��";
		cout<<"����"
		"\n";
	cout << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	
	cout << "������Ϣ��" << endl;
	cout << "          ���˻�" << endl;
	cout << "          2016060102020" << endl;
	cout << "          ��������ۺ�ʵ���γ�" << endl;
	cout << "          �ο���ʦ ������" << endl;
	cout << "          �汾 1.2" << endl;
	cout << "          ������ʱ�� 2017.12.1" << endl;
	cout << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	//cout << "��";
		cout<<"\n   ������� ����";
	system("pause");
	//FullScreen();
	//system("pause");
	int choice;
	while (1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		//cout << "��";
		system("cls");
		cout << endl;
		cout << "    �߼�ģʽ���Զ����ͼ" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);

		cout << "    1 ����ڵ����ڵ�����" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		//cout << "��";
		cout << "\n          ���ܣ�������Ͻ� �������½� ֻ��һ��·��  "
			"\n           ���ɷ�ʽ ������̽��" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << endl;
		cout << "    2  ����ڶ�����ķ���" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		//cout << "��";
		cout << "\n         ���ܣ��ĸ���Ϊ���ڻ���ڣ��ж���·��"
			" \n         ���ɷ�ʽ���ĸ�����ͬʱ̽�⣬ͨ�����鼯����ͨ" << endl;
		cout << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		//cout << "��";
		cout << "    ��ͨģʽ���̶���ͼ" << endl;
		cout << "    ���ļ������ͼ" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);

		cout << "    3  �����ϴεĵ�ͼ������ڵ����ڣ�" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);

		cout << "    4  �����ϴεĵ�ͼ������ڶ���ڣ�" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);

		cout << "    5  ��--����̶���ͼ������ڵ����� 25*25��" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);

		cout << "    6  �е�--����̶���ͼ������ڶ���� 25*25��" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);

		cout << "    7  ����--����̶���ͼ������ڶ���� 50*50��" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);

		cout << "    0  �Ƴ�" << endl; cout << endl;

		cout << "    ����������ѡ��";
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
			cout << "����ɹ���" << endl;
			print(hang, lie, inx, iny, outx, outy);
			cout << "��ʼ��Ϸ  ����  1"
				"\n��ӡ·��  ����  2"
				"\n�Ƴ�      ����  0" << endl;
			int daorucaozuo;
			cin >> daorucaozuo;
			if (daorucaozuo == 1)game2(hang, lie);
			else if (daorucaozuo == 2) {
				cout << "��ӡ·��" << endl;
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
			cout << "����ɹ���" << endl;
			print8(hang, lie);
			cout << "��ʼ��Ϸ  ����  1"
				"\n��ӡ·��  ����  2"
				"\n�Ƴ�      ����  0" << endl;
			int daorucaozuo4;
			cin >> daorucaozuo4;
			if (daorucaozuo4 == 1)game8(hang, lie);
			else if (daorucaozuo4 == 2) {
				cout << "��ӡ·��" << endl;
				find8road(hang, lie);
				//print8road(hang, lie);
			}
			else if (daorucaozuo4 == 0)
				goto ab4;
			///cout << "��ӡ·��" << endl;
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
			cout << "����ɹ���" << endl;
			print(hang, lie, inx, iny, outx, outy);
			cout << "��ʼ��Ϸ  ����  1"
				"\n��ӡ·��  ����  2"
				"\n�Ƴ�      ����  0" << endl;
			int daorucaozuo1;
			cin >> daorucaozuo1;
			if (daorucaozuo1 == 1)game2(hang, lie);
			else if (daorucaozuo1 == 2) {
				cout << "��ӡ·��" << endl;
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
			cout << "����ɹ���" << endl;
			print8(hang, lie);
			cout << "��ʼ��Ϸ  ����  1"
				"\n��ӡ·��  ����  2"
				"\n�Ƴ�      ����  0" << endl;
			int daorucaozuo6;
			cin >> daorucaozuo6;
			if (daorucaozuo6 == 1)game8(hang, lie);
			else if (daorucaozuo6 == 2) {
				cout << "��ӡ·��" << endl;
				find8road(hang, lie);
				//print8road(hang, lie);
			}
			else if (daorucaozuo6 == 0)
				goto ab6;
			//cout << "��ӡ·��" << endl;
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
			cout << "����ɹ���" << endl;
			print8(hang, lie);
			cout << "��ʼ��Ϸ  ����  1"
				"\n��ӡ·��  ����  2"
				"\n�Ƴ�      ����  0" << endl;
			int daorucaozuo7;
			cin >> daorucaozuo7;
			if (daorucaozuo7 == 1)game8(hang, lie);
			else if (daorucaozuo7 == 2) {
				cout << "��ӡ·��" << endl;
				find8road(hang, lie);
				//print8road(hang, lie);
			}
			else if (daorucaozuo7 == 0)
				goto ab7;
			//cout << "��ӡ·��" << endl;
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