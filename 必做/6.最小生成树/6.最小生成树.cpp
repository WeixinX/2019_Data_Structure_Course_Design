//���л�����Visual Studio 2017
#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<algorithm>
#pragma warning(disable:4996)

using namespace std;
const int maxv = 5000;
const int maxe = 5000;
const int nlen = 100;
const float inf = 1 << 30 - 1;

//ͼ�ṹ
typedef struct
{
	int vn,en;		//������,����
	char **vname;	//������
	float **A;		//�ڽӾ���
}GNode, *GH;

//�߽ṹ
struct e
{
	int u;
	int v;
	float w;
}edge[maxe];

//�����������
bool cmp(e a, e b) { return a.w < b.w; }


//���ݶ��������ض�Ӧ�ڽӾ����±�
int index(GH G, char vn[]) 
{
	for (int i = 0; i < G->vn; i++)
	{
		if ((strcmp(G->vname[i], vn)) == 0)return i;
	}
	return -1;
}

//��ͼ
void Create(GH & G)
{

	G = (GH)malloc(sizeof(GNode));
	if (!G)printf("��ͼ����ռ�ʧ�ܣ�\n");

	FILE *fp;
	if ((fp = fopen("data.txt", "r")) == NULL)
	{
		printf("���ļ�ʧ�ܣ�\n");
		exit(0);
	}

	int vnum;
	fscanf(fp,"%d",&vnum);
	G->vn = vnum;
	
	G->vname = (char **)malloc(G->vn * sizeof(char *));
	for (int i = 0; i < G->vn; i++)
	{
		G->vname[i] = (char *)malloc(nlen * sizeof(char));
	}
	//��ȡ������
	for (int i = 0; i < G->vn; i++)
	{
		char name[nlen];
		fscanf(fp,"%s", name);
		strcpy(G->vname[i], name);
	}

	G->A = (float **)malloc(G->vn * sizeof(float *));
	for (int i = 0; i < G->vn; i++)
	{
		G->A[i] = (float *)malloc(G->vn * sizeof(float));
	}
	//�ڽӾ����ʼ��
	for (int i = 0; i < G->vn; i++)
	{
		for (int j = 0; j < G->vn; j++)
			G->A[i][j] = inf;
	}
	//��ȡ��Ȩ
	char un[nlen], vn[nlen];
	float w;
	int en = 0;
	while ((fscanf(fp, "%s %s %f", un, vn, &w)) != EOF)
	{
		int u, v;//��������Ӧͼ���ڽӾ�����±�
		u = index(G, un);
		v = index(G, vn);
		G->A[u][v] = G->A[v][u] = w;
		edge[en].u = u;
		edge[en].v = v;
		edge[en].w = w;
		en++;
	}
	G->en = en;
	fclose(fp);
}

//Prim�㷨
void Prim(GH G)
{
	float d[maxv];//���㵽���������ϵľ���
	float ans = 0;//��С������Ȩֵ��
	int pre[maxv];//��¼�����µ��ǰ��
	bool vis[maxv] = { 0 };//vis[i] = 1��ʾ�±�Ϊi�ĵ��Ѿ�����
	for (int i = 0; i < G->vn; i++)
	{
		d[i] = inf;
		pre[i] = -1;
	}
	d[0] = 0;
	
	for (int i = 0; i < G->vn; i++)
	{
		int u = -1;
		float Min = inf;
		for (int i = 0; i < G->vn; i++)
		{
			if (!vis[i] && d[i] < Min)
			{
				u = i;
				Min = d[i];
			}
		}

		if (u == -1)return;//˵��ʣ�µĵ㲻��ͨ
		vis[u] = 1;

		ans += d[u];

		for (int v = 0; v < G->vn; v++)
		{
			if (!vis[v] && G->A[u][v] < d[v] && G->A[u][v] != inf)
			{
				d[v] = G->A[u][v];
				pre[v] = u;
			}
		}
	}
	printf("��С��������Ȩֵ֮�ͣ�%.2f\n", ans);
	printf("�õ��ıߣ�\n");
	for (int i = 0; i < G->vn; i++)
	{
		if (pre[i] != -1)
		{
			printf("%s ---- %s\n", G->vname[i], G->vname[pre[i]]);
		}
	}
}

//Kruskal�㷨
//���鼯
int fa[maxv];
int findeFa(int x)
{
	if (x == fa[x])return x;
	else return (fa[x] = findeFa(fa[x]));//·��ѹ��
}

void Kruskal(GH G)
{
	//���鼯��ʼ��
	for (int i = 0; i < G->vn; i++)fa[i] = i;

	float ans = 0;			//��С��������Ȩֵ֮��
	int num = 0,temp[maxe];	//��¼�������ߵ�����,temp[]��¼�õ��ıߵ��±�
	bool flag = 0;			//1��ʾ��������С������
	
	//������
	sort(edge, edge + G->en, cmp);
	
	//�ɲ��鼯��ʼ����һ��ʼ���ж��㶼����ͨ(�����ڵ������)
	//�ɱ�Ȩֵ������˳���������˵㲻��ͨ��������
	for (int i = 0; i < G->en; i++)
	{
		int fau = findeFa(edge[i].u);
		int fav = findeFa(edge[i].v);

		//�޹�ͬ���ڵ�˵�����˵㲻��ͨ
		if (fau != fav)
		{
			fa[fau] = fav;
			ans += edge[i].w;
			temp[num] = i;
			num++;
			if (num == G->vn - 1)break;
		}
	}
	if (num != G->vn - 1)flag = 1;
	if (flag == 0)
	{
		printf("��С��������Ȩֵ֮�ͣ�%.2f\n", ans);
		printf("�õ��ıߣ�\n");
		for (int i = 0; i < num; i++)
		{
			printf("%s ---- %s\n", G->vname[edge[temp[i]].u], G->vname[edge[temp[i]].v]);
		}
	}
	else printf("�޷��γ���С��������\n");
}

//���ڽӾ������ʽ���ͼ
void Gprint(GH G)
{
	for (int i = 0; i < G->vn; i++)
	{
		for (int j = 0; j < G->vn; j++)
		{
			if (G->A[i][j] != inf)printf("%-8.2f", G->A[i][j]);
			else printf("X       ");
		}
		printf("\n");
	}
}

int main()
{
	//��ͼ����
	GH G;
	Create(G);

	printf("Prim�㷨��\n");
	Prim(G);
	printf("\n");
	
	printf("Kruskal�㷨��\n");
	Kruskal(G);
	printf("\n");

	system("pause");
	return 0;
}