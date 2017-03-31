#include <lib.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    char* name = "topic111";
    pid_t temp = getpid();
    topicCreate(name);    
    topicPublisher(temp,name);
	message m;
	m.m1_i1 = 1;
	publish(getpid(), name, &m);
	message m2;
	m2.m1_i1 = 2;
	publish(getpid(), name, &m2);
	message m3;
	m3.m1_i1 = 3;
	publish(getpid(), name, &m3);
	message m4;
	m4.m1_i1 = 4;
	publish(getpid(), name, &m4);
	if(fork() == 0)
	{
		sleep(5);
		topicSubscriber(getpid(),name);
		message *r;
		for(int i = 0; i < 7;i++)
		{			
			sleep(2);
			r = retrieve(getpid(), name);
			printf("%d\n", r->m1_i1);
		}
		exit(0);
	}
	message m5;
	m5.m1_i1 = 5;
	publish(getpid(), name, &m5);
	message m6;
	m6.m1_i1 = 6;
	publish(getpid(), name, &m6);
	sleep(1);
	message m7;
	m7.m1_i1 = 7;
	publish(getpid(), name, &m7);
	//topicLookup();
	int status;
	wait(&status);
}
