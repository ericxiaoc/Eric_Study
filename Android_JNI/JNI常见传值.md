###JNI常用类型
|  Java   |   JNI    |     C/C++      |      描述      |
|:-------:|:--------:|:--------------:|:--------------:|
| boolean | jboolean | unsigned char  | 无符号8位整数  |
|  byte   |  jbyte   |  signed char   | 无符号8位整数  |
|  char   |  jchar   | unsigned short | 无符号16位整数 |
|  short  |  jshort  |  signed short  | 无符号16位整数 |
|   int   |   jint   |   signed int   | 无符号32位整数 |
|  long   |  jlong   |  signed long   | 无符号32位整数 |
|  float  |  jfloat  |     float      |   32位浮点数   |
| double  | jdouble  |     double     |   64位浮点数   |
###引用数据类型
|        Java         |      JNI      |
|:-------------------:|:-------------:|
|  java.lang.Object   |    jobject    |
|  java.lang.String   |    jstring    |
|   java.lang.Class   |    jclass     |
|      Object[]       | jobjectArray  |
|      boolean[]      | jbooleanArray |
|       byte[]        |  jbyteArray   |
|       char[]        |  jcharArray   |
|       short[]       |  jshortArray  |
|        int[]        |   jintArray   |
|       long[]        |  jlongArray   |
|       float[]       |  jfloatArray  |
|      double[]       | jdoubleArray  |
| java.lang.Throwable |  jthrowable   |
|        void         |     void      |
###数据类型描述符
|     Java     |   类型描述符   |
|:------------:|:--------------:|
|     int      |       I        |
|     long     |       J        |
|     byte     |       B        |
|    short     |       S        |
|     char     |       C        |
|    float     |       F        |
|    double    |       D        |
|   boolean    |       Z        |
|     void     |       V        |
| 其他引用类型 | L + 类全名 + ; |
|     数组     |       [        |
|     方法     |  (参数)返回值  |
###实例
* 表示一个String类
``` cpp
Java类型：java.lang.String
JNI描述符：Ljava/lang/String;  (L + 类全名 + ;)
```
* 表示数组
``` cpp
Java类型：String[]
JNI描述符：[Ljava/lang/String;

Java类型：int[][]
JNI描述符：[[I
```
* 表示方法
``` cpp
Java方法：long func(int n, String s, int[] arr);
JNI描述符：(ILjava/lang/String;[I)J

Java方法：void func();
JNI描述符：()V
```
