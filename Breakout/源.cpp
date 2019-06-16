#include<graphics.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#include<iostream>
#include<vector>
#include<map>
#include<fstream>
#include<string>
#include<algorithm>
#include<windows.h>
using namespace std;

//定义图形窗口的宽度、高度
const int  WIDTH = 400;
const int  HEIGHT = 600;
//设置砖块行数、列数
#define ROW 5
#define COLUMN 10

int i, j, color;
int levels[5][ROW][COLUMN] = 
{
	{
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1} ,{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
	}
	,{
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,0} ,{-1,-1,-1,-1,-1,-1,-1,-1,0,0},{-1,-1,-1,-1,-1,-1,-1,0,0,0},{-1,-1,-1,-1,-1,-1,0,0,0,0}
	}
	,{
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{0,-1,-1,-1,-1,-1,-1,-1,-1,0} ,{0,-1,-1,-1,-1,-1,-1,-1,0,0},{0,-1,-1,-1,-1,-1,-1,0,0,0},{0,-1,-1,-1,-1,-1,0,0,0,0}
	}
	,{
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{0,-1,-1,-1,-1,-1,-1,-1,-1,0} ,{0,0,-1,-1,-1,-1,-1,-1,0,0},{0,0,0,-1,-1,-1,-1,0,0,0},{0,0,0,0,-1,-1,0,0,0,0}
	}
	,{
		{0,0,0,0,-1,-1,0,0,0,0},{0,0,0,-1,-1,-1,-1,0,0,0},{0,0,-1,-1,-1,-1,-1,-1,0,0},{0,-1,-1,-1,-1,-1,-1,-1,-1,0} ,{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
	}
};//5个关卡 
int breakOut = 0;//记录已经打碎的砖块
int score = 0;//记录得分
int list_judge;
int now_level = 0;
string user_name;//用户名
map<string, int> scorelist;
int game_mode;//记录用户选择的游戏模式
MOUSEMSG m;       //记录鼠标消息



class Brick
{
public:
	const int length = 40;
	const int width = 20;
	int bricks[ROW][COLUMN];
	int count = 0;

	Brick();
	void Level(int level);

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
	bool BoardTopCollision(Board& board);
	bool BoardSideCollision(Board& board);

	//检测球是否与某砖块的上下面、侧面发生碰撞
	bool BricksUpDownCollision(Brick& brick);
	bool BricksSideCollision(Brick& brick);

	//预设吃到道具后让球大小变化、速度变化的函数
	void Bigger();
	void Smaller();
	void Faster();
	void Slower();

	//移动球
	void Move(Brick& brick, Board& board);
};


int Gaming();
void list(string user_name);
int list_out();
void WelcomePage();


int main()
{
	initgraph(400, 600);			//初始化图形界面
	WelcomePage();
	int ch;
	int flag = 0;
	while (1)
	{
		ch = _getch();
		switch (ch)
		{
		case 59:					//F1
			flag = 1;				//进入游戏(键盘）
			game_mode = 1;
			break;
		case 60:	                //F2
			flag = 1;
			game_mode = 2;          //进入游戏(鼠标）
			break;
		case 61:					//F3
			closegraph();
			list_out();
			system("pause");
			system("cls");
			initgraph(400, 600);
			WelcomePage();
			break;
		case 62:					//F3
			closegraph();			//关闭图形界面
			exit(EXIT_SUCCESS);		//退出
			break;
		default:
			break;
		}

		if (flag == 1 || flag == 2)
		{
			break;
		}
	}
	closegraph();

	initgraph(WIDTH, HEIGHT);  //初始化窗口
	while (1)
	{
		if (Gaming() == IDCANCEL)           //点击 取消
			break;
	}
	closegraph();//关闭窗口
	system("cls");

	cout << "你的得分是" << ' ' << score << endl;//排行榜显示
	cout << "是否计入排行榜" << endl;
	cout << "1.是" << endl;
	cout << "2.否" << endl;
	cin >> list_judge;
	if (list_judge == 1)
	{
		cout << "请输入你的名字" << endl;
		cin >> user_name;
		list(user_name);
	}
	return 0;
}



//Brick
//Brick
//Brick
Brick::Brick()
{
	memset(bricks, -1, sizeof(bricks));
	for (i = 0; i < ROW; ++i)
	{
		for (j = 0; j < COLUMN; ++j)
		{
			if (levels[now_level][i][j] == -1)
			{
				count++;
			}
		}
	}
}

void Brick::Level(int level)
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
				fillrectangle(j * length, i * width, (j + 1) * length, (i + 1) * width);
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
	if (game_mode == 1)
	{
		KeyBoardMove();
	}
	else if (game_mode == 2)
	{
		MouseMove();
	}
}





//Ball
//Ball
//Ball
Ball::Ball(int board_width)
{
	x = WIDTH / 2;
	y = HEIGHT - board_width - r;

	//初始化移动方向和小球状态
	deltaX = 1;
	deltaY = -1;
	GO = 0;
	isCatched = 1;
}

