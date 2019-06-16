#include<graphics.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#include<iostream>
using namespace std;

//定义图形窗口的宽度、高度
#define WIDTH 400
#define HEIGHT 600
//设置砖块行数、列数
#define ROW 5
#define COLUMN 10

int i, j,color;
int levels[5][ROW][COLUMN] = { 0 };//准备之后用来储存不同关卡用的，预设有5个关卡
int breakOut = 0;//记录已经打碎的砖块
int game_mode ;//记录用户选择的游戏模式
MOUSEMSG m;       //记录鼠标消息



class Brick
{
public:
	const int length = 40;
	const int width = 20;
	int bricks[ROW][COLUMN];
	int count;

	Brick();
	Brick(int level);

	void DrawBricks();//画出所有砖块
};


class Board  
{
public:
	int x, y;             
	int length = 60;  
	int width = 15;   

	Board();

	//预设吃到道具后让板变长、变短的两个函数
	void Longer();
	void Shorter();

	void Move();//根据game_mode的值判断选取下面两种控制方式的哪一个
	void KeyBoardMove();//键盘控制
	void MouseMove();//鼠标控制
};


class Ball
{
public:
	int x, y;
	int r = 8;
	int speed = 1;
	int deltaX, deltaY;//每帧的横纵位移增量

	bool GO;//判断是否发射
	bool isCatched;//判断木板是否接住球

	Ball(int board_width);

	//检测球是否与侧面、顶部、底部窗口边界发生碰撞
	bool SideBorderCollision();
	bool TopBorderCollision();
	bool BottomBorderCollision();

	//检测球是否与木板顶部、侧面发生碰撞
	bool BoardTopCollision(Board &board);
	bool BoardSideCollision(Board &board);

	//检测球是否与某砖块的上下面、侧面发生碰撞
	bool BricksUpDownCollision(Brick &brick);
	bool BricksSideCollision(Brick &brick);

	//预设吃到道具后让球大小变化、速度变化的函数
	void Bigger();
	void Smaller();
	void Faster();
	void Slower();

	//移动球
	void Move(Brick &brick, Board &board);
};



int Gaming()
{
	Brick brick;
	brick.DrawBricks();

	Board board;
	setfillcolor(LIGHTBLUE);
	solidrectangle(board.x, board.y, board.x + board.length, board.y + board.width);

	Ball ball(board.width);
	setfillcolor(LIGHTGRAY);
	solidcircle(ball.x, ball.y, ball.r);

	while (1)
	{
		//如果 木板没接住球 或 已经打碎全部砖块 时 游戏结束
		if (!ball.isCatched || brick.count == breakOut)
		{
			//清除球和木板
			setfillcolor(BLACK);
			solidcircle(ball.x, ball.y, ball.r);
			solidrectangle(board.x, board.y, board.x + board.length, board.y + board.width);

			if (brick.count > breakOut)
				return MessageBox(NULL, "You Lose!", "打砖块", MB_RETRYCANCEL);
			else if (brick.count == breakOut)
				return MessageBox(NULL, "You Win!", "打砖块", MB_RETRYCANCEL);
		}

		if (game_mode == 1)
		{
			if (_kbhit())
			{
				board.Move();
			}
		}
		else if(game_mode==2)
		{
			if (MouseHit())
				m = GetMouseMsg();

			if (m.uMsg == WM_MOUSEMOVE)
			{
				board.Move();
			}
		}
		
		ball.Move(brick, board);
	}
}


int main()
{
	//提示用户选择游戏模式
	cout << "请选择您的游戏模式：" << endl;
	cout << "1.键盘操作模式" << endl;
	cout << "2.鼠标操作模式" << endl;
	cout << "请输入您的选择（1/2):" << endl;

	cin >> game_mode;
	system("pause");
	initgraph(WIDTH, HEIGHT);  //初始化窗口
	while (1)
	{
		if (Gaming() == IDCANCEL)         
			break;
	}
	closegraph();//关闭窗口
	system("cls");
	//在这里加入排行榜显示
	//
	//
	//
	//
	system("pause");
	return 0;
}




//Brick
//Brick
//Brick
Brick::Brick()
{
	memset(bricks, -1, sizeof(bricks));
	count = ROW * COLUMN;
}

Brick::Brick(int level)
{
	for (i = 0; i < ROW; ++i)
	{
		for (j = 0; j < COLUMN; ++j)
		{
			bricks[i][j] = levels[level][i][j];
		}
	}
}

void Brick::DrawBricks()
{
	  //设置砖块颜色
	setlinecolor(BLACK);    //设置边框颜色
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COLUMN; j++)
		{
			if (bricks[i][j] == -1)
			{
				color = 5636095 + i * 30000 + j * 300000;
                                setfillcolor(color);
				fillrectangle(j*length, i*width, (j + 1)*length, (i + 1)*width);
			}
		}
	}
}





//Board
//Board
//Board
Board::Board()
{
	x = WIDTH / 2 - length / 2;
	y = HEIGHT - width;
}

void Board::Longer()
{
	length = 70;
}

void Board::Shorter()
{
	length = 50;
}

