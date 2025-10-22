#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>    // для exit()
#include <vector>
#include <cstdio>

using namespace std;

void process_creation_example() {
    cout << "\n=== СОЗДАНИЕ ПРОЦЕССОВ ===" << endl;
    
    // fork() - создает копию текущего процесса
    // Возвращает:
    // - 0 в дочернем процессе
    // - PID дочернего процесса в родительском
    // - -1 в случае ошибки
    
    pid_t child_pid = fork();
    
    if (child_pid == -1) {
        // Ошибка создания процесса
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (child_pid == 0) {
        // Этот код выполняется в ДОЧЕРНЕМ процессе
        cout << "Дочерний процесс: PID = " << getpid() 
             << ", PPID = " << getppid() << endl;
        
        // exec() семейство функций заменяет текущий процесс новым
        // execlp() ищет программу в PATH
        execlp("ls", "ls", "-l", "-a", nullptr);
        
        // Если execlp вернул управление - произошла ошибка
        perror("execlp failed");
        exit(EXIT_FAILURE);
    }
    else {
        // Этот код выполняется в РОДИТЕЛЬСКОМ процессе
        cout << "Родительский процесс: PID = " << getpid() 
             << ", создал дочерний с PID = " << child_pid << endl;
        
        // waitpid() - ожидает завершения дочернего процесса
        int status;
        waitpid(child_pid, &status, 0);
        
        if (WIFEXITED(status)) {
            cout << "Дочерний процесс завершился с кодом: " 
                 << WEXITSTATUS(status) << endl;
        }
    }
}

void multiple_processes_example() {
    cout << "\n=== МНОГОПРОЦЕССНОСТЬ ===" << endl;
    
    vector<pid_t> children;
    const int NUM_PROCESSES = 3;
    
    for (int i = 0; i < NUM_PROCESSES; i++) {
        pid_t pid = fork();
        
        if (pid == 0) {
            // Дочерний процесс
            cout << "Дочерний процесс " << i << " (PID: " << getpid() << ") запущен" << endl;
            sleep(2); // Имитация работы
            cout << "Дочерний процесс " << i << " завершен" << endl;
            exit(i); // Завершаем дочерний процесс
        }
        else if (pid > 0) {
            // Родительский процесс - сохраняем PID
            children.push_back(pid);
        }
        else {
            perror("fork failed");
        }
    }
    
    // Родительский процесс ожидает всех детей
    for (pid_t child_pid : children) {
        int status;
        waitpid(child_pid, &status, 0);
        cout << "Процесс " << child_pid << " завершен" << endl;
    }
}

int main() {
    process_creation_example();
    multiple_processes_example();
    return 0;
}