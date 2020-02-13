//���л�����Visual Studio 2017
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

//��ʽ���д洢ȡ����Կ�׵��¼�
struct Queue
{
	node *front;
};
//��ʼ��ȡ����Կ�׵��б�
void init(Queue &get, Queue &put)
{
	get.front = (node *)malloc(sizeof(node));
	get.front->next = NULL;
	put.front = (node *)malloc(sizeof(node));
	put.front->next = NULL;
}
//�ж��Ƿ�Ϊ��
bool empty(Queue Q)
{
	if (Q.front->next == NULL)return true;
	else return false;
}
//��ȡ������Ԫ��
node *gethead(Queue Q)
{
	if (!empty(Q))return Q.front->next;
}
//��ӣ���ʱ����ͬʱ���ձ�Ŵ�С���򣬷�����ʱ������
void enque(Queue &Q,int key,int time)
{
	//pΪ����λ�õ�ǰ��
	node *p = Q.front;
	//��ʱ��˳��
	while (p->next != NULL && p->next->time < time)p = p->next;
	//ʱ����ͬʱ����Կ�ױ�Ŵ�С
	while (p->next != NULL && p->next->time == time && p->next->key < key)p = p->next;
	node *s = (node *)malloc(sizeof(node));
	s->key = key;
	s->time = time;
	s->next = p->next;
	p->next = s;
}
//����
void deque(Queue &Q)
{
	if (!empty(Q))
	{
		node *p = Q.front->next;
		Q.front->next = p->next;
		free(p);
	}
}

//��ӡ����״̬
void stat_queue(Queue get,Queue put)
{
	node *p = get.front->next;
	printf("��ȡԿ���¼�����״̬��");
	if(p != NULL)printf("\nԿ�ױ��     ��ȡʱ��\n");
	else printf("���¼�\n");
	while (p != NULL)
	{
		printf("%-5d         %-5d\n", p->key, p->time);
		p = p->next;
	}
	p = put.front->next;
	printf("�黹Կ���¼�����״̬��");
	if(p != NULL)printf("\nԿ�ױ��     �黹ʱ��\n");
	else printf("���¼�\n");
	while (p != NULL)
	{
		printf("%-5d         %-5d\n", p->key, p->time);
		p = p->next;
	}
	printf("-------------------------------------\n");
}

//��ӡԿ�׺�״̬
void stat_key(int a[],int n)
{
	printf("Կ�׺�״̬��\n");
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
		printf("���ļ�ʧ�ܣ�\n");
		exit(0);
	}
	Queue get, put;
	init(get, put);
	int n, k;
	int a[maxn];//���Կ�ױ��
	int start = maxn, end = -1;//��ʼʱ��ͽ���ʱ��
	fscanf(fp,"%d%d", &n, &k);
	for (int i = 1; i <= n; i++)a[i] = i;//��ʼ��Կ�ױ��
	for (int i = 0; i < k; i++)
	{
		int key, s, con;
		fscanf(fp,"%d%d%d", &key, &s, &con);
		if (s < start)start = s;
		if (s + con > end)end = s + con;
		enque(get, key, s);			//ȡԿ���¼��������
		enque(put, key, s + con);	//��Կ���¼��������
	}
	fclose(fp);
	//��ӡ���к�Կ�׺г�ʼ״̬
	stat_key(a, n);
	stat_queue(get, put);

	for (int i = start; i <= end; i++)
	{
		//ִ�з�Կ���¼�
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
		//ִ��ȡԿ���¼�
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
	printf("Կ�׺�����״̬��");
	for (int i = 1; i <= n; i++)
	{
		printf("%d ", a[i]);
	}
	system("pause");
	return 0;
}