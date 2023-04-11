##Qt信号与槽的总结
* 信号是可以连接信号的
* 一个信号是可以连接多个槽函数的
* 多个信号可以连接同一个槽函数
* 信号与槽的参数类型必须一一对应
* 信号的参数个数可以多余槽函数的参数个数，
* 槽函数执行的顺序是随机的，不能控制；

###Qt练习题
* 新建一个按钮OPEN，点击打开一个新建的窗口，按钮变为CLOSE，再次点击，销毁窗口，并将按钮更改为OPEN;
``` cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPushButton *btn = new QPushButton("OPEN",this);

    QWidget *qw = new QWidget;

    connect(btn,&QPushButton::clicked,qw,
            [=](){
        if(!qw->isHidden()){//判断窗口是否存在
            qw->close();
            btn->setText("OPEN");
        }else{
            qw->show();
            btn->setText("CLOSE");
        }
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}
```
