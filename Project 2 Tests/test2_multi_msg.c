#include <lib.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    char* name = "topic2";
    pid_t temp = getpid();
    topicCreate(name); 
	if(fork() == 0)
	{
		sleep(3);
		topicSubscriber(getpid(),name);
		message *r;
		for(int i = 0; i < 5;i++)
		{			
			r = retrieve(getpid(), name);
			printf("%d\n", r->m1_i1);
		}
		exit(0);
	}	
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
	message m5;
	m5.m1_i1 = 5;
	publish(getpid(), name, &m5);
	int status;
	wait(&status);
}
