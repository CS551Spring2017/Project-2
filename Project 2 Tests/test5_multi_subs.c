#include <lib.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
	char* name = "topic5";
    pid_t temp = getpid();
    message m1;
    m1.m1_i1 = 1;
    message m2;
    m2.m1_i1 = 2;
    message m3;
    m3.m1_i1 = 3;
    topicCreate(name);
    topicPublisher(temp,name);
    publish(temp,name,&m1);
    publish(temp,name,&m2);
    publish(temp,name,&m3);

    if(fork() == 0){
    	topicSubscriber(getpid(),name);
    	message *r = retrieve(getpid(),name);
    	printf("%d\n", r->m1_i1);
    	message *s = retrieve(getpid(),name);
    	printf("%d\n", s->m1_i1);
    	message *t = retrieve(getpid(),name);
    	printf("%d\n", t->m1_i1);
    	exit(0);
    }
    if(fork()==0){
    	topicSubscriber(getpid(),name);
    	message *d = retrieve(getpid(),name);
		printf("%d\n", d->m1_i1);
		message *e = retrieve(getpid(),name);
		printf("%d\n", e->m1_i1);
		message *f = retrieve(getpid(),name);
		printf("%d\n", f->m1_i1);
		exit(0);
    }
    if(fork()==0){
    	topicSubscriber(getpid(),name);
    	message *a = retrieve(getpid(),name);
		printf("%d\n", a->m1_i1);
		message *b = retrieve(getpid(),name);
		printf("%d\n", b->m1_i1);
		message *c = retrieve(getpid(),name);
		printf("%d\n", c->m1_i1);
		exit(0);
    }
    if(fork()==0){
    	topicSubscriber(getpid(),name);
    	message *a = retrieve(getpid(),name);
		printf("%d\n", a->m1_i1);
		message *b = retrieve(getpid(),name);
		printf("%d\n", b->m1_i1);
		message *c = retrieve(getpid(),name);
		printf("%d\n", c->m1_i1);
		exit(0);
    }
    if(fork()==0){
    	topicSubscriber(getpid(),name);
    	message *a = retrieve(getpid(),name);
		printf("%d\n", a->m1_i1);
		message *b = retrieve(getpid(),name);
		printf("%d\n", b->m1_i1);
		message *c = retrieve(getpid(),name);
		printf("%d\n", c->m1_i1);
		exit(0);
    }
    sleep(5);
    topicLookup();
    int status;
    wait(&status);
	wait(&status);
	wait(&status);
	wait(&status);
	wait(&status);
}