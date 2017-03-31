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
	int retrieved[5][MAX_SUBSCRIBERS];
	int subIndex[MAX_SUBSCRIBERS];
    int full;
    pid_t subscribers[MAX_SUBSCRIBERS];
    pid_t publishers[MAX_PUBLISHERS];
	endpoint_t blocked[MAX_PUBLISHERS];
    int numBlocked;
	int blockedIndex;
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
  	int topicID = getTopicID(name);
    if(topicID>=0){
			for(int j = 0; j < topics[topicID].numPubs; j++){
            if(topics[topicID].publishers[j] == pid){
              foundPublisher = 1;
            }
          }
					if(!foundPublisher){
            printf("Error: You are not a publisher of topic: %s\n", name);
            return -1;
          }
          if(topics[topicID].full!=1){
            for(int k =0;k<5;k++){
                if(topics[topicID].messageQueue[k].m_type == -1){
                    topics[topicID].messageQueue[k] = *msg;
                		break;
                }
            }
            topics[topicID].numMsg++;
            if(topics[topicID].numMsg==5){
                 topics[topicID].full=1;
				 topics[topicID].blocked[topics[topicID].numBlocked++] = who_e;
				 sys_blockps(who_e,1);
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


int do_retrieve(){
  pid_t subpid = (pid_t)m_in.m1_i1; 
  char name[128];
  sys_datacopy(who_e, (vir_bytes)m_in.m1_p1, SELF, (vir_bytes)name,(m_in.m1_i2 + 1)*sizeof(char));
  //message *msg = m_in.m1_p2;
  message *msg = malloc(sizeof(message));
  int foundSubscriber = -1;
  int topicID = getTopicID(name);
  
  if(topicID>=0){
    //topic found
    for(int j = 0; j < topics[topicID].numSubs; j++) {
        if(topics[topicID].subscribers[j] == subpid){
          foundSubscriber = j;
        }
      }
      
      if(foundSubscriber == -1) {
        printf("Error: You are not a subscriber of topic: %s\n", name);
        return -1;
      }
      if(topics[topicID].numMsg <= 0) {
        // no messages
        printf("Error: No messages in topic: %s", name);
        return -1;
      }
      else {
        // get latest message
        // numMsg = 1, subIndex = 0
        if(topics[topicID].subIndex[foundSubscriber] >= topics[topicID].numMsg)
        {
          printf("Error: All messages retrieved\n");
          return -1;
        }
        *msg = topics[topicID].messageQueue[topics[topicID].subIndex[foundSubscriber]];
        topics[topicID].retrieved[topics[topicID].subIndex[foundSubscriber]][foundSubscriber] = 1;
        int allRead = 1;
        for(int k = 0; k < topics[topicID].numSubs; k++){
          if(topics[topicID].retrieved[topics[topicID].subIndex[foundSubscriber]][k] == 0){
            allRead = 0;
            break;
          }
        }
        topics[topicID].subIndex[foundSubscriber]++;
        if(allRead == 1){
          //remove message from messageQueue and shift Queue
          topics[topicID].numMsg--;
          deleteMessage(topicID);
		  if(topics[topicID].blockedIndex < topics[topicID].numBlocked)
		  {
		  sys_blockps(topics[topicID].blocked[topics[topicID].blockedIndex++], 0);
		  }
        }
		sys_datacopy(PM_PROC_NR,(vir_bytes)msg,who_e,(vir_bytes)(m_in.m1_p2), sizeof(message));
        return 0;
  }
  }
  else{
     // not found
    printf("Error: Topic NOT found\n");
    return -1; 
  }
}


int deleteMessage(int index){
	message empty;
	empty.m_type = -1;
	 topics[index].messageQueue[0] = topics[index].messageQueue[1];
	 topics[index].messageQueue[1] = topics[index].messageQueue[2];
	 topics[index].messageQueue[2] = topics[index].messageQueue[3];
	 topics[index].messageQueue[3] = topics[index].messageQueue[4];
	 topics[index].messageQueue[4] = empty;
  	for(int k=0;k<MAX_SUBSCRIBERS;k++){
       for(int l = 0;l < 4; l++){
    			topics[index].retrieved[l][k] = topics[index].retrieved[l+1][k];
         	if(l+1 == 5){
          	topics[index].retrieved[l+1][k] = 0;
        	}
       }
       topics[index].subIndex[k]--;
  	}
	topics[index].full = 0;
	return 1;
}

int getTopicID(char *name){
	  for (int i = 0; i < numOfTopics; i++){
      if(!strcmp(topics[i].name, name)){
      	return i;
        break;
      }
    }
  	return -1;
}
  

int do_topicCreate(){
    char name[128];
    sys_datacopy(who_e, (vir_bytes)m_in.m1_p1, SELF, (vir_bytes)name,(m_in.m1_i1 + 1)*sizeof(char));
	printf("Creating topic: %s\n",name);
  	int chkTopic = getTopicID(name);
  	if(chkTopic>=0){
    	printf("Error: Topic already exists\n");
      //printf("Please eneter a NEW topic\n");
      return -1;
  	}

    topic *newTopic = malloc(sizeof(topic));
    strcpy(newTopic->name,name);
    newTopic->numMsg = 0;
    newTopic->numPubs = 0;
    newTopic->numSubs = 0;
    newTopic->full = 0;
  	message *temp = malloc(sizeof(message));
    temp->m_type = -1;
    for(int j=0;j<5;j++){
    	newTopic->messageQueue[j] = *temp;
    }
    for(int k=0;k<MAX_SUBSCRIBERS;k++){
      for(int l = 0;l < 5; l++){
      	newTopic->retrieved[l][k] = 0; 
      }
      newTopic->subIndex[k] = 0;
    }
	for(int m = 0;m < MAX_PUBLISHERS; m++)
    {
        newTopic->blocked[m] = 0;
    }
    newTopic->numBlocked = 0;
	newTopic->blockedIndex = 0;
    topics[numOfTopics++] = *newTopic;
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
        for(int l=0;l<topics[i].numMsg;l++){
            printf("%d\n",topics[i].messageQueue[l].m1_i1);
        }
    }
    return 0;
}

int do_topicPublisher(){
	pid_t proc = (pid_t)m_in.m1_i1;
    char name[128];
    sys_datacopy(who_e, (vir_bytes)m_in.m1_p1, SELF, (vir_bytes)name,(m_in.m1_i2 + 1)*sizeof(char));
		int topicID = getTopicID(name);
    if(topicID>=0){
		for(int i = 0; i < topics[topicID].numPubs; i++){
			if(topics[topicID].publishers[i] == proc)
			{
				printf("Error: You are already a publisher of topic: %s", name);
				return -1;
			}
		}
      	if(topics[topicID].numPubs<MAX_PUBLISHERS){
            topics[topicID].publishers[topics[topicID].numPubs] = proc;
            topics[topicID].numPubs++;
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
  return 0;
}

int do_topicSubscriber(){
	pid_t proc = (pid_t)m_in.m1_i1;
    char name[128];
    sys_datacopy(who_e, (vir_bytes)m_in.m1_p1, SELF, (vir_bytes)name,(m_in.m1_i2 + 1)*sizeof(char));
		int topicID = getTopicID(name);
  
    if(topicID>=0){
      //topic found
      if(topics[topicID].numSubs<MAX_SUBSCRIBERS){
          topics[topicID].subscribers[topics[topicID].numSubs] = proc;
          topics[topicID].numSubs++;
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
    return 0;
}
