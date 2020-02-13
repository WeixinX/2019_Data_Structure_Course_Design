//运行环境：Visual Studio 2017
#include<cstdio>
#include<iostream>
#include<cstdlib>
#pragma warning(disable:4996)
using namespace std;
const int maxn = 100005;

struct node
{
	int key,time;
	node *next;
};

//链式队列存储取、放钥匙的事件
struct Queue
{
	node *front;
};
//初始化取、放钥匙的列表
void init(Queue &get, Queue &put)
{
	get.front = (node *)malloc(sizeof(node));
	get.front->next = NULL;
	put.front = (node *)malloc(sizeof(node));
	put.front->next = NULL;
}
//判断是否为空
bool empty(Queue Q)
{
	if (Q.front->next == NULL)return true;
	else return false;
}
//获取队列首元素
node *gethead(Queue Q)
{
	if (!empty(Q))return Q.front->next;
}
//入队，当时间相同时按照编号大小排序，否则按照时间排序
void enque(Queue &Q,int key,int time)
{
	//p为插入位置的前驱
	node *p = Q.front;
	//按时间顺序
	while (p->next != NULL && p->next->time < time)p = p->next;
	//时间相同时按照钥匙编号大小
	while (p->next != NULL && p->next->time == time && p->next->key < key)p = p->next;
	node *s = (node *)malloc(sizeof(node));
	s->key = key;
	s->time = time;
	s->next = p->next;
	p->next = s;
}
//出队
void deque(Queue &Q)
{
	if (!empty(Q))
	{
		node *p = Q.front->next;
		Q.front->next = p->next;
		free(p);
	}
}

//打印队列状态
void stat_queue(Queue get,Queue put)
{
	node *p = get.front->next;
	printf("获取钥匙事件队列状态：");
	if(p != NULL)printf("\n钥匙编号     获取时间\n");
	else printf("无事件\n");
	while (p != NULL)
	{
		printf("%-5d         %-5d\n", p->key, p->time);
		p = p->next;
	}
	p = put.front->next;
	printf("归还钥匙事件队列状态：");
	if(p != NULL)printf("\n钥匙编号     归还时间\n");
	else printf("无事件\n");
	while (p != NULL)
	{
		printf("%-5d         %-5d\n", p->key, p->time);
		p = p->next;
	}
	printf("-------------------------------------\n");
}

//打印钥匙盒状态
void stat_key(int a[],int n)
{
	printf("钥匙盒状态：\n");
	for (int i = 1; i <= n; i++)
	{
		if (a[i] == -1)printf("x ");
		else printf("%d ", a[i]);
	}
	printf("\n");
}

int main()
{
	FILE *fp;
	if ((fp = fopen("time.txt", "r")) == NULL)
	{
		printf("打开文件失败！\n");
		exit(0);
	}
	Queue get, put;
	init(get, put);
	int n, k;
	int a[maxn];//存放钥匙编号
	int start = maxn, end = -1;//开始时间和结束时间
	fscanf(fp,"%d%d", &n, &k);
	for (int i = 1; i <= n; i++)a[i] = i;//初始化钥匙编号
	for (int i = 0; i < k; i++)
	{
		int key, s, con;
		fscanf(fp,"%d%d%d", &key, &s, &con);
		if (s < start)start = s;
		if (s + con > end)end = s + con;
		enque(get, key, s);			//取钥匙事件加入队列
		enque(put, key, s + con);	//放钥匙事件加入队列
	}
	fclose(fp);
	//打印队列和钥匙盒初始状态
	stat_key(a, n);
	stat_queue(get, put);

	for (int i = start; i <= end; i++)
	{
		//执行放钥匙事件
		while (!empty(put))
		{
			node *head = gethead(put);
			if (head->time == i)
			{
				for (int j = 1; j <= n; j++)
				{
					if (a[j] == -1)
					{
						a[j] = head->key;
						deque(put);
						stat_key(a, n);
						stat_queue(get, put);
						break;
					}
				}
			}
			else break;
		}
		//执行取钥匙事件
		while (!empty(get))
		{
			node *head = gethead(get);
			if (head->time == i)
			{
				for (int j = 1; j <= n; j++)
				{
					if (a[j] == head->key)
					{
						a[j] = -1;
						deque(get);
						stat_key(a, n);
						stat_queue(get, put);
						break;
					}
				}
			}
			else break;
		}
	}
	printf("钥匙盒最终状态：");
	for (int i = 1; i <= n; i++)
	{
		printf("%d ", a[i]);
	}
	system("pause");
	return 0;
}