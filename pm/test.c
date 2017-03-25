#include <lib.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	char* name = "topic1";
	char* name2 = "topic2";
    topicCreate(name);
	topicCreate(name2);
    topicLookup();

}
