// Подключаем необходимые заголовочные файлы
#include <iostream>      // Для ввода-вывода в консоль (cout, endl)
#include <thread>        // Для работы с потоками (thread, this_thread)
#include <vector>        // Для использования динамического массива (vector)
#include <mutex>         // Для мьютексов (mutex, lock_guard, unique_lock)
#include <chrono>        // Для работы со временем (seconds, milliseconds)
#include <random>        // Для генерации случайных чисел (random_device, mt19937)
#include <string>        // Для работы со строками (string)
#include <atomic>        // Для атомарных операций (atomic)
#include <condition_variable> // Для condition_variable (используется в семафоре)

// Используем стандартное пространство имен для упрощения кода
using namespace std;

// Класс семафора для старых версий C++ (замена counting_semaphore)
class Semaphore {
private:
    mutex mtx;                   // Мьютекс для синхронизации
    condition_variable cv;       // Условная переменная для ожидания
    int count;                   // Счетчик семафора
    
public:
    Semaphore(int initial = 1) : count(initial) {}
    
    void acquire() {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this]() { return count > 0; });
        count--;
    }
    
    void release() {
        lock_guard<mutex> lock(mtx);
        count++;
        cv.notify_one();
    }
};

// Статический мьютекс для синхронизации вывода
static mutex cout_mutex;

// Класс представляющий философа
class Philosopher {
private:
    int id;                             // Уникальный идентификатор философа
    string name;                        // Имя философа
    mutex& left_fork;                   // Ссылка на левую вилку (мьютекс)
    mutex& right_fork;                  // Ссылка на правую вилку (мьютекс)
    Semaphore& table_semaphore;         // Ссылка на семафор стола
    atomic<int>& meals_eaten;           // Ссылка на счетчик съеденных блюд
    atomic<bool>& running;              // Флаг продолжения работы

    // Генератор случайных чисел для имитации времени размышлений и еды
    random_device rd;                   // Устройство для получения случайного seed
    mt19937 gen;                        // Генератор случайных чисел
    uniform_int_distribution<> think_dist;  // Распределение для времени размышлений
    uniform_int_distribution<> eat_dist;    // Распределение для времени еды

public:
    // Конструктор класса Philosopher
    Philosopher(int philosopher_id, 
                string philosopher_name, 
                mutex& left, 
                mutex& right, 
                Semaphore& sem, 
                atomic<int>& meals, 
                atomic<bool>& run_flag)
        : id(philosopher_id),                    // Инициализация ID
          name(move(philosopher_name)),          // Инициализация имени (move для эффективности)
          left_fork(left),                       // Инициализация ссылки на левую вилку
          right_fork(right),                     // Инициализация ссылки на правую вилку
          table_semaphore(sem),                  // Инициализация ссылки на семафор
          meals_eaten(meals),                    // Инициализация ссылки на счетчик блюд
          running(run_flag),                     // Инициализация ссылки на флаг работы
          gen(rd()),                             // Инициализация генератора случайных чисел
          think_dist(300, 1000),                // Время размышлений: 0.3-1 секунды
          eat_dist(200, 800)                    // Время еды: 0.2-0.8 секунды
    {
        // Выводим сообщение о создании философа
        lock_guard<mutex> lock(cout_mutex);
        cout << "Философ " << name << " (ID: " << id << ") присоединился к столу" << endl;
    }

    // Удаляем конструктор копирования и присваивания для безопасности потоков
    Philosopher(const Philosopher&) = delete;
    Philosopher& operator=(const Philosopher&) = delete;

    // Метод который выполняется в отдельном потоке
    void run() {
        // Цикл пока установлен флаг running
        while (running.load()) {
            think();    // Философ размышляет
            eat();      // Философ пытается поесть
        }
        
        // Сообщение когда философ завершает работу
        lock_guard<mutex> lock(cout_mutex);
        cout << "Философ " << name << " закончил трапезу" << endl;
    }

private:
    // Метод имитации размышлений философа
    void think() {
        // Генерируем случайное время размышлений
        int think_time = think_dist(gen);
        
        // Выводим сообщение о начале размышлений
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Философ " << name << " размышляет " << think_time << "мс" << endl;
        }
        
        // Останавливаем поток на время размышлений
        this_thread::sleep_for(chrono::milliseconds(think_time));
    }

    // Метод имитации приема пищи философа
    void eat() {
        // ЗАХВАТЫВАЕМ семафор стола (уменьшаем счетчик на 1)
        // Если семафор = 0, поток БЛОКИРУЕТСЯ пока место не освободится
        table_semaphore.acquire();

        // Пытаемся взять вилки в правильном порядке чтобы избежать дедлока
        // Философы с нечетными ID берут левую потом правую, с четными - наоборот
        
        // Берем вилки в зависимости от ID философа
        unique_lock<mutex> first_lock, second_lock;
        
        if (id % 2 == 1) {
            // Нечетные философы: левая, потом правая
            first_lock = unique_lock<mutex>(left_fork, defer_lock);  // defer_lock - не захватывать сразу
            second_lock = unique_lock<mutex>(right_fork, defer_lock);
        } else {
            // Четные философы: правая, потом левая (обратный порядок для предотвращения дедлока)
            first_lock = unique_lock<mutex>(right_fork, defer_lock);
            second_lock = unique_lock<mutex>(left_fork, defer_lock);
        }

        // Атомарно захватываем ОБЕ вилки одновременно
        // Это предотвращает deadlock (взаимную блокировку)
        lock(first_lock, second_lock);

        // В этот момент философ держит ОБЕ вилки
        // Это КРИТИЧЕСКАЯ СЕКЦИЯ - доступ к разделяемым ресурсам

        // Генерируем случайное время еды
        int eat_time = eat_dist(gen);
        
        // Увеличиваем счетчик съеденных блюд
        meals_eaten++;
        
        // Выводим сообщение о начале еды
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Философ " << name << " ест " << eat_time << "мс (" 
                 << meals_eaten.load() << " блюдо)" << endl;
        }
        
        // Имитируем процесс еды
        this_thread::sleep_for(chrono::milliseconds(eat_time));

        // Вилки автоматически освобождаются при разрушении unique_lock
        
        // ОСВОБОЖДАЕМ семафор стола (увеличиваем счетчик на 1)
        // Это позволяет другому философу сесть за стол
        table_semaphore.release();
    }
};

