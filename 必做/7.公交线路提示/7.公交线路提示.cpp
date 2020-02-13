//���л�����Visual Studio 2017
#include<map>
#include<queue>
#include<vector>
#include<string>
#include<cctype>
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#pragma warning(disable:4996)
using namespace std;
const int maxn = 50000;
const int inf = 1 << 30 - 1;

struct station
{
	string name;	//��վ��
	int line;		//��վ����·
	int index;		//��վ���
	int pass;		//��Ҫ����BFS��¼������վ����
	int transfer;	//��¼ת�˴���
};

//����Dijkstra�㷨���Ż������ȼ�����Ԫ�صĽṹ
struct node
{
	string name;
	int index;
	int dis;	//��Ȩ�����˴���
	int pass;

	//���������<,�������ȼ����У����˴���С���ȼ���
	friend bool operator < (node a, node b)
	{
		if (a.dis == b.dis) return a.pass > b.pass;
		return a.dis > b.dis;
	}
};

bool vis[maxn];
int d[maxn];	//d[i]��ʾ��ʼ��վ�����Ϊi�ĳ�վ��ת�˴���

vector<station>g[maxn];		//g[i]�洢�ű��Ϊi�ĳ�վ�ܵ���ĳ�վ
multimap<string, int>match;	//��վ�����ŵĶ�Ӧ��ϵ
typedef multimap<string, int>::iterator iter;

int cnt = 0;			//�ܳ�վ��
string sname[maxn];		//sname[i]��ʾ���Ϊi�ĳ�վ��
int num[maxn];			//num[i]��ʾ���Ϊi�ĳ�վ����·��
int spass[maxn];		//spass[i]��ʾ��㵽���Ϊi�ĳ�վ��Ҫ������վ��

