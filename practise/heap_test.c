#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "public.h"

int nInsert_min_heap(int *arr,int count,int val);
int nPrint_Bitree(int *arr,int count);
int main()
{
	int iNum_arr[] = {1,101,3,4,52,12,45,2,45,1,6,7};
	int iHeap_arr[10];
	int iElem_num = 12,i;
	
	memset(iHeap_arr,0x00,sizeof(iHeap_arr));
	printf("排序前序列为:\n");
	nPrint_Bitree(iNum_arr,iElem_num);	
	
	return 0;
}

/****************************************************
	函数说明：打印二叉树
	函数名:	nPrint_Bitree(int *arr,int count)
	入口参数：arr 存储堆排序数组的指针
	出/入口参数：count 本次排序前数组中的元素个数
****************************************************/
int nPrint_Bitree(int *arr,int count)
{
	int iLevel_num = 0;		/*层数*/
	int iFront_sep_num = 0;	/*一层中开头节点距开头偏移值*/
	int iNode_sep_num = 0;	/*一层中节点间的偏移值*/
	int iMax_len;			/*最下面一层节点总长度*/
	int iMax_node = 1;		/*最下面一层节点个数*/
	stack stBitree;
	
	int iNode_len = 2;		/*单个节点占据的长度,默认为2*/
	int iNode_sep = 4;		/*最小单元节点间距,即最下面一层的节点*/
	
	int tmp;
	
	if(count <= 0)
	{
		printf("空二叉树无法打印\n");
		return 0;
	}
	/*初始化堆栈*/
	init_stack(&stBitree);
	
	/*计算二叉树的层数*/
	tmp = count - 1;
	iLevel_num = 1;
	while(tmp > 0)
	{
		tmp >>= 1;
		iLevel_num++;
		iMax_node << 1;
	}
	
	/*计算每一层偏移值*/
	iMax_len = (iMax_node-1)*iNode_sep + iMax_node*iNode_len;
	iFront_sep_num = 0;
	iNode_sep_num = iNode_sep;
	while(tmp > 0)
	{
		/*入栈*/
		push_stack(&stBitree,iFront_sep_num);
		push_stack(&stBitree,iNode_sep_num);
		
		/*计算下一个偏移值*/
		iNode_sep_num = (iNode_sep_num - 2)/2 + 2*iNode_len + iNode_sep_num + (iNode_sep_num - 2)/2;
		iFront_sep_num = 
	}
	
	printf("二叉树节点总数为:%d,层数为:%d\n",count,iLevel_num);
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

