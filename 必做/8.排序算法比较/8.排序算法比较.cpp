//���л���:Dev-C++ 5.11 
//VS 2017 ����ʱ��ջ����
#include<cstdio>
#include<ctime>
#include<cstring>
#include<fstream>
#include<queue>
#include<iostream>
#pragma warning(disable:4996)
using namespace std;
const int maxn = 20000;
const int tot = 10;
int a[maxn + 5];

//�����洢�������Լ����ĵ�ʱ�䣬��������ʽ(��ʱ�ٵ����ȼ���)
struct node
{
	char name[20];
	double second;
	friend bool operator < (node a, node b)
	{
		return a.second > b.second;
	}
};

//��������
void insert_sort(int a[], int n) //nΪ��������±�
{
	for (int i = 2; i <= n; i++)
	{
		if (a[i] < a[i - 1])
		{
			a[0] = a[i];
			a[i] = a[i - 1];
			int j;
			for (j = i - 2; a[0] < a[j]; j--)
			{
				a[j + 1] = a[j];
			}
			a[j + 1] = a[0];
		}
	}
}

//ϣ������
	//��������
int delta[maxn];
	//���������ʼ��(���Զ���)
int delta_ini(int delta[], int n)
{
	int t = 0;
	while (n / 2)
	{
		delta[t++] = n / 2;
		n /= 2;
	}
	return t;
}

void shell_insert(int a[], int dk, int n)
{
	for (int i = dk + 1; i <= n; i++)
	{
		if (a[i] < a[i - dk])
		{
			a[0] = a[i];
			int j;
			for (j = i - dk; j > 0 && a[0] < a[j]; j -= dk)
			{
				a[j + dk] = a[j];
			}
			a[j + dk] = a[0];
		}
	}
}

void shell_sort(int a[], int n,int delta[]) //nΪ��������±�
{
	int t = delta_ini(delta, n);
	for (int k = 0; k < t; k++)
	{
		shell_insert(a, delta[k], n);
	}
}

//ð������
void bubble_sort(int a[], int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n - i - 1; j++)
		{
			if (a[j] > a[j + 1])
			{
				int temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;
			}
		}
	}
}

//��������
int partition(int a[], int left, int right)
{
	int temp = a[left];
	while (left < right)
	{
		while (left < right && a[right] > temp)right--;
		a[left] = a[right];
		while (left < right && a[left] <= temp)left++;
		a[right] = a[left];
	}
	a[left] = temp;
	return left;
}

void quick_sort(int a[], int left, int right)
{
	if (left < right)
	{
		int pos = partition(a, left, right);
		quick_sort(a, left, pos - 1);
		quick_sort(a, pos + 1, right);
	}
}

//ѡ������
void select_sort(int a[], int n)
{
	for (int i = 0; i < n; i++)
	{
		int k = i;
		for (int j = i + 1; j < n; j++)
		{
			if (a[j] < a[k])k = j;
		}
		if (k != i)
		{
			int temp = a[i];
			a[i] = a[k];
			a[k] = temp;
		}
	}
}

//������
	//�Զ����µ���(�󶥶�)
void downAdjust(int heap[], int pa, int child)
{
	int i = pa, j = i * 2;
	while (j <= child)
	{
		if (j + 1 <= child && heap[j + 1] > heap[j])j++;
		if (heap[j] > heap[i])
		{
			swap(heap[j], heap[i]);
			//����
			i = j;
			j = i * 2;
		}
		else break;
	}
}

	//����
void createHeap(int heap[], int n)
{
	for (int i = n / 2; i >= 1; i--)
	{
		downAdjust(heap, i, n);
	}
}

	//������
void heap_sort(int heap[], int n)
{
	createHeap(heap, n);
	for (int i = n; i > 1; i--)
	{
		swap(heap[i], heap[1]);
		downAdjust(heap, 1, i - 1);
	}
}

//�鲢����
void merge(int a[], int bl, int br, int cl, int cr)
{
	int temp[maxn];
	int i = bl, j = cl, t = 0;
	while (i <= br && j <= cr)
	{
		if (a[i] <= a[j])temp[t++] = a[i++];
		else temp[t++] = a[j++];
	}
	while (i <= br)temp[t++] = a[i++];
	while (j <= cr)temp[t++] = a[j++];
	for (int i = 0; i < t; i++)
	{
		a[bl + i] = temp[i];
	}
}

void merge_sort(int a[], int left, int right)
{
	if (left < right)
	{
		int mid = (left + right) / 2;
		merge_sort(a, left, mid);
		merge_sort(a, mid + 1, right);
		merge(a, left, mid, mid + 1, right);
	}
}

