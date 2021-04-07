#C++中的引用
* 引用可以看作一个已定义变量的别名；
* 引用的语法： Type& name = var;

###引用的基础用法
例1：
``` c++
int main(void) {
	int a = 1;
  //b相当于a的别名，b改变的是地址的值，所以a也会跟着改变。
	int& b = a;
	b = 5;
	printf("a = %d b = %d \n", a, b);
	return 0;
}
输出：
a = 5 b = 5
```
Tip：普通的引用在声明时必须使用其他的变量进行初始化；

###引用的意义
* 引用作为其他变量的别名而存在，因此在一些场合可以代替指针
* 引用相对于指针来说具有更好的可读性和实用性

例2：
swap函数的实现对比：
C++中：利用引用来实现值的交换；
``` c++
void swap(int& a,int& b){
	int tmp = b;
	b = a;
	a = tmp;
}
int main(void) {
	int a = 4;
	int b = 5;
	swap(a,b);
	printf("a = %d b = %d \n", a, b);
	return 0;
}
输出：
a = 5 b = 4
```
C语言中：利用的变量的地址来进行值的交换，相对C++来说，缺少易读性；
``` c
void swap(int *a, int *b) {
	int tmp = *b;
	*b = *a;
	*a = tmp;
}
int main(void) {
	int a = 4;
	int b = 5;
	swap(&a, &b);
	printf("a = %d b = %d \n", a, b);
	return 0;
}
输出：
a = 5 b = 4
```
Tip：引用作为函数参数的声明时不进行初始化；

###const引用
* 在C++中可以声明const引用
* const Type& name = var；
* const引用让变量拥有只读属性
* const引用可以初始化常量，而引用本身是无法初始化常量的；
例1：变量b成为只读属性了，所以无法被赋值了；
``` c++
int main(void) {
	int a = 4;
	const int& b = a;
	b = 5;
	printf("a = %d b = %d \n", a, b);
	return 0;
}
提示：
error: assignment of read-only reference 'b'
  b = 5;
      ^
正确的赋值方法：
int main(void) {
	int a = 4;
	const int& b = a;
	int *p = (int*)&b;
	*p = 5;
	printf("a = %d b = %d \n", a, b);
	return 0;
}
```
例2：当使用常量对const引用进行初始化时，C++编译器会为常量值分配空间，并将引用名作为这段空间的别名
``` c++
int main(void) {
	const int& b = 1; //const引用为只读变量  const为只读常量；
	int *p = (int*)&b;
	*p = 5;
	printf(" b = %d \n",b);
	return 0;
}
输出：
b = 5
```
Tip：使用常量对const引用初始化后将生成一个只读变量。

###疑问
疑问1：引用有自己的存储空间吗？
``` c++
struct student{
	int& a;
	int& b;
};
int main(void) {
	printf(" student size = %d \n",sizeof(student));
	return 0;
}
输出：这里是因为我的电脑环境是64位的，所以打印的是16；
 student size = 16
```

解答：
* 引用在C++内部实现是一个常指针
			``` Type& name <===>  Type* const name ```
* C++编译器在编译的过程中使用常指针作为引用的内部实现，因此引用所占用的空间大小与指针相同

再次验证:
``` c++
struct student{
	int& a;
	int& b;
	int& c;
};
int main(void) {
	int a = 1;
	int b = 2;
	int c = 3;
	student s = {a,b,c};
	printf("a = %08x \n",&a);
	printf("b = %08x \n",&b);
	printf("c = %08x \n",&c);
	printf("s = %08x \n",&s);
	printf("student s = %d \n",sizeof(s));
	return 0;
}

输出: abc在地址上是相邻的，而s却是毫无关联的地址，s与c之间也是间隔24个字节。所以引用是存在存储空间的。
a = 0061fe1c
b = 0061fe18
c = 0061fe14
s = 0061fdf0
student s = 24
```
###函数返回值为引用时

* 若返回栈变量
  - 不能成为其他应用的初始值；
  - 不能作为左值使用
* 若返回静态变量或者全局变量
  - 可以成为其他引用的初始值
  - 即可作为右值使用，也可以作为左值使用
``` c++
int& g(){
	int a = 0;
	return a;
}
int main(void) {
	int& b = g();
	printf("b = %d \n",b);
	return 0;
}
提示： 这里警告是因为局部变量返回之后，就变成随机数，所以无法确定b的值；
warning: reference to local variable 'a' returned [-Wreturn-local-addr]
  int a = 0;
      ^
```
``` c++
int& f(){
	static int a = 1;
	return a;
}
int main(void) {
	f() = 10;
	printf("f() = %d \n",f());
	return 0;
}
输出：静态变量或者全局变量则可以作为左右值；
f() = 10
```
