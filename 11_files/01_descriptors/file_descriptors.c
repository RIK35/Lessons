#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <cerrno>

using namespace std;

int main() {
    // Файловый дескриптор - целое число, представляющее открытый файл
    int file_descriptor; 
    
    // open() - системный вызов для открытия/создания файла
    // Параметры:
    // "example.txt" - имя файла
    // O_CREAT | O_WRONLY - флаги:
    //   O_CREAT - создать файл если не существует
    //   O_WRONLY - открыть только для записи
    // 0644 - права доступа (rw-r--r--)
    file_descriptor = open("example.txt", O_CREAT | O_WRONLY, 0644);
    
    if (file_descriptor == -1) {
        // errno - глобальная переменная, содержащая код ошибки
        // strerror - преобразует код ошибки в читаемое сообщение
        cerr << "Ошибка открытия файла: " << strerror(errno) << endl;
        return 1;
    }
    
    const char* message = "Hello, File Descriptors!\n";
    
    // write() - системный вызов для записи в файл
    // Параметры:
    // file_descriptor - дескриптор файла
    // message - указатель на данные для записи
    // strlen(message) - количество байт для записи
    ssize_t bytes_written = write(file_descriptor, message, strlen(message));
    
    if (bytes_written == -1) {
        cerr << "Ошибка записи: " << strerror(errno) << endl;
    }
    
    // close() - закрывает файловый дескриптор
    close(file_descriptor);
    
    return 0;
}