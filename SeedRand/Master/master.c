#include "stdlib.h"
#include "stdio.h"
#include <time.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>


int main(int argc, char** argv[])
{
        int seed = 0;
        int random_number = 0;

        int listenfd = 0, connfd = 0;
        struct sockaddr_in serv_addr;

        char sendBuff[125];
        time_t ticks;

        listenfd = socket(AF_INET, SOCK_STREAM, 0);
        memset(&serv_addr, '0', sizeof(serv_addr));
        memset(sendBuff, '0', sizeof(sendBuff));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        serv_addr.sin_port = htons(5000);

        bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

        listen(listenfd, 10);

        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

        while (1)
        {
                seed = time(NULL);

                srand(seed);

                random_number = (int)rand();

                printf("Seed is: %d\n", seed);

                printf("Random number is: %d\n", random_number);

                memset(sendBuff, '0', sizeof(sendBuff));
                snprintf(sendBuff, sizeof(sendBuff), "%d", random_number);
                write(connfd, sendBuff, strlen(sendBuff));

                sleep(1);
        }

        close(connfd);

        return 0;
}
