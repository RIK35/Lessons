#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstring>
#include <cerrno>

using namespace std;

bool copy_file_lowlevel(const char* source, const char* destination) {
    int src_fd, dst_fd;
    struct stat src_stat;
    char buffer[4096];
    ssize_t bytes_read, bytes_written;
    
    // Открываем исходный файл для чтения
    src_fd = open(source, O_RDONLY);
    if (src_fd == -1) {
        cerr << "Ошибка открытия исходного файла: " << strerror(errno) << endl;
        return false;
    }
    
    // Получаем информацию о исходном файле
    if (fstat(src_fd, &src_stat) == -1) {
        cerr << "Ошибка получения информации о файле: " << strerror(errno) << endl;
        close(src_fd);
        return false;
    }
    
    // Создаем целевой файл с теми же правами
    dst_fd = open(destination, O_CREAT | O_WRONLY | O_TRUNC, src_stat.st_mode);
    if (dst_fd == -1) {
        cerr << "Ошибка создания целевого файла: " << strerror(errno) << endl;
        close(src_fd);
        return false;
    }
    
    // Копируем данные
    while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) {
        bytes_written = write(dst_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            cerr << "Ошибка записи в целевой файл" << endl;
            close(src_fd);
            close(dst_fd);
            return false;
        }
    }
    
    if (bytes_read == -1) {
        cerr << "Ошибка чтения исходного файла" << endl;
        close(src_fd);
        close(dst_fd);
        return false;
    }
    
    // Закрываем файлы
    close(src_fd);
    close(dst_fd);
    
    cout << "Файл успешно скопирован: " << source << " -> " << destination << endl;
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Использование: " << argv[0] << " <исходный_файл> <целевой_файл>" << endl;
        return 1;
    }
    
    if (copy_file_lowlevel(argv[1], argv[2])) {
        return 0;
    } else {
        return 1;
    }
}