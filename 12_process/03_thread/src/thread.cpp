#include <iostream>
#include <pthread.h>  // POSIX threads
#include <unistd.h>
#include <cstring>

using namespace std;

// Функция, которая будет выполняться в потоке
void* thread_function(void* arg) {
    // Преобразуем аргумент обратно в int
    int thread_num = *(int*)arg;
    
    cout << "Поток " << thread_num << " запущен (ID: " 
         << pthread_self() << ")" << endl;
    
    // Имитация работы
    for (int i = 0; i < 3; i++) {
        cout << "Поток " << thread_num << " - шаг " << i << endl;
        sleep(1);
    }
    
    cout << "Поток " << thread_num << " завершен" << endl;
    return nullptr;
}

void basic_thread_example() {
    cout << "\n=== ОСНОВЫ ПОТОКОВ ===" << endl;
    
    // pthread_t - тип для идентификатора потока
    pthread_t thread1, thread2;
    int arg1 = 1, arg2 = 2;
    
    // pthread_create() - создает новый поток
    // Параметры:
    // &thread1 - указатель на переменную для хранения ID потока
    // nullptr - атрибуты потока (по умолчанию)
    // thread_function - функция для выполнения
    // &arg1 - аргумент для функции
    
    int result1 = pthread_create(&thread1, nullptr, thread_function, &arg1);
    int result2 = pthread_create(&thread2, nullptr, thread_function, &arg2);
    
    if (result1 != 0 || result2 != 0) {
        cerr << "Ошибка создания потока: " << strerror(result1) << endl;
        return;
    }
    
    // pthread_join() - ожидает завершения потока
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);
    
    cout << "Все потоки завершены" << endl;
}

void thread_attributes_example() {
    cout << "\n=== АТРИБУТЫ ПОТОКОВ ===" << endl;
    
    pthread_attr_t attr;  // Структура атрибутов потока
    pthread_t thread;
    
    // Инициализация атрибутов
    pthread_attr_init(&attr);
    
    // Установка размера стека
    size_t stack_size = 1024 * 1024; // 1 MB
    pthread_attr_setstacksize(&attr, stack_size);
    
    // Установка политики планирования
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    
    // Создание потока с атрибутами
    int thread_arg = 42;
    pthread_create(&thread, &attr, thread_function, &thread_arg);
    
    // Ожидание завершения
    pthread_join(thread, nullptr);
    
    // Освобождение ресурсов атрибутов
    pthread_attr_destroy(&attr);
}

int main() {
    basic_thread_example();
    thread_attributes_example();
    return 0;
}