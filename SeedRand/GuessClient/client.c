#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;

    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    }

    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    }

        int rand_received = 0;
        int test_rand = 0;
        int guess_seed = 0;
        int i = 0;

    while (1)
    {
        n = read(sockfd, recvBuff, sizeof(recvBuff)-1);

        recvBuff[n] = 0;

        rand_received = atoi(recvBuff);

        printf("received %d \r\n", rand_received);

        guess_seed = time(NULL) - 1000;

        for (i=0; i<11000; i++)
        {
                srand(guess_seed + i);
                test_rand = (int)rand();

                if (test_rand == rand_received)
                {
                        printf("Found seed: %d\r\n\r\n", guess_seed);
                        break;
                }
        }
    }

    if(n < 0)
    {
        printf("\n Read error \n");
    }

    return 0;
}

