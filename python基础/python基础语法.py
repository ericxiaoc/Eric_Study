# coding=utf-8
# 输出打印
print("hello world!")
# 注释的方式
# 第一个注释

'''
第二个注释
'''

"""
第三个注释
"""

print("welcome!")

####行与缩进####
if True:
    print ("True")
else:
    print ("False")
#错误的写法
"""
if True:
    print ("Answer")
    print ("True")
else:
    print ("Answer")
  print ("False")    # 缩进不一致，会导致运行错误
"""
#数字(Number)类型
'''
int
float
bool
complex(复数)   复数由实部（real）和虚部（imag）构成，在 Python 中，复数的虚部以j或者J作为后缀，具体格式为：a + bj
'''
#字符串(String)
"""
1.python中单引号和双引号使用完全相同。
2.使用三引号(''' 或 三个双引号)可以指定一个多行字符串。
3.转义符 \
4.反斜杠可以用来转义，使用r可以让反斜杠不发生转义。如 r"this is a line with \n" 则\n会显示，并不是换行
5.按字面意义级联字符串，如"this " "is " "string"会被自动转换为this is string
6.字符串可以用 + 运算符连接在一起，用 * 运算符重复
7.Python 中的字符串有两种索引方式，从左往右以 0 开始，从右往左以 -1 开始
8.Python中的字符串不能改变。
9.Python 没有单独的字符类型，一个字符就是长度为 1 的字符串
10.字符串的截取的语法格式如下：变量[头下标:尾下标:步长]
"""
str = '123456789'
# 输出字符串
print(str)
# 输出第一个到倒数第二个的所有字符
print(str[0:8:1]);
print(str[0:-1])
# 输出字符串第一个字符
print(str[0]);
# 输出从第三个开始到第五个的字符
print(str[2:5:1]);
print(str[2:5]);
# 输出从第三个开始后的所有字符
print(str[2:9:1]);
print(str[2:]);
# 输出从第二个开始到第五个且每隔一个的字符（步长为2）
print(str[1:5:2]);
# 输出字符串两次
print(str*2);
# 连接字符串
print(str+" hello");
# 使用反斜杠(\)+n转义特殊字符
print("hello\nrunoob");
# 在字符串前面添加一个 r，表示原始字符串，不会发生转义
print(r"hello\nrunoob");
#等待用户输入
#input("\n\n按下 enter 键后退出")
#同一行显示多条语句
import sys; x = 'runoob'; sys.stdout.write(x + '\n')
#print 输出
x = "a"
y = "b"
# 换行输出
print(x)
print(y)
print('---------')
# 不换行输出
print(x, end=" ")
print(y, end=" ")
####################################################
#import 与 from...import
'1.在 python 用 import 或者 from...import 来导入相应的模块' \
'2.将整个模块(somemodule)导入，格式为： import somemodule' \
'3.从某个模块中导入某个函数,格式为： from somemodule import somefunction' \
'4.从某个模块中导入多个函数,格式为： from somemodule import firstfunc, secondfunc, thirdfunc' \
'5.将某个模块中的全部函数导入，格式为： from somemodule import *'
###############"导入 sys 模块"###################
import sys
print('================Python import mode==========================')
print ('命令行参数为:')
for i in sys.argv:
    print (i)
print ('\n python 路径为',sys.path)
###############导入 sys 模块的 argv,path 成员##################
from sys import argv, path  # 导入特定的成员

print('================python from import===================================')
print('path:', path)  # 因为已经导入path成员，所以此处引用时不需要加sys.path
