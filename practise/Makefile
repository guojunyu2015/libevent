OBJS = heap_test.o Stack_deal.o
heap_test:$(OBJS)
		cc -o heap_test $(OBJS)
		@echo "[heap_test]编译完成"

simple_epoll:simple_epoll.c
	@echo "开始编译 simple_epoll.c"
	cc -o simple_epoll simple_epoll.c
	@echo "simple_epoll编译完成"

heap_deal.o:heap_deal.c Stack_deal.c pub_deal.h
		@echo "开始编译 heap_deal.c Stack_deal.c"
		cc -c Stack_deal.c heap_deal.c
clean:
		rm -f *.o
