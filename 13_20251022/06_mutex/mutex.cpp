#include <iostream>
#include <thread>
#include <mutex>       // Для std::mutex
#include <vector>

// Создаем глобальный мьютекс
// Мьютекс - это примитив синхронизации, который обеспечивает 
// взаимное исключение (mutual exclusion)
std::mutex mtx;        
int counter = 0;

void safe_increment() {
    for (int i = 0; i < 100000; ++i) {
        // Захватываем мьютекс перед доступом к общей переменной
        // lock() блокирует мьютекс. Если он уже заблокирован другим потоком,
        // текущий поток будет ждать его освобождения
        mtx.lock();
        
        // Критическая секция - код, который выполняется только одним потоком
        counter++;
        
        // Освобождаем мьютекс, чтобы другие потоки могли его захватить
        mtx.unlock();
    }
}

int main() {
    std::thread t1(safe_increment);
    std::thread t2(safe_increment);
    
    t1.join();
    t2.join();
    
    // Теперь результат всегда будет 200000
    std::cout << "Safe counter value: " << counter << std::endl;
    return 0;
}