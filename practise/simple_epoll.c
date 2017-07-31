/*epoll相关函数练习*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX_EVENTS 500

/*
	epoll使用的三个函数
	
	int epoll_create(int size);
	创建一个epoll的句柄,size用来告诉内核这个监听的数目有多大,这个参数不同于select函数中的第一个参数,给出最大监听数的+1值,
	当创建好一个epoll句柄后,它就会占用一个fd值,在linux下如果查看/proc/进程号/fd/,是可以看到这个fd的,因此必须调用close函数
	进行关闭。
	目前该参数已经废弃,传入一个大于0的值即可
	
	int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
	epoll的事件注册函数,它与select不同的是监听事件时告诉内核需要监听什么样的事件,而是先在这里注册监听事件的类型。
	第一个参数是epoll_create的返回值.
	第二个参数表示动作,用三个宏来表示:\
		EPOLL_CTL_ADD：注册新的fd到epfd中；
		EPOLL_CTL_MOD：修改已经注册的fd的监听事件；
		EPOLL_CTL_DEL：从epfd中删除一个fd；
	第三个参数是需要监听的fd。
	第四个参数是告诉内核需要监听什么事件,epoll_event结构体结构如下:
		typedef union epoll_data{
			void *ptr;
			int fd;
			__uint32_t u32;
			__uint64_t u64;
		}epoll_data_t;
		
		struct epoll_event{
			__uint32_t events;	Epoll events
			epoll_data data;	user data variable
		}
		events可以是以下几个宏的集合:
			EPOLLIN ：表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
			EPOLLOUT：表示对应的文件描述符可以写；
			EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
			EPOLLERR：表示对应的文件描述符发生错误；
			EPOLLHUP：表示对应的文件描述符被挂断；
			EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
			EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里
		
		在使用epoll_ctl时,是把fd添加、修改到内核事件表中,或从内核事件表中删除fd的事件。
		
	int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
	该函数调用成功返回就绪文件描述符个数，失败返回-1并且设置errno
	第一个参数epfd是epoll_create函数返回的文件描述符
	第二个参数 events为输出参数，输出结果为就绪事件的events结构体数组
	第三个参数 maxevents为最多监听多少个事件
	第四个参数 timeout为超时时间，和select和poll类似
*/

int g_epollFd;
void RecvData(int fd, int events, void *arg);
void SendData(int fd, int events, void *arg);

typedef struct myevent_ss
{
    int fd;
    void (*call_back)(int fd, int events, void *arg);
    int events;
    void *arg;
    int status;// 1: in epoll wait list, 0 not in
    char buff[128];// recv data buffer
    int len, s_offset;
    long last_active;// last active time
}myevent_s;
myevent_s g_Events[MAX_EVENTS+1];// g_Events[MAX_EVENTS] is used by listen fd

// set event    
void EventSet(myevent_s *ev, int fd, void (*call_back)(int, int, void*), void *arg)
{
    ev->fd = fd;
    ev->call_back = call_back;
    ev->events = 0;
    ev->arg = arg;
    ev->status = 0;
    bzero(ev->buff, sizeof(ev->buff));
    ev->s_offset = 0;
    ev->len = 0;
    ev->last_active = time(NULL);
}

// add/mod an event to epoll    
void EventAdd(int epollFd, int events, myevent_s *ev)
{
    struct epoll_event epv = {0, {0}};
    int op;
    epv.data.ptr = ev;
    epv.events = ev->events = events;
    if(ev->status == 1){
        op = EPOLL_CTL_MOD;
    }
    else{
        op = EPOLL_CTL_ADD;
        ev->status = 1;
    }
    if(epoll_ctl(epollFd, op, ev->fd, &epv) < 0)
        printf("Event Add failed[fd=%d], evnets[%d]\n", ev->fd, events);
    else
        printf("Event Add OK[fd=%d], op=%d, evnets[%0X]\n", ev->fd, op, events);
}

// delete an event from epoll
void EventDel(int epollFd, myevent_s *ev)
{
    struct epoll_event epv = {0, {0}};
    if(ev->status != 1) return;
    epv.data.ptr = ev;
    ev->status = 0;
    epoll_ctl(epollFd, EPOLL_CTL_DEL, ev->fd, &epv);
}

