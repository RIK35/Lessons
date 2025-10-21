#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <cstring>

using namespace std;

void demonstrate_file_locking() {
    const char* filename = "locked_file.txt";
    int fd = open(filename, O_CREAT | O_RDWR, 0644);
    
    if (fd == -1) {
        perror("open");
        return;
    }
    
    // Записываем тестовые данные
    const char* data = "Hello World!";
    write(fd, data, strlen(data));
    
    // Определяем структуру для блокировки
    struct flock lock;
    
    // Настраиваем блокировку:
    lock.l_type = F_WRLCK;   // Тип блокировки - запись
    lock.l_whence = SEEK_SET; // Отсчет от начала файла
    lock.l_start = 0;        // Начальная позиция
    lock.l_len = 5;          // Длина блокируемой области (5 байт)
    lock.l_pid = getpid();   // ID процесса
    
    // Устанавливаем блокировку
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        cout << "Не удалось установить блокировку" << endl;
    } else {
        cout << "Блокировка установлена на первые 5 байт" << endl;
        
        // Работа с заблокированной областью
        sleep(5); // Имитация работы
        
        // Снимаем блокировку
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        cout << "Блокировка снята" << endl;
    }
    
    close(fd);
}

// Пример конкурентного доступа
void concurrent_access_example() {
    pid_t pid = fork();
    
    if (pid == 0) {
        // Дочерний процесс
        int fd = open("test_lock.txt", O_CREAT | O_RDWR, 0644);
        struct flock lock;
        
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 10;
        
        cout << "Дочерний процесс пытается получить блокировку..." << endl;
        if (fcntl(fd, F_SETLKW, &lock) != -1) { // F_SETLKW - ждет блокировку
            cout << "Дочерний процесс получил блокировку" << endl;
            sleep(2);
            lock.l_type = F_UNLCK;
            fcntl(fd, F_SETLK, &lock);
        }
        close(fd);
    } else {
        // Родительский процесс
        sleep(1); // Даем дочернему процессу начать первым
        int fd = open("test_lock.txt", O_CREAT | O_RDWR, 0644);
        struct flock lock;
        
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 10;
        
        cout << "Родительский процесс проверяет блокировку..." << endl;
        if (fcntl(fd, F_GETLK, &lock) != -1) {
            if (lock.l_type == F_UNLCK) {
                cout << "Область не заблокирована" << endl;
            } else {
                cout << "Область заблокирована процессом " << lock.l_pid << endl;
            }
        }
        close(fd);
    }
}

int main() {
    cout << "=== Демонстрация блокировки файлов ===" << endl;
    demonstrate_file_locking();
    
    cout << "\n=== Демонстрация конкурентного доступа ===" << endl;
    concurrent_access_example();
    
    return 0;
}