#include <iostream>
#include <thread>
#include <semaphore>   // Для std::counting_semaphore
#include <vector>
#include <chrono>

// Создаем counting_semaphore с максимальным значением 3
// Семафор управляет доступом к ограниченному ресурсу
std::counting_semaphore<3> semaphore(3);  // Максимум 3 потока одновременно

void worker(int id) {
    std::cout << "Worker " << id << " waiting..." << std::endl;
    
    // acquire() уменьшает счетчик семафора
    // Если счетчик > 0, поток продолжает выполнение
    // Если счетчик = 0, поток блокируется до освобождения
    semaphore.acquire();
    
    std::cout << "Worker " << id << " started working..." << std::endl;
    
    // Имитируем работу
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    std::cout << "Worker " << id << " finished." << std::endl;
    
    // release() увеличивает счетчик семафора
    // и разблокирует ожидающие потоки
    semaphore.release();
}

int main() {
    std::vector<std::thread> workers;
    
    // Создаем 10 рабочих потоков
    for (int i = 0; i < 10; ++i) {
        workers.emplace_back(worker, i);
    }
    
    // Ждем завершения всех потоков
    for (auto& t : workers) {
        t.join();
    }
    
    return 0;
}