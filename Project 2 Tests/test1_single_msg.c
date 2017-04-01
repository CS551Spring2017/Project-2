#include <lib.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    char* name = "topic1";
    pid_t temp = getpid();
    topicCreate(name);    
    topicPublisher(temp,name);
	message m;
	m.m1_i1 = 1;
	publish(getpid(), name, &m);
	if(fork() == 0)
	{
		sleep(2);
		topicSubscriber(getpid(),name);
		message *r = retrieve(getpid(), name);
		printf("%d\n", r->m1_i1);
		exit(0);
	}
	int status;
	wait(&status);
}
