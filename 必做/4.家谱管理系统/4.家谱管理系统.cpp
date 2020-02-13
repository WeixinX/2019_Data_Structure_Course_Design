//���л�����Visual Studio 2017
#include<iostream>
#include<stdio.h>
#include<cstdlib>
#include<cctype>
#pragma warning(disable:4996)

using namespace std;
const int maxn = 30;
const int maxv = 1000;

struct date
{
	int year, moon, day;
};

struct person
{
	char name[50];		//����
	date birth;			//����
	bool hun;			//���
	char addr[60];		//��ַ
	bool jian;			//���ڷ�
	date death;			//��������
	int chilnum;		//������
	int dai;			//����
	person *chil[maxn];	//����ָ������
	person *pa;			//���ڵ�
};

person *p;

int tot = 0;//����������

//��������
date set_date(char arr[])
{
	int cnt, y, m, d;
	cnt = y = m = d = 0;
	for (int i = 0; arr[i] != '\0'; i++)
	{
		if (arr[i] == '/')cnt++;
		else
		{
			if (cnt == 0)
			{
				y = y * 10 + arr[i] - '0';
			}
			else if (cnt == 1)
			{
				m = m * 10 + arr[i] - '0';
			}
			else
			{
				d = d * 10 + arr[i] - '0';
			}
		}
	}
	date t;
	t.year = y;
	t.moon = m;
	t.day = d;
	return t;
}

//������ڣ���ʽΪ yy-mm-dd��
void print_date(date a)
{
	printf("%d-%d-%d", a.year, a.moon, a.day);
}

//������������ĳ�ڵ㣨����������ֲ��ظ���
person *name_search(person *root,char name[])
{
	if (!root)return NULL;
	if (strcmp(root->name, name) == 0) return root;
	for (int i = 0; i < root->chilnum; i++)
	{		person *p = name_search(root->chil[i], name);
		if (p) return p;
	}
	return NULL;
}

//�������ղ�ѯ��Ա����
void birth_search(person *root, int y, int m, int d)
{
	if (root->birth.year == y && root->birth.moon == m && root->birth.day == d) printf("%s\n", root->name);
	else
	{
		for (int i = 0; i < root->chilnum; i++)
		{
			birth_search(root->chil[i], y, m, d);
		}
	}
}

//��ȡ�ļ�����������
void read(person *&root)
{
	FILE *fp;
	if ((fp = fopen("family_tree.txt", "r")) == NULL)
	{
		printf("���ļ�ʧ�ܣ�\n");
		system("pause");
		exit(-1);
	}
	char buff[4096];//������
	char dai[4],name[64], birth[64], hun[8], addr[64], jian[8], death[64], chil[2048];

	fgets(buff, 4096, fp);//��ͷ����
	fgets(buff, 4096, fp);
	while (!feof(fp))
	{
		int i = 0,cnt = 0;

		//��
		while (buff[i] != '\t') dai[cnt++] = buff[i++];
		dai[cnt] = '\0';
		i++;cnt = 0;
		int d = 0,j = 0;
		while (dai[j] != '\0') d = d * 10 + dai[j++] - '0';

		//����
		while (buff[i] != '\t') name[cnt++] = buff[i++];
		name[cnt] = '\0';
		i++;cnt = 0;

		//��������
		while (buff[i] != '\t') birth[cnt++] = buff[i++];
		birth[cnt] = '\0';
		i++;cnt = 0;

		//���
		while (buff[i] != '\t') hun[cnt++] = buff[i++];
		hun[cnt] = '\0';
		i++;cnt = 0;

		//��ַ
		while (buff[i] != '\t') addr[cnt++] = buff[i++];
		addr[cnt] = '\0';
		i++;cnt = 0;

		//���ڷ�
		while (buff[i] != '\t') jian[cnt++] = buff[i++];
		jian[cnt] = '\0';
		i++;cnt = 0;

		//��������
		while (buff[i] != '\t') death[cnt++] = buff[i++];
		death[cnt] = '\0';
		i++;cnt = 0;

		//�����½ڵ�
		person *p = NULL;

		if (tot == 0)//¼����ڵ�
		{
			root = (person *)malloc(sizeof(person));
			for (j = 0; j < maxn; j++) root->chil[j] = NULL;
			root->pa = NULL;

			root->dai = d;									//��
			strcpy(root->name, name);						//����
			strcpy(root->addr, addr);						//��ַ
			if (strcmp(hun, "��") == 0) root->hun = 1;		//���
			else root->hun = 0;
			if (strcmp(jian, "����") == 0) root->jian = 1;	//���ڷ�
			else root->jian = 0;
			if (!root->jian) root->death = set_date(death);	//��������
			root->birth = set_date(birth);					//��������
			root->chilnum = 0;								//���Ӹ���
		}
		else//���Ǹ��Ļ�����ͨ�������ҵ���Ӧ�Ľڵ㣬�������Ǽ������ֲ��ظ�
		{
			p = name_search(root, name);
			//��д��Ϣ
			p->dai = d;
			strcpy(p->name, name);
			strcpy(p->addr, addr);
			if (strcmp(hun, "��") == 0) p->hun = 1;
			else p->hun = 0;
			if (strcmp(jian, "����") == 0) p->jian = 1;
			else p->jian = 0;
			if (!p->jian) p->death = set_date(death);
			p->birth = set_date(birth);
			p->chilnum = 0;
		}

		//����ָ������
		strcpy(chil, &buff[i]);
		i = 0;
		do
		{
			if (strcmp(chil, "��\n") == 0)break;
			else
			{
				cnt = 0;
				char temp[50];
				while (chil[i] != ' ' && chil[i] != '\n')
				{
					temp[cnt++] = chil[i++];
				}
				temp[cnt] = '\0';
				//printf("%s\n", temp);
				
				person *child = (person *)malloc(sizeof(person));
				for (j = 0; j < maxn; j++) child->chil[j] = NULL;

				child->chilnum = 0;
				strcpy(child->name, temp);

				if (tot == 0)//����Щ���ӽڵ�ĸ��ڵ��Ǹ�
				{
					child->pa = root;
					//printf("pa- %s \n", child->pa->name);
					root->chil[root->chilnum] = child;
					root->chilnum++;
				}
				else
				{
					child->pa = p;
					p->chil[p->chilnum] = child;
					p->chilnum++;
				}
				if (chil[i] == '\n')break;
				i++;
			}
		} while (chil[i] != '\n');
		tot++;
		fgets(buff, 4096, fp);
	}
	fclose(fp);
}

