#C����ָ����⼯
1. ����˵���� long *p,a;����ͨ��	scanf�����ȷ��������������ݵĳ������ ( )
A)	*p=&a ��	scanf("%ld"��p)��
B)	p=(long *)malloc(8) ��	scanf("%ld"��p)��
C)	scanf("%ld"��p=&a)��
D)	scanf("%ld" ��&a)��
###
2. �����³����
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
�������к����������	 (	)
A�� 11,11,11,12,12,20,20,20	B��20,10,11,10,11,10,11,10
C�� 11,11,11,12,12,13,20,20	D�� 20,10,11,20,11,12,20,20
###
3. �������������� ( )
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
4.  ��������˵������䣬	int c[4][5],(*p)[5];p=c; ����ȷ����	c ����Ԫ�ص��� ( )
A) p+1	B) *(p+3)	C) *(p+1)+3	D) *(p[0]+2))
###
5. �����ж��� : char *st="how are you";  ���г��������ȷ���� ( )
A) char	a[11], *p;	strcpy(p=a+1,&st[4]);
B) char	a[11];	strcpy(++a, st);
C) char	a[11];	strcpy(a, st);
D) char	a[], *p;	strcpy(p=&a[1],st+2);
###
6. �� p1 �� p2 ��ָ��ͬһ���ַ�����ָ�������	c Ϊ�ַ������������²�����ȷִ�еĸ�ֵ����� ( )
A)  c=*p1+*p2;
B)  p2=c
C)  p1=p2
D)  c=*p1*(*p2);
###
7. ������ȷ�ĳ������ ( )
``` c
A) char str[20];  scanf("%s",&str);
B) char *p;       scanf("%s",p);
C) char str[20];  scanf("%s",&str[2]);
D) char str[20],*p=str;  scanf("%s",p[2]);
```
###
8. ����˵����������²���ȷ�������� ( )
char a[]="It is mine";
char *p="It is mine";
a)	a+1��ʾ�����ַ�	t �ĵ�ַ
b)	p ָ��������ַ���ʱ���ַ����ĳ��Ȳ�������
c)	p �����д�ŵĵ�ֵַ���Ըı�
d)	a ��ֻ�ܴ�� 10 ���ַ�
###
9. �����������н���� ( )
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
a)	����	b)����	c)��	d)��ȷ����ֵ
###
10. ���ڻ�������ͬ������ָ�����֮�䣬���ܽ��е������ǣ� ����
``` java
A.  <
B.  =
C.  +
D.  -
```
###
11. ���³�����������ǣ� ��
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