bool Ball::SideBorderCollision()//右边界或左边界碰撞检测
{
	if (x >= WIDTH - r || x <= r)
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

bool Ball::BoardTopCollision(Board& board)//木板顶部碰撞检测
{
	if (GO && (x + 1 >= board.x - r) && (x - 1 <= board.x + board.length + r) && (y + 1 >= board.y - r) && (y + 1 <= board.y))
		return true;
	else
		return false;
}

bool Ball::BoardSideCollision(Board& board)//木板侧面碰撞检测
{
	if (GO && (x + 1 >= board.x - r) && (x - 1 <= board.x + board.length + r) && (y + 1 >= board.y - r) && (y <= HEIGHT - r))
		return true;
	else
		return false;
}

bool Ball::BricksUpDownCollision(Brick& brick)//砖块上下面碰撞检测
{
	if ((brick.bricks[i][j] == -1) && (x + 1 >= j * brick.length - r) && (x - 1 <= (j + 1) * brick.length + r) &&
		(y + 1 >= i * brick.width - r) && (y - 1 <= (i + 1) * brick.width + r) && (x + 1 >= j * brick.length) && (x - 1 <= (j + 1) * brick.length))
		return true;
	else
		return false;
}

bool Ball::BricksSideCollision(Brick& brick)//砖块侧面碰撞检测
{
	if ((brick.bricks[i][j] == -1) && (x + 1 >= j * brick.length - r) && (x - 1 <= (j + 1) * brick.length + r) &&
		(y + 1 >= i * brick.width - r) && (y - 1 <= (i + 1) * brick.width + r) && (y + 1 > i * brick.width - r) && (y - 1 < (i + 1) * brick.width))
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
	speed++;
}

void Ball::Slower()
{
	speed = 0.5;
}

void Ball::Move(Brick& brick, Board& board)
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
			fillrectangle(j * brick.length, i * brick.width, (j + 1) * brick.length, (i + 1) * brick.width);
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



int Gaming()
{
	Brick brick;
	brick.Level(now_level);
	brick.DrawBricks();

	Board board;
	setfillcolor(LIGHTBLUE);
	solidrectangle(board.x, board.y, board.x + board.length, board.y + board.width);

	Ball ball(board.width);
	setfillcolor(LIGHTGRAY);
	solidcircle(ball.x, ball.y, ball.r);

	while (1)
	{
		if (breakOut == 25)
		{
			ball.Smaller();
		}
		if (breakOut == 55)
		{
			board.Shorter();
		}
		if (breakOut == 95)
		{
			ball.Faster();
		}
		//如果 木板没接住球 或 已经打碎全部砖块 时 游戏结束
		if (!ball.isCatched || brick.count == breakOut)
		{
			//清除球和木板
			setfillcolor(BLACK);
			solidcircle(ball.x, ball.y, ball.r);
			solidrectangle(board.x, board.y, board.x + board.length, board.y + board.width);
			score = score + breakOut;

			if (brick.count > breakOut)
			{
				return MessageBox(NULL, "You Lose!", "打砖块", MB_RETRYCANCEL);

			}

			else if (brick.count == breakOut)
			{
				now_level++;
				breakOut = 0;
				return MessageBox(NULL, "You Win! 是否进入下一关", "打砖块", MB_OKCANCEL);
			}

		}

		if (game_mode == 1)
		{
			if (_kbhit())
			{
				board.Move();
			}
		}
		else if (game_mode == 2)
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
//排行榜
void list(string user_name)
{
	map<string, int> old_map;
	ifstream ins("list.txt", ios::in);//打开文件
	ofstream ous;

	if (!ins)
	{
		//不存在 新建
		ous.open("list.txt");
	}
	else
	{
		while (!ins.eof())
	    {
		    int score;
		    string name;
		    ins >> name >> score;
		    old_map.insert(make_pair(name, score));
	    }

	}
	

	map<string, int>::iterator it = old_map.find(user_name);//插入
	if (it != old_map.end() && score > it->second)
	{
		it->second = score;
	}
	else if (it == old_map.end())
	{
		old_map.insert(make_pair(user_name, score));

	}
	struct CmpByScore //排序
	{
		bool operator()(const pair<string, int>& lhs, const pair<string, int>& rhs)
		{
			return lhs.second > rhs.second;
		}
	};
	vector<pair<string, int>> old_vec(old_map.begin(), old_map.end());
	sort(old_vec.begin(), old_vec.end(), CmpByScore());
	ofstream out("list.txt", ios::ate);
	for (vector<pair<string, int>>::iterator it2 = old_vec.begin(); it2 != old_vec.end(); ++it2)
	{
		out << it2->first << "     "<< it2->second;
	}
}

int list_out()
{
	map<string, int> new_map;
	ifstream ins("list.txt");
	if (!ins)
	{
		cout << "您是第一个用户。" << endl;
		system("pause");
		ins.close();
		return 0;
	}
	cout<< "用户"<<"     "<< "得分" << endl << endl;
	while (!ins.eof())
	{
		int score;
		string name;
		ins >> name >> score;
		cout << name << "       " << score << endl << endl;
	}
	return 0;
}

void WelcomePage()
{
	setbkcolor(YELLOW);						//设置背景色
	cleardevice();								//刷新背景

	settextcolor(BLACK);
	settextstyle(60, 0, "黑体");
	outtextxy(80, 60, "打 砖 块");

	setfillcolor(BLUE);						//设置填充颜色为黄色
	solidroundrect(90, 250, 320, 290, 10, 10);	//开始游戏(键盘)选项
	solidroundrect(90, 300, 320, 340, 10, 10);	//开始游戏(鼠标)选项
	solidroundrect(90, 350, 320, 390, 10, 10);	//排行榜选项
	solidroundrect(90, 400, 320, 440, 10, 10); //退出选项

	setbkcolor(BLUE);							//设置文字的背景色
	setbkmode(OPAQUE);							//文字输出时的背景模式（背景用当前背景色填充）
	settextcolor(WHITE);							//设置文字颜色
	settextstyle(20, 0, "黑体");					//设置字体大小，模式
	outtextxy(110, 260, "开始游戏(键盘)   F1");			//打印选项
	outtextxy(110, 310, "开始游戏(鼠标)   F2");
	outtextxy(110, 360, "排行榜           F3");
	outtextxy(110, 410, "退出             F4");
}
