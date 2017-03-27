#include <lib.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char **argv) {
    char* name = "topic1";
    pid_t temp = getpid();
    printf("Test PID: %d\n", temp);
    topicCreate(name);    
    topicPublisher(temp,name);
    topicSubscriber(temp,name);
    topicLookup();
}
