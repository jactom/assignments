/* implementation of pipe IPC */

#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main()
{
    int pipefd[2];
    pid_t cpid;

    /* creating a new pipe. If failed exit programme. pipefd contains the file descriptors for read and write end of pipe */

    if(pipe(pipefd) ==-1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    /*fork a child process. Exit if child is not created */

    cpid = fork();

    if(cpid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }

    /* The Child Process execute this block*/

    if(cpid == 0){
        dup2(pipefd[0], STDIN_FILENO); /* duplicate the stdin to the read end of pipefd*/
        close(pipefd[1]); /* close the write end of pipefd */

        system("wc -l"); /* run the command operation */

        close(pipefd[0]);
        _exit(EXIT_SUCCESS); /* exit child process */

    }else{ /* parent process executes this block */
        dup2(pipefd[1], STDOUT_FILENO); /* duplicate stdout to write end of pipefd */
        close(pipefd[0]); /* close the unused read end */

        system("ls -l"); /*run system command */

        close(pipefd[1]);
        exit(EXIT_SUCCESS); /* exit process after completion */
    }

}
