//���л�����Visual Studio 2017
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<algorithm>
#pragma warning(disable:4996)
using namespace std;
const int maxn = 200000;//�����Ҫѹ���Ŀռ��С(bytes)
const int maxc = 200000;//01�������ռ��С

//Huffman���ڵ�ṹ
typedef struct
{
	int w, p, lc, rc;//Ȩֵ�����ڵ�λ�ã�����λ�ã��Һ���λ��
	char c;//�ַ�
}HTNode,*HuffmanTree;

//Huffman�����ַ�����
typedef char ** HuffmanCode;

//ͳ���ı��ַ���õ���ԭʼ���飬�����ַ��Լ������ֵ�Ƶ�ʣ����ִ�����
typedef struct
{
	int f;
	char c;
}Original;


//��ԭʼ������ѡ������p = 0��С�ڵ�λ��s1,s2
void Select(HuffmanTree HT,int len, int & s1, int & s2)
{
	s1 = s2 = 0;
	for (int i = 1; i <= len; i++)
	{
		if (HT[i].p == 0 && HT[i].w < HT[s1].w)s1 = i;
	}
	for (int i = 1; i <= len; i++)
	{
		if (HT[i].p == 0 && HT[i].w < HT[s2].w && i != s1)s2 = i;
	}
}

//����Huffman��
void CreatHT(HuffmanTree & HT,Original A[] , int n)
{
	if (n <= 1)return;
	int m = 2 * n - 1;//�ڵ�n + �м�ڵ�n-1
	HT = (HuffmanTree)malloc((m + 1) * sizeof(HTNode));//0��λ����
	int i;
	HT[0].w = 1 << 30 - 1;//����һ����������
	for (i = 1; i <= n; i++)
	{
		HT[i].p = HT[i].lc = HT[i].rc = 0;
		HT[i].w = A[i].f;
		HT[i].c = A[i].c;
	}
	for (; i <= m; i++)
	{
		HT[i].p = HT[i].lc = HT[i].rc = HT[i].w = HT[i].c = 0;
	}

	for (i = n + 1; i <= m; i++)
	{
		int s1, s2;
		Select(HT, i - 1, s1, s2);
		HT[s1].p = HT[s2].p = i;
		HT[i].lc = s1;
		HT[i].rc = s2;
		HT[i].w = HT[s1].w + HT[s2].w;
	}
}

//�ַ�����
void Encoding(HuffmanTree HT,HuffmanCode & HC, int n)
{
	HC = (HuffmanCode)malloc((n + 1) * sizeof(char *));
	char *cd = (char *)malloc(n * sizeof(char));//��ı��볤��Ϊn
	cd[n - 1] = '\0';//���������
	int i;
	for (i = 1; i <= n; i++)
	{
		int start = n - 1;
		for (int c = i, f = HT[i].p; f != 0; c = f, f = HT[f].p)
		{
			if (HT[f].lc == c)cd[--start] = '0';//��0����1
			else cd[--start] = '1';
		}
		HC[i] = (char *)malloc((n - start) * sizeof(char));
		strcpy(HC[i], &cd[start]);
	}
	free(cd);

	//д���ַ����ֵĴ���������
	FILE *fp;
	if ((fp = fopen("Huffman.txt", "w")) == NULL)
	{
		printf("���ļ�ʧ�ܣ�\n");
		exit(0);
	}
	char ch;
	for (int i = 1; i <= n; i++)
	{
		fprintf(fp, "%c:%d %s\n", HT[i].c, HT[i].w, HC[i]);//�ַ�ֵ�����ִ�����������
	}
	fclose(fp);
}

//����������
void PrintCode(Original A[],HuffmanCode HC, int n)
{
	for (int i = 1; i <= n; i++)
	{
		printf("%c : %s\n", A[i].c, HC[i]);
	}
}

