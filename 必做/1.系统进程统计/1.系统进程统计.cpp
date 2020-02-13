//���л�����Visual Studio 2017

#include<iostream>
#include<cstdio>
#include<cstdlib>
#include <windows.h>
#include <tlhelp32.h>	//���̿��պ���ͷ�ļ�
#include <psapi.h>
#include<time.h>
#pragma warning(disable:4996)
using namespace std;

//��ǰ���̵�˫������ڵ�ṹ
typedef struct Dnode
{
	int id;				//����id(PID)
	char name[70];		//������
	int conttime;		//����ʱ�䣬����Ϊ��λ
	int memory;			//�ڴ��С����KBΪ��λ
	Dnode *pre, *next;
}Dnode,*DList;

//�������̵�������ڵ�ṹ
typedef struct node
{
	int id;			//����id(PID)
	char name[70];	//������
	int conttime;	//����ʱ�䣬����Ϊ��λ
	int overtime;	//����ʱ�䣬����Ϊ��λ
	node *next;
}node,*List;

int t = 0;	//�������е���ʱ�䣬����Ϊ��λ
int filenum = 0;//�ļ�����

//��ʾ��ǰ���ѽ����������
void print(DList CurP, List OverP)
{
	printf("��ǰϵͳ���̣�\n");
	if (CurP->next != NULL)
	{
		Dnode *p = CurP->next;
		printf("�ڴ�ʹ��\t������\t\t\t\t\t\t\t\t\t\t����ʱ��  PID\n");
		printf("==============================================================================================================\n");
		while (p)
		{
			if (p->memory == -1)printf("  ��Ȩ��\t");
			else printf("%6d K\t", p->memory);
			printf("%-52s\t\t\t\t%5d\t %5d\n", p->name, p->conttime,p->id);
			p = p->next;
		}
	}
	else printf("��ǰ�޽��̣�\n");

	printf("�ѽ������̣�\n");
	if (OverP->next != NULL)
	{
		node *p;
		p = OverP->next;
		printf("������\t\t\t\t\t\t\t\t����ʱ��    ����ʱ��	  PID\n");
		printf("=============================================================================================\n");
		while (p)
		{
			printf("%-52s\t\t%5d s\t%5d s\t\t%5d\n", p->name, p->overtime, p->conttime,p->id);
			p = p->next;
		}
	}
	else printf("�޽������̣�\n");
}

//��������Ϣ�����ļ�
void outlog(DList CurP, List OverP)
{
	FILE *fp;
	char filename[] = "a.txt";
	filename[0] = 'a' + filenum;
	filenum++;
	if ((fp = fopen(filename, "w")) == NULL)
	{
		printf("�����ļ�ʧ��!\n");
		exit(-1);
	}
	
	//����ϵͳʱ��
	time_t timep;
	struct tm *p;
	time(&timep);
	p = gmtime(&timep);
	fprintf(fp,"%d/%d/%d %d:%d:%d\n", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min,p->tm_sec); 

	fprintf(fp,"��ǰϵͳ���̣�\n");
	if (CurP->next != NULL)
	{
		Dnode *p = CurP->next;
		fprintf(fp,"�ڴ�ʹ��		������								����ʱ��(s)      PID\n");
		fprintf(fp,"====================================================================================\n");
		while (p)
		{
			if (p->memory == -1) fprintf(fp,"  ��Ȩ��	");
			else fprintf(fp,"%6d K	", p->memory);
			fprintf(fp,"            %-70s				    %d            %d\n", p->name, p->conttime, p->id);
			p = p->next;
		}
	}
	else fprintf(fp,"��ǰ�޽��̣�\n");

	fprintf(fp,"�ѽ������̣�\n");
	if (OverP->next != NULL)
	{
		node *p;
		p = OverP->next;
		fprintf(fp,"������					����ʱ��(s)    ����ʱ��(s)    PID\n");
		fprintf(fp,"=====================================================================\n");
		while (p)
		{
			fprintf(fp,"%-70s	  %d\t  %d\t  %d\n", p->name, p->overtime, p->conttime,p->id);
			p = p->next;
		}
	}
	else fprintf(fp,"�޽������̣�\n");
	fclose(fp);
}

//�����ڴ�Ӵ�С��˳����뵱ǰ����
void DList_Insert(DList &L, Dnode *s)
{
	if (L == NULL)
	{
		printf("error!\n");
		exit(0);
	}

	Dnode *p = L;
	while (p->next != NULL && p->next->memory > s->memory) p = p->next;
	if (p->next != NULL) p->next->pre = s;
	s->next = p->next;
	s->pre = p;
	p->next = s;
}

