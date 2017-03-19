#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<signal.h>

#define MAX_SIZE 256
#define BACKLOG 3

int s_sock, c_sock; //file descriptors for sockets

/* signal handler */
void sig_handler(int signumber)
{
    if(signumber == SIGINT){
        printf("\nreceived CTRL-C\n Terminating....\n");
        close(s_sock);
        close(c_sock);
        exit(0);
    }
}

int main(int argc, char* argv[])
{
    if(argc != 2){// usage : ./cs <port number>
        fprintf(stderr,"usage : cs port\n");
        exit(1);
    }

    int i, numbytes;
    struct sockaddr_in server, client;
    char message_buf[MAX_SIZE];

	/* create socket */
    if((s_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("server: socket");
        exit(1);
    }

    printf("Socket created\n");

	/* clear memory to avoid undefined behaviour */
    memset(&server, '0', sizeof(server));
    memset(&message_buf, '0', sizeof(message_buf));

	/* configure socket */
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(atoi(argv[1]));

	/* binding socket */
    if(bind(s_sock, (struct sockaddr *)&server, sizeof(server)) < 0){
        perror("bind");
        exit(1);
    }

    printf("bind done\n");

	/* listening */
    listen(s_sock, BACKLOG);

	/*catch signal */
    if(signal(SIGINT, sig_handler) == SIG_ERR)
        printf("\n can't catch sigint\n");

	/* server waits for client */
    while(1){

        printf("Waiting for incoming connections...\n");
        i = sizeof(struct sockaddr_in);

		/* accept client connection */
        if((c_sock = accept(s_sock, (struct sockaddr *)&client, (socklen_t*)&i)) < 0){
            perror("accept failed");
            exit(1);
        }

        printf("Connection accepted\n");

		/* receive client messages */
        while((numbytes = recv(c_sock, message_buf, MAX_SIZE, 0)) > 0){
            message_buf[numbytes] = '\0';
            printf("%s", message_buf);
            fflush(stdout);
            memset(message_buf, '0', sizeof(message_buf));
        }

        if(numbytes == 0){// if client disconnected wait for another.
            printf("Client disconnected\n");
            fflush(stdout);
            continue;
        }

        if(numbytes == -1)
            perror("recv failed");
    }


    return 0;
}
