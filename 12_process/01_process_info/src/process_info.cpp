#include <iostream>
#include <unistd.h>   // для системных вызовов POSIX
#include <sys/wait.h> // для функций ожидания процессов

using namespace std;

void process_info_example() {
    // Каждый процесс в Linux имеет:
    // - PID (Process ID) - идентификатор процесса
    // - PPID (Parent Process ID) - идентификатор родительского процесса
    // - UID (User ID) - идентификатор пользователя
    
    cout << "=== ИНФОРМАЦИЯ О ПРОЦЕССЕ ===" << endl;
    
    // getpid() - возвращает PID текущего процесса
    pid_t current_pid = getpid();
    cout << "Текущий PID: " << current_pid << endl;
    
    // getppid() - возвращает PPID родительского процесса
    pid_t parent_pid = getppid();
    cout << "Родительский PID: " << parent_pid << endl;
    
    // getuid() - возвращает UID пользователя
    uid_t user_id = getuid();
    cout << "User ID: " << user_id << endl;
}

int main() {
    process_info_example();
    return 0;
}