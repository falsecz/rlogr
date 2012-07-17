#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include  <sys/time.h>
#include  <time.h>

#include "hiredis/hiredis.h"

#include <getopt.h>

double microtime() {
	struct timeval tim;
	struct timezone tz;
	gettimeofday(&tim, &tz);
	//return (tim.tv_sec+(tim.tv_usec/1000000.0));
	return (1000000*tim.tv_sec+(tim.tv_usec));
}

char *redisHost = (char *)"127.0.0.1";
int redisPort = 6379;
char *redisSet = NULL;
int transparent = 0;

void usage(){
	printf("Redirect stdout to redis set v0.9.0\n");
	printf("\t-h redis hostname [127.0.0.1]\n");
	printf("\t-p redis port [6379]\n");
	printf("\t-s redis setname (required)\n");
	printf("\t-t transparently dump output\n");
}

void setup(int argc, char **argv) {
	int c;
	char * file;

	while ((c = getopt(argc, argv, "h:p:s:t")) != -1) {
        switch (c) {
			case 'h':
				redisHost = optarg;
				break;		
			case 'p':
				redisPort = atoi(optarg);
				break;	
			case 's':	
				redisSet = optarg;
				break;
			case 't':	
				transparent = 1;
				break;
		}
	}

	if(redisSet == NULL) {
		usage();
		exit(1);
	}
	
}
int main(int argc, char **argv)
{
	char *cptr = NULL;
	char *timestamp;
	size_t thesize = 0;
	redisReply *reply;
	
	setup(argc, argv);
		
	redisContext *redis = redisConnect(redisHost, redisPort);	
	if (redis->err) {
		printf("Connection error: %s\n", redis->errstr);
        exit(1);
    }
		
	while(1) {
	    if(feof(stdin) != 0) { 
			break; 
		}
	    
	    getline(&cptr, &thesize, stdin);
		
		if(strlen(cptr)) {
			if(transparent) {
				printf("%s", cptr);
			}
 
			sprintf(timestamp, "%.0f", microtime());

			// nechapu, proc to musim orezavat o posledni znak
			reply = redisCommand(redis, "ZADD %s %s %b", redisSet, timestamp, cptr, strlen(cptr)-1);

			if(REDIS_REPLY_INTEGER != reply->type) {
			    printf("RedisError: %s\n", reply->str);
			}
			
			freeReplyObject(reply);
		}

		free(cptr); 
		cptr =  NULL;           
	}

	getc(stdin);
    return 0;
    
}