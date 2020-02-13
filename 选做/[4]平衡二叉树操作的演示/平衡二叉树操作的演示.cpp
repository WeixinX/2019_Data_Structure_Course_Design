//���л�����Viusal Studio 2017
#include<cstdio>
#include<cstdlib>
#include<iostream>
#pragma warning(disable:4996)

using namespace std;
const int maxn = 5000;

//ƽ��������ڵ�ṹ
typedef struct node
{
	int data;			//��ֵ
	node *left, *right;	//��������
	int height;			//����
}Anode,*AT;

//��������
int height(AT T)
{
	if (T == NULL)return 0;
	else return T->height;
}

//��������
void updateH(AT &T)
{
	int h1 = height(T->left);
	int h2 = height(T->right);
	if (h1 > h2) T->height = h1 + 1;
	else T->height = h2 + 1;
}

//������T��ƽ������(�������߶� - �������߶�)
int getBF(AT T)
{
	int BF = height(T->left) - height(T->right);
	return BF;
}

//��ѯ����
void search(AT T,int x)
{
	if (T == NULL)
	{
		printf("ƽ��������в����� %d \n",x);
		return;
	}
	else if (T->data == x)
	{
		printf("ƽ��������д��� %d \n", T->data);
	}
	else if (T->data > x)		//�����ѯ
	{
		search(T->left, x);
	}
	else search(T->right, x);	//���Ҳ�ѯ
}

//����
void L(AT &T)
{
	Anode *temp = T->right;
	T->right = temp->left;
	temp->left = T;
	
	//���¸߶�
	updateH(T);
	updateH(temp);
	//���¸��ڵ�
	T = temp;
	
	//���¸��ڵ�
	T = temp;
}

//����
void R(AT &T)
{
	Anode *temp = T->left;
	T->left = temp->right;
	temp->right = T;

	//���¸߶�
	updateH(T);
	updateH(temp);

}

//�������
//����ʱ���������жϽڵ��Ƿ�ƽ��
//�������������²���
//	����			�ж�����							����
//  LL			��ƽ������Ϊ2��������ƽ������Ϊ1		�Ը���������
//	LR			��ƽ������Ϊ2��������ƽ������Ϊ-1		���������������ٶԸ�����
//	RR			��ƽ������Ϊ2��������ƽ������Ϊ-1		�Ը���������
//	RL			��ƽ������Ϊ2��������ƽ������Ϊ1		���������������ٶԸ�����
void insert(AT &T,int x)
{
	if (T == NULL)
	{
		T = (AT)malloc(sizeof(Anode));
		T->data = x;
		T->left = T->right = NULL;
		T->height = 1;
		return;
	}
	else if (T->data > x)//����߲���
	{
		insert(T->left, x);
		//��������
		updateH(T);
		//��ʧ�⣬��ƽ������ >= 2 (ʵ����ֻ���� BF == 2 �����)
		if (getBF(T) >= 2)
		{
			//��������ѡ�����
			if (getBF(T->left) == 1)		//LL������
			{
				R(T);
			}
			else if (getBF(T->left) == -1)	//LR��������������
			{
				L(T->left);
				R(T);
			}
		}

	}
	else if (T->data < x)//���ұ߲���
	{
		insert(T->right, x);
		//��������
		updateH(T);
		//��ʧ�⣬��ƽ������ >= 2 (ʵ����ֻ���� BF == -2 �����)
		if (getBF(T) <= -2)
		{
			//��������ѡ�����
			if (getBF(T->right) == -1)		//RR������
			{
				L(T);
			}
			else if (getBF(T->right) == 1)	//RL��������������
			{
				R(T->right);
				L(T);
			}
		}
	}
}

//ƽ��������Ĵ���
void create(AT &T, int data[], int n)
{
	T = NULL;
	for (int i = 0; i < n; i++)
	{
		insert(T, data[i]);
	}
}

//Ѱ����TΪ�������ֵ�ڵ�(ǰ��)
Anode *findMax(AT T)
{
	while (T->right) T = T->right;
	return T;
}

