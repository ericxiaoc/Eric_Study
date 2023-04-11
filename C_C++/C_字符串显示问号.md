###字符串中会显示问号
``` c
char buff[8];

memcpy(version_date, TIMESTAMP+2, 2);
memcpy(version_date+2, TIMESTAMP+5, 2);
memcpy(version_date+4, TIMESTAMP+8, 2);
memcpy(version_date+6, "JJ", 2);

printf("version_date = %s \n",version_date);
```
输出显示
``` c
210906JJ?
```
    思考：为什么会多一个问号？
    解析：
        字符串的结束标志是'\0', 例中的buff大小为8,最后一位应该是要预留'\0',
        也就是说buff[8] = '\0';但是例子的操作，copy的8个元素到数组，也就是
        说元素覆盖掉了'\0',即'\0'丢失,数组并没有结束的标志，所以多的一个字符
        是系统随机加上的。
    解决办法：

``` c
char buff[9];
memcpy(version_date, TIMESTAMP+2, 2);
memcpy(version_date+2, TIMESTAMP+5, 2);
memcpy(version_date+4, TIMESTAMP+8, 2);
memcpy(version_date+6, "JJ", 2);
memcpy(version_date+8, "\0", 1);
printf("version_date = %s \n",version_date);
```
