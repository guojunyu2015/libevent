/*event_init函数,该函数初始化了一个全局的current_base变量,默认中,event都是向这个变量中添加*/
struct event_base *event_init()
{
	struct event_base *base = event_base_new();
	
	if(base != NULL)
		current_base = base;
		
	return base;
}

/*event_base_new函数*/
struct event_base *event_base_new()
{
	int i;
	struct event_base *base;
	
	if(base = calloc(1,sizeof(struct event_base)) == NULL)	/*为event_base分配内存失败*/\
		return -1;
	
	event_sigcb = NULL;		/*信号的回调函数,event_sigcb为一个全局函数指针*/
	event_gotsig = 0;
	
	detect_monotonic();
	gettime(base,&base->event_tv);
	
	min_heap_ctor(&base->timeheap);		/*初始化小顶堆*/
	TAILQ_INIT(&base->eventqueue);		/*初始化事件队列*/
	base->sig.ev_signal_pair[0] = -1;
	base->sig.ev_signal_pair[1] = -1;
}

/*event_set对event结构体进行基本的赋值操作*/
void event_set(struct event *ev,int fd,short events,void(*callback)(int,short,void *),void *arg)
{
	ev->ev_base = current_base;
	ev->ev_fd = fd;
	ev->ev_events = events;
	ev->ev_ncalls = 0;
	ev->ev_arg = arg;
	ev->ev_callback = callback;
	ev->ev_flags = EVLIST_INIT;		/*将ev_flags初始化为*/
	ev->ev_res = 0;
	ev->ev_ncalls = NULL;
	
	min_heap_elem_init(ev);
	
	if(current_base)
		ev->ev_pri = current_base->nactivequeues/2;
	
}

/*event_base_set函数将event结构体和event_base结构体变量进行绑定,同时设置了event结构体变量的优先级初始值*/
int event_base_set(struct event_base *base,struct event *ev)
{
	if(ev->ev_flags != EVLIST_INIT)		/*如果ev不是初始化状态,则直接报错*/
		return -1;
	
	 ev->ev_base = base;
	 ev->ev_pri = base->nactivequeues/2;
	 
	 return 0;
}

/*设置事件优先级函数*/
int event_priority_set(struct event *ev,int pri)
{
	if(ev->ev_flag & EVLIST_ACTIVE)		/*如果事件处于激活状态,则无法修改优先级*/
		return -1;
	
	/*疑问:nactivequeues变量的管理方式,如果要调整的优先级小于0则报错*/
	if(pri < 0 || pri >= ev->ev_base->nactivequeues)
		return -1;
	
	ev->ev_pri = pri;
	return 0;
}

/*event_add函数,将ev注册到event_base上,如果注册成功,ev将被插入到已注册链表中,如果tv不是NULL,则会同时
注册定时事件,将ev添加到timer堆上*/
int event_add(struct event *ev,const struct timeval *tv)
{
	struct event_base *base = ev->ev_base;
	
	/*event_base结构体中的evsel变量的作用*/
	const struct eventop *evsel = base->evsel;	/*base使用的系统I/O策略*/
	
	
	
}