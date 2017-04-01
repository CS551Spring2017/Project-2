#include <lib.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
	char* name = "topic7";
	if(fork()==0){
		pid_t temp = getpid();
		message m1;
	    m1.m1_i1 = 1;
	    message m2;
	    m2.m1_i1 = 2;
	    message m3;
	    m3.m1_i1 = 3;
	    message m4;
	    m4.m1_i1 = 4;
	    message m5;
	    m5.m1_i1 = 5;
	    message m6;
	    m6.m1_i1 = 6;
	    message m7;
	    m7.m1_i1 = 7;
	    topicCreate(name);
	    topicPublisher(temp,name);
	    publish(temp,name,&m1);
	    publish(temp,name,&m2);
	    publish(temp,name,&m3);
	    publish(temp,name,&m4);
	    publish(temp,name,&m5);
	    publish(temp,name,&m6);
	    publish(temp,name,&m7);
	    exit(0);
	}
    if(fork()==0){
  		topicSubscriber(getpid(),name);
  		message *r = retrieve(getpid(),name);
    	printf("%d\n", r->m1_i1);
    	exit(0);
  	}
  	sleep(5);
  	topicLookup();
  	int status;
  	wait(&status);
  	wait(&status);
}