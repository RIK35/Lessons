#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

void list_directory(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return;
    }
    
    struct dirent *entry;
    struct stat file_stat;
    char full_path[1024];
    
    printf("Contents of directory: %s\n", path);
    printf("----------------------------------------\n");
    
    while ((entry = readdir(dir)) != NULL) {
        // Пропускаем текущую и родительскую директории
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
            
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        
        if (stat(full_path, &file_stat) == 0) {
            printf("%s", entry->d_name);
            
            if (S_ISDIR(file_stat.st_mode)) {
                printf(" (directory)");
            } else if (S_ISREG(file_stat.st_mode)) {
                printf(" (file, size: %ld bytes)", file_stat.st_size);
            } else if (S_ISLNK(file_stat.st_mode)) {
                printf(" (symlink)");
            }
            
            printf("\n");
        }
    }
    
    closedir(dir);
}

int main(int argc, char *argv[]) {
    const char *path = (argc > 1) ? argv[1] : ".";
    list_directory(path);
    return 0;
}