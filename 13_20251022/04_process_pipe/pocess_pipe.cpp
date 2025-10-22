#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <cstring>

int main() {
    // Создаем pipe (канал для межпроцессного взаимодействия)
    // pipefd[0] - дескриптор для чтения
    // pipefd[1] - дескриптор для записи
    int pipefd[2];
    
    // Создаем pipe и проверяем на ошибки
    if (pipe(pipefd) == -1) {
        std::cerr << "Ошибка создания pipe" << std::endl;
        return 1;
    }

    pid_t pid = fork();

    if (pid == -1) {
        std::cerr << "Ошибка fork" << std::endl;
        return 1;
    }
    else if (pid == 0) {
        // Дочерний процесс - будет писать в pipe
        
        // Закрываем ненужный дескриптор для чтения
        close(pipefd[0]);
        
        std::string message = "Привет от дочернего процесса!";
        
        // Пишем сообщение в pipe
        // pipefd[1] - дескриптор для записи
        // message.c_str() - указатель на данные
        // message.length() + 1 - размер данных (включая нулевой байт)
        write(pipefd[1], message.c_str(), message.length() + 1);
        
        // Закрываем дескриптор записи
        close(pipefd[1]);
        
        exit(0);
    }
    else {
        // Родительский процесс - будет читать из pipe
        
        // Закрываем ненужный дескриптор для записи
        close(pipefd[1]);
        
        char buffer[256];
        
        // Читаем данные из pipe
        // pipefd[0] - дескриптор для чтения
        // buffer - буфер для данных
        // sizeof(buffer) - размер буфера
        ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer));
        
        if (bytes_read > 0) {
            std::cout << "Родитель получил сообщение: " << buffer << std::endl;
        }
        
        // Закрываем дескриптор чтения
        close(pipefd[0]);
        
        // Ожидаем завершения дочернего процесса
        wait(NULL);
    }

    return 0;
}