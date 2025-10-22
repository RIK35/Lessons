#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

std::mutex mtx;
std::condition_variable cv;  // Условная переменная для синхронизации
std::queue<int> data_queue;  // Очередь данных
const int MAX_SIZE = 5;      // Максимальный размер очереди

// Поток-производитель
void producer(int id) {
    for (int i = 0; i < 10; ++i) {
        // Создаем unique_lock для condition variable
        std::unique_lock<std::mutex> lock(mtx);
        
        // Ждем пока очередь не освободится
        // wait() временно освобождает мьютекс и блокирует поток
        // Когда очередь станет меньше MAX_SIZE, поток разблокируется
        cv.wait(lock, [] { 
            return data_queue.size() < MAX_SIZE; 
        });
        
        // Производим данные
        int data = i + id * 100;
        data_queue.push(data);
        std::cout << "Producer " << id << " produced: " << data << std::endl;
        
        // Освобождаем мьютекс перед уведомлением (хорошая практика)
        lock.unlock();
        
        // Уведомляем одного ожидающего потребителя
        cv.notify_one();
        
        // Имитируем работу
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// Поток-потребитель
void consumer(int id) {
    for (int i = 0; i < 10; ++i) {
        std::unique_lock<std::mutex> lock(mtx);
        
        // Ждем пока в очереди появятся данные
        cv.wait(lock, [] { 
            return !data_queue.empty(); 
        });
        
        // Потребляем данные
        int data = data_queue.front();
        data_queue.pop();
        std::cout << "Consumer " << id << " consumed: " << data << std::endl;
        
        lock.unlock();
        // Уведомляем одного ожидающего производителя
        cv.notify_one();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}

int main() {
    std::thread prod1(producer, 1);
    std::thread prod2(producer, 2);
    std::thread cons1(consumer, 1);
    std::thread cons2(consumer, 2);
    
    prod1.join();
    prod2.join();
    cons1.join();
    cons2.join();
    
    return 0;
}