#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int main() {
    pid_t pid;
    char *args[] = {"/bin/ls", "-l", NULL}; // Used for execv()

    printf("Parent PID: %d\n", getpid());

    pid = fork(); // create a child process

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // CHILD PROCESS
        printf("Child PID: %d. Running 'ls -l' using execv()\n", getpid());
        execv("/bin/ls", args); // replace child with ls
        perror("execv failed"); // only if execv fails
        exit(EXIT_FAILURE);
    } else {
        // PARENT PROCESS
        int status;
        wait(&status); // wait for child to complete
        printf("Child process finished.\n");

        printf("Now creating another child for exec()\n");

        pid = fork();
        if (pid == 0) {
            // Another CHILD
            execl("/bin/date", "date", NULL); // run `date`
            perror("execl failed");
            exit(EXIT_FAILURE);
        } else {
            wait(NULL);
            printf("Date printed.\n");

            // Killing a dummy process (example: itself after 3 seconds)
            printf("Creating a process to demonstrate kill().\n");
            pid = fork();
            if (pid == 0) {
                printf("Child PID %d running an infinite loop...\n", getpid());
                while (1); // Infinite loop
            } else {
                sleep(3);
                printf("Killing process %d\n", pid);
                kill(pid, SIGKILL);
                wait(NULL);
                printf("Process killed.\n");
            }
        }
    }

    return 0;
}
