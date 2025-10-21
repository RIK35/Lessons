#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    // Создание и запись в файл
    int fd = open("example.txt", O_CREAT | O_WRONLY, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    
    const char *text = "Hello, Linux API!\n";
    write(fd, text, strlen(text));
    close(fd);
    
    // Чтение из файла
    fd = open("example.txt", O_RDONLY);
    char buffer[256];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    buffer[bytes_read] = '\0';
    
    printf("Read from file: %s", buffer);
    close(fd);
    
    return 0;
}