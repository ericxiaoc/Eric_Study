/*
*给定一个整数数组 nums 和一个整数目标值 target，请你在该数组中找出 和为目标值 的那 两个 整数，并返回它们的数组下标
*你可以假设每种输入只会对应一个答案。但是，数组中同一个元素在答案里不能重复出现。
*/
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
#include <stdio.h>
#include <stdlib.h>
int* twoSum(int* nums, int numsSize, int target){
        int i,j;
        int *result=NULL;
        for(i=0;i<numsSize-1;i++)//遍历第一个元素到倒数第二个元素；
        {
            for(j=i+1;j<numsSize;j++)//遍历第二个元素到倒数第一个元素；
            {
                if(nums[i]+nums[j]==target)//计算第一个元素与第二个元素的和是否等于目标值；
                {
                    result=(int*)malloc(sizeof(int)*2);
                    result[0]=i;
                    result[1]=j;
                    return result;
                }
            }
        }
        return result;
}
int main(int argc, char const *argv[]) {
  int tmp[4];
  int i,target;
  printf("请输入数组元素 : \n");
	for (i = 0; i < 4; i++) {
		scanf("%d", &tmp[i]);
	}
  printf("请输入目标值 : \n");
  scanf("%d", &target);

  printf("twoSum = %d \n",twoSum(tmp,4,target)[0]);
  printf("twoSum = %d \n",twoSum(tmp,4,target)[1]);

  return 0;
}
/*
*/
