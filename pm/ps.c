#include <lib.h>
#include <unistd.h>
#include "pm.h"         // provides global variables such as m_in
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#define MAX_TOPICS 100
#define MAX_PUBLISHERS 100
#define MAX_SUBSCRIBERS 100

typedef struct{
    char name[128];
    message messageQueue[5];
    int full;
    pid_t subscribers[MAX_SUBSCRIBERS];
    pid_t publishers[MAX_PUBLISHERS];
    int numMsg;
    int numPubs;
    int numSubs;
}topic;

topic topics[MAX_TOPICS];
int numOfTopics = 0;

int do_publish(){
    pid_t pid = (pid_t)m_in.m1_i1;
    char name[128];
    sys_datacopy(who_e, (vir_bytes)m_in.m1_p1, SELF, (vir_bytes)name,(m_in.m1_i2 + 1)*sizeof(char));
    message *msg = malloc(sizeof(message));
    sys_datacopy(who_e, (vir_bytes)m_in.m1_p2, SELF, (vir_bytes)msg,sizeof(message));
    int foundPublisher = 0;
      for(int i=0;i<numOfTopics;i++){
        if(!strcmp(topics[i].name, name)){
          
          for(int j = 0; j < topics[i].numPubs; j++){
            if(topics[i].publishers[j] == pid){
              foundPublisher = 1;
            }
          }
          
          if(!foundPublisher){
            printf("Error: You are not a publisher of topic: %s\n", name);
            return -1;
          }
          if(topics[i].full!=1){
            for(int k =0;k<5;k++){
                if(topics[i].messageQueue[k].m_type == -1){
                    topics[i].messageQueue[k] = *msg;
                }
            }
            topics[i].numMsg++;
            if(topics[i].numMsg==5){
                 topics[i].full=1;
            }
            return 1;
          }
          else {
            printf("Error: Message buffer is full\n");
            return -1;
          }
        }
        else{
          printf("Error: %s Topic not found.\n",name);
          return -1;
        }
      }

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
        if(!strcmp(topics[i].name, name)) {
            printf("Error: Topic already exists\n");
            //printf("Please eneter a NEW topic\n");
            return -1;
        }
    }
    
    topic *newTopic = malloc(sizeof(topic));
    strcpy(newTopic->name,name);
    topics[numOfTopics++] = *newTopic;
    newTopic->numMsg = 0;
    newTopic->numPubs = 0;
    newTopic->numSubs = 0;
    newTopic->full = 0;
	memset(newTopic->messageQueue,-1,5*sizeof(message));
    return numOfTopics - 1;
}


int do_topicLookup(){
    
		printf("Total Number of topics:%d\n", numOfTopics);
    
    for(int i = 0;i < numOfTopics;i++){
        printf("ID: %d Name: %s\n",i, topics[i].name);

        printf("%d Publishers:\n",topics[i].numPubs);
        for(int j=0;j<topics[i].numPubs;j++){
            printf("Publisher PID:%d\n", topics[i].publishers[j]);
        }
        
        printf("%d Subscribers:\n",topics[i].numSubs);
        for(int k=0;k<topics[i].numSubs;k++){
            printf("Subscriber PID:%d\n", topics[i].subscribers[k]);
        }
        printf("#%d Messages in buffer\n",topics[i].numMsg);
        for(int l=0;l<5;l++){
          if(topics[i].messageQueue[l].m_type != -1){
            printf("%s\n",topics[i].messageQueue[l]);
          }
        }
    }
    return 0;
}

int do_topicPublisher(){
	pid_t proc = (pid_t)m_in.m1_i1;
    printf("Kernel passed PID: %d\n", proc);
    char name[128];
    sys_datacopy(who_e, (vir_bytes)m_in.m1_p1, SELF, (vir_bytes)name,(m_in.m1_i2 + 1)*sizeof(char));

    for(int i = 0;i<numOfTopics;i++){
        if(!strcmp(topics[i].name,name)){
            //topic found
            if(topics[i].numPubs<100){
                topics[i].publishers[topics[i].numPubs] = proc;
                topics[i].numPubs++;
            }
            else{
                //maximum number of pubs reached for topic
                printf("Error: Max number of Publishers reached\n");
                return -1;
            }
        }
        else{
            //topic not found
            printf("Error: Topic NOT found\n");
            return -1;
        }
    }
    return 0;
}

int do_topicSubscriber(){
	pid_t proc = (pid_t)m_in.m1_i1;
    char name[128];
    sys_datacopy(who_e, (vir_bytes)m_in.m1_p1, SELF, (vir_bytes)name,(m_in.m1_i2 + 1)*sizeof(char));

    for(int i = 0;i<numOfTopics;i++){
        if(!strcmp(topics[i].name,name)){
            //topic found
            if(topics[i].numSubs<100){
                topics[i].subscribers[topics[i].numSubs] = proc;
                topics[i].numSubs++;
            }
            else{
                //maximum number of pubs reached for topic
                printf("Error: Max number of subscribers reached\n");
                return -1;
            }
        }
        else{
            //topic not found
            printf("Error: Topic NOT found\n");
            return -1;
        }
    }
    return 0;
}


