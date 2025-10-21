#include <stdio.h>
#include <sys/mman.h>
#include <string.h>

int main() {
    // Создание анонимного отображения памяти
    size_t size = 4096;  // 4KB
    void *memory = mmap(NULL, size, PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (memory == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
    
    // Использование выделенной памяти
    strcpy((char*)memory, "Hello from mmap!");
    printf("Memory content: %s\n", (char*)memory);
    
    // Освобождение памяти
    munmap(memory, size);
    
    return 0;
}