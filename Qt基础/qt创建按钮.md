##Qt创建按钮
####创建按钮的方法
``` cpp
//第一种方式
//创建一个按钮
QPushButton *btn = new QPushButton;
//让btn对象依赖在Qwidget中
btn->setParent(this);
//显示文件
btn->setText("第一个按钮");

//第二种创建方式
QPushButton * btn2 = new QPushButton("第二个按钮",this);
```
####按钮的常用方法
``` cpp
//移动btn的位置
btn2->move(100,100);
//重置窗口大小
resize(600,400);
//设置固定的窗口大小,不允许用户缩放窗口
setFixedSize(600,400);
//设置窗口的标题
setWindowTitle("第一个窗口");
```
