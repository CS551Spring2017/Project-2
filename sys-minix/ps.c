#include <lib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int publish(pid_t pid, char* name, void *msg) {
    message m;      // Minix message to pass parameters to a system call
    
    m.m1_i1 = (int)pid;  // set first integer of message to val
    m.m1_p1 = name;
	m.m1_i2 = strlen(name);
  	m.m1_p2 = msg;
  
    return _syscall(PM_PROC_NR, PUBLISH, &m);  // invoke underlying system call
}

int retrieve(int val) {
    message m;      // Minix message to pass parameters to a system call
    
    m.m1_i1 = val;  // set first integer of message to val
    
    return _syscall(PM_PROC_NR, RETRIEVE, &m);  // invoke underlying system call
}

int topicCreate(char* name) {
    message m;      // Minix message to pass parameters to a system call
    
    m.m1_p1 = name;
    m.m1_i1 = strlen(name);
    return _syscall(PM_PROC_NR, TOPICCREATE, &m);  // invoke underlying system call
}

int topicLookup() {
    message m;      // Minix message to pass parameters to a system call
    
    
    return _syscall(PM_PROC_NR, TOPICLOOKUP, &m);  // invoke underlying system call
}

int topicPublisher(pid_t proc, char* name) {
    message m;      // Minix message to pass parameters to a system call
    
    m.m1_i1 = (int)proc;
    m.m1_p1 = name;
    m.m1_i2 = strlen(name);
    return _syscall(PM_PROC_NR, TOPICPUBLISHER, &m);  // invoke underlying system call
}

int topicSubscriber(pid_t proc, char* name) {
    message m;      // Minix message to pass parameters to a system call
    
    m.m1_i1 = (int)proc;
    m.m1_p1 = name;
    m.m1_i2 = strlen(name);
    return _syscall(PM_PROC_NR, TOPICSUBSCRIBER, &m);  // invoke underlying system call
}