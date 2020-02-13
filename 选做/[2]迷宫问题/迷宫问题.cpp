//���л�����Visual Studio 2017
#include<iostream>
#include<cstdio>
#include<cstdio>
#include<cstring>
#pragma warning(disable:4996)

using namespace std;
const int maxn = 10000;
const int inf = 1 << 30 - 1;


int maze[maxn][maxn] = {0};//�洢�Թ���ͼ��1��ʾ���ߣ�0��ʾ�ϰ���
int n, m;//�Թ���ͼ�С���ֵ

//�����ṹ��
struct pos
{
	int x, y;	//����
	int dir;	//����
	int step;	//����
};

pos S, T;//��㡢�յ�

//��������
int dx[4] = { 0,0,1,-1 };
int dy[4] = { 1,-1,0,0 };

//�Ƿ��߹���λ��
bool vis[maxn][maxn] = { 0 };

//��ȡ�Թ���ͼ
void read()
{
	FILE *fp;
	if ((fp = fopen("maze.txt", "r")) == NULL)
	{
		printf("��ȡ�Թ���ͼʧ�ܣ�\n");
		exit(-1);
	}
	char line[maxn];
	int row = 1;
	fscanf(fp, "%s", line);
	m = strlen(line);//�Թ�����
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
	n = row - 1;//�Թ�����

	fclose(fp);
}

//�ж������Ƿ���硢�����ϰ���
bool judge(int x, int y)
{
	if (x < 1 || x > n || y < 1 || y > m)return false;
	if (vis[x][y])return false;
	if (!maze[x][y])return false;
	return true;
}

//�ǵݹ�DFSѰ���·��
void dfs()
{
	pos path[maxn];	//�������·��
	pos stack[maxn];//����ջ
	int top = -1,ttop = -1, ptop = -1;;
	int step = inf;	//����㵽�յ�������Ҫ�ߵĲ���
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

		if (judge(next.x, next.y))//���¸�λ�ÿɴ����ջ
		{
			stack[++top] = next;
			if (next.x == T.x && next.y == T.y)//�������յ�
			{
				if (next.step < step)//���и��ŵ�·���������pathջ
				{
					step = next.step;
					ttop = top;
					ptop = -1;
					while (ttop >= 0)
					{
						path[++ptop] = stack[ttop--];
					}
				}
				top--;//��ջ
				t = stack[top];//����ջ������
				t.dir++;
				stack[top] = t;
				continue;
			}
			else
			{
				vis[next.x][next.y] = 1;
				continue;//��δ�����յ㣬����ȡջ��λ�û�ȡ��һ��λ��
			}
		}
		else//���¸�λ�ò��ɴ�򻻸�����
		{
			t.dir++;
			stack[top] = t;
			while (t.dir >= 4)
			{
				top--;
				vis[t.x][t.y] = 0;
				if (top < 0) break;
				else//����ջ��λ�÷���
				{
					t = stack[top];
					t.dir++;
					stack[top] = t;
				}
			}
		}
	}

	//���·��
	printf("�� ���( %d,%d ) ������Ҫ�� %d ���ɴ� �յ�( %d,%d ) \n", S.x, S.y, step, T.x, T.y);
	printf("·��Ϊ��\n");
	while (ptop >= 0)
	{
		if (ptop != 0) printf("( %d,%d ) - > ", path[ptop]);
		else printf("( %d,%d )\n", path[ptop]);
		ptop--;
	}
}

//��0��1����ʽ�����ͼ��0��ʾ�����ϰ���
void print_maze()
{
	//��������
	printf("      ");
	for (int i = 1; i <= m; i++)
	{
		printf("%-3d ", i);
	}
	printf("\n");
	printf("    ");
	for (int i = 1; i <= m; i++)
	{
		printf("����");
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
		int sx, sy;//�������
		int tx, ty;//�յ�����
		printf("�Թ���ͼ���� 1 < x < %d������ 1 < y < %d\n", n, m);
		printf("��ѡ��û���ϰ�����������Ϊ ��� �� �յ�\n");
		printf("�������(x,y)��");
		scanf("%d %d", &sx, &sy);
		
		printf("�յ�����(x,y)��");
		scanf("%d %d", &tx, &ty);

		if (!judge(sx, sy) && !judge(tx, ty))
		{
			printf("��㡢�յ������������������룡\n\n");
			continue;
		}
		else if (!judge(sx, sy) && judge(tx, ty))
		{
			printf("��������������������룡\n\n");
			continue;
		}
		else if (judge(sx, sy) && !judge(tx, ty))
		{
			printf("�յ������������������룡\n\n");
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