void Board::KeyBoardMove()
{
	int ch;         
	ch = _getch();//读取用户按键
	
	setfillcolor(BLACK);//除去原来木板
	solidrectangle(x, y, x + length, y + width);

	switch (ch)
	{
	case 'A':
	case 'a':
		x -= length / 2;
		break;

	case 'D':
	case 'd':
		x += length / 2;
		break;
	}

	//木板移动边界
	if (x <= 0)
		x = 0;
	if (x >= WIDTH - length)
		x = WIDTH - length;

	setfillcolor(LIGHTBLUE);//画出新木板   
	solidrectangle(x, y, x + length, y + width);
}

void Board::MouseMove()
{

	setfillcolor(BLACK);//除去原来木板
	solidrectangle(x, y, x + length, y + width);

	x = m.x;//令 木板横坐标 等于 鼠标横坐标

	//木板移动边界
	if (x <= 0)
		x = 0;
	if (x >= WIDTH - length)
		x = WIDTH - length;

	setfillcolor(LIGHTBLUE);//画出新木板
	solidrectangle(x, y, x + length, y + width);
}

void Board::Move()
{
	if (game_mode==1)
	{
		KeyBoardMove();
	}
	else if(game_mode==2)
	{
		MouseMove();
	}
}





//Ball
//Ball
//Ball
Ball::Ball(int board_width)
{
	x = WIDTH  / 2;
	y = HEIGHT - board_width - r;

	//初始化移动方向和小球状态
	deltaX = 1;
	deltaY = -1;
	GO = 0; 
	isCatched = 1;
}

bool Ball::SideBorderCollision()//右边界或左边界碰撞检测
{
	if (x >= WIDTH  - r || x <= r)
		return true;
	else
		return false;
}

bool Ball::TopBorderCollision()//顶部碰撞检测
{
	if (y <= r)
		return true;
	else
		return false;
}

bool Ball::BottomBorderCollision()//小球掉落出底部检测
{
	if (y >= HEIGHT - r)
		return true;
	else
		return false;
}

bool Ball::BoardTopCollision(Board &board)//木板顶部碰撞检测
{
	if (GO && (x + 1 >= board.x - r) && (x - 1 <= board.x + board.length + r) && (y + 1 >= board.y - r) && (y + 1 <= board.y))
		return true;
	else
		return false;
}

bool Ball::BoardSideCollision(Board &board)//木板侧面碰撞检测
{
	if (GO && (x + 1 >= board.x - r) && (x - 1 <= board.x + board.length + r) && (y + 1 >= board.y - r) && (y <= HEIGHT-r))
		return true;
	else
		return false;
}

bool Ball::BricksUpDownCollision(Brick &brick)//砖块上下面碰撞检测
{
	if ((brick.bricks[i][j] == -1) && (x + 1 >= j * brick.length - r) && (x - 1 <= (j + 1)*brick.length + r) &&
		(y + 1 >= i * brick.width - r) && (y - 1 <= (i + 1)*brick.width + r) && (x + 1 >= j * brick.length) && (x - 1 <= (j + 1)*brick.length))
		return true;
	else
		return false;
}

bool Ball::BricksSideCollision(Brick &brick)//砖块侧面碰撞检测
{
	if ((brick.bricks[i][j] == -1) && (x + 1 >= j * brick.length - r) && (x - 1 <= (j + 1)*brick.length + r) &&
		(y + 1 >= i * brick.width - r) && (y - 1 <= (i + 1)*brick.width + r) && (y + 1 > i*brick.width - r) && (y - 1 < (i + 1)*brick.width))
		return true;
	else
		return false;
}

void Ball::Bigger()
{
	r = 11;
}

void Ball::Smaller()
{
	r = 6;
}

void Ball::Faster()
{
	speed = 1.5;
}

void Ball::Slower()
{
	speed = 0.5;
}

void Ball::Move(Brick &brick, Board &board)
{
	BeginBatchDraw();//批绘图功能

	//碰撞后反射
	if (SideBorderCollision())
		deltaX *= -1;
	if (TopBorderCollision())
		deltaY *= -1;
	//掉落后停止移动
	if (BottomBorderCollision())
	{
		isCatched = 0;
		return;
	}

	//碰撞后反射
	if (BoardTopCollision(board))
		deltaY *= -1;
	if (BoardSideCollision(board))
	{
		deltaX *= -1;
		deltaY *= -1;
	}

	int flag = 0;//初始化 未击中下一个砖块
	for (i = 0; i < ROW && !flag; i++)
	{
		for (j = 0; j < COLUMN && !flag; j++)
		{
			if (BricksUpDownCollision(brick))
			{
				deltaY *= -1;
			}

			else if (BricksSideCollision(brick))
			{
				deltaX *= -1;
			}
			else
				continue;

			brick.bricks[i][j] = 0; //此处砖块被打碎
			breakOut++;             //击碎砖块数加一
			flag = 1;               //击中后跳出循环
			setfillcolor(BLACK);    //清除所打碎的砖块
			fillrectangle(j*brick.length, i*brick.width, (j + 1)*brick.length, (i + 1)*brick.width);
		}
	}

	if (y + 1 < board.y - r)//如果球在木板上方则 小球已经发射（图形窗口中纵坐标自上而下增长）
		GO = 1;   

	//小球位置移动
	setfillcolor(BLACK);
	solidcircle(x, y, r);
	x += deltaX * speed;
	y += deltaY * speed;
	setfillcolor(LIGHTGRAY);
	solidcircle(x, y, r);

	EndBatchDraw();//一次性完成清除图形和重新画图
	Sleep(3);//移动间隔
		

}
