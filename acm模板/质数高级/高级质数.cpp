#include<stdio.h>
#include<math.h>
#include <time.h>
#include<stdlib.h>
//#include"Python.h"
//#include"text.h"
// ��ĳ�����������ڵ�����
//int zhishu(int num);
int main() 
{
	int num=10000000;
    register int  i , m = 2,count=0;
	double n = 0;
	int *p ;
	p = (int*)calloc(2, sizeof(int));
	p[0] = 2;
	int start = clock();
	for (m = 2; m <= num; m++)
	{
		n = sqrt(m);
		for (i = 0; i <= count; i++)
		{
			if (m%p[i] == 0)
				break;
			else if (p[i] >= n)
				break;
		}
		if (p[i] > n)                  // ���m������
		{
			p[count] = m;
			p=(int *)realloc(p,(count+2)*sizeof(int));
			count++;
		}
	}
	int finish = clock();
	
	
	free(p);
  printf("this test use %d ms\n",(finish-start));
  printf("%d ��������������%d\n",num,count);
  return count;
}
