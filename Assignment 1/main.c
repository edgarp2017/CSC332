#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>


int main(int argc, char *argv[]) {
    int fd[2], child1, child2;

    //makes the pipe
    if (pipe(fd) == -1) { //checks if error with pipe
        fprintf(stderr, ": Failed.\n");
        return 0;
    }

    //child 1, this child will run the process ls -F
    child1 = fork();

    if(child1 == 0) {
        if(dup2(fd[1], STDOUT_FILENO) == -1) {
            fprintf(stderr, ": dup2 Failed.\n");
            return -1;
        }
        //closes the pipe
        close(fd[0]);
        close(fd[1]);

        char *process[] = {"ls", "-F", 0}; //this is for execvp()

        execvp(process[0], process);

        return -1;
    }
    else if (child1 < 0) {
        fprintf(stderr, ": child1 Failed.\n");
        return -1;
    }
    
    //child 2, runs the process nl
    child2 = fork();
    if(child2 == 0) {
        if(dup2(fd[0], STDIN_FILENO) == -1) {
            fprintf(stderr, ": dup2 Failed.\n");
            return -1;
        }
        //closes the pipe
        close(fd[0]);
        close(fd[1]);

        char *process[2] = {"nl", 0};

        execvp(process[0],process);
    }
    else if (child2 < 0) {
        fprintf(stderr, ": child2 Failed.\n");
        return -1;
    }

    //closes the pipe
    close(fd[0]);
    close(fd[1]);

    //waits for children to end
    wait(NULL);
    wait(NULL);

    return 0;
}