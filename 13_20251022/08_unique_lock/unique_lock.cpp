#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mtx;
int counter = 0;

void flexible_increment() {
    for (int i = 0; i < 100000; ++i) {
        // unique_lock более гибкий чем lock_guard
        // Может освобождать и снова захватывать мьютекс
        std::unique_lock<std::mutex> lock(mtx);
        counter++;
        
        // Можно временно освободить мьютекс для выполнения 
        // не-критических операций
        lock.unlock();
        
        // Выполняем какую-то работу, не требующую синхронизации
        // std::this_thread::sleep_for(std::chrono::microseconds(1));
        
        // И снова захватываем мьютекс если нужно
        lock.lock();
        // Дальнейшая работа с защищенными данными...
    }
}

int main() {
    std::thread t1(flexible_increment);
    std::thread t2(flexible_increment);
    
    t1.join();
    t2.join();
    
    std::cout << "Flexible counter: " << counter << std::endl;
    return 0;
}