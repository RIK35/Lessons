#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>     // Для strerror()
#include <cerrno>      // Для errno

int main() {
    pid_t pid = fork();

    if (pid == -1) {
        std::cerr << "Ошибка fork: " << strerror(errno) << std::endl;
        return 1;
    }
    else if (pid == 0) {
        // Дочерний процесс
        
        // Заменяем образ процесса новой программой
        // execlp ищет программу 'ls' в директориях PATH
        // "ls" - имя программы для выполнения
        // "ls" - первый аргумент (обычно имя программы)
        // "-l" - второй аргумент (подробный вывод)
        // "-a" - третий аргумент (показать скрытые файлы)
        // NULL - маркер конца аргументов
        execlp("ls", "ls", "-l", "-a", NULL);
        
        // Если execlp вернул управление, значит произошла ошибка
        // exec-функции возвращают управление только при ошибке
        std::cerr << "Ошибка exec: " << strerror(errno) << std::endl;
        exit(1);
    }
    else {
        // Родительский процесс
        std::cout << "Родитель ждет завершения дочернего процесса..." << std::endl;
        
        int status;
        pid_t finished_pid = wait(&status);
        
        if (finished_pid == -1) {
            std::cerr << "Ошибка wait: " << strerror(errno) << std::endl;
        }
        else {
            if (WIFEXITED(status)) {
                // WIFEXITED проверяет нормальное завершение
                std::cout << "Процесс " << finished_pid 
                          << " завершился с кодом: " << WEXITSTATUS(status) << std::endl;
            }
            else if (WIFSIGNALED(status)) {
                // WIFSIGNALED проверяет завершение по сигналу
                std::cout << "Процесс " << finished_pid 
                          << " убит сигналом: " << WTERMSIG(status) << std::endl;
            }
        }
    }

    return 0;
}