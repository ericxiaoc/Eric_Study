#C++中的布尔类型
* C++在C语言的基础类型系统之上增加了bool；
* C++中的bool可取得值只有true和false
* 也可以赋值为0(false)或者1(true)
* bool理论大小是占用一个字节
###bool值的转换
看如下例子：
例1：非零的bool即为true；
``` c++
#include <stdio.h>
#include <stdlib.h>
int main(void) {
	int a = 0;
	bool b = true;
	printf("111 b = %d \n",b);

	b = false;
	printf("222 b = %d \n",b);

	a = 5;
	b = a;
	printf("333 b = %d \n",b);

	b = -3;
	printf("444 b = %d \n",b);

	return 0;
}
输出：
111 b = 1
222 b = 0
333 b = 1
444 b = 1
```
例2：bool值进行运算
``` c++
#include <stdio.h>
#include <stdlib.h>
int main(void) {
	bool b = false;
	printf("111 b = %d \n", b);

	b++;
	printf("222 b = %d \n", b);

	b = b + 1;
	printf("333 b = %d \n", b);

	return 0;
}
输出：
111 b = 0
222 b = 1
333 b = 1
```
总结：
* C++的bool值只有true(非0)和false(0)；
* C++编译器在赋值时将非0的值转换为true，0值转换为false；