/*------------------------------------------------------------------------------------------------*/
//��ȡ�ļ���Ϣ
void read_file()
{
	FILE *fp;
	if ((fp = fopen("Nanjing_bus_routes.txt", "r")) == NULL)
	{
		printf("���ļ�ʧ�ܣ�\n");
		system("pause");
		exit(0);
	}

	char str[1024], line[20], blank[4];
	fscanf(fp, "%s", line);
	fgets(blank, 4, fp);
	fgets(str, 1024, fp);
	while (!feof(fp))
	{

		//cout <<  line << str << endl;
		int linenum = 0, t = 0;//����·����
		while (line[t] >= '0' && line[t] <= '9')
		{
			linenum = linenum * 10 + line[t] - '0';
			t++;
		}

		station temp1, temp2;
		int index1, index2;
		int step = 0;

		char *p = strtok(str, ",");//��ȡ��վ��
		while (p != NULL)
		{
			string name;
			//Ϊ�˷�ֹÿһ·���һ����վ����'\n'��ɲ���Ҫ���鷳
			for (; *p != '\0' && *p != '\n'; p++)
			{
				name += *p;
			}

			//��ͬһ����·�ϵĳ�վ����
			if (step == 0)
			{
				temp1.name = name;
				temp1.line = linenum;
				temp1.index = ++cnt;
				temp1.pass = temp1.transfer = 0;

				pair<string, int> p1(name, cnt);
				match.insert(p1);

				sname[cnt] = name;
				num[cnt] = linenum;
				index1 = cnt;

				p = strtok(NULL, ",");//��ȡ��һ����վ��

				name = "";
				for (; *p != '\0' && *p != '\n'; p++)
				{
					name += *p;
				}

				temp2.name = name;
				temp2.line = linenum;
				temp2.pass = temp2.transfer = 0;
				temp2.index = ++cnt;

				pair<string, int> p2(name, cnt);
				match.insert(p2);
				sname[cnt] = name;
				num[cnt] = linenum;
				index2 = cnt;

				g[index1].push_back(temp2);
				g[index2].push_back(temp1);

				step++;

				p = strtok(NULL, ",");
			}
			else
			{
				temp1 = temp2;
				index1 = index2;
				temp2.name = name;
				temp2.line = linenum;
				temp2.index = ++cnt;
				temp2.pass = temp2.transfer = 0;

				pair<string, int> p2(name, cnt);
				match.insert(p2);
				sname[cnt] = name;
				num[cnt] = linenum;
				index2 = cnt;

				g[index1].push_back(temp2);
				g[index2].push_back(temp1);

				p = strtok(NULL, ",");
			}

		}
		fscanf(fp, "%s", line);
		fgets(blank, 4, fp);
		fgets(str, 1024, fp);
	}

	//��������ͬ(������������·��ͬ)�ĳ�վ��������
	for (iter it = match.begin(); it != match.end(); it++)
	{
		//���복վ������multimap��equal_range����ȡ��վ�����ż�ֵ��
		pair<iter, iter> pr = match.equal_range(it->first);
		//index�����洢ͬ����վ����Ӧ�����б��
		vector<int>index;
		for (iter ii = pr.first; ii != pr.second; ii++) index.push_back(ii->second);
		//˵����ͬ����վ�ֲ��ڲ�ͬ��·��
		//���������ķ�ʽ�÷ֲ��ڲ�ͬ��·��ͬһ��վ��ͨ������ظ�������Ӧ�ڽ�ͼʱȥ�أ����ͺ��Ѱ·�����ĸ��Ӷ�
		if (index.size() > 1)
		{
			int flag = 0;
			for (int i = 0; i < index.size() - 1; i++)
			{
				/*------------������Ϊ�˱����ظ������еĲ���------------*/
				if (flag == 0)
				{
					int j;
					for (j = 0; j < g[index[i]].size(); j++)
					{
						//��ʾ���Ϊindex[i]�Ŀɱ䳤�����д�����������֣�˵������Ҫ�ظ�ִ�иò���
						if (g[index[i]][j].name == it->first)
						{
							flag = 1;
							break;
						}
					}
					if (j == g[index[i]].size())flag = -1;//������ִ��һ��ѭ��˵�����ظ�,����Ҫ�ظ�ִ�����ϲ���
				}
				if (flag == 1)break;
				/*------------������Ϊ�˱����ظ������еĲ���------------*/

				for (int j = i + 1; j < index.size(); j++)
				{
					station temp1, temp2;
					temp1.name = temp2.name = it->first;
					temp1.line = num[index[i]]; temp2.line = num[index[j]];
					temp1.index = index[i]; temp2.index = index[j];
					temp1.pass = temp2.pass = temp1.transfer = temp2.transfer = 0;
					g[index[i]].push_back(temp2);
					g[index[j]].push_back(temp1);
				}
			}
		}
	}

	//�������
	/*for (int i = 1; i <= cnt; i++)
	{
		cout << i << sname[i] << " ";
		for (int j = 0; j < g[i].size(); j++)
		{
			cout << g[i][j].line << ":" << g[i][j].name << " index:" << g[i][j].index << "  ";
		}
		cout << endl;
	}*/
	fclose(fp);
}

/*------------------------------------------------------------------------------------------------*/
//����˳�·��
void print_path(int t, int pre[])
{
	if (pre[t] == -1)
	{
		cout << sname[t] << endl;
		return;
	}
	else
	{
		print_path(pre[t], pre);
		if (num[pre[t]] != num[t])
		{
			printf
			(
				"��\n"
				"��( ����%d· )\n"
				"��\n"
				, num[t]);
			cout << sname[t] << endl;
		}
		else cout << "��\n��\n" << sname[t] << endl;
	}
}

