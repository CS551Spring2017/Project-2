#include <lib.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    char* name = "topic10";
    pid_t temp = getpid();
    topicCreate(name);    
    topicPublisher(temp,name);
	message m;
	m.m1_i1 = 1;
	if(fork() == 0)
	{
		sleep(5);
		topicSubscriber(getpid(),name);
		message *r;
		r = retrieve(getpid(), name);
		printf("%d\n", r->m1_i1);
		sleep(2);
		r = retrieve(getpid(), name);
		printf("%d\n", r->m1_i1);
		exit(0);
	}
	for(int i = 0; i < 100;i++){
		m.m1_i1 = i;
		publish(getpid(), name, &m);
	}
}