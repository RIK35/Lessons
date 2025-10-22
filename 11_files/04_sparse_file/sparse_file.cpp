#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

using namespace std;

void create_sparse_file() {
    // Разреженный файл - файл, в котором пустые блоки не занимают места на диске
    
    const char* filename = "sparse_file.bin";
    
    // Создаем разреженный файл
    int fd = open(filename, O_CREAT | O_WRONLY, 0644);
    
    if (fd == -1) {
        perror("open");
        return;
    }
    
    // Записываем данные в начало файла
    const char* start_data = "START";
    write(fd, start_data, strlen(start_data));
    
    // Перемещаемся на большую позицию (создаем "дыру" в файле)
    off_t offset = 1024 * 1024; // 1 МБ
    lseek(fd, offset, SEEK_CUR);
    
    // Записываем данные в конец файла
    const char* end_data = "END";
    write(fd, end_data, strlen(end_data));
    
    close(fd);
    
    // Проверяем размер файла
    struct stat st;
    stat(filename, &st);
    
    cout << "Размер файла: " << st.st_size << " байт" << endl;
    cout << "Занимаемое место: " << st.st_blocks * 512 << " байт" << endl;
    cout << "Файл разреженный: " << (st.st_blocks * 512 < st.st_size ? "Да" : "Нет") << endl;
}

int main() {
    create_sparse_file();
    return 0;
}