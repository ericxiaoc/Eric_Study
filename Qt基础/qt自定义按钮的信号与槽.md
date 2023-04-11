##Qt自定义按钮
######创建自定义按钮程序
``` cpp
//自定义按钮
myWidget::myWidget(QWidget *parent)
    : QWidget(parent)
{
     //自定义按钮
     MyPushButton * mybtn = new MyPushButton;
     //设置标题
     mybtn->setText("自定义窗口");
     //移动窗口
     mybtn->move(200,200);
     //设置到对象树中
     mybtn->setParent(this);

     //连接信号与槽，实现点击按钮，关闭窗口
     //参数1 按钮 参数2 信号发送 参数3 信号接收  参数4 槽处理函数
     //方式1
     connect(mybtn,&MyPushButton::clicked,this,&myWidget::close);
     //方式2
      connect(mybtn,&QPushButton::clicked,this,&QWidget::close);
}
```