// Класс для управления обеденным столом
class DiningTable {
private:
    vector<mutex> forks;                    // Вектор вилок (мьютексов)
    vector<thread> philosophers;            // Вектор потоков философов
    Semaphore table_semaphore;              // Семафор стола (максимум 4 философа из 5 могут есть одновременно)
    atomic<int> total_meals;                // Общий счетчик съеденных блюд
    atomic<bool> running;                   // Флаг продолжения работы
    const int num_philosophers;             // Количество философов

public:
    // Конструктор класса DiningTable
    DiningTable(int number_of_philosophers = 5) 
        : forks(number_of_philosophers),           // Создаем N вилок-мьютексов
          table_semaphore(number_of_philosophers - 1), // Семафор на N-1 (предотвращает дедлок)
          total_meals(0),                          // Инициализируем счетчик блюд
          running(true),                           // Устанавливаем флаг работы в true
          num_philosophers(number_of_philosophers) // Сохраняем количество философов
    {
        // Выводим информацию о создании стола
        lock_guard<mutex> lock(cout_mutex);
        cout << "Создан обеденный стол для " << num_philosophers 
             << " философов" << endl;
        cout << "Одновременно могут есть " << (num_philosophers - 1) 
             << " философов" << endl;
    }

    // Метод запуска обеда философов
    void start_dinner() {
        // Массив имен философов
        vector<string> names = {"Аристотель", "Платон", "Сократ", "Декарт", "Кант"};
        
        // Создаем и запускаем потоки для каждого философа
        for (int i = 0; i < num_philosophers; ++i) {
            // Определяем индексы вилок для текущего философа
            int left_fork_index = i;                               // Левая вилка - с тем же индексом
            int right_fork_index = (i + 1) % num_philosophers;     // Правая вилка - следующий индекс по кругу
            
            // Создаем философа и добавляем его поток в вектор с помощью лямбда-функции
            philosophers.emplace_back([this, i, left_fork_index, right_fork_index, &names]() {
                Philosopher philo(i + 1,                                // ID философа
                           names[i % names.size()],                     // Имя философа (защита от выхода за границы)
                           forks[left_fork_index],                // Левая вилка
                           forks[right_fork_index],               // Правая вилка  
                           table_semaphore,                       // Семафор стола
                           total_meals,                           // Счетчик блюд
                           running                               // Флаг работы
                );
                philo.run();  // Запускаем выполнение философа
            });
        }
        
        // Выводим сообщение о начале обеда
        lock_guard<mutex> lock(cout_mutex);
        cout << "Все философы сели за стол. Обед начался!" << endl;
    }

    // Метод для работы в основном потоке (мониторинг и управление)
    void monitor() {
        // Ждем некоторое время чтобы философы поели
        for (int i = 0; i < 10; ++i) {
            // Ждем 2 секунды
            this_thread::sleep_for(chrono::seconds(2));
            
            // Выводим текущую статистику
            {
                lock_guard<mutex> lock(cout_mutex);
                cout << "=== СТАТИСТИКА: съедено " << total_meals.load() 
                     << " блюд за " << (i + 1) * 2 << " секунд ===" << endl;
            }
            
            // Проверяем не пора ли завершать обед
            if (total_meals.load() >= 20) {
                lock_guard<mutex> lock(cout_mutex);
                cout << "Достигнут лимит блюд. Завершаем обед..." << endl;
                break;
            }
        }
        
        // Сигнализируем философам о завершении
        stop_dinner();
    }

    // Метод остановки обеда
    void stop_dinner() {
        // Устанавливаем флаг running в false
        running.store(false);
        
        // Выводим сообщение о завершении
        lock_guard<mutex> lock(cout_mutex);
        cout << "Завершаем обед..." << endl;
        
        // Ожидаем завершения всех потоков философов
        for (auto& philosopher : philosophers) {
            if (philosopher.joinable()) {
                philosopher.join();  // Блокируем текущий поток до завершения философа
            }
        }
        
        // Выводим итоговую статистику
        cout << "=== ОБЕД ЗАВЕРШЕН ===" << endl;
        cout << "Всего съедено блюд: " << total_meals.load() << endl;
    }

    // Деструктор класса
    ~DiningTable() {
        // Гарантируем что все потоки завершены
        stop_dinner();
    }
};

// Главная функция программы
int main() {
    // Устанавливаем локаль для корректного вывода русских символов
    setlocale(LC_ALL, "ru_RU.UTF-8");

    cout << "==================================================" << endl;
    cout << "    РЕШЕНИЕ ЗАДАЧИ ОБЕДАЮЩИХ ФИЛОСОФОВ" << endl;
    cout << "    Использование семафоров и мьютексов" << endl;
    cout << "==================================================" << endl << endl;

    // Создаем обеденный стол на 5 философов
    DiningTable table(5);

    // Запускаем обед
    table.start_dinner();

    // Запускаем мониторинг и управление
    table.monitor();

    cout << "Программа завершена успешно!" << endl;
    return 0;  // Успешное завершение программы
}