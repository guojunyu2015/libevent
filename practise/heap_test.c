#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int nInsert_min_heap(int *arr,int count,int val);
int main()
{
	int iNum_arr[] = {1,101,3,4,52,12,45};
	int iHeap_arr[10];
	int iElem_num = 7,i;
	
	memset(iHeap_arr,0x00,sizeof(iHeap_arr));
	
	/*将iNum_arr数组构建成一个小顶堆并存放在iHeap_arr数组中*/
	for(i = 0;i < iElem_num;i++)
		nInsert_min_heap(iHeap_arr,i,iNum_arr[i]);
	
	printf("堆排序结果序列为:");
	for(i = 0;i < iElem_num;i++)
		printf("%d ",iHeap_arr[i]);
		
	printf("\n");
	return 0;
}

/****************************************************
	函数说明：将指定元素插入到小顶堆中
	函数名:	nInsert_min_heap(int *arr,int count,int val)
	入口参数：arr 存储堆排序数组的指针
	入口参数：count 本次排序前数组中的元素个数
	入口参数：val 本次插入的值
****************************************************/
int nInsert_min_heap(int *arr,int count,int val)
{
	int j;
	
	if(count == 0)
	{
		arr[count] = val;
		return 0;
	}
	
	j = (count - 1) / 2;
	while(j >= 0 && count != 0)
	{
		if(val >= arr[j])
		{
			arr[count] = val;
			break;
		}
		arr[count] = arr[j];
		arr[j] = val;
		count = j;
		j = (count - 1) / 2;			
	}
	
	return 0;
}