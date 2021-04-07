#C语言与C++的区别
###全局变量
C语言中：可以存在多个同名的全局变量的，实际使用的地址则为一个；
``` c
#include <stdio.h>
#include <stdlib.h>
int a;
int a = 1;
int main(void) {
	puts("const"); /* prints const */

	printf("%d",a);
	return EXIT_SUCCESS;
}
输出：
const
1
```
C++中：不允许存在多个同名的全局变量；
``` c++
#include <iostream>
using namespace std;
int a;
int a = 1;
int main() {
	cout << "const" << endl; // prints const
	return 0;
}

编译提示：
 error: redefinition of 'int a'
 int a = 1;
     ^
'int a' previously declared here
 int a;
     ^
```
###Const变量
C语言中：C的const表示修饰只读变量，但是我们可以恶意强制操作const对象。如下则是：
``` c
#include <stdio.h>
#include <stdlib.h>
int main(void) {
	printf("start eric\n");
	const int tmp = 0;
	int* p = (int*)&tmp;
	*p = 5;
	printf("tmp = %d\n",tmp);
	printf("end eric\n");
	return EXIT_SUCCESS;
}
输出：
start eric
tmp = 5
end eric
```
C++中：C++的const修饰的变量有内存实体，但是只能是右值； C++的const变量一旦初始化，就会强制告诉编译器对这块内存的修改无效，不可能通过它去操作这块内存的值。
``` c++
#include <iostream>
using namespace std;
int main() {
	printf("start eric\n");
	const int tmp = 0;
	int* p = (int*)&tmp;
	*p = 5;
	printf("tmp = %d\n",tmp);
	printf("end eric\n");
	return 0;
}
输出：
start eric
tmp = 0
end eric
```
###Struct的区别
C语言中：struct只是一组变量的集合，运行一下代码会解析不了student
``` c
#include <iostream>
struct student{
	const char* name;
	int age;
};
int main(void) {
	printf("start eric\n");
	student s1 = {"xiaohong",18};
	student s2 = {"xiaoming",20};
	printf("end eric\n");
	return 0;
}
提示：
..\src\Const_C_Pro.c:19:2: error: unknown type name 'student'; use 'struct' keyword to refer to the type
  student s1 = {"xiaohong",18};
```
C++中：struct是一个新类型的定义声明；
``` cpp
#include <iostream>
struct student{
	const char* name;
	int age;
};
int main(void) {
	printf("start eric\n");
	student s1 = {"xiaohong",18};
	student s2 = {"xiaoming",20};
	printf("S1 name = %s \n ",s1.name);
	printf("S1 age = %d \n ",s1.age);
	printf("end eric\n");
	return 0;
}
输出：
start eric
S1 name = xiaohong
S1 age = 18
end eric
```
###C/C++默认类型
C中的默认类型在C++中不合法；C++中所有变量和函数都必须有类型
如下：
``` c
#include <stdio.h>
#include <stdlib.h>
g(i) {
	printf("i = %d \n", i);
}
f() {
	return 5;
}
int main(void) {
	g(10);
	printf("f() = %d \n",f());
	return 0;
}
输出：
i = 10
f() = 5
```
``` c++
#include <stdio.h>
#include <stdlib.h>
g(i) {
	printf("i = %d \n", i);
}
f() {
	return 5;
}
int main(void) {
	g(10);
	printf("f() = %d \n",f());
	return 0;
}
 error: 'g' was not declared in this scope
  g(10);
  ^
```
C语言中：
* int f();表达返回的值为int，可以接受任意类型的参数为无数个；
* int f(void);表示返回值为无参数的int；

C++中：
* int f();和int f(void)是相同的意思，都表示返回值为int的无参函数

###三目运算符的区别
C语言中：三目运算符返回的是变量的值，不能作为左值使用
C++中：三目运算符可直接返回变量的本身，因此可以出现在程序的任意地方；
如下方例子：
``` c++
int main(void) {
	int a = 1;
	int b = 2;

	(a < b ? a : b) = 3;

	printf("a = %d b = %d \n", a, b);
	return 0;
}
```
C语言中提示：左值必须作为赋值的左操作数
```
error: lvalue required as left operand of assignment
  (a < b ? a : b) = 3;
```
C++中输出：
```a = 3 b = 2  ```

注意：C++中的三目运算符中如果存在一个常量的话，则不能作为左值使用。
