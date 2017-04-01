#include <lib.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
	char* name = "topic11";
	pid_t temp = getpid();
	message m1;
	char *test = "test";
	m1.m1_p1 = test;
	
	topicCreate(name);
	topicPublisher(temp,name);
	publish(temp,name,&m1);
	
	//sub
	if(fork()==0){
		sleep(2);
		topicSubscriber(getpid(),name);
		message *r = retrieve(getpid(),name);
    	printf("%s\n", r->m1_p1);
    	exit(0);
	}

	int status;
	wait(&status);
}