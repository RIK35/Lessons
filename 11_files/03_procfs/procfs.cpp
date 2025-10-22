#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>

using namespace std;

void read_proc_filesystem() {
    // /proc - виртуальная файловая система
    // Содержит информацию о процессах и системе
    
    // Чтение информации о процессоре
    ifstream cpuinfo("/proc/cpuinfo");
    string line;
    
    cout << "Информация о процессоре:" << endl;
    for (int i = 0; i < 5 && getline(cpuinfo, line); i++) {
        cout << line << endl;
    }
    
    // Чтение информации о памяти
    ifstream meminfo("/proc/meminfo");
    cout << "\nИнформация о памяти:" << endl;
    for (int i = 0; i < 3 && getline(meminfo, line); i++) {
        cout << line << endl;
    }
    
    // Чтение информации о текущем процессе
    string pid = to_string(getpid());
    string stat_path = "/proc/" + pid + "/stat";
    
    ifstream proc_stat(stat_path);
    if (proc_stat) {
        cout << "\nИнформация о процессе " << pid << ":" << endl;
        getline(proc_stat, line);
        cout << line << endl;
    }
}

int main() {
    read_proc_filesystem();
    return 0;
}