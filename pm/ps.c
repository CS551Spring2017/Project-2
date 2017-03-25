#include <lib.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "pm.h"         // provides global variables such as m_in
#define MAX_TOPICS 100
#define MAX_PUBLISHERS 100
#define MAX_SUBSCRIBERS 100

typedef struct {
	char name[128];
	message messageQueue[5];
	int full;
	pid_t subscribers[MAX_SUBSCRIBERS];
	pid_t publishers[MAX_PUBLISHERS];
	int numMsg;
} topic;

topic topics[MAX_TOPICS];
int numOfTopics = 0;

int do_publish(){
	int i = m_in.m1_i1; /* m_in is a global variable set to PM's
                         * incoming message, 
                         * So m_in.m1_i1 is the integer parameter set in our
                         * test program above.
                         */
    
    printf("System call do_printmsg called with value %d\n", i);
    return 0;
}

int do_retrieve(){
	int i = m_in.m1_i1; /* m_in is a global variable set to PM's
                         * incoming message, 
                         * So m_in.m1_i1 is the integer parameter set in our
                         * test program above.
                         */
    
    printf("System call do_printmsg called with value %d\n", i);
    return 0;
}

int do_topicCreate(){
	char name[128];
	sys_datacopy(who_e, (vir_bytes)m_in.m1_p1, SELF, (vir_bytes)name,(m_in.m1_i1 + 1)*sizeof(char));
	
	for(int i = 0;i < numOfTopics;i++)
	{
		if(!strcmp(topics[i].name, name))
		{
			  printf("Topic already exists\n");
			  return -1;
		}
	}
	topic *newTopic = malloc(sizeof(topic));
	strcpy(newTopic->name,name);
	newTopic->full = 0;
	topics[numOfTopics++] = *newTopic;  
	return numOfTopics - 1;
}

int do_topicLookup(){
	printf("Topics: \n");
	for(int i = 0;i < numOfTopics;i++)
	{
		printf("ID: %d		Name: %s \n",i, topics[i].name);
	}
	return 1;
}

int do_topicPublisher(){
	int i = m_in.m1_i1; /* m_in is a global variable set to PM's
                         * incoming message, 
                         * So m_in.m1_i1 is the integer parameter set in our
                         * test program above.
                         */
    
    printf("System call do_printmsg called with value %d\n", i);
    return 0;
}

int do_topicSubscriber(){
	int i = m_in.m1_i1; /* m_in is a global variable set to PM's
                         * incoming message, 
                         * So m_in.m1_i1 is the integer parameter set in our
                         * test program above.
                         */
    
    printf("System call do_printmsg called with value %d\n", i);
    return 0;
}