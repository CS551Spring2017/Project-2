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
	m.m1_i1 = 69;
	publish(getpid(), name, &m);
	message m2;
	m2.m1_i1 = 12;
	publish(getpid(), name, &m2);
	if(fork() == 0)
	{
		topicSubscriber(getpid(),name);
		message *r = retrieve(getpid(), name);
		printf("%d\n", r->m1_i1);
		//sleep(2);
		message *d = retrieve(getpid(), name);
		printf("%d\n", d->m1_i1);
		exit(0);
	}
	if(fork() == 0)
	{
		topicSubscriber(getpid(),name);
		message *r = retrieve(getpid(), name);
		printf("%d\n", r->m1_i1);
		//sleep(2);
		message *d = retrieve(getpid(), name);
		printf("%d\n", d->m1_i1);
		exit(0);
	}
	int status;
	wait(&status);
	wait(&status);
}
