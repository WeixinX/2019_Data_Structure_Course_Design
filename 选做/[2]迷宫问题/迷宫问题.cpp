//运行环境：Visual Studio 2017
#include<iostream>
#include<cstdio>
#include<cstdio>
#include<cstring>
#pragma warning(disable:4996)

using namespace std;
const int maxn = 10000;
const int inf = 1 << 30 - 1;


int maze[maxn][maxn] = {0};//存储迷宫地图，1表示可走，0表示障碍物
int n, m;//迷宫地图行、列值

//坐标点结构体
struct pos
{
	int x, y;	//坐标
	int dir;	//方向
	int step;	//步数
};

pos S, T;//起点、终点

//方向数组
int dx[4] = { 0,0,1,-1 };
int dy[4] = { 1,-1,0,0 };

//是否走过该位置
bool vis[maxn][maxn] = { 0 };

//读取迷宫地图
void read()
{
	FILE *fp;
	if ((fp = fopen("maze.txt", "r")) == NULL)
	{
		printf("读取迷宫地图失败！\n");
		exit(-1);
	}
	char line[maxn];
	int row = 1;
	fscanf(fp, "%s", line);
	m = strlen(line);//迷宫列数
	while(!feof(fp))
	{
		int col = 1;
		for (int i = 0; i < strlen(line); i++)
		{
			if (line[i] == '.')maze[row][col] = 1;
			col++;
		}
		fscanf(fp, "%s", line);
		row++;
	}
	n = row - 1;//迷宫行数

	fclose(fp);
}

//判断坐标是否出界、存在障碍物
bool judge(int x, int y)
{
	if (x < 1 || x > n || y < 1 || y > m)return false;
	if (vis[x][y])return false;
	if (!maze[x][y])return false;
	return true;
}

//非递归DFS寻最短路径
void dfs()
{
	pos path[maxn];	//存放最优路径
	pos stack[maxn];//定义栈
	int top = -1,ttop = -1, ptop = -1;;
	int step = inf;	//从起点到终点最少需要走的步数
	stack[++top] = S;
	vis[S.x][S.y] = 1;
	
	while (top >= 0)
	{
		pos t = stack[top];
		pos next;

		next.dir = 0;
		next.step = t.step + 1;
		next.x = t.x + dx[t.dir];
		next.y = t.y + dy[t.dir];

		if (judge(next.x, next.y))//若下个位置可达，则入栈
		{
			stack[++top] = next;
			if (next.x == T.x && next.y == T.y)//若到达终点
			{
				if (next.step < step)//若有更优的路径，则存入path栈
				{
					step = next.step;
					ttop = top;
					ptop = -1;
					while (ttop >= 0)
					{
						path[++ptop] = stack[ttop--];
					}
				}
				top--;//出栈
				t = stack[top];//更新栈顶方向
				t.dir++;
				stack[top] = t;
				continue;
			}
			else
			{
				vis[next.x][next.y] = 1;
				continue;//若未到达终点，继续取栈顶位置获取下一个位置
			}
		}
		else//若下个位置不可达，则换个方向
		{
			t.dir++;
			stack[top] = t;
			while (t.dir >= 4)
			{
				top--;
				vis[t.x][t.y] = 0;
				if (top < 0) break;
				else//更新栈顶位置方向
				{
					t = stack[top];
					t.dir++;
					stack[top] = t;
				}
			}
		}
	}

	//输出路径
	printf("从 起点( %d,%d ) 最少需要走 %d 步可达 终点( %d,%d ) \n", S.x, S.y, step, T.x, T.y);
	printf("路径为：\n");
	while (ptop >= 0)
	{
		if (ptop != 0) printf("( %d,%d ) - > ", path[ptop]);
		else printf("( %d,%d )\n", path[ptop]);
		ptop--;
	}
}

//以0，1的形式输出地图。0表示存在障碍物
void print_maze()
{
	//横坐标轴
	printf("      ");
	for (int i = 1; i <= m; i++)
	{
		printf("%-3d ", i);
	}
	printf("\n");
	printf("    ");
	for (int i = 1; i <= m; i++)
	{
		printf("——");
	}
	printf("\n");

	for (int i = 1; i <= n; i++)
	{
		printf("%3d|", i);
		for (int j = 1; j <= m; j++)
		{
			printf("%3d ", maze[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}


int main()
{
	read();
	print_maze();
	while (1)
	{
		int sx, sy;//起点坐标
		int tx, ty;//终点坐标
		printf("迷宫地图行数 1 < x < %d，列数 1 < y < %d\n", n, m);
		printf("请选择没有障碍物的坐标点作为 起点 和 终点\n");
		printf("起点坐标(x,y)：");
		scanf("%d %d", &sx, &sy);
		
		printf("终点坐标(x,y)：");
		scanf("%d %d", &tx, &ty);

		if (!judge(sx, sy) && !judge(tx, ty))
		{
			printf("起点、终点输入有误，请重新输入！\n\n");
			continue;
		}
		else if (!judge(sx, sy) && judge(tx, ty))
		{
			printf("起点输入有误，请重新输入！\n\n");
			continue;
		}
		else if (judge(sx, sy) && !judge(tx, ty))
		{
			printf("终点输入有误，请重新输入！\n\n");
			continue;
		}
		else
		{
			S.x = sx;
			S.y = sy;
			S.dir = 0;
			S.step = 0;

			T.x = tx;
			T.y = ty;
			T.dir = 0;
			T.step = 0;

			break;
		}
	}
	dfs();
	system("pause");
	return 0;
}