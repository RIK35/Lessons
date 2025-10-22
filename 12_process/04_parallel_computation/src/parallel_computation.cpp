#include <iostream>
#include <pthread.h>
#include <vector>
#include <queue>
#include <unistd.h>

using namespace std;

// Общие данные для потоков
struct SharedData {
    queue<int> task_queue;
    int counter = 0;
    bool done = false;
};

// Функция производителя
void* producer_function(void* arg) {
    SharedData* data = (SharedData*)arg;
    
    for (int i = 0; i < 10; i++) {
        data->task_queue.push(i);
        data->counter++;
        cout << "Производитель: создана задача " << i << endl;
        sleep(1); // Имитация работы
    }
    
    data->done = true;
    cout << "Производитель завершил работу" << endl;
    return nullptr;
}

// Функция потребителя
void* consumer_function(void* arg) {
    SharedData* data = (SharedData*)arg;
    
    while (!data->done || !data->task_queue.empty()) {
        if (!data->task_queue.empty()) {
            int task = data->task_queue.front();
            data->task_queue.pop();
            cout << "Потребитель: обработана задача " << task << endl;
        }
        usleep(500000); // 0.5 секунды
    }
    
    cout << "Потребитель завершил работу" << endl;
    return nullptr;
}

void producer_consumer_example() {
    cout << "\n=== ПРОИЗВОДИТЕЛЬ-ПОТРЕБИТЕЛЬ ===" << endl;
    
    SharedData shared_data;
    pthread_t producer, consumer;
    
    // Создаем потоки
    pthread_create(&producer, nullptr, producer_function, &shared_data);
    pthread_create(&consumer, nullptr, consumer_function, &shared_data);
    
    // Ожидаем завершения
    pthread_join(producer, nullptr);
    pthread_join(consumer, nullptr);
    
    cout << "Итоговое значение счетчика: " << shared_data.counter << endl;
}

// Параллельные вычисления
void* calculation_function(void* arg) {
    int thread_num = *(int*)arg;
    long long sum = 0;
    
    // Каждый поток вычисляет свою часть суммы
    int start = thread_num * 1000000 + 1;
    int end = (thread_num + 1) * 1000000;
    
    for (int i = start; i <= end; i++) {
        sum += i;
    }
    
    cout << "Поток " << thread_num << ": сумма от " << start 
         << " до " << end << " = " << sum << endl;
    
    // Возвращаем результат через указатель
    long long* result = new long long(sum);
    return result;
}

void parallel_computation_example() {
    cout << "\n=== ПАРАЛЛЕЛЬНЫЕ ВЫЧИСЛЕНИЯ ===" << endl;
    
    const int NUM_THREADS = 4;
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];
    long long total_sum = 0;
    
    // Создаем потоки для вычислений
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i;
        pthread_create(&threads[i], nullptr, calculation_function, &thread_args[i]);
    }
    
    // Собираем результаты
    for (int i = 0; i < NUM_THREADS; i++) {
        void* result_ptr;
        pthread_join(threads[i], &result_ptr);
        
        long long* partial_sum = (long long*)result_ptr;
        total_sum += *partial_sum;
        delete partial_sum;
    }
    
    cout << "Общая сумма: " << total_sum << endl;
}

int main() {
    producer_consumer_example();
    parallel_computation_example();
    return 0;
}