//���л�����Visual Studio 2017
#include<iostream>
#include<cstdio>
#include<cstdlib>
#pragma warning(disable:4996)

using namespace std;
const int maxv = 505;
const int inf = 1 << 30 - 1;

//��ʽ�ڽӱ�ṹ
struct node
{
	int flag;		//1��ʾС����0��ʾ���
	int v;			//����ĵ�
	long long dis;	//�����ľ���
	node *next;		//ָ����һ��
}Adj[maxv];

//�����ڽӵ�
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

bool vis[maxv];		//0��ʾ�õ�δ������
long long to[maxv];	//to[i]��ʾ����iʱǰ���ܹ��߹���·��
long long d[maxv];	//d[i]��ʾ���s����i����̾���
int pre[maxv];		//pre[i]��ʾ��i��ǰ��,ʹ�õݹ�������·��

int n, m;
//�ؼ��㷨
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

		if (u == -1)return;//˵��ʣ��δ�����ʵĵ㲻��ͨ
		vis[u] = 1;
		for (node *p = &Adj[u]; p != NULL; p = p->next)
		{
			int v = p->v, flag = p->flag;
			long long dis = p->dis;
			if (!vis[v])
			{
				if (flag)//��ΪС��
				{
					long long tempd = (d[u] - to[u] * to[u]) + (to[u] + dis) * (to[u] + dis);	//Ϊ�˵õ�����С��·�̵�ƽ������Ҫ�ȼ�ȥto[u]*to[u](��֮ǰ����С����ƽ��),
					if (tempd < d[v])															//����֮�������С��+��һ��С������,ȡƽ�����õ�������һ������·��
					{
						d[v] = tempd;
						to[v] = to[u] + dis;
						pre[v] = u;//��¼v��ǰ��u
					}
				}
				else//��Ϊ���
				{
					long long tempd = d[u] + dis;
					if (tempd < d[v])
					{
						d[v] = tempd;
						to[v] = 0;
						pre[v] = u;//��¼v��ǰ��u
					}
				}
			}
		}
	}
}

//�ݹ���ʾ·��
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
		printf("���ļ�ʧ�ܣ�\n");
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
	printf("��Сƣ�Ͷ�Ϊ��%lld\n", d[n]);
	printf("·��Ϊ��\n");
	path(n);
	printf("\n�ӳ����������������Сƣ��ֵ��\n");
	for (int i = 2; i <= n; i++)
	{
		printf("��%d��%lld\n", i, d[i]);
	}

	system("pause");
	return 0;
}