//��λѹ�����ѹ
//����ַaddress byte�ֽں�ĵ�posλ��λ0
void Zero(char *address, int bits)
{
	char zero[8] = { 0b01111111,0b10111111,0b11011111,0b11101111,0b11110111,0b11111011,0b11111101,0b11111110 };
	char *addr = address;
	int byte = bits / 8;	//Ҫ�����ƶ����ֽ���
	int pos = bits % 8;		//�ƶ���׼ȷbyte�ֽں��Ŀ��λ
	addr += byte;
	*addr = *addr & zero[pos];
}
//�������ƣ���Ϊ1
void One(char *address, int bits)
{
	char one[8] = { 0b10000000,0b01000000,0b00100000,0b00010000,0b00001000,0b00000100,0b00000010,0b00000001 };
	char *addr = address;
	int byte = bits / 8;	//Ҫ�����ƶ����ֽ���
	int pos = bits % 8;		//�ƶ���׼ȷ�ֽں��Ŀ��λ
	addr += byte;
	*addr = *addr | one[pos];
}

//ѹ��(01����>��λ�洢)
void Compress(char encode[])
{
	FILE *bitencode;
	if ((bitencode = fopen("code.dat", "wb")) == NULL)
	{
		printf("��ѹ���ļ�ʧ�ܣ�\n");
		exit(0);
	}

	char tempStr[maxn] = { 0 };	//�洢ѹ���������
	int bits = 0;				//��bits��λ��λ��

	while (encode[bits] != '\0')
	{
		if (encode[bits] == '1')One(tempStr, bits);
		else if (encode[bits] == '0')Zero(tempStr, bits);
		bits++;
	}

	int morebits = bits % 8;		//����Ĳ���8λ��bit��
	int morebyte, bytes;			//morebyte�Ƿ���Ҫ��һ���ֽ�
	if (bits % 8 == 0)morebyte = 0;
	else morebyte = 1;
	bytes = bits / 8 + morebyte;	//bytes�ܹ���Ҫ���ֽ���

	//��¼morebits
	fwrite(&morebits, 4, 1, bitencode);
	//���ֽ���һд���ļ�
	for (int i = 0; i < bytes; i++)
	{
		fwrite(tempStr + i, 1, 1, bitencode);
	}
	fclose(bitencode);
}

//��ѹ(�������ļ���ԭΪ01��,Ϊ��ԭΪ�ı��ļ���׼��)
void Decompress(char encode[])//encode[]�洢01��
{
	int morebits;
	int bytes = 0;
	char tempStr[maxn];	//�洢����ѹ������
	FILE * bitencode;
	if ((bitencode = fopen("code.dat", "rb")) == NULL)
	{
		printf("��ѹ���ļ�ʧ�ܣ�\n");
		exit(0);
	}

	fread(&morebits, 4, 1, bitencode);
	while (fread(tempStr + bytes, 1, 1, bitencode) == 1)bytes++;
	fclose(bitencode);

	//����ѹ�������ݻ�ԭΪ01��
	int bits = 0;
	char one[8] = { 0b10000000,0b01000000,0b00100000,0b00010000,0b00001000,0b00000100,0b00000010,0b00000001 };
	for (int i = 0; i < bytes - 1; i++)//��ʱ���������һ���ֽ�����(��Ϊ���ܲ���8λ)
	{
		for (int bit = 0; bit < 8; bit++)
		{
			if ((tempStr[i] & one[bit]) == one[bit])encode[bits++] = '1';
			else encode[bits++] = '0';
		}
	}

	for (int bit = 0; bit < morebits; bit++)//�����ǲ���8λ��bitλ
	{
		if ((tempStr[bytes - 1] & one[bit]) == one[bit])encode[bits++] = '1';
		else encode[bits++] = '0';
	}
	encode[bits] = '\0';
}

