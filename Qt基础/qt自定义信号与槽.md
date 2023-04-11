##Qt自定义信号与槽
######实现功能
* 下课铃响了，老师说起立，学生回答老师好;
* 下课铃响了，老师说辛苦了，学生回答老师辛苦了; (重载)
######代码实现
``` cpp
widget.cpp
```
``` cpp
#include "widget.h"
#include "ui_widget.h"
#include <QPushButton>
#include <QDebug>
//实现需求，下课铃响了 老师喊起立，学生喊老师好
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //创建一个老师的对象
    this->th = new Teacher(this);
    //创建一个学生的对象
    this->sd = new Student(this);

    //连接信号与槽,不带参数,
    connect(th,&Teacher::StandUp,sd,&Student::HelloTeacher);
    //执行条件 下课铃响了
    classIsOver();

    //连接信号于槽，带参数, 重载信号与槽
    void (Teacher:: *StandUpPointer)(QString) = &Teacher::StandUp;
    void (Student:: *HelloTeacherPointer)(QString) = &Student::HelloTeacher;
    connect(th,StandUpPointer,sd,HelloTeacherPointer);
    classIsOver1();

    //点击按钮触发下课铃响，老师听到响铃
    QPushButton *qpb = new QPushButton("下课铃响",this);
    //无参数和槽函数连接
    connect(th,StandUpPointer1,sd,HelloTeacherPointer1);
    //信号连接信号
    connect(qpb,&QPushButton::clicked,th,StandUpPointer1);

    //也可以断开信号的连接
    //disconnect(th,StandUpPointer1,sd,HelloTeacherPointer1);

    //QT4版本的型号与槽的连接方式
    //利用QT4信号槽 连接无参数版本
    //connect(th,SIGNAL(StandUp()),sd,SIGNAL(HelloTeacher()));
}
/*
emit是不同窗口/类间的触发信号。
当对象改变其状态时，信号就由该对象发射 (emit) 出去，通过槽函数实现用户想要的效果。
*/
void Widget::classIsOver()
{
    qDebug() << "老师: 起立";
    emit th->StandUp();
}

void Widget::classIsOver1()
{
    qDebug() << "老师:";
    emit th->StandUp("早上好");
}

Widget::~Widget()
{
    delete ui;
}
```
``` cpp
widget.h
```
``` cpp
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "teacher.h"
#include "student.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    Teacher * th;
    Student * sd;

    void classIsOver();
    void classIsOver1();
};
#endif // WIDGET_H
```
``` cpp
//teacher.c--------------------------------------------------
#include "teacher.h"
Teacher::Teacher(QObject *parent) : QObject(parent)
{

}
// teacher.h-------------------------------------------------
#ifndef TEACHER_H
#define TEACHER_H

#include <QObject>

class Teacher : public QObject
{
    Q_OBJECT
public:
    explicit Teacher(QObject *parent = nullptr);

signals:
    //自定义信号写在这里，老师喊起来发送信号
    //信号不用实现，返回值是void，只需要声明，
    //可以带参数,可以重载
    void StandUp();

    void StandUp(QString speak);

public slots:
};

#endif // TEACHER_H
```
``` cpp
//===========
//student.h =
//===========
#ifndef STUDENT_H
#define STUDENT_H

#include <QObject>

class Student : public QObject
{
    Q_OBJECT
public:
    explicit Student(QObject *parent = nullptr);

signals:

public slots:
    //自定义槽函数早期的QT需要写到public slots下，后面的可以写到public下；
    //学生听到老师喊起立，喊老师好
    //槽函数可以带参数
    //需要实现槽函数;
    void HelloTeacher();

    void HelloTeacher(QString sepak);
};

#endif // STUDENT_H

//===========
//student.c =
//===========
#include "student.h"
#include <QDebug>

Student::Student(QObject *parent) : QObject(parent)
{

}

void Student::HelloTeacher(){
    qDebug() << "学生：老师好";
}

void Student::HelloTeacher(QString speak)
{
    qDebug() << speak.toUtf8().data();
    qDebug() << "学生：老师好:";
}
```
