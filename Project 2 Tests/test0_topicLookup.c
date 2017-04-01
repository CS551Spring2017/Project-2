#include <lib.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    char* name = "topic0";
    pid_t temp = getpid();
    topicCreate(name);    
	topicLookup();
}
