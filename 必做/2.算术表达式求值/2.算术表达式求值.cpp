//���л�����Visual Studio 2017
#include<cstdio>
#include<iostream>
#include<cmath>
#include<cctype>
#include<cstring>
#pragma warning(disable:4996)
using namespace std;
const int maxn = 10000;
const double error = -(1 << 30 - 1);

//Ԫ�ؽṹ
struct node
{
	double num;	//������
	char op;	//������
	bool flag;	//1��ʾ��Ԫ��Ϊ��������0��ʾΪ������
};

//ջ
typedef struct
{
	node data[maxn];
	int top;
}Stack;

//ջ�Ļ�������
void InitStack(Stack & s){s.top = -1;}

void push(Stack & s,node a){s.data[++s.top] = a;}

void pop(Stack & s){if (s.top >= 0)s.top--;}

node gettop(Stack s){if (s.top >= 0)return s.data[s.top];}

bool sempty(Stack s)
{
	if (s.top >= 0)return false;
	else return true;
}


//����
typedef struct
{
	node data[maxn];
	int head, tail;
}Queue;

//���еĻ�������
void InitQueue(Queue & q){q.head = q.tail = 0;}

void enque(Queue & q,node a){q.data[q.tail++] = a;}

void deque(Queue & q){if(q.head < q.tail)q.head++;}

node gethead(Queue q){if (q.head < q.tail)return q.data[q.head];}

bool qempty(Queue q)
{
	if (q.head < q.tail)return false;
	else return true;
}


//��������ȼ�
//0,1,2,3,4,5 == +,-,*,/,(,)
int OP[6] = { 1,1,2,2,0,0 };

//�ж�������±�
int index(char c)
{
	switch (c)
	{
	case '+':return 0;
	case '-':return 1;
	case '*':return 2;
	case '/':return 3;
	case '(':return 4;
	case ')':return 5;
	}
}

//���ʽת��Ϊ��׺���ʽ
void Change(char str[],Queue & q)
{
	Stack s;
	node temp;
	InitStack(s);//������ջ
	InitQueue(q);//���д洢��׺���ʽ
	bool IsPoint = 0;	//��¼�Ƿ�ΪС����
	int t = 0;			//����ֵ��ʾС�����λ		

	printf("��׺���ʽת��׺���ʽ�ı仯���̣�\n");
	for (int i = 0; str[i] != '\0';)
	{
		//������
		if (str[i] >= '0' && str[i] <= '9')
		{
			temp.flag = 1;
			temp.num = str[i++] - '0';
			//�ϲ���λ��
			while (str[i] != '\0' && ((str[i] >= '0' && str[i] <= '9') || str[i] == '.'))
			{
				//����С����
				if (str[i] == '.')
				{
					IsPoint = 1;
					t = -1;
				}
				//�ϲ�С��������
				else if (IsPoint)
				{
					temp.num = temp.num + (str[i] - '0') * pow(10, t);
					t--;
				}
				else
				{
					temp.num = temp.num * 10 + (str[i] - '0');
				}
				i++;
			}
			enque(q,temp);
		}
		//������
		else
		{
			IsPoint = t = 0;//����¼С���������ֵ��ʼ��
			temp.flag = 0;
			//��������Ϊ������,ѹ��
			if (str[i] == '(')
			{
				temp.op = '(';
				push(s, temp);
				i++;
			}
			//��Ϊ������,��ջֱ���������ŵ���
			else if(str[i] == ')')
			{
				//���������ڵĲ���������׺���ʽ������
				while (!sempty(s) && gettop(s).op != '(')
				{
					enque(q, gettop(s));
					pop(s);
				}
				//����������
				if (gettop(s).op == '(')pop(s);
				i++;
			}
			//������Ϊ+,-,*,/
			//ջ�����������ȼ� < ������,��������ѹ��ջ
			//ջ�����������ȼ� >= ������,��ջ��ջ�����������ȼ� < ������,�ٽ�������ѹ��ջ
			else
			{
				while (!sempty(s) && OP[index(gettop(s).op)] >= OP[index(str[i])])
				{
					enque(q, gettop(s));
					pop(s);
				}
				temp.op = str[i];
				push(s, temp);
				i++;
			}
		}
	}
	while (!sempty(s))
	{
		enque(q, gettop(s));
		pop(s);
	}
}

//�ú�׺���ʽ��ֵ
double Calculate(Queue & q)
{
	Stack s;
	node temp,a,b,c;
	int step = 1;//ջ�Լ����б仯����
	InitStack(s);

	//��ʾ���г�ʼ���
	printf("%-4d", step++);
	for (int i = q.head; i < q.tail; i++)
	{
		if (q.data[i].flag)printf("%-8.2lf ", q.data[i].num);
		else printf("%c ", q.data[i].op);
	}
	printf("\n");

	while (!qempty(q))
	{
		temp = gethead(q);
		deque(q);
		//������,ֱ����ջ
		if (temp.flag)push(s, temp);
		//������
		else
		{
			c.flag = 1;		//�洢������c = a op b
			b = gettop(s);
			pop(s);
			a = gettop(s);
			pop(s);

			switch (temp.op)
			{
			case '+':c.num = a.num + b.num; break;
			case '-':c.num = a.num - b.num; break;
			case '*':c.num = a.num * b.num; break;
			case '/':
				if (b.num == 0)return error;
				else c.num = a.num / b.num; 
				break;
			}
			push(s, c);

			//��ʾջ�Լ����б仯�ı仯����
			printf("%-4d", step++);
			for (int i = 0; i <= s.top; i++)
			{
				printf("%-8.2lf ", s.data[i].num);
			}
			for (int i = q.head; i < q.tail; i++)
			{
				if (q.data[i].flag)printf("%-8.2lf ", q.data[i].num);
				else printf("%c ", q.data[i].op);
			}
			printf("\n");
		}
	}
	return gettop(s).num;
}

