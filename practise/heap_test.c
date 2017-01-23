#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int nInsert_min_heap(int *arr,int count,int val);
int nPrint_Bitree(int *arr,int count);
int main()
{
	int iNum_arr[] = {1,101,3,4,52,12,45,2,45,1,6,7};
	int iHeap_arr[10];
	int iElem_num = 12,i;
	
	memset(iHeap_arr,0x00,sizeof(iHeap_arr));
	
	/*将iNum_arr数组构建成一个小顶堆并存放在iHeap_arr数组中*/
//	for(i = 0;i < iElem_num;i++)
//		nInsert_min_heap(iHeap_arr,i,iNum_arr[i]);
	
	printf("排序前序列为:\n");
	nPrint_Bitree(iNum_arr,iElem_num);	
	
//	printf("堆排序结果序列为:\n");
//	nPrint_Bitree(iHeap_arr,iElem_num);
	
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

/****************************************************
	函数说明：删除堆顶元素
	函数名:	nDelete_min_heap(int *arr,int *count,int *val)
	入口参数：arr 存储堆排序数组的指针
	出/入口参数：count 本次排序前数组中的元素个数
	出口参数：val 本次插入的值
****************************************************/
int nDelete_min_heap(int *arr,int *count,int *val)
{
	int j,top = 0;
	int elem_num = *count;
	/*如果堆中元素个数为0,则处理失败*/
	if(elem_num <= 0)
	{
		printf("heap is empty,fail\n");
		return -1;
	}
	
	*val = arr[top];
	
	/*将最后一个元素赋值给第一个元素*/
	arr[top] = arr[elem_num-1];
	elem_num--;
	j = 2*top + 1;		/*j初始化为根节点的左子节点*/
//	while(j < elem_num)
//	{
//		if(j+1< n && arr[j+1] < arr[j])
//			j++;
//		if(arr[top] <= arr[j])
//			break;
//		
//		arr[0] = arr[j];
//		
//		
//	}
	
}

/****************************************************
	函数说明：打印二叉树
	函数名:	nPrint_Bitree(int *arr,int count)
	入口参数：arr 存储堆排序数组的指针
	出/入口参数：count 本次排序前数组中的元素个数
****************************************************/
int nPrint_Bitree(int *arr,int count)
{
	int i = 1,j = 0,tmp = 1;
	char aTmp[12+1];
	char aSpilt_tmp[1024];
	int iLevel_num = 0;
	int iLast_node = 1;
	int iSum_len = 0;
	
	memset(aTmp,0x00,sizeof(aTmp));
	memset(aSpilt_tmp,0x00,sizeof(aSpilt_tmp));
	strcpy(aTmp,"------");
	
	tmp = count;
	while(tmp > 0)
	{
		tmp = tmp / 2;
		iLevel_num++;
	}
	printf("the bitree level num is %d\n",iLevel_num);
	
	for(tmp = 0;tmp < iLevel_num -1;tmp++)
	{
		iLast_node *= 2;
	}
	iSum_len = strlen(aTmp) * iLast_node / 2;
	printf("the last node is %d,last len is %d\n",iLast_node,iSum_len);
	tmp = 1;
	while(i <= count && j < count)
	{
		memset(aSpilt_tmp,0x00,sizeof(aSpilt_tmp));
		memset(aSpilt_tmp,aTmp[0],iSum_len);
		iSum_len /= 2;
		while(tmp-- >= 1)
		{
			/*计算前面应该打印多少个空格*/
			printf("%s%3d%s",aSpilt_tmp,arr[j],aSpilt_tmp);
			j++;
		}
		printf("\n\n");
		
		if(i*2 > count)
			i = i*2 - (i*2)%count;
		else
			i = i*2 ;
		
		tmp = i;
	}
	return 0;
}

