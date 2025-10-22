#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mtx;
int counter = 0;

void safe_increment_auto() {
    for (int i = 0; i < 100000; ++i) {
        // Создаем lock_guard - он автоматически захватывает мьютекс при создании
        // и освобождает его при разрушении (при выходе из области видимости)
        // Это защищает от забывания вызвать unlock()
        std::lock_guard<std::mutex> lock(mtx);
        counter++;
        
        // Мьютекс автоматически освободится когда lock выйдет из области видимости
    }
    // Здесь destructor lock_guard вызовет mtx.unlock()
}

int main() {
    std::thread t1(safe_increment_auto);
    std::thread t2(safe_increment_auto);
    
    t1.join();
    t2.join();
    
    std::cout << "Auto-safe counter: " << counter << std::endl;
    return 0;
}