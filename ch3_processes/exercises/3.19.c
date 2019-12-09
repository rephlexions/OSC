#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(void)
{
    char write_msg[BUFFER_SIZE] = "Hi there";
    char read_msg[BUFFER_SIZE];
    char upper_msg[BUFFER_SIZE];
    // pipe one
    int send[2];
    // pipe two
    int receive[2];
    pid_t pid;

    /* create a pipes */
    if (pipe(send) == -1)
    {
        fprintf(stderr, "Pipe failed");
        return 1;
    }

    if (pipe(receive) == -1)
    {
        fprintf(stderr, "Pipe failed");
        return 1;
    }

    /* fork a child process */
    pid = fork();

    if (pid < 0)
    { /* error occured */
        fprintf(stderr, "Fork failed");
        return 1;
    }

    if (pid > 0)
    { /* parent process */
        /* close the unused end of the pipe */
        close(send[READ_END]);

        /* write to pipe one */
        write(send[WRITE_END], write_msg, strlen(write_msg) + 1);
        printf("write %s\n", write_msg);
        /* close the write end of the pipe */
        close(send[WRITE_END]);

        wait(NULL);

        /* close the unused end of pipe one */
        close(receive[WRITE_END]);

        /* read from pipe two */
        read(receive[READ_END], upper_msg, BUFFER_SIZE);
        printf("read %s\n", upper_msg);

        /* close the read end of pipe two */
        close(receive[READ_END]);
    }
    else
    { /* child process */
        /* close the unused end of pipe one */
        close(send[WRITE_END]);

        /* read from pipe one */
        read(send[READ_END], read_msg, BUFFER_SIZE);
        printf("read %s\n", read_msg);
        /* close the read end of pipe one */
        close(send[READ_END]);

        // string manipulation
        for (int i = 0; read_msg[i] != '\0'; i++)
        {
            if (read_msg[i] >= 'a' && read_msg[i] <= 'z')
            {
                read_msg[i] = read_msg[i] - 32;
            }
        }

        /* write to pipe two */
        close(receive[READ_END]);
        write(receive[WRITE_END], read_msg, strlen(read_msg) + 1);
        printf("write %s\n", read_msg);
        /* close the write end of pipe two */
        close(receive[WRITE_END]);

        exit(1);
    }

    return 0;
}
