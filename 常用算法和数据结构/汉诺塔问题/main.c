
#include<stdio.h>

 

void move(int n,char a,char b,char c)

{

    if(n==1)

        printf("\t%c->%c\n",a,c);    //当n只有1个的时候直接从a移动到c 

    else

    {   

        move(n-1,a,c,b);            //把a的n-1个盘子通过c移动到b 

        printf("\t%c->%c\n",a,c);   //把a的最后1个盘(最大的盘)移动到c 

        move(n-1,b,a,c);            //吧b上面的n-1个盘通过a移动到c 

    }   

}

 

main()

{

    int n;

    printf("请输入要移动的块数："); 

    scanf("%d",&n);

    move(n,'a','b','c');

}
