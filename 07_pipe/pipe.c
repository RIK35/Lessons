#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    char buffer[256];
    
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // Дочерний процесс - запись в канал
        close(pipefd[0]);  // закрываем чтение
        
        const char *message = "Hello from child process!";
        write(pipefd[1], message, strlen(message) + 1);
        close(pipefd[1]);
        _exit(0);
    } else {
        // Родительский процесс - чтение из канала
        close(pipefd[1]);  // закрываем запись
        
        ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer));
        printf("Received from child: %s\n", buffer);
        close(pipefd[0]);
        waitpid(pid, NULL, 0);
    }
    
    return 0;
}