//����ڵ�p�������Ϣ
void print_info(person *p)
{
	if (p)
	{
		printf("������%s\n", p->name);
		
		printf("�������ڣ�");
		print_date(p->birth);
		printf("\n");
		
		printf("��ַ��%s\n", p->addr);
		
		printf("���");
		if (p->hun) printf("�ѻ�\n");
		else printf("δ��\n");
		
		printf("���ڷ�");
		if (p->jian) printf("����\n");
		else printf("�ѹ�\n");
		
		printf("�������ڣ�");
		if (!p->jian)
		{
			print_date(p->death);
			printf("\n");
		}
		else printf("��\n");

		printf("��ĸ��%s\n", p->pa->name);

		printf("���ӣ�");
		if (p->chilnum == 0)printf("��");
		for (int i = 0; i < p->chilnum; i++)
		{
			printf("%s  ", p->chil[i]->name);
		}
		printf("\n");
	}
}

//��α��������n���˵���Ϣ
void layer(person *root,int n)
{
	if (root)
	{
		person *Q[maxv];//����
		int front = 0, rear = 0,lastrear = 0,d = 1;
		Q[rear++] = root;
		lastrear = rear;//���ڷֲ�
		while (front < rear)
		{
			person *fnt = Q[front++];
			if (d == n)
			{
				print_info(fnt);
				printf("======================\n");
			}

			//�������
			for (int i = 0; i < fnt->chilnum; i++)
			{
				Q[rear++] = fnt->chil[i];
			}

			if (front == lastrear)
			{
				lastrear = rear;
				d++;
			}
		}
		printf("\n");
	}
}

//�޸Ļ�����Ϣ
void modify(person *p)
{
	char birth[64], death[64], hun[8], jian[8];
	printf("������");
	scanf("%s", p->name);

	printf("��������(yy/mm/dd)��");
	scanf("%s", birth);
	p->birth = set_date(birth);

	printf("��ַ��");
	scanf("%s", p->addr);

	printf("���(��/��)��");
	scanf("%s", hun);
	if (strcmp(hun, "��") == 0) p->hun = 1;
	else p->hun = 0;

	printf("���ڷ�(����/�ѹ�)��");
	scanf("%s", jian);
	if (strcmp(jian, "����") == 0) p->jian = 1;
	else p->jian = 0;

	if (p->jian == 0)
	{
		printf("��������(yy/mm/dd)��");
		scanf("%s", death);
		p->death = set_date(death);
	}
}

