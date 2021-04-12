#C语言指针错题集
1. 若有说明： long *p,a;则不能通过	scanf语句正确给输入项读入数据的程序段是 ( )
A)	*p=&a ；	scanf("%ld"，p)；
B)	p=(long *)malloc(8) ；	scanf("%ld"，p)；
C)	scanf("%ld"，p=&a)；
D)	scanf("%ld" ，&a)；
###
2. 有以下程序段
``` c
#include <stdio.h>
int main()
{	int x[] = {10, 20, 30};
int *px = x;
printf("%d,", ++*px);	printf("%d,", *px); px = x;

printf("%d,", (*px)++);	printf("%d,", *px); px = x;
printf("%d,", *px++);	printf("%d,", *px); px = x;
printf("%d,", *++px);	printf("%d\n", *px); return 0;
}
```
程序运行后的输出结果是	 (	)
A） 11,11,11,12,12,20,20,20	B）20,10,11,10,11,10,11,10
C） 11,11,11,12,12,13,20,20	D） 20,10,11,20,11,12,20,20
###
3. 下述代码输出结果 ( )
``` c
int arr[]={6,7,8,9,10};
int *ptr; ptr=arr;
*(ptr+2)+=2;
printf ("%d,%d\n",*ptr,*(ptr+2));
```
A)  8,10
B)  6,8
C)  7,9
D)  6,10
###
4.  若有以下说明和语句，	int c[4][5],(*p)[5];p=c; 能正确引用	c 数组元素的是 ( )
A) p+1	B) *(p+3)	C) *(p+1)+3	D) *(p[0]+2))
###
5. 设已有定义 : char *st="how are you";  下列程序段中正确的是 ( )
A) char	a[11], *p;	strcpy(p=a+1,&st[4]);
B) char	a[11];	strcpy(++a, st);
C) char	a[11];	strcpy(a, st);
D) char	a[], *p;	strcpy(p=&a[1],st+2);
###
6. 设 p1 和 p2 是指向同一个字符串的指针变量，	c 为字符变量，则以下不能正确执行的赋值语句是 ( )
A)  c=*p1+*p2;
B)  p2=c
C)  p1=p2
D)  c=*p1*(*p2);
###
7. 以下正确的程序段是 ( )
``` c
A) char str[20];  scanf("%s",&str);
B) char *p;       scanf("%s",p);
C) char str[20];  scanf("%s",&str[2]);
D) char str[20],*p=str;  scanf("%s",p[2]);
```
###
8. 若有说明语句则以下不正确的叙述是 ( )
char a[]="It is mine";
char *p="It is mine";
a)	a+1表示的是字符	t 的地址
b)	p 指向另外的字符串时，字符串的长度不受限制
c)	p 变量中存放的地址值可以改变
d)	a 中只能存放 10 个字符
###
9. 下面程序的运行结果是 ( )
``` c
#include	<stdio.h>
#include	<string.h>
main()
{
  char *s1 = "AbDeG";
  char *s2 = "AbdEg";
  s1 += 2;
  s2 += 2;
  printf("%d\n",strcmp(s1,s2));
}
```
a)	正数	b)负数	c)零	d)不确定的值
###
10. 对于基类型相同的两个指针变量之间，不能进行的运算是（ ）。
``` java
A.  <
B.  =
C.  +
D.  -
```
###
11. 以下程序的输出结果是（ ）
``` c
void main()
{
    int *v, b;
    v= & b;
    b = 100;
    *v += b;
    printf("%d\n",b);
}
```
A. 100
B. 50
C. 101
D. 200
