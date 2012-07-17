#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include  <sys/time.h>
#include  <time.h>

#include "hiredis/hiredis.h"


double microtime() {
	struct timeval tim;
	struct timezone tz;
	gettimeofday(&tim, &tz);
	//return (tim.tv_sec+(tim.tv_usec/1000000.0));
	return (1000000*tim.tv_sec+(tim.tv_usec));
}

int main()
{
	char *cptr = NULL;
	size_t thesize = 0;

	char *setName = "myset";
    
	redisReply *reply;
	
	redisContext *redis = redisConnect((char*)"127.0.0.1", 6379);	
	if (redis->err) {
		printf("Connection error: %s\n", redis->errstr);
        exit(1);
    }
	printf("Connected to redis \n");
		
	while(1) {
	    if(feof(stdin) != 0) { 
			break; 
		}
	    
	    getline(&cptr, &thesize, stdin);
		
		// hloupy odstraneni \n  TODO crlf lf cr
		// int last = (int)strlen(cptr) - 1;
		// if(last >= 0 ) {
		// 			cptr[last] = '\0';
		// }
		
		// printf("input string is %f: -------\n", microtime());
		if(strlen(cptr)) {
			printf("input string is : %s  -%i\n", cptr, (int)strlen(cptr));
 
			// nechapu
			// reply = redisCommand(redis, "ZADD %s %.0f %b", setName, microtime()/1000, cptr, strlen(cptr)-1);
			reply = redisCommand(redis, "ZADD %s 1342511726543 %b", setName, cptr, strlen(cptr)-1);
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