//������������ȷ�����ϵ
void relat(person *root, char name1[], char name2[])
{
	person *p1, *p2, *temp = (person *)malloc(sizeof(person));
	p1 = name_search(root, name1);
	p2 = name_search(root, name2);
	if (!p1 && !p2) printf("�������� %s ��%s\n", name1, name2);
	else if (!p1 && p2)printf("�������� %s\n", name1);
	else if (p1 && !p2) printf("�������� %s\n", name2);
	else
	{
		int d;
		//��p1Ϊ��������������ԽС�򱲷�Խ��
		if (p1->dai >= p2->dai)
		{
			d = p1->dai - p2->dai;
			temp = p1;
			p1 = p2;
			p2 = temp;
		}
		else d = p2->dai - p1->dai;
		switch (d)
		{
		case 0:
			if (p2->pa == p1->pa) printf("%s �� %s ���ֵ�\n", p1->name, p2->name);
			else printf("%s �� %s �����ֵ�\n", p1->name, p2->name);
			break;
		case 1:
			if (p2->pa == p1) printf("%s �� %s �ĸ�ĸ\n", p1->name, p2->name);
			else printf("%s �� %s �Ĳ���ĸ\n", p1->name, p2->name);
			break;
		case 2:
			if (p2->pa->pa == p1) printf("%s �� %s ���游ĸ\n", p1->name, p2->name);
			else printf("%s �� %s �Ĳ��游ĸ\n", p1->name, p2->name);
			break;
		case 3:
			if (p2->pa->pa->pa == p1) printf("%s �� %s �����游ĸ\n", p1->name, p2->name);
			else printf("%s �� %s �Ĳ����游\n", p1->name, p2->name);
			break;
		case 4:
			if (p2->pa->pa->pa->pa == p1) printf("%s �� %s �ĸ��游ĸ\n", p1->name, p2->name);
			else printf("%s �� %s �Ĳ����游\n", p1->name, p2->name);
			break;
		}
	}
}

//ɾ��ĳ��Ա(���к����һ��ɾ��)
//�ݹ�������pΪ��������
void destroy(person *&p)
{
	if (!p)return;
	for (int i = 0; i < p->chilnum; i++)
	{
		destroy(p->chil[i]);
	}
	//printf("%s\n", root->name);
	free(p);
	p = NULL;
	tot--;
}

//ɾ��ĳ��Ա
void del_member(person *&root)
{
	char name[64],ch;
	printf("����Ҫɾ����Ա��������");
	scanf("%s", name);
	person *p = name_search(root, name);
	if (p)
	{
		print_info(p);
		getchar();
		printf("ȷ��ɾ����(y/n)��");
		scanf("%c", &ch);
		bool flag = 1;//������ɾ�����ü�����
		if (toupper(ch) == 'Y')
		{
			//��p��Ϊ��ʱ������p��ĸ�ڵ�ĺ���ָ�����鲢����������һ
			if (p != root)
			{
				flag = 0;
				person *pa = p->pa;
				int i = 0;
				//�ҵ�Ҫɾ����λ��
				while (pa->chil[i] != p) i++;
				//����
				for (int j = i; j < pa->chilnum - 1; j++)
				{
					pa->chil[j] = pa->chil[j + 1];
				}
				pa->chilnum--;
			}
			destroy(p);
			if (flag) root = NULL;
		}
	}
	else printf("�������޴��ˣ�\n");
}

//ĳ��Ա��Ӻ���
void add_child(person *&root)
{
	char name[64];
	printf("������Ҫ��Ӻ��ӵĳ�Ա������");
	scanf("%s", name);
	person *p = name_search(root, name);
	if (p)
	{
		print_info(p);
		person *child = (person *)malloc(sizeof(person));
		for (int i = 0; i < maxn; i++)
		{
			child->chil[i] = NULL;
		}
		printf("===================\n");
		printf("Ҫ��ӵĺ�����Ϣ\n");
		modify(child);
		p->chil[p->chilnum++] = child;	//������ָ�����������Ԫ��
		child->pa = p;					//ָ�򸸽ڵ�
		child->dai = p->dai + 1;		//�����ڸ��ڵ�����Ļ����ϼ�1
		child->chilnum = 0;				//������Ϊ0
	}
	else printf("�������޴��ˣ�\n");
}

//�������
void test(person *root)
{
	if (root)
	{
		person *Q[maxv];//����
		int front = 0, rear = 0, lastrear = 0;
		Q[rear++] = root;
		lastrear = rear;//���ڷֲ�
		while (front < rear)
		{
			person *fnt = Q[front++];
			printf("%s   ", fnt->name);

			//�������
			for (int i = 0; i < fnt->chilnum; i++)
			{
				Q[rear++] = fnt->chil[i];
			}

			if (front == lastrear)
			{
				printf("\n");
				lastrear = rear;
			}
		}
		printf("\n");
	}
}

//���ӻ����(���ò�α�������ɡ���test)
void view_print(person *root)
{
	
}

