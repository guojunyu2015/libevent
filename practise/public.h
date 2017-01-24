#define MAX_SIZE 512		/*堆栈中最大元素个数*/
typedef struct stack_dtl
{
    int data[MAX_SIZE];
    int base;    /*栈底*/
    int head;    /*栈顶*/
}stack;

void init_stack(stack *st_stack);
int push_stack(stack *st_stack,int data);
int pop_stack(stack *st_stack,int *data);
int empty_stack(stack *st_stack);
void print_stack(stack *st_stack);