//��������
//ѡ�����������������ֵ��ȷ���ж���λ
int getMax(int a[], int n)
{
	int mx = a[0];
	for (int i = 1; i < n; i++)
	{
		if (a[i] > mx)mx = a[i];
	}
	return mx;
}
int output[maxn + 5];
void countSort(int a[], const int n, int exp)
{
	int i, count[10] = { 0 };
	//ͳ��ͬһλ�ϸ�����(0-9)���ֵĴ���
	for (i = 0; i < n; i++)count[(a[i] / exp) % 10]++;
	//ȷ��λ��
	for (i = 1; i < 10; i++)count[i] += count[i - 1];

	for (i = n - 1; i >= 0; i--)//��������Ӧ��Ҳ���԰ɣ�
	{
		output[count[(a[i] / exp) % 10] - 1] = a[i];
		count[(a[i] / exp) % 10]--;
	}
	for (i = 0; i < n; i++)a[i] = output[i];
}
void radix_sort(int a[],int n) 
{
	int m = getMax(a, n);
	for (int exp = 1; m / exp > 0; exp *= 10)countSort(a, n, exp);
}


//1��ԭʼ���ݴ����ļ��У�����ͬ�����Բ�ͬ�㷨���в���
void task1()
{
	char filename[10] = "rand0.txt";
	fstream File,outFile;
	clock_t startTime, endTime;

	outFile.open("task1.txt", ios::out);
	for (int i = 0; i < tot; i++)
	{
		//ʹ�����ȼ����У����պ�ʱ������˳��洢����
		priority_queue<node>q;
		node newnode;
		filename[4] = i + '0';
		outFile << "����" << i << ":" << endl;
/*---------------------------------------------------------------------------------------------*/
		File.open(filename, ios::in);
		for (int j = 0; j < maxn; j++)File >> a[j];
		File.close();

		startTime = clock();//��ʱ��ʼ
		heap_sort(a, maxn);
		endTime = clock();//��ʱ����
		strcpy(newnode.name, "������   \0");
		newnode.second = (double)(endTime - startTime) / CLOCKS_PER_SEC;
		q.push(newnode);
/*---------------------------------------------------------------------------------------------*/
		File.open(filename, ios::in);
		for (int j = 0; j < maxn; j++)File >> a[j];
		File.close();

		startTime = clock();
		bubble_sort(a, maxn);
		endTime = clock();
		strcpy(newnode.name, "ð������\0");
		newnode.second = (double)(endTime - startTime) / CLOCKS_PER_SEC;
		q.push(newnode);
/*---------------------------------------------------------------------------------------------*/
		File.open(filename, ios::in);
		for (int j = 0; j < maxn; j++)File >> a[j];
		File.close();

		startTime = clock();
		select_sort(a, maxn);
		endTime = clock();
		strcpy(newnode.name, "ѡ������\0");
		newnode.second = (double)(endTime - startTime) / CLOCKS_PER_SEC;
		q.push(newnode);
/*---------------------------------------------------------------------------------------------*/
		File.open(filename, ios::in);
		for (int j = 0; j < maxn; j++)File >> a[j];
		File.close();

		startTime = clock();
		merge_sort(a, 0, maxn - 1);
		endTime = clock();
		strcpy(newnode.name, "�鲢����\0");
		newnode.second = (double)(endTime - startTime) / CLOCKS_PER_SEC;
		q.push(newnode);
/*---------------------------------------------------------------------------------------------*/
		File.open(filename, ios::in);
		for (int j = 0; j < maxn; j++)File >> a[j];
		File.close();

		startTime = clock();
		quick_sort(a, 0, maxn - 1);
		endTime = clock();
		strcpy(newnode.name, "��������\0");
		newnode.second = (double)(endTime - startTime) / CLOCKS_PER_SEC;
		q.push(newnode);
/*---------------------------------------------------------------------------------------------*/
		File.open(filename, ios::in);
		for (int j = 1; j <= maxn; j++)File >> a[j];//��0���±���Ϊ���ڱ���
		File.close();

		startTime = clock();
		insert_sort(a, maxn);
		endTime = clock();
		strcpy(newnode.name, "��������\0");
		newnode.second = (double)(endTime - startTime) / CLOCKS_PER_SEC;
		q.push(newnode);
/*---------------------------------------------------------------------------------------------*/
		File.open(filename, ios::in);
		for (int j = 1; j <= maxn; j++)File >> a[j];
		File.close();

		startTime = clock();
		shell_sort(a, maxn,delta);
		endTime = clock();
		strcpy(newnode.name, "ϣ������\0");
		newnode.second = (double)(endTime - startTime) / CLOCKS_PER_SEC;
		q.push(newnode);
/*---------------------------------------------------------------------------------------------*/
		File.open(filename, ios::in);
		for (int j = 0; j < maxn; j++)File >> a[j];
		File.close();

		startTime = clock();
		radix_sort(a, maxn);
		endTime = clock();
		strcpy(newnode.name, "��������\0");
		newnode.second = (double)(endTime - startTime) / CLOCKS_PER_SEC;
		q.push(newnode);

		while (!q.empty())
		{
			node top = q.top();
			q.pop();
			outFile << top.name << ": " << top.second << "s" << endl;
		}
		outFile << "---------------------------------" << endl;
	}
	outFile.close();
}