//���ı��ļ�����
void EncodeText(HuffmanTree HT, HuffmanCode HC, int n)
{
	char encode[maxc];	//encode[]�洢�������ı�
	FILE *data;
	if ((data = fopen("source.txt", "r")) == NULL)
	{
		printf("�ļ���ʧ�ܣ�\n");
		exit(0);
	}

	int t = 0;
	while (!feof(data))
	{
		char ch = fgetc(data);
		for (int i = 1; i <= n; i++)
		{
			if (HT[i].c == ch)
			{
				for (int j = 0; HC[i][j] != '\0'; j++)
				{
					encode[t++] = HC[i][j];
				}
			}
		}
	}
	encode[t] = '\0';
	fclose(data);
	//ѹ��
	Compress(encode);
}

//�Զ������ļ�����
void DecodeText(HuffmanTree HT, int n)//��ȡ�ļ��ַ������ֵĴ����ٹ���Huffman�������������һ���ԡ�
{
	char encode[maxc], decode[maxc];	//encode[]�洢������01����decode[]�洢�������ı�
	Decompress(encode);
	FILE *fp;
	if ((fp = fopen("recode.txt", "w")) == NULL)
	{
		printf("�ļ���ʧ�ܣ�\n");
		exit(0);
	}
	int len = strlen(encode);
	int root = 2 * n - 1,t = 0;
	for (int i = 0,j = root; i < len; i++)
	{
		if (encode[i] == '0')j = HT[j].lc;
		else if(encode[i] == '1')j = HT[j].rc;
		//˵���Ѿ�������Ҷ�ӽڵ�
		if (HT[j].lc == 0 && HT[j].rc == 0)
		{
			decode[t++] = HT[j].c;
			j = root;
		}
	}
	decode[t] = '\0';
	fputs(decode, fp);
	fclose(fp);
}

//У��Դ�ļ��������ļ�
bool Comp()
{
	FILE *data, *decode;
	if ((data = fopen("source.txt", "r")) == NULL)
	{
		printf("���ļ�ʧ�ܣ�\n");
		exit(0);
	}
	if ((decode = fopen("recode.txt", "r")) == NULL)
	{
		printf("���ļ�ʧ�ܣ�\n");
		exit(0);
	}
	char ch1, ch2;
	//��ֹ�ȽϽ����ַ����³���
	ch1 = fgetc(data);
	ch2 = fgetc(decode);
	while (!feof(data) && !feof(decode))
	{
		if (ch1 != ch2)
		{
			fclose(data);
			fclose(decode);
			return false;
		}
		ch1 = fgetc(data);
		ch2 = fgetc(decode);
	}
	fclose(data);
	fclose(decode);
	return true;
}

//ͳ���ı����ַ����ֵ�Ƶ������ԭʼ����(Original A[])
void Frequency(Original A[],int & n)
{
	//��ʱ�洢ASCII��0-255���ַ����ֵĴ���
	int temp[256] = {0};
	FILE *data;
	if ((data = fopen("source.txt","r")) == NULL)
	{
		printf("��Դ�ļ�ʧ�ܣ�\n");
		exit(0);
	}
	char ch;
	ch = fgetc(data);
	while (!feof(data))
	{
		temp[(int)ch]++;
		ch = fgetc(data);
	}
	//nΪA�����±�
	n = 0;
	for (int i = 0; i < 256; i++)
	{
		if (temp[i] > 0)
		{
			n++;
			A[n].c = (char)i;
			A[n].f = temp[i];
		}
	}
	fclose(data);
}

int main()
{
	int n;
	Original A[257];//ASCII 0-255,�±�0����
	HuffmanTree HT;
	HuffmanCode HC;

	Frequency(A, n);		//ͳ���ı����ֵ��ַ�������
	CreatHT(HT, A, n);		//����Huffman��
	Encoding(HT, HC, n);	//���ַ����б���
	//PrintCode(A, HC, n);	//test

	EncodeText(HT, HC, n);	//���ı��ļ�ѹ�����벢д��code.dat�ļ�
	DecodeText(HT,n);		//�Զ������ļ����벢д��recode.txt�ļ�
	if (Comp())
	{
		printf("У�Խ����Դ�ļ��������ļ���ȫһ�£�\n");
	}
	else printf("У�Խ����Դ�ļ��������ļ���һ�£�\n");

	system("pause");
	return 0;
}