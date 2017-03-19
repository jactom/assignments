// client program for message qeue.
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

struct my_msgbuf { //the message qeue structure.
    pid_t myid;
    char mtext[200];
};

int main(void)
{
    struct my_msgbuf buf;
    int msqid;
    key_t key;

    if ((key = ftok("server.c", 'B')) == -1) { //generating same token as server.c
        perror("ftok");
        exit(1);
    }

    if ((msqid = msgget(key, 0644 )) == -1) {
        perror("msgget");
        exit(1);
    }

    printf("Enter operations a op b:\n");//enter the operations, eg: 5 + 3

    buf.myid = getpid(); /* we don't really care in this case */

    while(fgets(buf.mtext, sizeof buf.mtext, stdin) != NULL) { //pass message to the qeue.
        int len = strlen(buf.mtext);

        /* ditch newline at end, if it exists */
        if (buf.mtext[len-1] == '\n') buf.mtext[len-1] = '\0';

        if (msgsnd(msqid, &buf, sizeof buf, 0) == -1) /* +1 for '\0' */
            perror("msgsnd");

    }


    return 0;
}
