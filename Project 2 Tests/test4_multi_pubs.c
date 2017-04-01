#include <lib.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    char* name = "topic4";
    pid_t temp = getpid();
    topicCreate(name);
	if(fork() == 0)
	{
		topicPublisher(getpid(),name);
		message m2;
		m2.m1_i1 = 2;
		publish(getpid(), name, &m2);
		exit(0);
	}
	if(fork() == 0)
	{
		sleep(2);
		topicSubscriber(getpid(),name);
		message *r = retrieve(getpid(),name);
		printf("%d\n", r->m1_i1);
		message *d = retrieve(getpid(),name);
		printf("%d\n", d->m1_i1);
		exit(0);
	}
    topicPublisher(temp,name);
	message m;
	m.m1_i1 = 1;
	publish(getpid(), name, &m);
	int status;
	wait(&status);
	wait(&status);
}