/*------------------------------------------------------------------------------------------------*/
//��������������������ҵ�����վ�������ٳ�վ��·��
//����ֵΪ��\�٣���ʾ�Ƿ����������վ���������û�����ĳ�վ����������
bool BFS(string s, string t)
{
	if (match.count(s) == 0 && match.count(t) == 0)
	{
		printf("��ʼ��վ��Ŀ�ĳ�վ��������,���������룡\n");
		return false;
	}
	else if (match.count(s) == 0)
	{
		printf("��ʼ��վ������,���������룡\n");
		return false;
	}
	else if (match.count(t) == 0)
	{
		printf("Ŀ�ĳ�վ������,���������룡\n");
		return false;
	}
	else
	{
		int pre[maxn];		//��¼�ó�վ��ǰ����ͨ���ݹ�ɵ�·��
		int pass = inf;		//������վ��
		int transfer = inf;	//ת�˵Ĵ���
		int fstart, fend;	//����ѡ��������յ�
		pair<iter, iter> sp = match.equal_range(s);
		for (iter sii = sp.first; sii != sp.second; sii++)
		{
			int start = sii->second;	//��ʼ��

			pair<iter, iter> tp = match.equal_range(t);
			for (iter tii = tp.first; tii != tp.second; tii++)
			{
				int end = tii->second;	//�յ�

				queue<station>Q;		//���ѱز����ٵĶ���
				int temppre[maxn];		//��¼�ó�վ��ǰ������ʱ����ͨ���Ƚϵõ����Ž�

				for (int i = 1; i <= cnt; i++)//��ʼ��
				{
					vis[i] = 0;
					temppre[i] = -1;
				}

				station temp;
				temp.name = s;
				temp.index = start;
				temp.line = num[start];
				temp.pass = 0;		//����վ����
				temp.transfer = 0;	//���˴���
				Q.push(temp);
				vis[start] = 1;

				while (!Q.empty())
				{
					station fnt = Q.front();
					Q.pop();
					if (fnt.index == end)//��Ϊ�յ����������
					{
						//��������վ�����٣����߾���վ�����䵫���˴�������ʱ����pre���顢������վ����ת�˴��������յ������յ�
						if (fnt.pass < pass || (fnt.pass == pass && fnt.transfer < transfer))
						{
							for (int i = 1; i <= cnt; i++)
							{
								pre[i] = temppre[i];
							}
							pass = fnt.pass;
							transfer = fnt.transfer;
							fstart = start;
							fend = end;
						}
						break;
					}
					else
					{
						int u = fnt.index;
						for (int j = 0; j < g[u].size(); j++)
						{
							int v = g[u][j].index;
							if (!vis[v])
							{
								temp.name = sname[v];
								temp.line = num[v];
								temp.index = v;

								//��ǰ����վ�ĳ�վ����ͬ�����㾭����һ����վ
								//��ǰ����վ�ĳ�վ����ͬ����·��ͬ��˵������
								if (sname[u] == sname[v])
								{
									temp.pass = fnt.pass;

									if (num[u] != num[v])temp.transfer = fnt.transfer + 1;
									else temp.transfer = fnt.transfer;

									//pre[v] = pre[u];
								}
								else
								{
									temp.pass = fnt.pass + 1;

									if (num[u] != num[v])temp.transfer = fnt.transfer + 1;
									else temp.transfer = fnt.transfer;

									//pre[v] = u;
								}
								temppre[v] = u;	//��¼���Ϊv�ĳ�վ��ǰ��Ϊu
								Q.push(temp);
								vis[v] = 1;
							}
						}
					}

				}
			}
		}
		cout << "��㣺" << num[fstart] << "·" << sname[fstart] << "\n��\n�յ㣺" << num[fend] << "·" << sname[fend] << endl << endl;
		printf
		(
			"������Ҫ����%d��վ��,ת��%d��\n"
			"·��Ϊ��\n"
			"( %d·�ϳ� )\n"
			, pass, transfer, num[fstart]);
		print_path(fend, pre);
		printf("( %d·�³� )\n", num[fend]);
	}
}

/*------------------------------------------------------------------------------------------------*/
//�Ͻ�˹�����㷨���������ҵ�����վת�˴������ٵĳ�վ·��
//����STL���ȼ����ж�����ж��Ż�.�ܽ�O(n*n)�ĸ��Ӷ��½���O(nlogn)

