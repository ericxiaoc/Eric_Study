##Lambada表达式
``` cpp
表现形式：[](){};
常用表达式：[=](){};
[]: 标识符，匿名函数
  = 值传递
  & 引用传递
(): 参数
{}: 实现体
mutable 修饰 值传递变量，可以修改拷贝出的数据;
返回值写法 []()->int{}

例子：一个信号，可以多个槽函数
QPushButton *btn = new QPushButton("Lambda",this);
connect(btn,&QPushButton::clicked,[=](){
    btn->setText("suceess");
    emit th->StandUp();
});
```
