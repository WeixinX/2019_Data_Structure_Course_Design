//���л�����Visual Studio 2017
//AC
#include<iostream>
#include<cstdio>
#include<cstring>
#include<string>
#include<vector>
#pragma warning(disable:4996)
using namespace std;
const int maxn = 105;

//URLƥ�����Ľṹ
struct Rule 
{
	vector<string>p;	//URLƥ��Ĺ���
	char name[64];		//URLƥ�������
	bool flag;			//��Ǹù�������Ƿ���"/" 
}a[maxn];


//�ַ����ָ���ַ���s��"/"Ϊ�ָ����ָ�����������v�� 
void split(vector<string> &v, char s[])
{
	char *temp = strtok(s, "/");
	while (temp)
	{
		v.push_back(temp);
		temp = strtok(NULL, "/");
	}
}

//�ж��ַ���s�Ƿ������� 
bool isNum(string s)
{
	for (int i = 0; i < s.length(); i++)
	if (!(s[i] >= '0' && s[i] <= '9')) return false;
	return true;
}

int n, m;//����Ͳ�ѯ������

//����URL��ַ��flag��Ǵ�URL��ַ����Ƿ���"/" 
void solve(vector<string>URL, bool flag)
{
	int i;
	vector<string>ans;//��Ų���
	//˳�����n������ 
	for (i = 0; i < n; i++)
	{
		ans.clear();//����� 
		int j = 0, k = 0;
		vector<string>t = a[i].p;//��i������
		//�鿴URL�Ƿ�ʹ˹���ƥ��
		while (j < t.size() && k < URL.size())
		{
			//���1: <int> ��Ҫ�����������URL��ַ��k�����Ƿ�Ϊ���֣�������Ҫ��������ǰ����
			if (t[j] == "<int>")
			{
				if (isNum(URL[k]))//����������� 
				{
					int sub;
					for (sub = 0; sub < URL[k].size() - 1 && URL[k][sub] == '0'; sub++);//����ǰ���� 
					ans.push_back(URL[k].substr(sub));//��¼
					j++, k++;//ƥ����һ���� 
					continue;
				}
			}
			//���2: <str> ��Ϊ���豣֤���ַ����Ĺ淶������ֱ�Ӽ�¼�����¼�������ƥ��
			else if (t[j] == "<str>")
			{
				ans.push_back(URL[k]);//ֱ�Ӽ�¼���� 
				j++, k++;
				continue;
			}
			//���3: <path> ��ΪԤ����ʱ����ַ��"/"Ϊ�ָ����ָ���������������Ҫƴ��������ȥ����һ��"/"
			else if (t[j] == "<path>")
			{
				string s;
				while (k < URL.size()) s = s + "/" + URL[k++];
				ans.push_back(s.substr(1));//ȥ����һ��"/"����¼
				j++;//ʵ����k�Ѿ�������ˣ�����Ҫ�ڴ�����Ҳ������ѭ��
				continue;
			}
			//���4: ƥ�� URLƥ������еĹ̶��ַ���
			else if (t[j] == URL[k])
			{
				j++, k++;
				continue;
			}
			//���������������������������˵��URL��ַ�Ƿ���ֱ������ѭ��
			break;
		}
		//��ƥ��ɹ�������(ע��URLƥ���������URL��ַ����Ƿ���"/")
		if (j == t.size() && k == URL.size() && flag == a[i].flag) break; 
	}
	//URL������ƥ�����ʧ��,���"404" 
	if (i == n) printf("404\n");
	else
	{
		printf("%s", a[i].name);//���ƥ��Ĺ�������� 
		for (int w = 0; w < ans.size(); w++)//����������� 
		{
			cout << " " << ans[w];
		}
		printf("\n");
	}
}

int main()
{
	scanf("%d%d", &n, &m);
	string rule;
	//����URLƥ�����
	for (int i = 0; i < n; i++)
	{
		char temp[128];
		scanf("%s", temp);		//URLƥ�����
		scanf("%s", a[i].name);	//ƥ����������
		//��¼���������Ƿ���"/" 
		if (temp[strlen(temp) - 1] == '/') a[i].flag = 1;
		else a[i].flag = 0;

		split(a[i].p, temp);//�ָ��ƥ�������"/"Ϊ�ָ����ָ�ɶ�������Ա�ƥ��
	}
	for (int i = 0; i < m; i++)
	{
		vector<string>URL;	//������װ��URL��ַ�ĸ�������
		char temp[128];		//����URL��ַ
		scanf("%s", temp);
		//��¼���������Ƿ���"/" 
		bool flag = 0;
		if (temp[strlen(temp) - 1] == '/') flag = 1;

		split(URL, temp);//�ָ� 
		solve(URL, flag);//�жϴ���
	}
	system("pause");
	return 0;
}