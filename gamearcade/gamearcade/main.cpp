#include <iostream>
#include <windows.h>
#include <string>
#include <conio.h>
#include <time.h>
#include <fstream>
using namespace std;

int xChar1 = 1;
int yChar1 = 1;
int xChar2 = 18;
int yChar2 = 3;
int l = 1;
int score11 = 0;
int score22 = 0;
int fire = 1;
int level = 1;
int Hub = 1;
bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP1 = 1, LEFT1, RIGHT1, UP1, DOWN1 };
enum eDir { STOP = 0, LEFT = 1, UPLEFT = 2, DOWNLEFT = 3, RIGHT = 4, UPRIGHT = 5, DOWNRIGHT = 6 };
eDirection dir;
void Setup()
{
	gameOver = false;
	dir = STOP1;
	x = width / 2;
	y = height / 2;
	fruitX = rand() % width;
	fruitY = rand() % height;
	score = 0;
	tailX == 0;
	tailY == 0;
}
void Draw()
{
	system("cls"); //system("clear");
	cout << "Snake   " << "By:" << "Erik Nilsson" << endl;
	for (int i = 0; i < width + 2; i++)
		cout << "#";
	cout << endl;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (j == 0)
				cout << "#";
			if (i == y && j == x)
				cout << "O";
			else if (i == fruitY && j == fruitX)
				cout << "F";
			else
			{
				bool print = false;
				for (int k = 0; k < nTail; k++)
				{
					if (tailX[k] == j && tailY[k] == i)
					{
						cout << "o";
						print = true;
					}
				}
				if (!print)
					cout << " ";
			}


			if (j == width - 1)
				cout << "#";
		}
		cout << endl;
	}

	for (int i = 0; i < width + 2; i++)
		cout << "#";
	cout << endl;
	cout << "Score:" << score << endl;
}
void Input()
{
	if (_kbhit())
	{
		switch (_getch())
		{
		case 'a':
			dir = LEFT1;
			break;
		case 'd':
			dir = RIGHT1;
			break;
		case 'w':
			dir = UP1;
			break;
		case 's':
			dir = DOWN1;
			break;
		case 'x':
			gameOver = true;
			break;
		}
	}
}
void Logic()
{
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;
	for (int i = 1; i < nTail; i++)
	{
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}
	switch (dir)
	{
	case LEFT1:
		x--;
		break;
	case RIGHT1:
		x++;
		break;
	case UP1:
		y--;
		break;
	case DOWN1:
		y++;
		break;
	default:
		break;
	}
	//if (x > width || x < 0 || y > height || y < 0)
	//gameOver = true;
	//walls that kill you
	for (int i = 0; i < nTail; i++)
		if (tailX[i] == x && tailY[i] == y)
			gameOver = true;
	if (x == fruitX && y == fruitY)
	{
		score += 10;
		fruitX = rand() % width;
		fruitY = rand() % height;
		nTail++;
	}
	if (x >= width) x = 0; else if (x < 0) x = width - 1;
	if (y >= height) y = 0; else if (y < 0) y = height - 1;
	//walls that don't kill you
}
class cBall
{
private:
	int x, y;
	int originalX, originalY;
	eDir direction;
public:
	cBall(int posX, int posY)
	{
		originalX = posX;
		originalY = posY;
		x = posX; y = posY;
		direction = STOP;
	}
	void Reset()
	{
		x = originalX; y = originalY;
		direction = STOP;
	}
	void changeDirection(eDir d)
	{
		direction = d;
	}
	void randomDirection()
	{
		direction = (eDir)((rand() % 6) + 1);
	}
	inline int getX() { return x; }
	inline int getY() { return y; }
	inline eDir getDirection() { return direction; }
	void Move()
	{
		switch (direction)
		{
		case STOP:
			break;
		case LEFT:
			x--;
			break;
		case RIGHT:
			x++;
			break;
		case UPLEFT:
			x--; y--;
			break;
		case DOWNLEFT:
			x--; y++;
			break;
		case UPRIGHT:
			x++; y--;
			break;
		case DOWNRIGHT:
			x++; y++;
			break;
		default:
			break;
		}
	}
	friend ostream & operator<<(ostream & o, cBall c)
	{
		o << "Ball [" << c.x << "," << c.y << "][" << c.direction << "]";
		return o;
	}
};
class cPaddle
{
private:
	int x, y;
	int originalX, originalY;
public:
	cPaddle()
	{
		x = y = 0;
	}
	cPaddle(int posX, int posY) : cPaddle()
	{
		originalX = posX;
		originalY = posY;
		x = posX;
		y = posY;
	}
	inline void Reset() { x = originalX; y = originalY; }
	inline int getX() { return x; }
	inline int getY() { return y; }
	inline void moveUp() { y--; }
	inline void moveDown() { y++; }
	friend ostream & operator<<(ostream & o, cPaddle c)
	{
		o << "Paddle [" << c.x << "," << c.y << "]";
		return o;
	}
};
class cGameManger
{
private:
	int width, height;
	int score1, score2;
	char up1, down1, up2, down2;
	bool quit;
	cBall * ball;
	cPaddle *player1;
	cPaddle *player2;
public:
	cGameManger(int w, int h)
	{
		srand(time(NULL));
		quit = false;
		up1 = 'w'; up2 = 'i';
		down1 = 's'; down2 = 'k';
		score1 = score2 = 0;
		width = w; height = h;
		ball = new cBall(w / 2, h / 2);
		player1 = new cPaddle(1, h / 2 - 3);
		player2 = new cPaddle(w - 2, h / 2 - 3);
	}
	~cGameManger()
	{
		delete ball, player1, player2;
	}
	void ScoreUp(cPaddle * player)
	{
		if (player == player1)
			score1++;
		else if (player == player2)
			score2++;

		ball->Reset();
		player1->Reset();
		player2->Reset();
	}
	void Draw()
	{
		system("cls");
		for (int i = 0; i < width + 2; i++)
			cout << "\xB2";
		cout << endl;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				int ballx = ball->getX();
				int bally = ball->getY();
				int player1x = player1->getX();
				int player2x = player2->getX();
				int player1y = player1->getY();
				int player2y = player2->getY();

				if (j == 0)
					cout << "\xB2";

				if (ballx == j && bally == i)
					cout << "O"; //ball
				else if (player1x == j && player1y == i)
					cout << "\xDB"; //player1
				else if (player2x == j && player2y == i)
					cout << "\xDB"; //player2

				else if (player1x == j && player1y + 1 == i)
					cout << "\xDB"; //player1
				else if (player1x == j && player1y + 2 == i)
					cout << "\xDB"; //player1
				else if (player1x == j && player1y + 3 == i)
					cout << "\xDB"; //player1

				else if (player2x == j && player2y + 1 == i)
					cout << "\xDB"; //player1
				else if (player2x == j && player2y + 2 == i)
					cout << "\xDB"; //player1
				else if (player2x == j && player2y + 3 == i)
					cout << "\xDB"; //player1
				else
					cout << " ";

				if (j == width - 1)
					cout << "\xB2";
			}
			cout << endl;
		}

		for (int i = 0; i < width + 2; i++)
			cout << "\xB2";
		cout << endl;

		cout << "Score 1: " << score1 << endl << "Score 2: " << score2 << endl;
	}
	void Input()
	{
		ball->Move();

		int ballx = ball->getX();
		int bally = ball->getY();
		int player1x = player1->getX();
		int player2x = player2->getX();
		int player1y = player1->getY();
		int player2y = player2->getY();

		if (_kbhit())
		{
			char current = _getch();
			if (current == up1)
				if (player1y > 0)
					player1->moveUp();
			if (current == up2)
				if (player2y > 0)
					player2->moveUp();
			if (current == down1)
				if (player1y + 4 < height)
					player1->moveDown();
			if (current == down2)
				if (player2y + 4 < height)
					player2->moveDown();

			if (ball->getDirection() == STOP)
				ball->randomDirection();

			if (current == 'q')
				quit = true;
		}
	}
	void Logic()
	{
		int ballx = ball->getX();
		int bally = ball->getY();
		int player1x = player1->getX();
		int player2x = player2->getX();
		int player1y = player1->getY();
		int player2y = player2->getY();

		//left paddle
		for (int i = 0; i < 4; i++)
			if (ballx == player1x + 1)
				if (bally == player1y + i)
					ball->changeDirection((eDir)((rand() % 3) + 4));

		//right paddle
		for (int i = 0; i < 4; i++)
			if (ballx == player2x - 1)
				if (bally == player2y + i)
					ball->changeDirection((eDir)((rand() % 3) + 1));

		//bottom wall
		if (bally == height - 1)
			ball->changeDirection(ball->getDirection() == DOWNRIGHT ? UPRIGHT : UPLEFT);
		//top wall
		if (bally == 0)
			ball->changeDirection(ball->getDirection() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);
		//right wall
		if (ballx == width - 1)
			ScoreUp(player1);
		//left wall
		if (ballx == 0)
			ScoreUp(player2);
	}
	void Run()
	{
		while (!quit)
		{
			Draw();
			Input();
			Logic();
			Sleep(40);
		}
	}
};
void level1()
{
	int xCharacter = 1;
	int yCharacter = 1;
	char Map1[10][11] = {
		"##########",
		"#@#   ##!#",
		"# # # ## #",
		"# # #  # #",
		"#   #### #",
		"# #      #",
		"# ###### #",
		"# #   ## #",
		"#   #    #",
		"##########",
	};
	system("cls");
	cout << "use arrow keys to move" << endl;
	cout << "player: @" << endl;
	cout << "end: !" << endl;
	Sleep(4000);
	while (level = 1)
	{
		Sleep(60);
		system("cls");
		cout << "level 1" << endl;
		for (int y = 0; y < 10; y++)
		{
			cout << Map1[y] << endl;
		}
		if (GetAsyncKeyState(VK_UP))
		{
			if (Map1[(yCharacter - 1)][xCharacter] == ' ')
			{
				Map1[yCharacter][xCharacter] = ' ';
				yCharacter -= 1;
				Map1[yCharacter][xCharacter] = '@';
			}
			if (Map1[(yCharacter - 1)][xCharacter] == '!')
			{
				cout << "YOU WIN" << endl;
				level = 2;
				break;
			}
		}
		else if (GetAsyncKeyState(VK_DOWN))
		{
			if (Map1[(yCharacter + 1)][xCharacter] == ' ')
			{
				Map1[yCharacter][xCharacter] = ' ';
				yCharacter += 1;
				Map1[yCharacter][xCharacter] = '@';
			}
			if (Map1[(yCharacter + 1)][xCharacter] == '!')
			{
				cout << "YOU WIN" << endl;
				level = 2;
				break;
			}
		}
		else if (GetAsyncKeyState(VK_RIGHT))
		{
			if (Map1[yCharacter][(xCharacter + 1)] == ' ')
			{
				Map1[yCharacter][xCharacter] = ' ';
				xCharacter += 1;
				Map1[yCharacter][xCharacter] = '@';
			}
			if (Map1[yCharacter][(xCharacter + 1)] == '!')
			{
				cout << "YOU WIN" << endl;
				level = 2;
				break;
			}
		}
		else if (GetAsyncKeyState(VK_LEFT))
		{
			if (Map1[yCharacter][(xCharacter - 1)] == ' ')
			{
				Map1[yCharacter][xCharacter] = ' ';
				xCharacter -= 1;
				Map1[yCharacter][xCharacter] = '@';
			}
			if (Map1[yCharacter][(xCharacter - 1)] == '!')
			{
				cout << "YOU WIN" << endl;
				level = 2;
				break;
			}
		}
		cout << "Maze By: Erik Nilsson" << endl;
	}
};
void level2()
{
	int xCharacter = 1;
	int yCharacter = 1;
	char Map2[10][21] = {
		"####################",
		"#@     #     #    !#",
		"# #### # ### # #####",
		"#   #### #   #     #",
		"# # #    ##### #####",
		"# ### ####     #   #",
		"# #      # ##### # #",
		"# ###### # ####### #",
		"#                  #",
		"####################",
	};
	while (level = 2)
	{
		Sleep(60);
		system("cls");
		cout << "level 2" << endl;
		for (int y = 0; y < 10; y++)
		{
			cout << Map2[y] << endl;
		}
		if (GetAsyncKeyState(VK_UP))
		{
			if (Map2[(yCharacter - 1)][xCharacter] == ' ')
			{
				Map2[yCharacter][xCharacter] = ' ';
				yCharacter -= 1;
				Map2[yCharacter][xCharacter] = '@';
			}
			if (Map2[(yCharacter - 1)][xCharacter] == '!')
			{
				cout << "YOU WIN" << endl;
				level = 3;
				break;
			}
		}
		else if (GetAsyncKeyState(VK_DOWN))
		{
			if (Map2[(yCharacter + 1)][xCharacter] == ' ')
			{
				Map2[yCharacter][xCharacter] = ' ';
				yCharacter += 1;
				Map2[yCharacter][xCharacter] = '@';
			}
			if (Map2[(yCharacter + 1)][xCharacter] == '!')
			{
				cout << "YOU WIN" << endl;
				level = 3;
				break;
			}
		}
		else if (GetAsyncKeyState(VK_RIGHT))
		{
			if (Map2[yCharacter][(xCharacter + 1)] == ' ')
			{
				Map2[yCharacter][xCharacter] = ' ';
				xCharacter += 1;
				Map2[yCharacter][xCharacter] = '@';
			}
			if (Map2[yCharacter][(xCharacter + 1)] == '!')
			{
				cout << "YOU WIN" << endl;
				level = 3;
				break;
			}
		}
		else if (GetAsyncKeyState(VK_LEFT))
		{
			if (Map2[yCharacter][(xCharacter - 1)] == ' ')
			{
				Map2[yCharacter][xCharacter] = ' ';
				xCharacter -= 1;
				Map2[yCharacter][xCharacter] = '@';
			}
			if (Map2[yCharacter][(xCharacter - 1)] == '!')
			{
				cout << "YOU WIN" << endl;
				level = 3;
				break;
			}
		}
		cout << "Maze By: Erik Nilsson" << endl;
	}
};
void level3()
{
	int xCharacter = 1;
	int yCharacter = 1;
	char Map3[10][31] = {
		"##############################",
		"#@               #   #      !#",
		"####### ######## # # # #######",
		"#       #        # ### ### # #",
		"# ####### ######## #    ## # #",
		"# #   #   ######## #  # ## # #",
		"# # # #####           #    # #",
		"# # # #   ########### ###### #",
		"#   #   #                    #",
		"##############################",
	};
	while (level = 3)
	{
		Sleep(60);
		system("cls");
		cout << "Level 3" << endl;
		for (int y = 0; y < 10; y++)
		{
			cout << Map3[y] << endl;
		}
		if (GetAsyncKeyState(VK_UP))
		{
			if (Map3[(yCharacter - 1)][xCharacter] == ' ')
			{
				Map3[yCharacter][xCharacter] = ' ';
				yCharacter -= 1;
				Map3[yCharacter][xCharacter] = '@';
			}
			if (Map3[(yCharacter - 1)][xCharacter] == '!')
			{
				cout << "YOU WIN" << endl;
				level = 4;
				break;
			}
		}
		else if (GetAsyncKeyState(VK_DOWN))
		{
			if (Map3[(yCharacter + 1)][xCharacter] == ' ')
			{
				Map3[yCharacter][xCharacter] = ' ';
				yCharacter += 1;
				Map3[yCharacter][xCharacter] = '@';
			}
			if (Map3[(yCharacter + 1)][xCharacter] == '!')
			{
				cout << "YOU WIN" << endl;
				level = 4;
				break;
			}
		}
		else if (GetAsyncKeyState(VK_RIGHT))
		{
			if (Map3[yCharacter][(xCharacter + 1)] == ' ')
			{
				Map3[yCharacter][xCharacter] = ' ';
				xCharacter += 1;
				Map3[yCharacter][xCharacter] = '@';
			}
			if (Map3[yCharacter][(xCharacter + 1)] == '!')
			{
				cout << "YOU WIN" << endl;
				level = 4;
				break;
			}
		}
		else if (GetAsyncKeyState(VK_LEFT))
		{
			if (Map3[yCharacter][(xCharacter - 1)] == ' ')
			{
				Map3[yCharacter][xCharacter] = ' ';
				xCharacter -= 1;
				Map3[yCharacter][xCharacter] = '@';
			}
			if (Map3[yCharacter][(xCharacter - 1)] == '!')
			{
				cout << "YOU WIN" << endl;
				level = 4;
				break;
			}
		}
		cout << "Maze By: Erik Nilsson" << endl;
	}
};
void level4()
{
	int xCharacter = 1;
	int yCharacter = 1;
	char Map4[20][31] = {
		"##############################",
		"#@           #             #!#",
		"### ######## # ############# #",
		"#   #        # #             #",
		"# ### ######## # #############",
		"# ###          #             #",
		"# ######### ################ #",
		"#####       # #            # #",
		"#   # ####### # ############ #",
		"# # # #       # #          # #",
		"# #   # ####### # ########## #",
		"# # ### ####### # #       ## #",
		"# # #   #       # # #####    #",
		"# # # ### ####### #     ######",
		"# #               ### #      #",
		"# ################### ###### #",
		"# #                   #      #",
		"# # # ####### ######### ######",
		"#   #         #              #",
		"##############################",
	};
	while (level = 4)
	{
		Sleep(60);
		system("cls");
		cout << "level 4" << endl;
		for (int y = 0; y < 20; y++)
		{
			cout << Map4[y] << endl;
		}
		if (GetAsyncKeyState(VK_UP))
		{
			if (Map4[(yCharacter - 1)][xCharacter] == ' ')
			{
				Map4[yCharacter][xCharacter] = ' ';
				yCharacter -= 1;
				Map4[yCharacter][xCharacter] = '@';
			}
			if (Map4[(yCharacter - 1)][xCharacter] == '!')
			{
				cout << "YOU WIN" << endl;
				level = 5;
				break;
			}
		}
		else if (GetAsyncKeyState(VK_DOWN))
		{
			if (Map4[(yCharacter + 1)][xCharacter] == ' ')
			{
				Map4[yCharacter][xCharacter] = ' ';
				yCharacter += 1;
				Map4[yCharacter][xCharacter] = '@';
			}
			if (Map4[(yCharacter + 1)][xCharacter] == '!')
			{
				cout << "YOU WIN" << endl;
				level = 5;
				break;
			}
		}
		else if (GetAsyncKeyState(VK_RIGHT))
		{
			if (Map4[yCharacter][(xCharacter + 1)] == ' ')
			{
				Map4[yCharacter][xCharacter] = ' ';
				xCharacter += 1;
				Map4[yCharacter][xCharacter] = '@';
			}
			if (Map4[yCharacter][(xCharacter + 1)] == '!')
			{
				cout << "YOU WIN" << endl;
				level = 5;
				break;
			}
		}
		else if (GetAsyncKeyState(VK_LEFT))
		{
			if (Map4[yCharacter][(xCharacter - 1)] == ' ')
			{
				Map4[yCharacter][xCharacter] = ' ';
				xCharacter -= 1;
				Map4[yCharacter][xCharacter] = '@';
			}
			if (Map4[yCharacter][(xCharacter - 1)] == '!')
			{
				cout << "YOU WIN" << endl;
				level = 5;
				break;
			}
		}
		cout << "Maze By: Erik Nilsson" << endl;
	}
};
void level5()
{
	int xCharacter = 1;
	int yCharacter = 1;
	char Map5[20][40] = {
		"#######################################",
		"#@           #       ####           #!#",
		"# # ############## # #### # ####### # #",
		"# #        #       #      #       # # #",
		"# ######## ##### ######## ####### ### #",
		"# ###        #   #      # #     # #   #",
		"# ############ ### ####   # ###   # ###",
		"# #   #        #   #  ### #   ##### # #",
		"# # # # ############ ## # ###       # #",
		"#   # #           ## #  # ########### #",
		"# ### # ######### ## #  #             #",
		"# #   # #       #    #  ###############",
		"# # ### ####### ######          #   # #",
		"# # #     #   #    ### ########## # # #",
		"# # ##### # # #### ###            # # #",
		"### #       #      ################ # #",
		"#   # # ##########        #   #     # #",
		"# ### # ######   ######## # # # ##### #",
		"#     #        #        #   #         #",
		"#######################################",
	};
	while (level = 5)
	{
		Sleep(60);
		system("cls");
		cout << "Level 5" << endl;
		for (int y = 0; y < 20; y++)
		{
			cout << Map5[y] << endl;
		}
		if (GetAsyncKeyState(VK_UP))
		{
			if (Map5[(yCharacter - 1)][xCharacter] == ' ')
			{
				Map5[yCharacter][xCharacter] = ' ';
				yCharacter -= 1;
				Map5[yCharacter][xCharacter] = '@';
			}
			if (Map5[(yCharacter - 1)][xCharacter] == '!')
			{
				cout << "YOU WIN" << endl;
				level = 6;
				break;
			}
		}
		else if (GetAsyncKeyState(VK_DOWN))
		{
			if (Map5[(yCharacter + 1)][xCharacter] == ' ')
			{
				Map5[yCharacter][xCharacter] = ' ';
				yCharacter += 1;
				Map5[yCharacter][xCharacter] = '@';
			}
			if (Map5[(yCharacter + 1)][xCharacter] == '!')
			{
				cout << "YOU WIN" << endl;
				level = 6;
				break;
			}
		}
		else if (GetAsyncKeyState(VK_RIGHT))
		{
			if (Map5[yCharacter][(xCharacter + 1)] == ' ')
			{
				Map5[yCharacter][xCharacter] = ' ';
				xCharacter += 1;
				Map5[yCharacter][xCharacter] = '@';
			}
			if (Map5[yCharacter][(xCharacter + 1)] == '!')
			{
				cout << "YOU WIN" << endl;
				level = 6;
				break;
			}
		}
		else if (GetAsyncKeyState(VK_LEFT))
		{
			if (Map5[yCharacter][(xCharacter - 1)] == ' ')
			{
				Map5[yCharacter][xCharacter] = ' ';
				xCharacter -= 1;
				Map5[yCharacter][xCharacter] = '@';
			}
			if (Map5[yCharacter][(xCharacter - 1)] == '!')
			{
				cout << "YOU WIN" << endl;
				level = 6;
				break;
			}
		}
		cout << "Maze By: Erik Nilsson" << endl;
	}
};
void shooter()
{
	int xCharacter1 = 1;
	int yCharacter1 = 1;
	int xCharacter2 = 18;
	int yCharacter2 = 3;
	int xBullet1 = 2;
	int yBullet1;
	int xBullet2 = 17;
	int yBullet2;
	int pos1 = 1;
	int pos2 = 2;
	int pos3 = 3;
	int pos12 = 1;
	int pos22 = 2;
	int pos32 = 3;
	int bullet11 = 0;
	int bullet22 = 0;
	int score1 = 0;
	int score2 = 0;
	char Map[5][21] = {
		"####################",
		"#                  #",
		"#                  #",
		"#                  #",
		"####################",
	};
	xCharacter1 = xChar1;
	yCharacter1 = yChar1;
	xCharacter2 = xChar2;
	yCharacter2 = yChar2;
	score1 = score11;
	score2 = score22;
	while (l = 1)
	{
		if (score1 == 30)
		{
			while (1)
			{
				system("cls");
				cout << "Player 1 Wins!" << endl;
				Sleep(1000000);
			}
		}
		if (score2 == 30)
		{
			while (1)
			{
				system("cls");
				cout << "Player 2 Wins!" << endl;
				Sleep(1000000);
			}
		}
		xChar1 = xCharacter1;
		yChar1 = yCharacter1;
		xChar2 = xCharacter2;
		yChar2 = yCharacter2;
		score11 = score1;
		score22 = score2;
		if (bullet11 == 1)
		{
			if (Map[yBullet1][(xBullet1 + 1)] == ' ')
			{
				Map[yBullet1][xBullet1] = ' ';
				xBullet1 += 1;
				Map[yBullet1][xBullet1] = '-';
			}
			if (Map[yBullet1][(xBullet1 + 1)] == '#')
			{
				Map[yBullet1][xBullet1] = ' ';
				xBullet1 = 2;
				bullet11 = 0;
				shooter();
			}
			if (Map[yBullet1][(xBullet1 + 1)] == 'O')
			{
				score1 += 1;
				score11 = score1;
				shooter();
			}
			if (Map[yBullet1][(xBullet1 + 1)] == '-')
			{
				Map[yBullet1][xBullet1] = ' ';
				xBullet1 += 1;
				Map[yBullet1][xBullet1] = '-';
			}
		}
		if (bullet22 == 1)
		{
			if (Map[yBullet2][(xBullet2 - 1)] == ' ')
			{
				Map[yBullet2][xBullet2] = ' ';
				xBullet2 -= 1;
				Map[yBullet2][xBullet2] = '-';
			}
			if (Map[yBullet2][(xBullet2 - 1)] == '#')
			{
				Map[yBullet2][xBullet2] = ' ';
				xBullet2 = 17;
				bullet22 = 0;
				shooter();
			}
			if (Map[yBullet2][(xBullet2 - 1)] == '@')
			{
				score2 += 1;
				score22 = score2;
				shooter();
			}
			if (Map[yBullet2][(xBullet2 - 1)] == '-')
			{
				Map[yBullet1][xBullet1] = ' ';
				xBullet1 -= 1;
				Map[yBullet1][xBullet1] = '-';
			}
		}
		Sleep(30);
		system("cls");
		for (int y = 0; y < 5; y++)
		{
			cout << Map[y] << endl;
		}
		cout << "Player 1: " << score1 << endl;
		cout << "Player 2: " << score2 << endl;
		Map[yCharacter1][xCharacter1] = '@';
		Map[yCharacter2][xCharacter2] = 'O';
		if (GetAsyncKeyState(VK_LSHIFT))
		{
			if (Map[(yCharacter1 - 1)][xCharacter1] == ' ')
			{
				Map[yCharacter1][xCharacter1] = ' ';
				yCharacter1 -= 1;
				Map[yCharacter1][xCharacter1] = '@';
			}
		}
		if (GetAsyncKeyState(VK_LCONTROL))
		{
			if (Map[(yCharacter1 + 1)][xCharacter1] == ' ')
			{
				Map[yCharacter1][xCharacter1] = ' ';
				yCharacter1 += 1;
				Map[yCharacter1][xCharacter1] = '@';
			}
		}
		if (bullet11 == 0)
		{
			if (fire == 1)
			{
				if (yCharacter1 == pos1)
				{
					yBullet1 = 1;
					if (Map[yBullet1][(xBullet1 + 1)] == ' ')
					{
						bullet11 = 1;
					}
				}
				if (yCharacter1 == pos2)
				{
					yBullet1 = 2;
					if (Map[yBullet1][(xBullet1 + 1)] == ' ')
					{
						bullet11 = 1;
					}
				}
				if (yCharacter1 == pos3)
				{
					yBullet1 = 3;
					if (Map[yBullet1][(xBullet1 + 1)] == ' ')
					{
						bullet11 = 1;
					}
				}
			}
		}

		if (GetAsyncKeyState(VK_UP))
		{
			if (Map[(yCharacter2 - 1)][xCharacter2] == ' ')
			{
				Map[yCharacter2][xCharacter2] = ' ';
				yCharacter2 -= 1;
				Map[yCharacter2][xCharacter2] = 'O';
			}
		}
		if (GetAsyncKeyState(VK_DOWN))
		{
			if (Map[(yCharacter2 + 1)][xCharacter2] == ' ')
			{
				Map[yCharacter2][xCharacter2] = ' ';
				yCharacter2 += 1;
				Map[yCharacter2][xCharacter2] = 'O';
			}
		}
		if (bullet22 == 0)
		{
			if (fire == 1)
			{
				if (yCharacter2 == pos12)
				{
					yBullet2 = 1;
					if (Map[yBullet2][(xBullet2 - 1)] == ' ')
					{
						bullet22 = 1;
					}
				}
				if (yCharacter2 == pos22)
				{
					yBullet2 = 2;
					if (Map[yBullet2][(xBullet2 - 1)] == ' ')
					{
						bullet22 = 1;
					}
				}
				if (yCharacter2 == pos32)
				{
					yBullet2 = 3;
					if (Map[yBullet2][(xBullet2 - 1)] == ' ')
					{
						bullet22 = 1;
					}
				}
			}
		}
	}
};
void hunt3()
{
	int xCharacter = 15;
	int yCharacter = 3;
	int xE1 = 28;
	int yE1 = 3;
	int xE2 = 1;
	int yE2 = 5;
	int xE3 = 28;
	int yE3 = 5;
	int xE4 = 14;
	int yE4 = 7;
	int xE5 = 2;
	int yE5 = 15;
	int xE6 = 28;
	int yE6 = 16;
	int xE7 = 2;
	int yE7 = 17;
	int xE8 = 28;
	int yE8 = 18;
	int turn1 = 1;
	int turn2 = 1;
	int turn3 = 1;
	int turn4 = 1;
	int turn5 = 1;
	int turn6 = 1;
	int turn7 = 1;
	int turn8 = 1;
	int loop = 1;
	char Map3[20][31] = {
		"##############################",
		"#                            #",
		"# ##### ####### ###### ##### #",
		"# #           #@#          #E#",
		"### ###################### ###",
		"#E                          E#",
		"# ############################",
		"#              #             #",
		"# ############## ########### #",
		"#              #           # #",
		"############## ########### # #",
		"#              #           # #",
		"# ############## ########### #",
		"#                ########### #",
		"############################ #",
		"#!                           #",
		"#!                           #",
		"#!                           #",
		"#!                           #",
		"##############################",
	};
	while (loop = 1)
	{
		Sleep(60);
		system("cls");
		cout << yCharacter << " " << xCharacter << endl;
		for (int i = 0; i < 20; i++)
		{
			cout << Map3[i] << endl;
		}
		if (GetAsyncKeyState(VK_UP))
		{
			if (Map3[(yCharacter - 1)][xCharacter] == ' ')
			{
				Map3[yCharacter][xCharacter] = ' ';
				yCharacter -= 1;
				Map3[yCharacter][xCharacter] = '@';
			}
			if (Map3[(yCharacter - 1)][xCharacter] == '!')
			{
				while (1)
				{
					system("cls");
					cout << "You Win!" << endl;
					Sleep(1000000);
				}
			}
		}
		if (GetAsyncKeyState(VK_DOWN))
		{
			if (Map3[(yCharacter + 1)][xCharacter] == ' ')
			{
				Map3[yCharacter][xCharacter] = ' ';
				yCharacter += 1;
				Map3[yCharacter][xCharacter] = '@';
			}
			if (Map3[(yCharacter + 1)][xCharacter] == '!')
			{
				while (1)
				{
					system("cls");
					cout << "You Win!" << endl;
					Sleep(1000000);
				}
			}
		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			if (Map3[yCharacter][(xCharacter + 1)] == ' ')
			{
				Map3[yCharacter][xCharacter] = ' ';
				xCharacter += 1;
				Map3[yCharacter][xCharacter] = '@';
			}
			if (Map3[yCharacter][(xCharacter + 1)] == '!')
			{
				while (1)
				{
					system("cls");
					cout << "You Win!" << endl;
					Sleep(1000000);
				}
			}
		}
		if (GetAsyncKeyState(VK_LEFT))
		{
			if (Map3[yCharacter][(xCharacter - 1)] == ' ')
			{
				Map3[yCharacter][xCharacter] = ' ';
				xCharacter -= 1;
				Map3[yCharacter][xCharacter] = '@';
			}
			if (Map3[yCharacter][(xCharacter - 1)] == '!')
			{
				while (1)
				{
					system("cls");
					cout << "You Win!" << endl;
					Sleep(1000000);
				}
			}
		}
		if (turn1 == 1)
		{
			if (Map3[(yE1 - 1)][xE1] == ' ')
			{
				Map3[yE1][xE1] = ' ';
				yE1 -= 1;
				Map3[yE1][xE1] = 'E';
			}
			if (Map3[(yE1 - 1)][xE1] == 'E')
			{
				Map3[yE1][xE1] = ' ';
				yE1 -= 1;
				Map3[yE1][xE1] = 'E';
			}
			if (Map3[(yE1 - 1)][xE1] == '#')
			{
				turn1 = 2;
			}
			if (Map3[(yE1 - 1)][xE1] == '@')
			{
				hunt3();
			}
		}
		if (turn1 == 2)
		{
			if (Map3[yE1][(xE1 - 1)] == ' ')
			{
				Map3[yE1][xE1] = ' ';
				xE1 -= 1;
				Map3[yE1][xE1] = 'E';
			}
			if (Map3[yE1][(xE1 - 1)] == 'E')
			{
				Map3[yE1][xE1] = ' ';
				xE1 -= 1;
				Map3[yE1][xE1] = 'E';
			}
			if (Map3[yE1][(xE1 - 1)] == '#')
			{
				turn1 = 3;
			}
			if (Map3[yE1][(xE1 - 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn1 == 3)
		{
			if (Map3[(yE1 + 1)][xE1] == ' ')
			{
				Map3[yE1][xE1] = ' ';
				yE1 += 1;
				Map3[yE1][xE1] = 'E';
			}
			if (Map3[(yE1 + 1)][xE1] == 'E')
			{
				Map3[yE1][xE1] = ' ';
				yE1 += 1;
				Map3[yE1][xE1] = 'E';
			}
			if (Map3[(yE1 + 1)][xE1] == '#')
			{
				turn1 = 4;
			}
			if (Map3[(yE1 + 1)][xE1] == '@')
			{
				hunt3();
			}
		}
		if (turn1 == 4)
		{
			if (Map3[(yE1 - 1)][xE1] == ' ')
			{
				Map3[yE1][xE1] = ' ';
				yE1 -= 1;
				Map3[yE1][xE1] = 'E';
			}
			if (Map3[(yE1 - 1)][xE1] == 'E')
			{
				Map3[yE1][xE1] = ' ';
				yE1 -= 1;
				Map3[yE1][xE1] = 'E';
			}
			if (Map3[(yE1 - 1)][xE1] == '#')
			{
				turn1 = 5;
			}
			if (Map3[(yE1 - 1)][xE1] == '@')
			{
				hunt3();
			}
		}
		if (turn1 == 5)
		{
			if (Map3[yE1][(xE1 + 1)] == ' ')
			{
				Map3[yE1][xE1] = ' ';
				xE1 += 1;
				Map3[yE1][xE1] = 'E';
			}
			if (Map3[yE1][(xE1 + 1)] == 'E')
			{
				Map3[yE1][xE1] = ' ';
				xE1 += 1;
				Map3[yE1][xE1] = 'E';
			}
			if (Map3[yE1][(xE1 + 1)] == '#')
			{
				turn1 = 6;
			}
			if (Map3[yE1][(xE1 + 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn1 == 6)
		{
			if (Map3[(yE1 + 1)][xE1] == ' ')
			{
				Map3[yE1][xE1] = ' ';
				yE1 += 1;
				Map3[yE1][xE1] = 'E';
			}
			if (Map3[(yE1 + 1)][xE1] == 'E')
			{
				Map3[yE1][xE1] = ' ';
				yE1 += 1;
				Map3[yE1][xE1] = 'E';
			}
			if (Map3[(yE1 + 1)][xE1] == '#')
			{
				turn1 = 1;
			}
			if (Map3[(yE1 + 1)][xE1] == '@')
			{
				hunt3();
			}
		}
		if (turn2 == 1)
		{
			if (Map3[yE2][(xE2 + 1)] == ' ')
			{
				Map3[yE2][xE2] = ' ';
				xE2 += 1;
				Map3[yE2][xE2] = 'E';
			}
			if (Map3[yE2][(xE2 + 1)] == 'E')
			{
				Map3[yE2][xE2] = ' ';
				xE2 += 1;
				Map3[yE2][xE2] = 'E';
			}
			if (Map3[yE2][(xE2 + 1)] == '#')
			{
				turn2 = 2;
			}
			if (Map3[yE2][(xE2 + 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn2 == 2)
		{
			if (Map3[yE2][(xE2 - 1)] == ' ')
			{
				Map3[yE2][xE2] = ' ';
				xE2 -= 1;
				Map3[yE2][xE2] = 'E';
			}
			if (Map3[yE2][(xE2 - 1)] == 'E')
			{
				Map3[yE2][xE2] = ' ';
				xE2 -= 1;
				Map3[yE2][xE2] = 'E';
			}
			if (Map3[yE2][(xE2 - 1)] == '#')
			{
				turn2 = 1;
			}
			if (Map3[yE2][(xE2 - 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn3 == 1)
		{
			if (Map3[yE3][(xE3 - 1)] == ' ')
			{
				Map3[yE3][xE3] = ' ';
				xE3 -= 1;
				Map3[yE3][xE3] = 'E';
			}
			if (Map3[yE3][(xE3 - 1)] == 'E')
			{
				Map3[yE3][xE3] = ' ';
				xE3 -= 1;
				Map3[yE3][xE3] = 'E';
			}
			if (Map3[yE3][(xE3 - 1)] == '#')
			{
				turn3 = 2;
			}
			if (Map3[yE3][(xE3 - 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn3 == 2)
		{
			if (Map3[yE3][(xE3 + 1)] == ' ')
			{
				Map3[yE3][xE3] = ' ';
				xE3 += 1;
				Map3[yE3][xE3] = 'E';
			}
			if (Map3[yE3][(xE3 + 1)] == 'E')
			{
				Map3[yE3][xE3] = ' ';
				xE3 += 1;
				Map3[yE3][xE3] = 'E';
			}
			if (Map3[yE3][(xE3 + 1)] == '#')
			{
				turn3 = 1;
			}
			if (Map3[yE3][(xE3 + 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn4 == 1)
		{
			if (Map3[yE4][(xE4 - 1)] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				xE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 - 1)] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				xE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 - 1)] == '#')
			{
				turn4 = 2;
			}
			if (Map3[yE4][(xE4 - 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn4 == 2)
		{
			if (Map3[(yE4 + 1)][xE4] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				yE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[(yE4 + 1)][xE4] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				yE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[(yE4 + 1)][xE4] == '#')
			{
				turn4 = 3;
			}
			if (Map3[(yE4 + 1)][xE4] == '@')
			{
				hunt3();
			}
		}
		if (turn4 == 3)
		{
			if (Map3[yE4][(xE4 + 1)] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				xE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 + 1)] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				xE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 + 1)] == '#')
			{
				turn4 = 4;
			}
			if (Map3[yE4][(xE4 + 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn4 == 4)
		{
			if (Map3[(yE4 + 1)][xE4] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				yE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[(yE4 + 1)][xE4] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				yE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[(yE4 + 1)][xE4] == '#')
			{
				turn4 = 5;
			}
			if (Map3[(yE4 + 1)][xE4] == '@')
			{
				hunt3();
			}
		}
		if (turn4 == 5)
		{
			if (Map3[yE4][(xE4 - 1)] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				xE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 - 1)] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				xE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 - 1)] == '#')
			{
				turn4 = 6;
			}
			if (Map3[yE4][(xE4 - 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn4 == 6)
		{
			if (Map3[(yE4 + 1)][xE4] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				yE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[(yE4 + 1)][xE4] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				yE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[(yE4 + 1)][xE4] == '#')
			{
				turn4 = 7;
			}
			if (Map3[(yE4 + 1)][xE4] == '@')
			{
				hunt3();
			}
		}
		if (turn4 == 7)
		{
			if (Map3[yE4][(xE4 + 1)] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				xE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 + 1)] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				xE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 + 1)] == '#')
			{
				turn4 = 8;
			}
			if (Map3[yE4][(xE4 + 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn4 == 8)
		{
			if (Map3[(yE4 - 1)][xE4] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				yE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[(yE4 - 1)][xE4] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				yE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[(yE4 - 1)][xE4] == '#')
			{
				turn4 = 9;
			}
			if (Map3[(yE4 - 1)][xE4] == '@')
			{
				hunt3();
			}
		}
		if (turn4 == 9)
		{
			if (Map3[yE4][(xE4 + 1)] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				xE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 + 1)] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				xE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 + 1)] == '#')
			{
				turn4 = 10;
			}
			if (Map3[yE4][(xE4 + 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn4 == 10)
		{
			if (Map3[(yE4 - 1)][xE4] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				yE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[(yE4 - 1)][xE4] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				yE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[(yE4 - 1)][xE4] == '#')
			{
				turn4 = 11;
			}
			if (Map3[(yE4 - 1)][xE4] == '@')
			{
				hunt3();
			}
		}
		if (turn4 == 11)
		{
			if (Map3[yE4][(xE4 - 1)] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				xE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 - 1)] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				xE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 - 1)] == '#')
			{
				turn4 = 12;
			}
			if (Map3[yE4][(xE4 - 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn4 == 12)
		{
			if (Map3[(yE4 - 1)][xE4] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				yE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[(yE4 - 1)][xE4] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				yE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[(yE4 - 1)][xE4] == '#')
			{
				turn4 = 13;
			}
			if (Map3[(yE4 - 1)][xE4] == '@')
			{
				hunt3();
			}
		}
		if (turn4 == 13)
		{
			if (Map3[yE4][(xE4 + 1)] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				xE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 + 1)] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				xE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 + 1)] == '#')
			{
				turn4 = 14;
			}
			if (Map3[yE4][(xE4 + 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn4 == 14)
		{
			if (Map3[yE4][(xE4 - 1)] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				xE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 - 1)] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				xE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 - 1)] == '#')
			{
				turn4 = 15;
			}
			if (Map3[yE4][(xE4 - 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn4 == 15)
		{
			if (Map3[(yE4 + 1)][xE4] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				yE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[(yE4 + 1)][xE4] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				yE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[(yE4 + 1)][xE4] == '#')
			{
				turn4 = 16;
			}
			if (Map3[(yE4 + 1)][xE4] == '@')
			{
				hunt3();
			}
		}
		if (turn4 == 16)
		{
			if (Map3[yE4][(xE4 + 1)] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				xE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 + 1)] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				xE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 + 1)] == '#')
			{
				turn4 = 17;
			}
			if (Map3[yE4][(xE4 + 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn4 == 17)
		{
			if (Map3[(yE4 + 1)][xE4] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				yE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[(yE4 + 1)][xE4] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				yE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[(yE4 + 1)][xE4] == '#')
			{
				turn4 = 18;
			}
			if (Map3[(yE4 + 1)][xE4] == '@')
			{
				hunt3();
			}
		}
		if (turn4 == 18)
		{
			if (Map3[yE4][(xE4 - 1)] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				xE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 - 1)] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				xE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 - 1)] == '#')
			{
				turn4 = 19;
			}
			if (Map3[yE4][(xE4 - 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn4 == 19)
		{
			if (Map3[(yE4 + 1)][xE4] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				yE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[(yE4 + 1)][xE4] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				yE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[(yE4 + 1)][xE4] == '#')
			{
				turn4 = 20;
			}
			if (Map3[(yE4 + 1)][xE4] == '@')
			{
				hunt3();
			}
		}
		if (turn4 == 20)
		{
			if (Map3[yE4][(xE4 - 1)] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				xE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 - 1)] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				xE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 - 1)] == '#')
			{
				turn4 = 21;
			}
			if (Map3[yE4][(xE4 - 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn4 == 21)
		{
			if (Map3[(yE4 - 1)][xE4] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				yE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[(yE4 - 1)][xE4] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				yE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[(yE4 - 1)][xE4] == '#')
			{
				turn4 = 22;
			}
			if (Map3[(yE4 - 1)][xE4] == '@')
			{
				hunt3();
			}
		}
		if (turn4 == 22)
		{
			if (Map3[yE4][(xE4 + 1)] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				xE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 + 1)] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				xE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 + 1)] == '#')
			{
				turn4 = 23;
			}
			if (Map3[yE4][(xE4 + 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn4 == 23)
		{
			if (Map3[(yE4 - 1)][xE4] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				yE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[(yE4 - 1)][xE4] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				yE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[(yE4 - 1)][xE4] == '#')
			{
				turn4 = 24;
			}
			if (Map3[(yE4 - 1)][xE4] == '@')
			{
				hunt3();
			}
		}
		if (turn4 == 24)
		{
			if (Map3[yE4][(xE4 - 1)] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				xE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 - 1)] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				xE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 - 1)] == '#')
			{
				turn4 = 25;
			}
			if (Map3[yE4][(xE4 - 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn4 == 25)
		{
			if (Map3[(yE4 - 1)][xE4] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				yE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[(yE4 - 1)][xE4] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				yE4 -= 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[(yE4 - 1)][xE4] == '@')
			{
				hunt3();
			}
			if (yE4 == 7)
			{
				turn4 = 26;
			}
		}
		if (turn4 == 26)
		{
			if (Map3[yE4][(xE4 + 1)] == ' ')
			{
				Map3[yE4][xE4] = ' ';
				xE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 + 1)] == 'E')
			{
				Map3[yE4][xE4] = ' ';
				xE4 += 1;
				Map3[yE4][xE4] = 'E';
			}
			if (Map3[yE4][(xE4 + 1)] == '#')
			{
				turn4 = 1;
			}
			if (Map3[yE4][(xE4 + 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn5 == 1)
		{
			if (Map3[yE5][(xE5 + 1)] == ' ')
			{
				Map3[yE5][xE5] = ' ';
				xE5 += 1;
				Map3[yE5][xE5] = 'E';
			}
			if (Map3[yE5][(xE5 + 1)] == 'E')
			{
				Map3[yE5][xE5] = ' ';
				xE5 += 1;
				Map3[yE5][xE5] = 'E';
			}
			if (Map3[yE5][(xE5 + 1)] == '#')
			{
				turn5 = 2;
			}
			if (Map3[yE5][(xE5 + 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn5 == 2)
		{
			if (Map3[yE5][(xE5 - 1)] == ' ')
			{
				Map3[yE5][xE5] = ' ';
				xE5 -= 1;
				Map3[yE5][xE5] = 'E';
			}
			if (Map3[yE5][(xE5 - 1)] == 'E')
			{
				Map3[yE5][xE5] = ' ';
				xE5 -= 1;
				Map3[yE5][xE5] = 'E';
			}
			if (xE5 == 2)
			{
				turn5 = 1;
			}
			if (Map3[yE5][(xE5 - 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn6 == 1)
		{
			if (Map3[yE6][(xE6 + 1)] == ' ')
			{
				Map3[yE6][xE6] = ' ';
				xE6 += 1;
				Map3[yE6][xE6] = 'E';
			}
			if (Map3[yE6][(xE6 + 1)] == 'E')
			{
				Map3[yE6][xE6] = ' ';
				xE6 += 1;
				Map3[yE6][xE6] = 'E';
			}
			if (Map3[yE6][(xE6 + 1)] == '#')
			{
				turn6 = 2;
			}
			if (Map3[yE6][(xE6 + 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn6 == 2)
		{
			if (Map3[yE6][(xE6 - 1)] == ' ')
			{
				Map3[yE6][xE6] = ' ';
				xE6 -= 1;
				Map3[yE6][xE6] = 'E';
			}
			if (Map3[yE6][(xE6 - 1)] == 'E')
			{
				Map3[yE6][xE6] = ' ';
				xE6 -= 1;
				Map3[yE6][xE6] = 'E';
			}
			if (xE6 == 2)
			{
				turn6 = 1;
			}
			if (Map3[yE6][(xE6 - 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn7 == 1)
		{
			if (Map3[yE7][(xE7 + 1)] == ' ')
			{
				Map3[yE7][xE7] = ' ';
				xE7 += 1;
				Map3[yE7][xE7] = 'E';
			}
			if (Map3[yE7][(xE7 + 1)] == 'E')
			{
				Map3[yE7][xE7] = ' ';
				xE7 += 1;
				Map3[yE7][xE7] = 'E';
			}
			if (xE7 == 28)
			{
				turn7 = 2;
			}
			if (Map3[yE7][(xE7 + 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn7 == 2)
		{
			if (Map3[yE7][(xE7 - 1)] == ' ')
			{
				Map3[yE7][xE7] = ' ';
				xE7 -= 1;
				Map3[yE7][xE7] = 'E';
			}
			if (Map3[yE7][(xE7 - 1)] == 'E')
			{
				Map3[yE7][xE7] = ' ';
				xE7 -= 1;
				Map3[yE7][xE7] = 'E';
			}
			if (xE7 == 2)
			{
				turn7 = 1;
			}
			if (Map3[yE7][(xE7 - 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn8 == 1)
		{
			if (Map3[yE8][(xE8 + 1)] == ' ')
			{
				Map3[yE8][xE8] = ' ';
				xE8 += 1;
				Map3[yE8][xE8] = 'E';
			}
			if (Map3[yE8][(xE8 + 1)] == 'E')
			{
				Map3[yE8][xE8] = ' ';
				xE8 += 1;
				Map3[yE8][xE8] = 'E';
			}
			if (Map3[yE8][(xE8 + 1)] == '#')
			{
				turn8 = 2;
			}
			if (Map3[yE8][(xE8 + 1)] == '@')
			{
				hunt3();
			}
		}
		if (turn8 == 2)
		{
			if (Map3[yE8][(xE8 - 1)] == ' ')
			{
				Map3[yE8][xE8] = ' ';
				xE8 -= 1;
				Map3[yE8][xE8] = 'E';
			}
			if (Map3[yE8][(xE8 - 1)] == 'E')
			{
				Map3[yE8][xE8] = ' ';
				xE8 -= 1;
				Map3[yE8][xE8] = 'E';
			}
			if (xE8 == 2)
			{
				turn8 = 1;
			}
			if (Map3[yE8][(xE8 - 1)] == '@')
			{
				hunt3();
			}
		}
	}
};
void hunt2()
{
	int xCharacter = 9;
	int yCharacter = 3;
	int xE1 = 1;
	int yE1 = 5;
	int xE2 = 18;
	int yE2 = 5;
	int turn1 = 1;
	int turn2 = 1;
	int loop1 = 1;
	char Map2[10][21] = {
		"####################",
		"#                  #",
		"# #######  ####### #",
		"#   #   #@ #   #   #",
		"### # # #### # # ###",
		"#E# # # #!!# # # #E#",
		"# #   # #  # #   # #",
		"# ##### #  # ##### #",
		"#                  #",
		"####################",
	};
	while (loop1 = 1)
	{
		Sleep(60);
		system("cls");
		cout << "Level 2" << endl;
		for (int y = 0; y < 10; y++)
		{
			cout << Map2[y] << endl;
		}
		if (GetAsyncKeyState(VK_UP))
		{
			if (Map2[(yCharacter - 1)][xCharacter] == ' ')
			{
				Map2[yCharacter][xCharacter] = ' ';
				yCharacter -= 1;
				Map2[yCharacter][xCharacter] = '@';
			}
			if (Map2[(yCharacter - 1)][xCharacter] == '!')
			{
				hunt3();
			}
		}
		if (GetAsyncKeyState(VK_DOWN))
		{
			if (Map2[(yCharacter + 1)][xCharacter] == ' ')
			{
				Map2[yCharacter][xCharacter] = ' ';
				yCharacter += 1;
				Map2[yCharacter][xCharacter] = '@';
			}
			if (Map2[(yCharacter + 1)][xCharacter] == '!')
			{
				hunt3();
			}
		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			if (Map2[yCharacter][(xCharacter + 1)] == ' ')
			{
				Map2[yCharacter][xCharacter] = ' ';
				xCharacter += 1;
				Map2[yCharacter][xCharacter] = '@';
			}
			if (Map2[yCharacter][(xCharacter + 1)] == '!')
			{
				hunt3();
			}
		}
		if (GetAsyncKeyState(VK_LEFT))
		{
			if (Map2[yCharacter][(xCharacter - 1)] == ' ')
			{
				Map2[yCharacter][xCharacter] = ' ';
				xCharacter -= 1;
				Map2[yCharacter][xCharacter] = '@';
			}
			if (Map2[yCharacter][(xCharacter - 1)] == '!')
			{
				hunt3();
			}
		}
		if (turn1 == 1)
		{
			if (Map2[(yE1 + 1)][xE1] == ' ')
			{
				Map2[yE1][xE1] = ' ';
				yE1 += 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[(yE1 + 1)][xE1] == 'E')
			{
				Map2[yE1][xE1] = ' ';
				yE1 += 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[(yE1 + 1)][xE1] == '#')
			{
				turn1 = 2;
			}
			if (Map2[(yE1 + 1)][xE1] == '@')
			{
				hunt2();
			}
		}
		if (turn1 == 2)
		{
			if (Map2[yE1][(xE1 + 1)] == ' ')
			{
				Map2[yE1][xE1] = ' ';
				xE1 += 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[yE1][(xE1 + 1)] == 'E')
			{
				Map2[yE1][xE1] = ' ';
				xE1 += 1;
				Map2[yE1][xE1] = 'E';
			}
			if (xE1 == 7)
			{
				turn1 = 3;
			}
			if (Map2[yE1][(xE1 + 1)] == '@')
			{
				hunt2();
			}
		}
		if (turn1 == 3)
		{
			if (Map2[(yE1 - 1)][xE1] == ' ')
			{
				Map2[yE1][xE1] = ' ';
				yE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[(yE1 - 1)][xE1] == 'E')
			{
				Map2[yE1][xE1] = ' ';
				yE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[(yE1 - 1)][xE1] == '#')
			{
				turn1 = 4;
			}
			if (Map2[(yE1 - 1)][xE1] == '@')
			{
				hunt2();
			}
		}
		if (turn1 == 4)
		{
			if (Map2[yE1][(xE1 - 1)] == ' ')
			{
				Map2[yE1][xE1] = ' ';
				xE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[yE1][(xE1 - 1)] == 'E')
			{
				Map2[yE1][xE1] = ' ';
				xE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[yE1][(xE1 - 1)] == '#')
			{
				turn1 = 5;
			}
			if (Map2[yE1][(xE1 - 1)] == '@')
			{
				hunt2();
			}
		}
		if (turn1 == 5)
		{
			if (Map2[(yE1 + 1)][xE1] == ' ')
			{
				Map2[yE1][xE1] = ' ';
				yE1 += 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[(yE1 + 1)][xE1] == 'E')
			{
				Map2[yE1][xE1] = ' ';
				yE1 += 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[(yE1 + 1)][xE1] == '#')
			{
				turn1 = 6;
			}
			if (Map2[(yE1 + 1)][xE1] == '@')
			{
				hunt2();
			}
		}
		if (turn1 == 6)
		{
			if (Map2[yE1][(xE1 - 1)] == ' ')
			{
				Map2[yE1][xE1] = ' ';
				xE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[yE1][(xE1 - 1)] == 'E')
			{
				Map2[yE1][xE1] = ' ';
				xE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[yE1][(xE1 - 1)] == '#')
			{
				turn1 = 7;
			}
			if (Map2[yE1][(xE1 - 1)] == '@')
			{
				hunt2();
			}
		}
		if (turn1 == 7)
		{
			if (Map2[(yE1 - 1)][xE1] == ' ')
			{
				Map2[yE1][xE1] = ' ';
				yE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[(yE1 - 1)][xE1] == 'E')
			{
				Map2[yE1][xE1] = ' ';
				yE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[(yE1 - 1)][xE1] == '#')
			{
				turn1 = 8;
			}
			if (Map2[(yE1 - 1)][xE1] == '@')
			{
				hunt2();
			}
		}
		if (turn1 == 8)
		{
			if (Map2[yE1][(xE1 - 1)] == ' ')
			{
				Map2[yE1][xE1] = ' ';
				xE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[yE1][(xE1 - 1)] == 'E')
			{
				Map2[yE1][xE1] = ' ';
				xE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[yE1][(xE1 - 1)] == '#')
			{
				turn1 = 9;
			}
			if (Map2[yE1][(xE1 - 1)] == '@')
			{
				hunt2();
			}
		}
		if (turn1 == 9)
		{
			if (Map2[(yE1 - 1)][xE1] == ' ')
			{
				Map2[yE1][xE1] = ' ';
				yE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[(yE1 - 1)][xE1] == 'E')
			{
				Map2[yE1][xE1] = ' ';
				yE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[(yE1 - 1)][xE1] == '#')
			{
				turn1 = 10;
			}
			if (Map2[(yE1 - 1)][xE1] == '@')
			{
				hunt2();
			}
		}
		if (turn1 == 10)
		{
			if (Map2[yE1][(xE1 + 1)] == ' ')
			{
				Map2[yE1][xE1] = ' ';
				xE1 += 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[yE1][(xE1 + 1)] == 'E')
			{
				Map2[yE1][xE1] = ' ';
				xE1 += 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[yE1][(xE1 + 1)] == '#')
			{
				turn1 = 11;
			}
			if (Map2[yE1][(xE1 + 1)] == '@')
			{
				hunt2();
			}
		}
		if (turn1 == 11)
		{
			if (Map2[(yE1 + 1)][xE1] == ' ')
			{
				Map2[yE1][xE1] = ' ';
				yE1 += 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[(yE1 + 1)][xE1] == 'E')
			{
				Map2[yE1][xE1] = ' ';
				yE1 += 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[(yE1 + 1)][xE1] == '#')
			{
				turn1 = 12;
			}
			if (Map2[(yE1 + 1)][xE1] == '@')
			{
				hunt2();
			}
		}
		if (turn1 == 12)
		{
			if (Map2[yE1][(xE1 - 1)] == ' ')
			{
				Map2[yE1][xE1] = ' ';
				xE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[yE1][(xE1 - 1)] == 'E')
			{
				Map2[yE1][xE1] = ' ';
				xE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[yE1][(xE1 - 1)] == '#')
			{
				turn1 = 13;
			}
			if (Map2[yE1][(xE1 - 1)] == '@')
			{
				hunt2();
			}
		}
		if (turn1 == 13)
		{
			if (Map2[(yE1 + 1)][xE1] == ' ')
			{
				Map2[yE1][xE1] = ' ';
				yE1 += 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[(yE1 + 1)][xE1] == 'E')
			{
				Map2[yE1][xE1] = ' ';
				yE1 += 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[(yE1 + 1)][xE1] == '#')
			{
				turn1 = 14;
			}
			if (Map2[(yE1 + 1)][xE1] == '@')
			{
				hunt2();
			}
		}
		if (turn1 == 14)
		{
			if (Map2[yE1][(xE1 - 1)] == ' ')
			{
				Map2[yE1][xE1] = ' ';
				xE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[yE1][(xE1 - 1)] == 'E')
			{
				Map2[yE1][xE1] = ' ';
				xE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[yE1][(xE1 - 1)] == '#')
			{
				turn1 = 15;
			}
			if (Map2[yE1][(xE1 - 1)] == '@')
			{
				hunt2();
			}
		}
		if (turn1 == 15)
		{
			if (Map2[(yE1 - 1)][xE1] == ' ')
			{
				Map2[yE1][xE1] = ' ';
				yE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[(yE1 - 1)][xE1] == 'E')
			{
				Map2[yE1][xE1] = ' ';
				yE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[(yE1 - 1)][xE1] == '#')
			{
				turn1 = 16;
			}
			if (Map2[(yE1 - 1)][xE1] == '@')
			{
				hunt2();
			}
		}
		if (turn1 == 16)
		{
			if (Map2[yE1][(xE1 - 1)] == ' ')
			{
				Map2[yE1][xE1] = ' ';
				xE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[yE1][(xE1 - 1)] == 'E')
			{
				Map2[yE1][xE1] = ' ';
				xE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[yE1][(xE1 - 1)] == ' ')
			{
				Map2[yE1][xE1] = ' ';
				xE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[yE1][(xE1 - 1)] == '#')
			{
				turn1 = 17;
			}
			if (Map2[yE1][(xE1 - 1)] == '@')
			{
				hunt2();
			}
		}
		if (turn1 == 17)
		{
			if (Map2[(yE1 + 1)][xE1] == ' ')
			{
				Map2[yE1][xE1] = ' ';
				yE1 += 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[(yE1 + 1)][xE1] == 'E')
			{
				Map2[yE1][xE1] = ' ';
				yE1 += 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[(yE1 + 1)][xE1] == '#')
			{
				turn1 = 18;
			}
			if (Map2[(yE1 + 1)][xE1] == '@')
			{
				hunt2();
			}
		}
		if (turn1 == 18)
		{
			if (Map2[yE1][(xE1 + 1)] == ' ')
			{
				Map2[yE1][xE1] = ' ';
				xE1 += 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[yE1][(xE1 + 1)] == 'E')
			{
				Map2[yE1][xE1] = ' ';
				xE1 += 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[yE1][(xE1 + 1)] == '#')
			{
				turn1 = 19;
			}
			if (Map2[yE1][(xE1 + 1)] == '@')
			{
				hunt2();
			}
		}
		if (turn1 == 19)
		{
			if (Map2[(yE1 - 1)][xE1] == ' ')
			{
				Map2[yE1][xE1] = ' ';
				yE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[(yE1 - 1)][xE1] == 'E')
			{
				Map2[yE1][xE1] = ' ';
				yE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[(yE1 - 1)][xE1] == '#')
			{
				turn1 = 20;
			}
			if (Map2[(yE1 - 1)][xE1] == '@')
			{
				hunt2();
			}
		}
		if (turn1 == 20)
		{
			if (Map2[(yE1 + 1)][xE1] == ' ')
			{
				Map2[yE1][xE1] = ' ';
				yE1 += 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[(yE1 + 1)][xE1] == 'E')
			{
				Map2[yE1][xE1] = ' ';
				yE1 += 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[(yE1 + 1)][xE1] == '#')
			{
				turn1 = 21;
			}
			if (Map2[(yE1 + 1)][xE1] == '@')
			{
				hunt2();
			}
		}
		if (turn1 == 21)
		{
			if (Map2[yE1][(xE1 + 1)] == ' ')
			{
				Map2[yE1][xE1] = ' ';
				xE1 += 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[yE1][(xE1 + 1)] == 'E')
			{
				Map2[yE1][xE1] = ' ';
				xE1 += 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[yE1][(xE1 + 1)] == '#')
			{
				turn1 = 22;
			}
			if (Map2[yE1][(xE1 + 1)] == '@')
			{
				hunt2();
			}
		}
		if (turn1 == 22)
		{
			if (Map2[yE1][(xE1 - 1)] == ' ')
			{
				Map2[yE1][xE1] = ' ';
				xE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[yE1][(xE1 - 1)] == 'E')
			{
				Map2[yE1][xE1] = ' ';
				xE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[yE1][(xE1 - 1)] == '#')
			{
				turn1 = 23;
			}
			if (Map2[yE1][(xE1 - 1)] == '@')
			{
				hunt2();
			}
		}
		if (turn1 == 23)
		{
			if (Map2[(yE1 - 1)][xE1] == ' ')
			{
				Map2[yE1][xE1] = ' ';
				yE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[(yE1 - 1)][xE1] == 'E')
			{
				Map2[yE1][xE1] = ' ';
				yE1 -= 1;
				Map2[yE1][xE1] = 'E';
			}
			if (Map2[(yE1 - 1)][xE1] == '#')
			{
				turn1 = 1;
			}
			if (Map2[(yE1 - 1)][xE1] == '@')
			{
				hunt2();
			}
		}
		if (turn2 == 1)
		{
			if (Map2[(yE2 + 1)][xE2] == ' ')
			{
				Map2[yE2][xE2] = ' ';
				yE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[(yE2 + 1)][xE2] == 'E')
			{
				Map2[yE2][xE2] = ' ';
				yE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[(yE2 + 1)][xE2] == '#')
			{
				turn2 = 2;
			}
			if (Map2[(yE2 + 1)][xE2] == '@')
			{
				hunt2();
			}
		}
		if (turn2 == 2)
		{
			if (Map2[yE2][(xE2 - 1)] == ' ')
			{
				Map2[yE2][xE2] = ' ';
				xE2 -= 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[yE2][(xE2 - 1)] == 'E')
			{
				Map2[yE2][xE2] = ' ';
				xE2 -= 1;
				Map2[yE2][xE2] = 'E';
			}
			if (xE2 == 12)
			{
				turn2 = 3;
			}
			if (Map2[yE2][(xE2 - 1)] == '@')
			{
				hunt2();
			}
		}
		if (turn2 == 3)
		{
			if (Map2[(yE2 - 1)][xE2] == ' ')
			{
				Map2[yE2][xE2] = ' ';
				yE2 -= 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[(yE2 - 1)][xE2] == 'E')
			{
				Map2[yE2][xE2] = ' ';
				yE2 -= 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[(yE2 - 1)][xE2] == '#')
			{
				turn2 = 4;
			}
			if (Map2[(yE2 - 1)][xE2] == '@')
			{
				hunt2();
			}
		}
		if (turn2 == 4)
		{
			if (Map2[yE2][(xE2 + 1)] == ' ')
			{
				Map2[yE2][xE2] = ' ';
				xE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[yE2][(xE2 + 1)] == 'E')
			{
				Map2[yE2][xE2] = ' ';
				xE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[yE2][(xE2 + 1)] == '#')
			{
				turn2 = 5;
			}
			if (Map2[yE2][(xE2 + 1)] == '@')
			{
				hunt2();
			}
		}
		if (turn2 == 5)
		{
			if (Map2[(yE2 + 1)][xE2] == ' ')
			{
				Map2[yE2][xE2] = ' ';
				yE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[(yE2 + 1)][xE2] == 'E')
			{
				Map2[yE2][xE2] = ' ';
				yE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[(yE2 + 1)][xE2] == '#')
			{
				turn2 = 6;
			}
			if (Map2[(yE2 + 1)][xE2] == '@')
			{
				hunt2();
			}
		}
		if (turn2 == 6)
		{
			if (Map2[yE2][(xE2 + 1)] == ' ')
			{
				Map2[yE2][xE2] = ' ';
				xE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[yE2][(xE2 + 1)] == 'E')
			{
				Map2[yE2][xE2] = ' ';
				xE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[yE2][(xE2 + 1)] == '#')
			{
				turn2 = 7;
			}
			if (Map2[yE2][(xE2 + 1)] == '@')
			{
				hunt2();
			}
		}
		if (turn2 == 7)
		{
			if (Map2[(yE2 - 1)][xE2] == ' ')
			{
				Map2[yE2][xE2] = ' ';
				yE2 -= 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[(yE2 - 1)][xE2] == 'E')
			{
				Map2[yE2][xE2] = ' ';
				yE2 -= 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[(yE2 - 1)][xE2] == '#')
			{
				turn2 = 8;
			}
			if (Map2[(yE2 - 1)][xE2] == '@')
			{
				hunt2();
			}
		}
		if (turn2 == 8)
		{
			if (Map2[yE2][(xE2 + 1)] == ' ')
			{
				Map2[yE2][xE2] = ' ';
				xE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[yE2][(xE2 + 1)] == 'E')
			{
				Map2[yE2][xE2] = ' ';
				xE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[yE2][(xE2 + 1)] == '#')
			{
				turn2 = 9;
			}
			if (Map2[yE2][(xE2 + 1)] == '@')
			{
				hunt2();
			}
		}
		if (turn2 == 9)
		{
			if (Map2[(yE2 - 1)][xE2] == ' ')
			{
				Map2[yE2][xE2] = ' ';
				yE2 -= 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[(yE2 - 1)][xE2] == 'E')
			{
				Map2[yE2][xE2] = ' ';
				yE2 -= 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[(yE2 - 1)][xE2] == '#')
			{
				turn2 = 10;
			}
			if (Map2[(yE2 - 1)][xE2] == '@')
			{
				hunt2();
			}
		}
		if (turn2 == 10)
		{
			if (Map2[yE2][(xE2 - 1)] == ' ')
			{
				Map2[yE2][xE2] = ' ';
				xE2 -= 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[yE2][(xE2 - 1)] == 'E')
			{
				Map2[yE2][xE2] = ' ';
				xE2 -= 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[yE2][(xE2 - 1)] == '#')
			{
				turn2 = 11;
			}
			if (Map2[yE2][(xE2 - 1)] == '@')
			{
				hunt2();
			}
		}
		if (turn2 == 11)
		{
			if (Map2[(yE2 + 1)][xE2] == ' ')
			{
				Map2[yE2][xE2] = ' ';
				yE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[(yE2 + 1)][xE2] == 'E')
			{
				Map2[yE2][xE2] = ' ';
				yE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[(yE2 + 1)][xE2] == '#')
			{
				turn2 = 12;
			}
			if (Map2[(yE2 + 1)][xE2] == '@')
			{
				hunt2();
			}
		}
		if (turn2 == 12)
		{
			if (Map2[yE2][(xE2 + 1)] == ' ')
			{
				Map2[yE2][xE2] = ' ';
				xE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[yE2][(xE2 + 1)] == 'E')
			{
				Map2[yE2][xE2] = ' ';
				xE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[yE2][(xE2 + 1)] == '#')
			{
				turn2 = 13;
			}
			if (Map2[yE2][(xE2 + 1)] == '@')
			{
				hunt2();
			}
		}
		if (turn2 == 13)
		{
			if (Map2[(yE2 + 1)][xE2] == ' ')
			{
				Map2[yE2][xE2] = ' ';
				yE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[(yE2 + 1)][xE2] == 'E')
			{
				Map2[yE2][xE2] = ' ';
				yE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[(yE2 + 1)][xE2] == '#')
			{
				turn2 = 14;
			}
			if (Map2[(yE2 + 1)][xE2] == '@')
			{
				hunt2();
			}
		}
		if (turn2 == 14)
		{
			if (Map2[yE2][(xE2 + 1)] == ' ')
			{
				Map2[yE2][xE2] = ' ';
				xE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[yE2][(xE2 + 1)] == 'E')
			{
				Map2[yE2][xE2] = ' ';
				xE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[yE2][(xE2 + 1)] == '#')
			{
				turn2 = 15;
			}
			if (Map2[yE2][(xE2 + 1)] == '@')
			{
				hunt2();
			}
		}
		if (turn2 == 15)
		{
			if (Map2[(yE2 - 1)][xE2] == ' ')
			{
				Map2[yE2][xE2] = ' ';
				yE2 -= 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[(yE2 - 1)][xE2] == 'E')
			{
				Map2[yE2][xE2] = ' ';
				yE2 -= 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[(yE2 - 1)][xE2] == '#')
			{
				turn2 = 16;
			}
			if (Map2[(yE2 - 1)][xE2] == '@')
			{
				hunt2();
			}
		}
		if (turn2 == 16)
		{
			if (Map2[yE2][(xE2 + 1)] == ' ')
			{
				Map2[yE2][xE2] = ' ';
				xE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[yE2][(xE2 + 1)] == 'E')
			{
				Map2[yE2][xE2] = ' ';
				xE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[yE2][(xE2 + 1)] == ' ')
			{
				Map2[yE2][xE2] = ' ';
				xE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[yE2][(xE2 + 1)] == '#')
			{
				turn2 = 17;
			}
			if (Map2[yE2][(xE2 + 1)] == '@')
			{
				hunt2();
			}
		}
		if (turn2 == 17)
		{
			if (Map2[(yE2 + 1)][xE2] == ' ')
			{
				Map2[yE2][xE2] = ' ';
				yE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[(yE2 + 1)][xE2] == 'E')
			{
				Map2[yE2][xE2] = ' ';
				yE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[(yE2 + 1)][xE2] == '#')
			{
				turn2 = 18;
			}
			if (Map2[(yE2 + 1)][xE2] == '@')
			{
				hunt2();
			}
		}
		if (turn2 == 18)
		{
			if (Map2[yE2][(xE2 - 1)] == ' ')
			{
				Map2[yE2][xE2] = ' ';
				xE2 -= 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[yE2][(xE2 - 1)] == 'E')
			{
				Map2[yE2][xE2] = ' ';
				xE2 -= 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[yE2][(xE2 - 1)] == '#')
			{
				turn2 = 19;
			}
			if (Map2[yE2][(xE2 - 1)] == '@')
			{
				hunt2();
			}
		}
		if (turn2 == 19)
		{
			if (Map2[(yE2 - 1)][xE2] == ' ')
			{
				Map2[yE2][xE2] = ' ';
				yE2 -= 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[(yE2 - 1)][xE2] == 'E')
			{
				Map2[yE2][xE2] = ' ';
				yE2 -= 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[(yE2 - 1)][xE2] == '#')
			{
				turn2 = 20;
			}
			if (Map2[(yE2 - 1)][xE2] == '@')
			{
				hunt2();
			}
		}
		if (turn2 == 20)
		{
			if (Map2[(yE2 + 1)][xE2] == ' ')
			{
				Map2[yE2][xE2] = ' ';
				yE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[(yE2 + 1)][xE2] == 'E')
			{
				Map2[yE2][xE2] = ' ';
				yE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[(yE2 + 1)][xE2] == '#')
			{
				turn2 = 21;
			}
			if (Map2[(yE2 + 1)][xE2] == '@')
			{
				hunt2();
			}
		}
		if (turn2 == 21)
		{
			if (Map2[yE2][(xE2 - 1)] == ' ')
			{
				Map2[yE2][xE2] = ' ';
				xE2 -= 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[yE2][(xE2 - 1)] == 'E')
			{
				Map2[yE2][xE2] = ' ';
				xE2 -= 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[yE2][(xE2 - 1)] == '#')
			{
				turn2 = 22;
			}
			if (Map2[yE2][(xE2 - 1)] == '@')
			{
				hunt2();
			}
		}
		if (turn2 == 22)
		{
			if (Map2[yE2][(xE2 + 1)] == ' ')
			{
				Map2[yE2][xE2] = ' ';
				xE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[yE2][(xE2 + 1)] == 'E')
			{
				Map2[yE2][xE2] = ' ';
				xE2 += 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[yE2][(xE2 + 1)] == '#')
			{
				turn2 = 23;
			}
			if (Map2[yE2][(xE2 + 1)] == '@')
			{
				hunt2();
			}
		}
		if (turn2 == 23)
		{
			if (Map2[(yE2 - 1)][xE2] == ' ')
			{
				Map2[yE2][xE2] = ' ';
				yE2 -= 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[(yE2 - 1)][xE2] == 'E')
			{
				Map2[yE2][xE2] = ' ';
				yE2 -= 1;
				Map2[yE2][xE2] = 'E';
			}
			if (Map2[(yE2 - 1)][xE2] == '#')
			{
				turn2 = 1;
			}
			if (Map2[(yE2 - 1)][xE2] == '@')
			{
				hunt2();
			}
		}
	}
};
void hunt1()
{
	int xCharacter = 4;
	int yCharacter = 3;
	int xE1 = 8;
	int yE1 = 8;
	int up = 1;
	int right = 0;
	int down = 0;
	int left = 0;
	int loop = 1;
	char Map1[10][11] = {
		"##########",
		"#        #",
		"# ##  ## #",
		"# ##@ ## #",
		"# ###### #",
		"# ###### #",
		"# #!!!!# #",
		"# #    # #",
		"#       E#",
		"##########",
	};
	while (loop = 1)
	{
		Sleep(60);
		system("cls");
		cout << "Level 1" << endl;
		for (int y = 0; y < 10; y++)
		{
			cout << Map1[y] << endl;
		}
		if (GetAsyncKeyState(VK_UP))
		{
			if (Map1[(yCharacter - 1)][xCharacter] == ' ')
			{
				Map1[yCharacter][xCharacter] = ' ';
				yCharacter -= 1;
				Map1[yCharacter][xCharacter] = '@';
			}
			if (Map1[(yCharacter - 1)][xCharacter] == '!')
			{
				system("cls");
				hunt2();
				break;
			}
		}
		if (GetAsyncKeyState(VK_DOWN))
		{
			if (Map1[(yCharacter + 1)][xCharacter] == ' ')
			{
				Map1[yCharacter][xCharacter] = ' ';
				yCharacter += 1;
				Map1[yCharacter][xCharacter] = '@';
			}
			if (Map1[(yCharacter + 1)][xCharacter] == '!')
			{
				system("cls");
				hunt2();
				break;
			}
		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			if (Map1[yCharacter][(xCharacter + 1)] == ' ')
			{
				Map1[yCharacter][xCharacter] = ' ';
				xCharacter += 1;
				Map1[yCharacter][xCharacter] = '@';
			}
			if (Map1[yCharacter][(xCharacter + 1)] == '!')
			{
				system("cls");
				hunt2();
				break;
			}
		}
		if (GetAsyncKeyState(VK_LEFT))
		{
			if (Map1[yCharacter][(xCharacter - 1)] == ' ')
			{
				Map1[yCharacter][xCharacter] = ' ';
				xCharacter -= 1;
				Map1[yCharacter][xCharacter] = '@';
			}
			if (Map1[yCharacter][(xCharacter - 1)] == '!')
			{
				system("cls");
				hunt2();
				break;
			}
		}
		if (up == 1)
		{
			Map1[yE1][xE1] = ' ';
			yE1 -= 1;
			Map1[yE1][xE1] = 'E';
			if (Map1[(yE1 - 1)][xE1] == '@')
			{
				hunt1();
			}
			if (yE1 == 1)
			{
				up = 0;
				left = 1;
			}
		}
		if (left == 1)
		{
			Map1[yE1][xE1] = ' ';
			xE1 -= 1;
			Map1[yE1][xE1] = 'E';
			if (Map1[yE1][(xE1 - 1)] == '@')
			{
				hunt1();
			}
			if (xE1 == 1)
			{
				left = 0;
				down = 1;
			}
		}
		if (down == 1)
		{
			Map1[yE1][xE1] = ' ';
			yE1 += 1;
			Map1[yE1][xE1] = 'E';
			if (Map1[(yE1 + 1)][xE1] == '@')
			{
				hunt1();
			}
			if (yE1 == 8)
			{
				down = 0;
				right = 1;
			}
		}
		if (right == 1)
		{
			Map1[yE1][xE1] = ' ';
			xE1 += 1;
			Map1[yE1][xE1] = 'E';
			if (Map1[yE1][(xE1 + 1)] == '@')
			{
				hunt1();
			}
			if (xE1 == 8)
			{
				right = 0;
				up = 1;
			}
		}
	}
};
void shoot()
{
	system("cls");
	cout << "controls" << endl;
	cout << " " << endl;
	cout << "Player 1:  Left shift = up. Left ctrl = down." << endl;
	cout << "Player 2: Up arrow key = up. Down arrow key = down." << endl;
	Sleep(5000);
	shooter();
}
void Snake()
{
	Setup();
	system("cls");
	cout << "Use w,a,s,d controls" << endl;
	Sleep(3000);
	while (!gameOver)
	{
		Draw();
		Input();
		Logic();
		Sleep(80);
	}
	while (1)
	{
		cout << "Game Over" << endl;
		Sleep(100000);
		system("cls");
	}
};
void Pong()
{
	system("cls");
	cout << "Player 1 controls = w,s" << endl;
	cout << "Player 2 controls = i,k" << endl;
	Sleep(5000);
	cGameManger c(40, 20);
	c.Run();
};
void Maze()
{
	level1();
	level2();
	level3();
	level4();
	level5();
};
void hunt()
{
	hunt1();
};
void hub()
{
	int xCharacter = 8;
	int yCharacter = 10;
	char Map[22][50] = {
		"#################################################",
		"#####################Pi-Cade#####################",
		"#################################################",
		"#@# ##Maze#!#   #     Snake     #     Pong      #",
		"# #       # ### #               # #           # #",
		"#   #######   # #               # #   O       # #",
		"# ####   #### # #    ooooO F    # #           # #",
		"# #    #      # #    o          # #           # #",
		"# #### # ###### # oooo          # #           # #",
		"#               #               #               #",
		"####### O ############# _ ############# _ #######",
		"#################################################",
		"#################################################",
		"####### _ ############# _ ############# _ #######",
		"#    Shooter    #     Hunt      #      GO       #",
		"#               #               #    Back to    #",
		"# @    -        # ######@###### #    Website    #",
		"#     -       O # #    ###    # #               #",
		"#               # #    #!#    # #               #",
		"#               # ###### ######E#               #",
		"#    2 Player   #               #               #",
		"#################################################",
	};
	while (Hub = 1)
	{
		Sleep(60);
		system("cls");
		cout << yCharacter << " " << xCharacter << endl;
		cout << "Use Arrow Keys To Select Game. Press Enter To Start" << endl;
		for (int i = 0; i < 22; i++)
		{
			cout << Map[i] << endl;
		}
		if (GetAsyncKeyState(VK_UP))
		{
			if (Map[(yCharacter - 3)][xCharacter] == '_')
			{
				Map[yCharacter][xCharacter] = '_';
				yCharacter -= 3;
				Map[yCharacter][xCharacter] = 'O';
			}
		}
		if (GetAsyncKeyState(VK_DOWN))
		{
			if (Map[(yCharacter + 3)][xCharacter] == '_')
			{
				Map[yCharacter][xCharacter] = '_';
				yCharacter += 3;
				Map[yCharacter][xCharacter] = 'O';
			}
		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			if (Map[yCharacter][(xCharacter + 16)] == '_')
			{
				Map[yCharacter][xCharacter] = '_';
				xCharacter += 16;
				Map[yCharacter][xCharacter] = 'O';
			}
		}
		if (GetAsyncKeyState(VK_LEFT))
		{
			if (Map[yCharacter][(xCharacter - 16)] == '_')
			{
				Map[yCharacter][xCharacter] = '_';
				xCharacter -= 16;
				Map[yCharacter][xCharacter] = 'O';
			}
		}
		if (GetAsyncKeyState(VK_RETURN))
		{
			if (xCharacter == 40)
			{
				if (yCharacter == 10)
				{
					Hub = 0;
					Pong();
					break;
				}
			}
			if (xCharacter == 24)
			{
				if (yCharacter == 10)
				{
					Hub = 0;
					Snake();
					break;
				}
			}
			if (xCharacter == 8)
			{
				if (yCharacter == 10)
				{
					Hub = 0;
					Maze();
					break;
				}
			}
			if (xCharacter == 8)
			{
				if (yCharacter == 13)
				{
					Hub = 0;
					shoot();
					break;
				}
			}
			if (xCharacter == 24)
			{
				if (yCharacter == 13)
				{
					Hub = 0;
					hunt();
					break;
				}
			}
		}
	}
};
int main()
{
	hub();
}