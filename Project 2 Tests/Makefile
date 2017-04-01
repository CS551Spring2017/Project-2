OBJS = test0_topicLookup.c test1_single_msg.c test2_multi_msg.c test3_multi_topics.c test4_multi_pubs.c test5_multi_subs.c test6_multi_pubs_subs.c test7_blocking_publish.c test8_unblocking_publish.c test9_blocking_multi_pubs_subs.c test10_unblocking_multi_pubs_subs.c test11_something_besides_int.c topicReset.c
TESTOBJS_OUT = test0 test1 test2 test3 test4 test5 test6 test7 test8 test9 test10 test11 reset
CC = clang
DEBUG = 
FLAGS = -Wall $(DEBUG)

	
clean:
	rm -f $(TESTOBJS_OUT)
	
tests:
	$(CC) $(FLAGS) test0_topicLookup.c -o test0
	$(CC) $(FLAGS) test1_single_msg.c -o test1
	$(CC) $(FLAGS) test2_multi_msg.c -o test2
	$(CC) $(FLAGS) test3_multi_topics.c -o test3
	$(CC) $(FLAGS) test4_multi_pubs.c -o test4
	$(CC) $(FLAGS) test5_multi_subs.c -o test5
	$(CC) $(FLAGS) test6_multi_pubs_subs.c -o test6
	$(CC) $(FLAGS) test7_blocking_publish.c -o test7
	$(CC) $(FLAGS) test8_unblocking_publish.c -o test8
	$(CC) $(FLAGS) test9_blocking_multi_pubs_subs.c -o test9
	$(CC) $(FLAGS) test10_unblocking_multi_pubs_subs.c -o test10
	$(CC) $(FLAGS) test11_something_besides_int.c -o test11
	$(CC) $(FLAGS) topicReset.c -o reset
	
all: tests