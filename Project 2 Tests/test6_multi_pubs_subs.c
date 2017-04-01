#include <lib.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
	char* name = "topic6";
    topicCreate(name);

    //pubs
    if(fork()==0){
    	topicPublisher(getpid(),name);
    	message m1;
    	m1.m1_i1 = 1;
    	publish(getpid(),name,&m1);
    	exit(0);
    }
    if(fork()==0){
    	topicPublisher(getpid(),name);
		message m2;
    	m2.m1_i1 = 2;
    	publish(getpid(),name,&m2);
    	exit(0);
    }

    //subs
    if(fork()==0){
    	topicSubscriber(getpid(),name);
    	message *r = retrieve(getpid(),name);
    	printf("%d\n", r->m1_i1);
    	message *s = retrieve(getpid(),name);
    	printf("%d\n", s->m1_i1);
    	
    	exit(0);
    }
    if(fork()==0){
    	topicSubscriber(getpid(),name);
    	message *d = retrieve(getpid(),name);
		printf("%d\n", d->m1_i1);
		message *e = retrieve(getpid(),name);
		printf("%d\n", e->m1_i1);
		
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