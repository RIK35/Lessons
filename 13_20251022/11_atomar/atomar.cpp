#include <iostream>
#include <thread>
#include <atomic>      // Для std::atomic
#include <vector>

// Атомарная переменная - операции с ней неделимы
// Не требует использования мьютексов
std::atomic<int> atomic_counter(0);

void atomic_increment() {
    for (int i = 0; i < 100000; ++i) {
        // Атомарная операция - не требует синхронизации
        //atomic_counter.fetch_add(1, std::memory_order_relaxed);
        atomic_counter++;
    }
}

int main() {
    std::thread t1(atomic_increment);
    std::thread t2(atomic_increment);
    
    t1.join();
    t2.join();
    
    // Результат всегда будет 200000, но работает быстрее чем с мьютексом
    std::cout << "Atomic counter: " << atomic_counter << std::endl;
    return 0;
}