//��ʼ��������
bool InitCurP(DList &L)
{

	//Ϊ��ǰ��������ͷ�ڵ����ռ�
	L = (DList)malloc(sizeof(Dnode));
	if (L == NULL)
	{
		printf("�����ڴ�ʧ�ܣ�\n");
		exit(-1);
	}
	L->next = L->pre = NULL;

	PROCESSENTRY32 curP;				//��ſ��ս�����Ϣ�Ľṹ��
	curP.dwSize = sizeof(curP);			//��ʹ������ṹ֮ǰ�����������Ĵ�С
	HANDLE Pshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//��ȡϵͳ���̿���
	HANDLE handle = GetCurrentProcess();//��ȡ��ǰ����

	PROCESS_MEMORY_COUNTERS pmc;

	if (Pshot == INVALID_HANDLE_VALUE)
	{
		printf("��ȡ����ʧ��!\n");
		exit(0);
	}

	bool flag = Process32First(Pshot, &curP);//��ȡ��һ��������Ϣ 
	while (flag)
	{
		handle = OpenProcess(PROCESS_ALL_ACCESS, 0, curP.th32ProcessID);//handleΪ���ΪPID���̵�Ȩ��

		GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));//��ȡ�ڴ��С
		Dnode *s = (DList)malloc(sizeof(Dnode));
		if (s == NULL)
		{
			printf("�����ڴ�ʧ�ܣ�\n");
			exit(-1);
		}

		s->id = curP.th32ProcessID;
		string name = curP.szExeFile;
		const char *tname = name.c_str();
		strcpy(s->name, tname);
		s->conttime = 0;
		s->next = s->pre = NULL;
		if (handle)s->memory = pmc.WorkingSetSize / 1024;
		else s->memory = -1;	//��Ȩ���޷���ȡ�ڴ��С
		DList_Insert(L, s);

		flag = Process32Next(Pshot, &curP);    //������һ������
	}

	CloseHandle(Pshot);	//���Pshot���

	return 1;
}

//���½���ʱ�����½������У�ɾ���ɽ��������µĽ�����Ԫ�صĽ������½�������ʣ�µı����½���
//����ʣ�µĲ��뵽�ɽ�������
void DList_Del(DList &L, int id)
{
	if (L == NULL)
	{
		printf("error!\n");
		exit(0);
	}

	Dnode *p = L,*q;//pΪҪɾ�ڵ��ǰ����qΪҪɾ���Ľڵ�
	while (p->next != NULL && p->next->id != id) p = p->next;
	q = p->next;
	//�����ڸýڵ�
	if (q != NULL)
	{
		//��Ҫɾ����Ϊ��β
		if (q->next == NULL)
		{
			p->next = NULL;
			q->pre = NULL;
			free(q);
		}
		else
		{
			p->next = q->next;
			q->next->pre = p;
			free(q);
		}
	}
}

//����ʱ�����ɽ��������С����½�����û�еĽ��̲������������
//������Щ���̴Ӿɽ�������ɾȥ
void List_Insert(List &L, node *s)
{
	if (L == NULL)
	{
		printf("error!\n");
		exit(0);
	}

	node *p = L;
	while (p->next != NULL && p->next->overtime > s->overtime) p = p->next;
	s->next = p->next;
	p->next = s;
}


//���µ�ǰ��������mainP�ͽ�����������OverP
//���ɽ����е����н���p��һ���½���cur�Աȣ����������½��̶�δ���ָý���(flag = 1)��˵���ý��̽���,��������������OverP��
//������ͬ�ģ���ѵ�ǰ��������ĸý���ɾ�������ʣ�µĽ���(��Щʣ�µ�Ϊ�½���)������һ��Ľ�������mainP��
void update(DList &mainP, List &OverP)
{
	//��ȡ��ǰ�������еĽ��̣������µĽ�����
	DList curP;
	InitCurP(curP);
	
	Dnode *p = mainP->next;
	Dnode *cur;
	bool flag;
	while (p)
	{
		flag = 1;//1��ʾ�����½�������δ���ָý���
		cur = curP->next;
		while (cur)
		{
			if (p->id == cur->id && strcmp(p->name,cur->name) == 0)//��ֹ�´򿪵Ľ��̺͸չص��Ľ��̷��䵽ͬ��PID
			{
				//���½������д��ڸý��̣������ھɽ������е����ӳ���ʱ�䡢�����ڴ�ʹ������������½�����curP��ɾ��
				flag = 0;
				p->conttime += 5;
				p->memory = cur->memory;
				DList_Del(curP, p->id);
				break;
			}
			cur = cur->next;
		}
		if (flag)//�ý�������������
		{
			node *s = (List)malloc(sizeof(node));
			s->id = p->id;
			strcpy(s->name, p->name);
			s->conttime = p->conttime + 5;	//���Ϊ5s
			s->overtime = t;
			s->next = NULL;
			List_Insert(OverP, s);
			p = p->next;//�������Ľ��̴�mainPɾ��ǰ�������ƶ�һ��
			DList_Del(mainP, s->id);
		}
		else p = p->next;
	}
	while (curP->next)
	{
		cur = curP->next;
		Dnode *s = (DList)malloc(sizeof(Dnode));
		s->id = cur->id;
		strcpy(s->name, cur->name);
		s->memory = cur->memory;
		s->conttime = cur->conttime;
		s->next = s->pre = NULL;
		DList_Insert(mainP, s);
		DList_Del(curP, cur->id);
	}
	free(curP);
}

int main()
{
	DList mainP;//��ǰ������
	List OverP = (List)malloc(sizeof(node));//����������
	if (!InitCurP(mainP))
	{
		printf("��ǰ���������ʼ��ʧ�ܣ�\n");
		exit(-1);
	}

	if (!OverP)
	{
		printf("�������������ʼ��ʧ�ܣ�\n");
		exit(-1);
	}
	OverP->next = NULL;

	outlog(mainP, OverP);
	print(mainP, OverP);
	while (1)
	{
		t += 5;		//���5sִ��һ�β���
		Sleep(5000);//�Ժ���Ϊ��λ
		system("cls");
		update(mainP, OverP);
		outlog(mainP, OverP);
		print(mainP, OverP);
	}

	system("pause");
	return 0;
}