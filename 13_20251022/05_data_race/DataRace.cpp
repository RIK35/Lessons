#include <iostream>
#include <thread>
#include <vector>

int counter = 0; // Общая переменная-счетчик

void increment() {
    for (int i = 0; i < 100000; ++i) {
        counter++; // ПРОБЛЕМА: несколько потоков пытаются изменить одну переменную
    }
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);
    
    t1.join();
    t2.join();
    
    // Результат будет непредсказуем! Может быть меньше 200000
    std::cout << "Counter value: " << counter << std::endl;
    return 0;
}