//��׺���ʽ�������
void RPNprint(char str[])
{
	Queue q;
	Change(str, q);
	while (!qempty(q))
	{
		node temp;
		temp = gethead(q);
		if (temp.flag)
		{
			printf("%.2lf ", temp.num);
		}
		else printf("%c ", temp.op);
		deque(q);
	}
	printf("\n");
}

//��ת���ɺ�׺���ʽǰ�ļ��,�����쳣����0
bool Before_Check(char str[])
{
	Stack s;
	node temp;
	InitStack(s);
	temp.flag = 0;	//0��ʾ��������1��ʾ������
	bool flag = 0;	//0��ʾ������,1��ʾ������
	
	//����ƥ�� �� �쳣���ż��
	for (int i = 0; str[i] != '\0';i++)
	{
		temp.op = str[i];
		//���1: ������  ֱ����ջ
		if (str[i] == '(')push(s, temp);
		//���2: ������ ��ջ��Ԫ����"("�����������쳣���������Ų�ƥ�� 
		else if (str[i] == ')')
		{
			if(gettop(s).op == '(')pop(s);
			else
			{
				flag = 1;
				break;
			}
		}
		//���쳣���Ź涨Ϊ +,-,*,/,.,(,),����0-9
		else if (!(str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '.' || (str[i] >= '0' && str[i] <= '9')))
		{
			printf("���ʽ�����쳣���ţ�\n");
			return 0;
		}
	}
	//���ջ����˵������Ҳ��ƥ��
	if (!sempty(s))flag = 1;
	if (flag == 1)
	{
		printf("���ʽ����ƥ�����\n");
		return 0;
	}

	//���������Ͳ�������λ�ù�ϵ
	for (int i = 0; str[i] != '\0'; i++)
	{
		if (i >= 1 && str[i + 1] != '\0')
		{
			if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/')
			{
				if (!(str[i - 1] >= '0' && str[i - 1] <= '9' && str[i + 1] >= '0' && str[i + 1] <= '9' || str[i - 1] == '(' || str[i - 1] == ')' || str[i + 1] == '(' || str[i + 1] == ')'))
				{
					printf("���ʽ�в������������λ�ù�ϵ����\n");
					return 0;
				}
			}
		}
	}
	//else printf("���쳣��\n");
	return 1;
}

//ת��Ϊ��׺���ʽ��ļ�飬�����쳣����0
bool After_Check(Queue q)
{
	int numcnt = 0,opcnt = 0;//��¼�������Ͳ������ĸ���
	int p = 0;
	while (p < q.tail)
	{
		
		//������
		if (q.data[p].flag)numcnt++;
		else opcnt++;
		p++;
	}
	if (opcnt < numcnt - 1)
	{
		printf("���ʽ�в��������٣�\n");
		return 0;
	}
	else if (opcnt > numcnt - 1)
	{
		printf("���ʽ�д��ڶ����������\n");
		return 0;
	};
	//else printf("���쳣��\n");
	return 1;
}

int main()
{
	while (1)
	{
		system("cls");
		printf
		(
			"�������ʽ��ֵ\n"
			"�涨:\n"
			"1.�������޶�Ϊ��ʵ��\n"
			"2.����������+,-,*,/,(,)\n"
			"3.ע�����ŵ�ƥ��\n"
			"4.ע���������Ϊ0\n"
			"5.ע�����������\n"
			"6.ע������ż��޿ո�\n"
			"7.������ʽ���磺6+15*(21-8/4)\n\n"
		);
		Queue q;//�洢��׺���ʽ
		char str[maxn];
		bool flag = 1;//1��ʾ���ʽ���쳣
		printf("��������ȷ���������ʽ��\n");
		scanf("%s", str);
		getchar();

		//����ƥ�䡢�쳣���š�������������λ�ù�ϵ����
		if (!Before_Check(str))flag = 0;
		
		//��׺���ʽת��Ϊ��׺���ʽ
		if(flag)Change(str, q);
		
		//���������������������
		if (flag)
		{
			if (!After_Check(q))flag = 0;
		}
		
		//flag == 1���쳣�ż������м���,������error˵������Ϊ0
		double res = 0;
		if (flag)res = Calculate(q);
		if (res == error)printf("���ڳ���Ϊ0��\n");
		if (res != error && flag != 0)
		{
			printf("%s = ", str);
			printf("%.2lf\n", res);
		}

		//ѯ���Ƿ��������
		char ch;
		printf("�Ƿ�������㣿��y/n����");
		scanf("%c", &ch);
		if (toupper(ch) == 'Y')continue;
		else break;
	}

	system("pause");
	return 0;
}
