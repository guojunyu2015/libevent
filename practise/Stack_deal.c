/*********************************************************
 ** 文件名  :   stack_deal.c
 ** 功能    :   堆栈公共处理函,包含了堆栈的初始化,入栈,出栈等常用操作
 ** 作者    :   guojy
 ** 建立日期:    2016年6月7日22:03:01
 ** 修改日期:
 ***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 512

typedef struct stack_dtl
{
    char data[MAX_SIZE];
    int base;    /*栈底*/
    int head;    /*栈顶*/
}stack;

/*********************************************************
 ** 函数名  :   init_stack(stack *st_stack)
 ** 功能    :   初始化栈
 ** 作者    :   guojy
 ** 建立日期:   2016年6月7日22:36:031
 ** 调用其他函数:
 ** 返回值:        无
 ***********************************************************/
void init_stack(stack *st_stack)
{
    memset(st_stack->data,0x00,sizeof(st_stack->data));
    st_stack->base = 0;
    st_stack->head = 0;
}

/*********************************************************
 ** 函数名  :   push_stack(stack *st_stack,char data)
 ** 功能    :   入栈
 ** 作者    :   guojy
 ** 建立日期:   2016年6月7日22:36:03
 ** 调用其他函数:
 ** 返回值:        0:入栈成功    -1:入栈失败
 ***********************************************************/
int push_stack(stack *st_stack,char data)
{
    if(st_stack->head + 1 == MAX_SIZE)    /*如果栈中元素已经满了,入栈失败*/
        return -1;
    st_stack->data[st_stack->head] = data;
    st_stack->head++;
    return 0;
}

/*********************************************************
 ** 函数名  :   pop_stack(stack *st_stack,char *data)
 ** 功能    :   出栈
 ** 作者    :   guojy
 ** 建立日期:   2016年6月7日22:42:49
 ** 调用其他函数:
 ** 返回值:        0:出栈成功    -1:出栈失败
 ***********************************************************/
int pop_stack(stack *st_stack,char *data)
{
    if(st_stack->head == 0)        /*如果栈为空,则出栈失败*/
        return -1;
    *data = st_stack->data[st_stack->head - 1];
    st_stack->data[st_stack->head] = 0x00;
    st_stack->head--;
    return 0;
}

/*********************************************************
 ** 函数名  :   empty_stack(stack *st_stack)
 ** 功能    :   判断栈是否为空
 ** 作者    :   guojy
 ** 建立日期:   2016年6月7日22:42:49
 ** 调用其他函数:
 ** 返回值:        0:栈为空    1:栈非空
 ***********************************************************/
int empty_stack(stack *st_stack)
{
    if(st_stack->head == st_stack->base)
        return 0;
    else
        return 1;
}

/*********************************************************
 ** 函数名  :   print_stack(stack *st_stack)
 ** 功能    :   打印栈的内容
 ** 作者    :   guojy
 ** 建立日期:   2016年6月7日22:42:49
 ** 调用其他函数:
 ** 返回值:        无
 ***********************************************************/
void print_stack(stack *st_stack)
{
    int i;
    for(i = 0;i <= st_stack->head;i++)
        printf("%c ",st_stack->data[i]);

    printf("\n");
}

/*********************************************************
 ** 函数名  :   nSwitch_nortoafter(char *aIn_expr,char *aOut_expr)
 ** 功能    :   将常规表达式转换为后缀表达式
 ** 作者    :   guojy
 ** 建立日期:   2016年6月8日16:11:04
 ** 调用其他函数:
 ** 返回值:        无
 ***********************************************************/
int nSwitch_nortoafter(char *aIn_expr,char *aOut_expr)
{
	stack st_stack;
	int i,j,iNum_count;
	int ret;
	char a;
	
	init_stack(&st_stack);
	printf("输入的表达式为:[%s]\n",aIn_expr);
	for(i = 0;i < strlen(aIn_expr);i++)
	{
		if(aIn_expr[i] == ' ')
			continue;
		else if(aIn_expr[i] == '+' || aIn_expr[i] == '-' || aIn_expr[i] == '*' || aIn_expr[i] == '/')
		{
			ret = push_stack(&st_stack,aIn_expr[i]);
			if(ret)
			{
				printf("[%d]进栈失败\n",__LINE__);
				return -1;
			}
		}
		else if(aIn_expr[i] >= '0' && aIn_expr[i] <= '9')
		{
			iNum_count++;
			strncat(aOut_expr,aIn_expr+i,1);
		}
		else if(aIn_expr[i] == '(')
			iNum_count = 0;
		else if(aIn_expr[i] == ')')
		{
			ret = pop_stack(&st_stack,&a);
			if(ret)
			{
				printf("[%d]出栈失败\n",__LINE__);
				return -1;
			}
			sprintf(aOut_expr,"%s%c",aOut_expr,a);
			iNum_count = 0;
		}
		else
		{
			printf("表达式中存在非法字符\n");
			return -1;
		}
		
		if(iNum_count == 2)
		{
			ret = pop_stack(&st_stack,&a);
			if(ret)
			{
				printf("[%d]出栈失败\n",__LINE__);
				return -1;
			}
			sprintf(aOut_expr,"%s%c",aOut_expr,a);
			iNum_count = 0;
		}
	}
	iNum_count = st_stack.head;
	for(i = 0;i < iNum_count;i++)
	{
		ret = pop_stack(&st_stack,&a);
		if(ret)
		{
			printf("[%d]出栈失败\n",__LINE__);
			return -1;
		}
		sprintf(aOut_expr,"%s%c",aOut_expr,a);
	}
	printf("执行完毕,栈中剩余元素个数为:[%d]\n",st_stack.head);
	return 0;
}

/*********************************************************
 ** 函数名  :   getSymbolEffective(char *src)
 ** 功能    :   判断括号表达式是否为有效表达式(如"(("为非法,"()"为有效)
 ** 作者    :   guojy
 ** 建立日期:   
 ** 调用其他函数:
 ** 返回值:        无
 ***********************************************************/
int getSymbolEffective(char *src)
{
	stack st_stack;
	int left_flag = 0,right_flag = 0;
	int ret;
	init_stack(&st_stack);
	char tmp;
	
	printf("输入的表达式为:[%s]\n",src);
	
	if(src == NULL)
	{
		printf("输入串为空,无法判断");
		return 0;
	}
	
	/*左括号入栈,遇到右括号则出栈*/
	while(*src)
	{
		if(*src == '(')
		{
			if(left_flag == 0)
				left_flag = 1;
			
			/*左括号入栈*/
			push_stack(&st_stack,*src);
			src++;
		}
		else if(*src == ')')
		{
			if(right_flag == 0)
				right_flag = 1;
			/*遇到右括号则左括号出栈*/
			if(pop_stack(&st_stack,&tmp) == -1)		/*如果出栈失败*/
			{
				printf("表达式非法\n");
				return -1;
			}
			src++;
		}
		else
			src++;
	}
	
	/*如果栈不空或者左括号没有入过栈*/
	if(empty_stack(&st_stack) == 1 || (left_flag == 0 && right_flag == 1) )
	{
		printf("表达式非法\n");
		return -1;
	}
	
	printf("表达式合法\n");
	return 0;
}
