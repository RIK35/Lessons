#include <iostream>
#include <thread>
#include <mutex>

std::recursive_mutex rec_mtx;  // Рекурсивный мьютекс

void recursive_function(int depth) {
    if (depth <= 0) return;
    
    // Может быть захвачен несколько раз одним потоком
    rec_mtx.lock();
    std::cout << "Depth " << depth << " locked by thread " 
              << std::this_thread::get_id() << std::endl;
    
    // Рекурсивный вызов - обычный мьютекс здесь заблокировал бы поток
    recursive_function(depth - 1);
    
    std::cout << "Depth " << depth << " unlocked" << std::endl;
    rec_mtx.unlock();  // Нужно столько же unlock, сколько было lock
}

int main() {
    std::thread t1(recursive_function, 3);
    std::thread t2(recursive_function, 3);
    
    t1.join();
    t2.join();
    
    return 0;
}