//Ѱ����TΪ������Сֵ�ڵ�(���)
Anode *findMin(AT T)
{
	while (T->left) T = T->left;
	return T;
}

//ɾ������
void del(AT &T, int x)
{
	if (!T)return;
	//�ҵ���Ҫɾ���Ľڵ�T
	if (T->data == x)
	{
		//��ΪҶ�ӽڵ㣬��ֱ��ɾ��������Ƿ�ƽ��
		if (!T->left && !T->right)
		{
			T = NULL;
			free(T);
			return;
		}
		//������������������ǰ������Ҫɾ���Ľڵ㣬��ɾ��ǰ��
		if (T->left && !T->right)
		{
			Anode *pre = findMax(T->left);
			T->data = pre->data;
			del(T->left, pre->data);
		}
		//�����ú�̸��ǣ���ɾ�����
		else
		{
			Anode *next = findMin(T->right);
			T->data = next->data;
			del(T->right, next->data);
		}
	}
	else if (T->data > x) del(T->left, x);
	else del(T->right, x);

	//��������
	updateH(T);
	//printf("%d-H:%d , %d\n", T->data, T->height, getBF(T));
	//��ʧ�⣬��ƽ������ >= 2 (ʵ����ֻ���� BF == 2 �����)(ɾ���ҽڵ�)
	if (getBF(T) >= 2)
	{
		//��������ѡ�����
		if (getBF(T->left) == 1 || getBF(T->left) == 0)		//LL������
		{
			R(T);
		}
		else if (getBF(T->left) == -1)						//LR��������������
		{
			L(T->left);
			R(T);
		}
	}

	//����ƽ������ <= -2 (ʵ����ֻ���� BF == -2 �����)(ɾ����ڵ�)
	else if (getBF(T) <= -2)
	{
		//��������ѡ�����
		if (getBF(T->right) == -1 || getBF(T->right) == 0)		//RR������
		{
			L(T);
		}
		else if (getBF(T->right) == 1)							//RL��������������
		{
			R(T->right);
			L(T);
		}
	}
}

//�԰�������ƽ�������
void print(AT T, int i) 
{
	if (T)
	{
		print(T->right, i + 1);
		for (int j = 0; j < i * 4; j++) printf(" ");
		printf("%d\n", T->data,T->height);
		print(T->left, i + 1);
	}
}

//�������
void in(AT T)
{
	if (T)
	{
		in(T->left);
		printf("%d ", T->data);
		in(T->right);
	}
}

int main()
{
	int n,data[maxn];
	printf("����ƽ�������\n");
	printf("������Ԫ�ظ�����");
	scanf("%d", &n);
	printf("������Ԫ��ֵ��");
	for (int i = 0; i < n; i++)
	{
		scanf("%d", &data[i]);
	}
	
	AT T;
	create(T, data, n);
	printf("�����\n");
	print(T, 0);
	printf("\n");

	while (1)
	{
		system("pause");
		system("cls");
		printf
		(
			"1.��ʾ\n"
			"2.����\n"
			"3.����\n"
			"4.ɾ��\n"
			"5.�˳�\n"
			"������ѡ�"
		);
		int c,x,flag = 0;
		scanf("%d", &c);
		switch (c)
		{
			case 1:
				printf("�����\n");
				print(T, 0);
				printf("\n");
				break;
			case 2:
				printf("������Ҫ���ҵ�ֵ��");
				scanf("%d", &x);
				search(T, x);
				break;
			case 3:
				printf("������Ҫ�����ֵ��");
				scanf("%d", &x);
				insert(T, x);

				printf("�����\n");
				print(T, 0);
				printf("\n");
				break;
			case 4:
				printf("������Ҫɾ����ֵ��");
				scanf("%d", &x);
				del(T,x);

				printf("�����\n");
				print(T, 0);
				printf("\n");
				break;
			case 5:
				flag = 1;
				break;
		}
		if (flag == 1)break;
	}
	return 0;
}