bool Dijkstra(string s, string t)
{
	if (match.count(s) == 0 && match.count(t) == 0)
	{
		printf("��ʼ��վ��Ŀ�ĳ�վ��������,���������룡\n");
		return false;
	}
	else if (match.count(s) == 0)
	{
		printf("��ʼ��վ������,���������룡\n");
		return false;
	}
	else if (match.count(t) == 0)
	{
		printf("Ŀ�ĳ�վ������,���������룡\n");
		return false;
	}
	else
	{
		int pre[maxn];				//��¼�ó�վ��ǰ����ͨ���ݹ�ɵ�·��
		int temppre[maxn];			//��ʱ���ǰ��
		int fpass = inf;			//������վ��
		int ftransfer = inf;		//ת�˵Ĵ���
		int fstart = -1, fend = -1;	//����ѡ��������յ�
		pair<iter, iter> sp = match.equal_range(s);
		for (iter sii = sp.first; sii != sp.second; sii++)
		{
			int start = sii->second;	//��ʼ��
			pair<iter, iter> tp = match.equal_range(t);
			for (iter tii = tp.first; tii != tp.second; tii++)
			{
				int end = tii->second;	//�յ�
				//cout << start << sname[start] << " " << end << sname[end] << endl;
				for (int i = 1; i <= cnt; i++)//��ʼ��
				{
					d[i] = inf;
					spass[i] = inf;
					vis[i] = 0;
					temppre[i] = -1;
				}
				d[start] = 0;
				spass[start] = 0;

				priority_queue<node>Q;
				node temp;
				temp.name = s;
				temp.index = start;
				temp.dis = 0;
				temp.pass = 0;
				Q.push(temp);

				while (!Q.empty())
				{
					node fnt = Q.top();
					Q.pop();

					int u = fnt.index, dis = fnt.dis, pass = fnt.pass;
					if (u == end)break;
					if (vis[u])continue;
					vis[u] = 1;

					for (int i = 0; i < g[u].size(); i++)
					{
						int v = g[u][i].index;
						if (!vis[v])
						{
							int tempdis, temppass;

							if (num[u] == num[v])	//����·��ͬ
							{
								tempdis = dis;		//˵������Ҫ����
								temppass = pass + 1;//������վ����һ
							}
							else//����·��ͬ
							{
								tempdis = dis + 1;	//����
								temppass = pass;	//��ͬ����վ���л��ˣ�����������һ����վ
							}

							if ((tempdis < d[v]) || (tempdis == d[v] && temppass < spass[v]))//������Ϊv�ĳ�վת�������Ż�
							{
								d[v] = tempdis;
								spass[v] = temppass;
								temp.name = g[u][i].name;
								temp.index = g[u][i].index;
								temp.dis = d[v];
								temp.pass = spass[v];
								temppre[v] = u;		//temppre[v]��ʾ���Ϊv�ĳ�վ��ǰ����վ���Ϊu
								Q.push(temp);
							}
						}
					}
				}
				//�����˴������١������ڻ��˴�����ͬ������¾�����վ������ʱ����·��
				if ((d[end] < ftransfer) || (d[end] == ftransfer && spass[end] < fpass))
				{
					for (int i = 1; i <= cnt; i++)
					{
						pre[i] = temppre[i];
					}
					fstart = start;
					fend = end;
					ftransfer = d[end];
					fpass = spass[end];
				}
			}
		}
		cout << "��㣺" << num[fstart] << "·" << sname[fstart] << "\n��\n�յ㣺" << num[fend] << "·" << sname[fend] << endl << endl;
		printf
		(
			"������Ҫ����%d��,������%d����վ\n"
			"·��Ϊ:\n"
			"( %d·�ϳ� )\n"
			, ftransfer, fpass, num[fstart]);
		print_path(fend, temppre);
		printf("( %d·�³� )\n", num[fend]);
	}
}

int main()
{
	read_file();
	string s, t;
	while (1)
	{
		system("cls");

		printf("��������ʼ��վ��");
		cin >> s;
		printf("������Ŀ�ĳ�վ��");
		cin >> t;

		printf("---------------------------\n");
		if (!BFS(s, t))
		{
			system("pause");
			continue;
		}
		printf("\n");
		printf("---------------------------\n");
		if (!Dijkstra(s, t))
		{
			system("pause");
			continue;
		}

		char ch;
		printf("�Ƿ������ѯ����y/n��:");
		getchar();
		scanf("%c", &ch);
		if (toupper(ch) == 'N')break;
	}

	system("pause");
	return 0;
}
