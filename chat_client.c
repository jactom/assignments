/* client programme for the chat application. client connect to server and send messages*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<signal.h>

#define MAX_SIZE 256 //character buffer size

int sock; //socket file descriptor.

/* client signal handler */
void sig_handler(int signumber)
{
    char ans[2];
    if(signumber == SIGINT){
        printf("receved CTRL-C\n");
        printf("Terminate y/n :");
        scanf("%s", ans);

        if(strcmp(ans, "y") == 0){
            printf("Exiting....\n");
            close(sock);
            exit(0);
        }else{
            printf("Continuing..\n");
        }
    }
}

int main(int argc, char* argv[])
{
    if(argc != 3){ //usage is .cc <ip> <port>
        fprintf(stderr,"Usage:cc ip port\n");
        exit(1);
    }

    int c = 0;
    struct sockaddr_in server;
    char sendbuf[MAX_SIZE];

	/* create socket*/
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("server: socket");
        exit(1);
    }

    printf("Socket created\n");

	/* configure socket */
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));

	/* connect */
    if(connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0){
        perror("Connect failed.");
        exit(1);
    }

    printf("Connected\n");

	/* catch signal*/
    if(signal(SIGINT, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");

	/*client alwas run! */
    while(1){
        printf("$ ");//input terminal

        while((sendbuf[c++] = getc(stdin)) != '\n' && c < MAX_SIZE);// read the whole line
        sendbuf[c] = '\0';
        c = 0;
        if(send(sock, sendbuf, strlen(sendbuf), 0) < 0){ //send to server
            perror("Send ");
            break;
        }

    }

    close(sock);
    return 0;
}
