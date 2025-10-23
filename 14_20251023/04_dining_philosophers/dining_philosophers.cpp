// Подключаем необходимые заголовочные файлы
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <random>
#include <string>
#include <atomic>
#include <condition_variable>
#include <memory>

using namespace std;

// Класс семафора для старых версий C++
class Semaphore {
private:
    mutex mtx;
    condition_variable cv;
    int count;
    
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

class Philosopher {
private:
    int id;
    string name;
    mutex& left_fork;
    mutex& right_fork;
    Semaphore& table_semaphore;
    atomic<int>& meals_eaten;
    atomic<bool>& running;
    random_device rd;
    mt19937 gen;
    uniform_int_distribution<> think_dist;
    uniform_int_distribution<> eat_dist;

public:
    Philosopher(int philosopher_id, 
                string philosopher_name, 
                mutex& left, 
                mutex& right, 
                Semaphore& sem,
                atomic<int>& meals, 
                atomic<bool>& run_flag)
        : id(philosopher_id),
          name(move(philosopher_name)),
          left_fork(left),
          right_fork(right),
          table_semaphore(sem),
          meals_eaten(meals),
          running(run_flag),
          gen(rd()),
          think_dist(300, 1000),
          eat_dist(200, 800)
    {
        lock_guard<mutex> lock(cout_mutex);
        cout << "Философ " << name << " (ID: " << id << ") присоединился к столу" << endl;
    }

    // Удаляем конструктор копирования и присваивания
    Philosopher(const Philosopher&) = delete;
    Philosopher& operator=(const Philosopher&) = delete;

    // Разрешаем перемещение
    Philosopher(Philosopher&&) = default;
    Philosopher& operator=(Philosopher&&) = default;

    void run() {
        while (running.load()) {
            think();
            eat();
        }
        
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Философ " << name << " закончил трапезу" << endl;
        }
    }

private:
    void think() {
        int think_time = think_dist(gen);
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Философ " << name << " размышляет " << think_time << "мс" << endl;
        }
        this_thread::sleep_for(chrono::milliseconds(think_time));
    }

    void eat() {
        table_semaphore.acquire();

        // Предотвращение дедлока - разный порядок захвата
        unique_lock<mutex> first_lock, second_lock;
        
        if (id % 2 == 1) {
            // Нечетные философы: левая, потом правая
            first_lock = unique_lock<mutex>(left_fork, defer_lock);
            second_lock = unique_lock<mutex>(right_fork, defer_lock);
        } else {
            // Четные философы: правая, потом левая
            first_lock = unique_lock<mutex>(right_fork, defer_lock);
            second_lock = unique_lock<mutex>(left_fork, defer_lock);
        }

        lock(first_lock, second_lock);

        int eat_time = eat_dist(gen);
        meals_eaten++;
        
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Философ " << name << " ест " << eat_time << "мс (" 
                 << meals_eaten.load() << " блюдо)" << endl;
        }
        
        this_thread::sleep_for(chrono::milliseconds(eat_time));
        table_semaphore.release();
    }
};

class DiningTable {
private:
    vector<mutex> forks;
    vector<thread> philosophers;
    Semaphore table_semaphore;
    atomic<int> total_meals;
    atomic<bool> running;
    const int num_philosophers;

public:
    DiningTable(int number_of_philosophers = 5) 
        : forks(number_of_philosophers),
          table_semaphore(number_of_philosophers - 1),
          total_meals(0),
          running(true),
          num_philosophers(number_of_philosophers)
    {
        lock_guard<mutex> lock(cout_mutex);
        cout << "Создан обеденный стол для " << num_philosophers 
             << " философов" << endl;
        cout << "Одновременно могут есть " << (num_philosophers - 1) 
             << " философов" << endl;
    }

    void start_dinner() {
        vector<string> names = {"Аристотель", "Платон", "Сократ", "Декарт", "Кант"};
        
        for (int i = 0; i < num_philosophers; ++i) {
            int left_fork_index = i;
            int right_fork_index = (i + 1) % num_philosophers;
            
            // Создаем философа и запускаем в отдельном потоке с помощью лямбды
            philosophers.emplace_back([this, i, left_fork_index, right_fork_index, &names]() {
                Philosopher philo(i + 1,
                                names[i % names.size()],
                                forks[left_fork_index],
                                forks[right_fork_index],  
                                table_semaphore,
                                total_meals,
                                running);
                philo.run();
            });
        }
        
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Все философы сели за стол. Обед начался!" << endl;
        }
    }

    void monitor() {
        for (int i = 0; i < 10; ++i) {
            this_thread::sleep_for(chrono::seconds(2));
            
            {
                lock_guard<mutex> lock(cout_mutex);
                cout << "=== СТАТИСТИКА: съедено " << total_meals.load() 
                     << " блюд за " << (i + 1) * 2 << " секунд ===" << endl;
            }
            
            if (total_meals.load() >= 20) {
                {
                    lock_guard<mutex> lock(cout_mutex);
                    cout << "Достигнут лимит блюд. Завершаем обед..." << endl;
                }
                break;
            }
        }
        
        stop_dinner();
    }

    void stop_dinner() {
        running.store(false);
        
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Завершаем обед..." << endl;
        }
        
        for (auto& philosopher : philosophers) {
            if (philosopher.joinable()) {
                philosopher.join();
            }
        }
        
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "=== ОБЕД ЗАВЕРШЕН ===" << endl;
            cout << "Всего съедено блюд: " << total_meals.load() << endl;
        }
    }

    ~DiningTable() {
        stop_dinner();
    }
};

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    cout << "==================================================" << endl;
    cout << "    РЕШЕНИЕ ЗАДАЧИ ОБЕДАЮЩИХ ФИЛОСОФОВ" << endl;
    cout << "    Использование семафоров и мьютексов" << endl;
    cout << "==================================================" << endl << endl;

    DiningTable table(5);
    table.start_dinner();
    table.monitor();

    cout << "Программа завершена успешно!" << endl;
    return 0;
}