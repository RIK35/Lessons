#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>

int main() {
    const int NUM_PROCESSES = 3;
    std::vector<pid_t> child_pids;

    // Создаем несколько дочерних процессов
    for (int i = 0; i < NUM_PROCESSES; ++i) {
        pid_t pid = fork();
        
        if (pid == -1) {
            std::cerr << "Ошибка при создании процесса " << i << std::endl;
            continue;  // Продолжаем создавать остальные процессы
        }
        else if (pid == 0) {
            // Дочерний процесс
            std::cout << "Дочерний процесс " << i 
                      << " с PID " << getpid() << " запущен" << std::endl;
            
            // Имитируем работу процесса
            sleep(2 + i);  // Каждый процесс спит разное время
            
            std::cout << "Дочерний процесс " << i << " завершается" << std::endl;
            exit(i);  // Возвращаем номер процесса как код завершения
        }
        else {
            // Родительский процесс сохраняет PID дочернего
            child_pids.push_back(pid);
        }
    }

    // Родительский процесс ожидает завершения всех дочерних
    std::cout << "Родительский процесс (PID " << getpid() 
              << ") ожидает завершения " << child_pids.size() 
              << " дочерних процессов" << std::endl;

    for (size_t i = 0; i < child_pids.size(); ++i) {
        int status;
        // waitpid ожидает конкретный дочерний процесс
        // child_pids[i] - PID процесса, который нужно ждать
        // &status - указатель на переменную для статуса
        // 0 - опции (0 означает без специальных опций)
        pid_t finished_pid = waitpid(child_pids[i], &status, 0);
        
        if (finished_pid == -1) {
            std::cerr << "Ошибка при ожидании процесса " << child_pids[i] << std::endl;
        }
        else {
            std::cout << "Процесс " << finished_pid 
                      << " завершился с кодом: " << WEXITSTATUS(status) << std::endl;
        }
    }

    std::cout << "Все дочерние процессы завершены" << std::endl;
    return 0;
}