//2����Ļ��ʾÿ�������㷨�Բ�ͬ����������ʱ��
void task2()
{
	char filename[10] = "rand0.txt";
	fstream File,outFile;
	clock_t startTime, endTime;

	outFile.open("task2.txt", ios::out);
/*---------------------------------------------------------------------------------------------*/
	outFile << "������:" << endl;
	for (int i = 0; i < tot; i++)
	{
		filename[4] = i + '0';
		File.open(filename, ios::in);
		if (!File)cout << "fail" << endl;
		for (int j = 0; j < maxn; j++)File >> a[j];
		File.close();

		startTime = clock();
		heap_sort(a, maxn);
		endTime = clock();
		outFile << "����" << i << ": " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}
	outFile << "---------------------------------" << endl;
/*---------------------------------------------------------------------------------------------*/
	outFile << "ð������:" << endl;
	for (int i = 0; i < tot; i++)
	{
		filename[4] = i + '0';
		File.open(filename, ios::in);
		if (!File)cout << "fail" << endl;
		for (int j = 0; j < maxn; j++)File >> a[j];
		File.close();

		startTime = clock();
		bubble_sort(a, maxn);
		endTime = clock();
		outFile << "����" << i << ": " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}
	outFile << "---------------------------------" << endl;
/*---------------------------------------------------------------------------------------------*/
	outFile << "ѡ������:" << endl;
	for (int i = 0; i < tot; i++)
	{
		filename[4] = i + '0';
		File.open(filename, ios::in);
		if (!File)cout << "fail" << endl;
		for (int j = 0; j < maxn; j++)File >> a[j];
		File.close();

		startTime = clock();
		select_sort(a, maxn);
		endTime = clock();
		outFile << "����" << i << ": " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}
	outFile << "---------------------------------" << endl;
/*---------------------------------------------------------------------------------------------*/
	outFile << "�鲢����:" << endl;
	for (int i = 0; i < tot; i++)
	{
		filename[4] = i + '0';
		File.open(filename, ios::in);
		if (!File)cout << "fail" << endl;
		for (int j = 0; j < maxn; j++)File >> a[j];
		File.close();

		startTime = clock();
		merge_sort(a,0, maxn - 1);
		endTime = clock();
		outFile << "����" << i << ": " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}
	outFile << "---------------------------------" << endl;
/*---------------------------------------------------------------------------------------------*/
	outFile << "��������:" << endl;
	for (int i = 0; i < tot; i++)
	{
		filename[4] = i + '0';
		File.open(filename, ios::in);
		if (!File)cout << "fail" << endl;
		for (int j = 0; j < maxn; j++)File >> a[j];
		File.close();

		startTime = clock();
		quick_sort(a,0, maxn - 1);
		endTime = clock();
		outFile << "����" << i << ": " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}
	outFile << "---------------------------------" << endl;
/*---------------------------------------------------------------------------------------------*/
	outFile << "��������:" << endl;
	for (int i = 0; i < tot; i++)
	{
		filename[4] = i + '0';
		File.open(filename, ios::in);
		if (!File)cout << "fail" << endl;
		for (int j = 1; j <= maxn; j++)File >> a[j];
		File.close();

		startTime = clock();
		insert_sort(a, maxn);
		endTime = clock();
		outFile << "����" << i << ": " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}
	outFile << "---------------------------------" << endl;
/*---------------------------------------------------------------------------------------------*/
	outFile << "ϣ������:" << endl;
	for (int i = 0; i < tot; i++)
	{
		filename[4] = i + '0';
		File.open(filename, ios::in);
		if (!File)cout << "fail" << endl;
		for (int j = 1; j <= maxn; j++)File >> a[j];
		File.close();

		startTime = clock();
		shell_sort(a, maxn,delta);
		endTime = clock();
		outFile << "����" << i << ": " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}
	outFile << "---------------------------------" << endl;
/*---------------------------------------------------------------------------------------------*/
	outFile << "��������:" << endl;
	for (int i = 0; i < tot; i++)
	{
		filename[4] = i + '0';
		File.open(filename, ios::in);
		if (!File)cout << "fail" << endl;
		for (int j = 0; j < maxn; j++)File >> a[j];
		File.close();

		startTime = clock();
		radix_sort(a, maxn);
		endTime = clock();
		outFile << "����" << i << ":" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}
	outFile << "---------------------------------" << endl;
	outFile.close();
}

int main()
{
	cout << "���ݴ����� ... " << endl; 
	task1();
	task2();
	cout << "���ݴ�����ϣ�" << endl;
	system("pause");
	return 0;
}
