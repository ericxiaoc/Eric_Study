/*
*����һ���������� nums ��һ������Ŀ��ֵ target�������ڸ��������ҳ� ��ΪĿ��ֵ ���� ���� ���������������ǵ������±�
*����Լ���ÿ������ֻ���Ӧһ���𰸡����ǣ�������ͬһ��Ԫ���ڴ��ﲻ���ظ����֡�
*/
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
#include <stdio.h>
#include <stdlib.h>
int* twoSum(int* nums, int numsSize, int target){
        int i,j;
        int *result=NULL;
        for(i=0;i<numsSize-1;i++)//������һ��Ԫ�ص������ڶ���Ԫ�أ�
        {
            for(j=i+1;j<numsSize;j++)//�����ڶ���Ԫ�ص�������һ��Ԫ�أ�
            {
                if(nums[i]+nums[j]==target)//�����һ��Ԫ����ڶ���Ԫ�صĺ��Ƿ����Ŀ��ֵ��
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
  printf("����������Ԫ�� : \n");
	for (i = 0; i < 4; i++) {
		scanf("%d", &tmp[i]);
	}
  printf("������Ŀ��ֵ : \n");
  scanf("%d", &target);

  printf("twoSum = %d \n",twoSum(tmp,4,target)[0]);
  printf("twoSum = %d \n",twoSum(tmp,4,target)[1]);

  return 0;
}
/*
*/