//�����������˳��д���ļ�
void output(person *root)
{
	FILE *fp;
	if ((fp = fopen("update_family_tree.xls","w")) == NULL)
	{
		printf("�ļ���ʧ�ܣ�\n");
		exit(-1);
	}
	//д���ͷ
	fprintf(fp, "��\t����\t��������\t���\t��ַ\t���ڷ�\t��������\t����\n");
	person *Q[maxv];//����
	int front = 0, rear = 0;
	Q[rear++] = root;
	while (front < rear)
	{
		person *fnt = Q[front++];
		//д�������������������
		fprintf(fp, "%d\t%s\t%d/%d/%d\t", fnt->dai, fnt->name, fnt->birth.year, fnt->birth.moon, fnt->birth.day);
		//д����
		if (fnt->hun) fprintf(fp, "��\t");
		else fprintf(fp, "��\t");
		//д���ַ
		fprintf(fp, "%s\t", fnt->addr);
		//д�뽡�ڷ�
		if (fnt->jian) fprintf(fp, "����\t");
		else fprintf(fp, "�ѹ�\t");
		//д����������
		if (fnt->jian) fprintf(fp, "��\t");
		else fprintf(fp, "%d/%d/%d\t", fnt->death.year, fnt->death.moon, fnt->death.day);
		//д�뺢�ӵ�ͬʱ������ָ�����
		for (int i = 0; i < fnt->chilnum; i++)
		{
			Q[rear++] = fnt->chil[i];
			fprintf(fp, "%s", fnt->chil[i]->name);
			if (i != fnt->chilnum - 1) fprintf(fp, " ");
			else fprintf(fp, "\n");
		}
		if (fnt->chilnum == 0) fprintf(fp, "��\n");
	}
	fclose(fp);
	printf("д���ļ��ɹ���\n");
}

int main()
{
	person *root,*p = NULL;
	read(root);
	while (1)
	{
		system("cls");
		printf
		(
			"1.��ʾ����\n"
			"2.��Ϣ��ѯ\n"
			"3.��ϵ��ѯ\n"
			"4.ɾ����Ա\n"
			"5.��Ӻ���\n"
			"6.�޸���Ϣ\n"
			"7.д���ļ�\n"
			"8.�˳�"
			"���������ѡ��"
		);
		int ch,flag = 0;
		scanf("%d", &ch);
		system("cls");
		switch (ch)
		{
			case 1://��ʾ����
				test(root);
				break;
			case 2://��Ϣ��ѯ
				printf
				(
					"1.��������ѯ\n"
					"2.��������ѯ\n"
					"3.�����ղ�ѯ\n"
					"���������ѡ��"
				);
				int c;
				scanf("%d", &c);
				system("cls");
				switch (c)
				{
					case 1:
						char name[64];
						printf("������������");
						scanf("%s", name);
						p = NULL;
						p = name_search(root, name);
						if (!p) printf("�������� %s\n", name);
						else
						{
							print_info(p);
							if (p->pa)//�����ĸ��Ϣ
							{
								printf("\n=======��ĸ��Ϣ=======\n");
								print_info(p->pa);
							}
							if (p->chilnum != 0)
							{
								printf("\n=======������Ϣ=======\n");
								for (int i = 0; i < p->chilnum; i++)
								{
									printf("======================\n");
									print_info(p->chil[i]);
								}
							}
						}
						break;
					case 2:
						int n;
						printf("�����������");
						scanf("%d", &n);
						layer(root, n);
						break;
					case 3:
						int y, m, d;
						printf("�꣺");
						scanf("%d",&y);
						printf("�£�");
						scanf("%d", &m);
						printf("�գ�");
						scanf("%d", &d);
						printf("�����Ա������\n");
						birth_search(root, y, m, d);
						printf("\n");
						break;
				}
				break;
			case 3://��ϵ��ѯ
				char name1[64], name2[64];
				printf("����1��");
				scanf("%s", name1);
				printf("����2��");
				scanf("%s", name2);
				relat(root, name1, name2);
				break;
			case 4://ɾ����Ա
				del_member(root);
				break;
			case 5://��Ӻ���
				add_child(root);
				break;
			case 6://�޸���Ϣ
				char name[64];
				printf("���������޸ĵ�������");
				scanf("%s", name);
				p = NULL;
				p = name_search(root, name);
				if (p)
				{
					printf("�޸�ǰ\n");
					print_info(p);
					printf("======================\n");
					modify(p);
				}
				else printf("�������� %s\n", name);
				break;
			case 7://д���ļ�
				output(root);
				break;
			case 8://�˳�
				flag = 1;
				break;
		}
		if (flag) break;
		system("pause");
	}
	return 0;
}