// accept new connections from clients
void AcceptConn(int fd, int events, void *arg)
{
    struct sockaddr_in sin;
    socklen_t len = sizeof(struct sockaddr_in);
    int nfd, i;
    // accept
    if((nfd = accept(fd, (struct sockaddr*)&sin, &len)) == -1)
    {
        if(errno != EAGAIN && errno != EINTR)
        {
        }
        printf("%s: accept, %d", __func__, errno);
        return;
    }
    do
    {
        for(i = 0;i < MAX_EVENTS;i++)    
        {
            if(g_Events[i].status == 0)    
            {
                break;
            }
        }
        if(i == MAX_EVENTS)    
        {
            printf("%s:max connection limit[%d].", __func__, MAX_EVENTS);
            break;
        }
        // set nonblocking  
        int iret = 0;
        if((iret = fcntl(nfd, F_SETFL, O_NONBLOCK)) < 0)  
        {
            printf("%s: fcntl nonblocking failed:%d", __func__, iret);
            break;
        }
        // add a read event for receive data    
        EventSet(&g_Events[i], nfd, RecvData, &g_Events[i]);
        EventAdd(g_epollFd, EPOLLIN, &g_Events[i]);
    }while(0);
    printf("new conn[%s:%d][time:%ld], pos[%d]\n", inet_ntoa(sin.sin_addr),   
            ntohs(sin.sin_port), g_Events[i].last_active, i);
}
// receive data    
void RecvData(int fd, int events, void *arg)    
{
    myevent_s *ev = (myevent_s*)arg;
    int len;
    // receive data  
    len = recv(fd, ev->buff+ev->len, sizeof(ev->buff)-1-ev->len, 0);
    EventDel(g_epollFd, ev);
    if(len > 0)
    {
        ev->len += len;
        ev->buff[len] = '\0';
        printf("C[%d]:%s\n", fd, ev->buff);
        // change to send event    
        EventSet(ev, fd, SendData, ev);
        EventAdd(g_epollFd, EPOLLOUT, ev);
    }
    else if(len == 0)    
    {
        close(ev->fd);
//        printf("[fd=%d] pos[%d], closed gracefully.\n", fd, ev-g_Events);
    }
    else    
    {
        close(ev->fd);
        printf("recv[fd=%d] error[%d]:%s\n", fd, errno, strerror(errno));
    }
}
// send data     
void SendData(int fd, int events, void *arg)    
{
    myevent_s *ev = (myevent_s*)arg;
    int len;
    // send data    
    len = send(fd, ev->buff + ev->s_offset, ev->len - ev->s_offset, 0);
    printf("len = %d\n",len);
    if(len > 0)    
    {
        printf("send[fd=%d], [%d<->%d]%s\n", fd, len, ev->len, ev->buff);
        ev->s_offset += len;
        if(ev->s_offset == ev->len)  
        {
            // change to receive event  
            EventDel(g_epollFd, ev);
            EventSet(ev, fd, RecvData, ev);
            EventAdd(g_epollFd, EPOLLIN, ev);
        }
    }
    else    
    {
        close(ev->fd);
        EventDel(g_epollFd, ev);
        printf("send[fd=%d] error=%d[%s]\n", fd, errno,strerror(errno));
    }
}
void InitListenSocket(int epollFd, short port)    
{
    int listenFd = socket(AF_INET, SOCK_STREAM, 0);
    
    /*将socket描述符设为非阻塞模式*/
    fcntl(listenFd, F_SETFL, O_NONBLOCK);
    printf("server listen fd=%d\n", listenFd);
    
    EventSet(&g_Events[MAX_EVENTS], listenFd, AcceptConn, &g_Events[MAX_EVENTS]);
    // add listen socket    
    EventAdd(epollFd, EPOLLIN, &g_Events[MAX_EVENTS]);
    // bind & listen    
    struct sockaddr_in sin;
    bzero(&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);
    bind(listenFd, (const struct sockaddr*)&sin, sizeof(sin));
    listen(listenFd, 5);
}

int main(int argc, char **argv)    
{
    unsigned short port = 8888;// default port    
    if(argc == 2){
        port = atoi(argv[1]);
    }
	
	/*创建epoll文件描述符*/
    g_epollFd = epoll_create(MAX_EVENTS);
    printf("epoll_fd 值为: [%d]\n",g_epollFd);
    printf("errno = %d [%s]\n",errno,strerror(errno));
	if(g_epollFd <= 0)
	{
		printf("create epoll failed.%d,errno = %d[%s]\n", g_epollFd,errno,strerror(errno));
		return -1;
    }
    // create & bind listen socket, and add to epoll, set non-blocking    
    InitListenSocket(g_epollFd, port);
    // event loop
    struct epoll_event events[MAX_EVENTS];
    printf("server running:port[%d]\n", port);
    int checkPos = 0;
    while(1){
        // a simple timeout check here, every time 100, better to use a mini-heap, and add timer event    
        long now = time(NULL);
        for(int i = 0;i < 100;i++, checkPos++) // doesn't check listen fd    
        {
            if(checkPos == MAX_EVENTS){
            	checkPos = 0;// recycle
            }
            if(g_Events[checkPos].status != 1){
            	continue;
            }
            long duration = now - g_Events[checkPos].last_active;
            if(duration >= 60) // 60s timeout    
            {
                close(g_Events[checkPos].fd);
                printf("[fd=%d] timeout[%ld--%ld].\n", g_Events[checkPos].fd,g_Events[checkPos].last_active, now);
                EventDel(g_epollFd, &g_Events[checkPos]);
            }
        }
        // wait for events to happen    
        int fds = epoll_wait(g_epollFd, events, MAX_EVENTS, 1000);
        if(fds < 0){
            printf("epoll_wait error, exit\n");
            break;
        }
        for(int i = 0;i < fds;i++){
            myevent_s *ev = (myevent_s*)events[i].data.ptr;
            if((events[i].events&EPOLLIN)&&(ev->events&EPOLLIN)) // read event    
            {
                ev->call_back(ev->fd, events[i].events, ev->arg);
            }
            if((events[i].events&EPOLLOUT)&&(ev->events&EPOLLOUT)) // write event    
            {
                ev->call_back(ev->fd, events[i].events, ev->arg);
            }
        }
    }
    // free resource    
    return 0;
}