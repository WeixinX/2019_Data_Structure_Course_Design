//运行环境：Visual Studio 2017
#include<iostream>
#include<cstdio>
#include<cstdlib>
#pragma warning(disable:4996)

using namespace std;
const int maxv = 505;
const int inf = 1 << 30 - 1;

//链式邻接表结构
struct node
{
	int flag;		//1表示小道，0表示大道
	int v;			//到达的点
	long long dis;	//两点间的距离
	node *next;		//指向下一个
}Adj[maxv];

//加入邻接点
void add(int flag, int u, int v, long long dis)
{
	node *p = &Adj[u];
	while (p->next != NULL)p = p->next;
	node *s = (node *)malloc(sizeof(node));
	s->v = v;
	s->dis = dis;
	s->flag = flag;
	p->next = s;
	s->next = NULL;
}

bool vis[maxv];		//0表示该点未被访问
long long to[maxv];	//to[i]表示到达i时前面总共走过的路长
long long d[maxv];	//d[i]表示起点s到点i的最短距离
int pre[maxv];		//pre[i]表示点i的前驱,使用递归便可生成路径

int n, m;
//关键算法
void dijkstra(int s)
{
	for (int i = 1; i <= n; i++)
	{
		vis[i] = 0;
		d[i] = inf;
		pre[i] = i;
	}
	d[s] = 0;

	for (int i = 1; i <= n; i++)
	{
		int u = -1;
		long long Min = inf;
		for (int j = 1; j <= n; j++)
		{
			if (!vis[j] && d[j] < Min)
			{
				Min = d[j];
				u = j;
			}
		}

		if (u == -1)return;//说明剩下未被访问的点不连通
		vis[u] = 1;
		for (node *p = &Adj[u]; p != NULL; p = p->next)
		{
			int v = p->v, flag = p->flag;
			long long dis = p->dis;
			if (!vis[v])
			{
				if (flag)//若为小道
				{
					long long tempd = (d[u] - to[u] * to[u]) + (to[u] + dis) * (to[u] + dis);	//为了得到连续小道路程的平方，需要先减去to[u]*to[u](即之前连续小道的平方),
					if (tempd < d[v])															//再让之间的连续小道+下一段小道距离,取平方即得到接下来一步的总路程
					{
						d[v] = tempd;
						to[v] = to[u] + dis;
						pre[v] = u;//记录v的前驱u
					}
				}
				else//若为大道
				{
					long long tempd = d[u] + dis;
					if (tempd < d[v])
					{
						d[v] = tempd;
						to[v] = 0;
						pre[v] = u;//记录v的前驱u
					}
				}
			}
		}
	}
}

//递归显示路径
void path(int s)
{
	if (pre[s] == s)
	{
		printf("%d", s);
		return;
	}
	else
	{
		path(pre[s]);
		printf("->%d", s);
	}
}

int main()
{
	FILE *fp;
	if ((fp = fopen("data.txt", "r")) == NULL)
	{
		printf("打开文件失败！\n");
		exit(0);
	}

	fscanf(fp, "%d %d", &n, &m);

	for (int i = 1; i <= n; i++)Adj[i].next = NULL;
	for (int i = 0; i < m; i++)
	{
		int flag, u, v;
		long long dis;
		fscanf(fp, "%d %d %d %lld", &flag, &u, &v, &dis);
		add(flag, u, v, dis);
		add(flag, v, u, dis);
	}
	fclose(fp);

	dijkstra(1);
	printf("最小疲劳度为：%lld\n", d[n]);
	printf("路径为：\n");
	path(n);
	printf("\n从出发点至各个点的最小疲劳值：\n");
	for (int i = 2; i <= n; i++)
	{
		printf("点%d：%lld\n", i, d[i]);
	}

	system("pause");
	return 0;
}