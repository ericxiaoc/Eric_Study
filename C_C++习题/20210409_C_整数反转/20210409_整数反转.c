/*
题目：
给你一个 32 位的有符号整数 x ，返回将 x 中的数字部分反转后的结果。
如果反转后整数超过 32 位的有符号整数的范围 -2^31 ~ 2^31-1，就返回 0。
示例 1：
输入：x = 123
输出：321

示例 2：
输入：x = -123
输出：-321

示例 3：
输入：x = 120
输出：21

示例 4：
输入：x = 0
输出：0
*/
#include <stdio.h>
#include <stdlib.h>
int reverse(int x){
    int tmp = 0;//末尾数;
    int res = 0;//局部变量一定要初始化;
    while(x != 0){

        tmp = x % 10;//获取整数的最低位
        if((res > 214748364 ) || ((res == 214748364)&&(tmp > 7) )){
            return 0;
        }
        if((res < -214748364) || ((res == -214748364)&&(tmp < -8))){
            return 0;
        }

        res = res * 10 + tmp; //res 存储最低位的数值，并将最低位的数字移到高位；
        x = x / 10;// x 整数获取前几位的数值，再次进行循环获取最低位；
    }
    return res;
}
int main(int argc, char const *argv[]) {
  /* code */
  int x;
  printf("请输入一个整数: \n");
  scanf("%d",&x);
  printf("reverse = %d\n",reverse(x));
  return 0;
}
