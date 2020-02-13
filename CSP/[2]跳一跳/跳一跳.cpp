//AC
#include<iostream>
#include<cstdio>
#pragma warning(disable:4996)
using namespace std;

int main()
{
	int a[40],t = 0,ans = 0,i = 0;
	do
	{
		i++;
		scanf("%d", &a[i]);
	} while (a[i] != 0);

	i = 1;
	while (a[i] != 0)
	{
		if (a[i] == 2)
		{
			t += 2;	//得分累加
			ans += t;
		}
		else
		{
			t = 0;	//累加得分清零
			ans += 1;
		}
		i++;
	}
	printf("%d\n", ans);
	system("pause");
	return 0;
}