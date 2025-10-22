#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <cstring>

using namespace std;

void demonstrate_file_operations() {
    // Создание и запись в файл
    int fd = open("test_file.txt", O_CREAT | O_RDWR, 0644);
    
    if (fd != -1) {
        write(fd, "Test data", 9);
        
        // lseek() - перемещает указатель позиции в файле
        // SEEK_SET - отсчет от начала файла
        lseek(fd, 0, SEEK_SET);
        
        char buffer[100];
        // read() - чтение из файла
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        buffer[bytes_read] = '\0';
        
        cout << "Прочитано: " << buffer << endl;
        close(fd);
    }
    
    // Работа с метаданными файлов
    struct stat file_info;
    if (stat("test_file.txt", &file_info) == 0) {
        cout << "Размер файла: " << file_info.st_size << " байт" << endl;
        cout << "Владелец: " << file_info.st_uid << endl;
        cout << "Права доступа: " << oct << file_info.st_mode << dec << endl;
    }
    
    // Работа с каталогами
    DIR* dir = opendir(".");
    if (dir) {
        struct dirent* entry;
        cout << "Файлы в текущем каталоге:" << endl;
        while ((entry = readdir(dir)) != nullptr) {
            cout << "  " << entry->d_name << endl;
        }
        closedir(dir);
    }
}

int main() {
    demonstrate_file_operations();
    return 0;
}