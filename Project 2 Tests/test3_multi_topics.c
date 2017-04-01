#include <lib.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    char* name = "topic3";
	char* name2 = "topic3.5";
    pid_t temp = getpid();
    topicCreate(name);
	topicCreate(name2);
    topicPublisher(temp,name);
	topicPublisher(temp,name2);
	message m;
	m.m1_i1 = 1;
	publish(getpid(), name, &m);
	message m2;
	m2.m1_i1 = 2;
	publish(getpid(), name2, &m2);
	if(fork() == 0)
	{
		sleep(1);
		topicSubscriber(getpid(),name);
		topicSubscriber(getpid(),name2);
		message *r = retrieve(getpid(), name);
		printf("%d\n", r->m1_i1);
		r = retrieve(getpid(), name2);
		printf("%d\n", r->m1_i1);
		exit(0);
	}
	int status;
	wait(&status);
}
