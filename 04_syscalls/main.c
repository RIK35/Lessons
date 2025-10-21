#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    
    if (pid == 0) {
        // Дочерний процесс
        printf("Child process: PID = %d, Parent PID = %d\n", 
               getpid(), getppid());
        _exit(0);
    } else {
        // Родительский процесс
        printf("Parent process: PID = %d, Child PID = %d\n", 
               getpid(), pid);
        waitpid(pid, NULL, 0);
    }
    return 0;
}