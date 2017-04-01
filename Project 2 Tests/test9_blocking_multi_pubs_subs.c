#include <lib.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
	char* name = "topic9";
	topicCreate(name);

	//pubs
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

	    topicPublisher(temp,name);
	    publish(temp,name,&m1);
	    publish(temp,name,&m2);
	    publish(temp,name,&m3);
	    publish(temp,name,&m4);
	    printf("First Publisher is Back\n");
	    exit(0);
	}

	if(fork()==0){
		pid_t temp2 = getpid();
		message m1;
	    m1.m1_i1 = 8;
	    message m2;
	    m2.m1_i1 = 9;
	    message m3;
	    m3.m1_i1 = 10;
	    message m4;
	    m4.m1_i1 = 11;
	    message m5;

	    topicPublisher(temp2,name);
	    publish(temp2,name,&m1);
	    publish(temp2,name,&m2);
	    publish(temp2,name,&m3);
	    publish(temp2,name,&m4);

	    printf("Second Publisher is Back\n");
	    exit(0);
	}

	//subs
	if(fork()==0){
		sleep(2);
		topicSubscriber(getpid(),name);
  		message *r = retrieve(getpid(),name);
    	printf("%d\n", r->m1_i1);
    	exit(0);
	}
	if(fork()==0){
		sleep(2);
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
	wait(&status);
	wait(&status);

}