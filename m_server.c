/* sever programme for message que*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/msg.h>
#include <string.h>

typedef struct my_msgbuf { //the messge qeue struct.
    pid_t myid;
    char mtext[200];
} msgbuf;

/* the mathematical engine */

int calculate(msgbuf* ip)
{
    int a, b, d;
    char c, op[100];

    strcpy(op, ip -> mtext);
    sscanf(op, "%d %c %d", &a, &c, &b);

    switch(c){
            case '+':

            d = a + b;
            break;
        case '-':

            d = a - b;
            break;
        case '*':

            d = a * b;
            break;
        case '/':
            if(b!=0)
                d = a/b;
            else{
                printf("%d error division by zero\n", ip -> myid);
                return -1;

            }
            break;
        default:
            printf("%d error:unknown operator\n", ip -> myid);
            return -1;
            break;
    }

    printf("%d the output is: %d\n", ip -> myid, d);
    return 0;
}

int main(void)
{
    msgbuf buf; // declare a message qeue.
    int msqid;
    key_t key;

    if ((key = ftok("server.c", 'B')) == -1) {  /* same key as server.c */
        perror("ftok");
        exit(1);
    }

    if ((msqid = msgget(key, 0644 | IPC_CREAT)) == -1) { /* connect to the queue */
        perror("msgget");
        exit(1);
    }

    printf("waiting for clients \n");

    for(;;) { /*  never quits! */
        if (msgrcv(msqid, &buf, sizeof buf, 0, 0) == -1) //loading operations from the qeue.
        {
            perror("msgrcv");
            exit(1);
        }

        calculate(&buf);

   
    }

    return 0;
}
