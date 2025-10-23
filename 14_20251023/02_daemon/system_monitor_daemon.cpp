// Подключаем необходимые системные заголовочные файлы
#include <iostream>     // Для ввода-вывода
#include <fstream>      // Для работы с файлами
#include <string>       // Для работы со строками
#include <cstdlib>      // Для стандартных функций
#include <cstring>      // Для строковых функций
#include <csignal>      // Для обработки сигналов
#include <sys/stat.h>   // Для работы с правами файлов
#include <sys/types.h>  // Для системных типов
#include <unistd.h>     // Для POSIX API
#include <fcntl.h>      // Для управления файлами
#include <syslog.h>     // Для системного журнала
#include <chrono>       // Для работы со временем
#include <thread>       // Для работы с потоками
#include <atomic>       // Для атомарных операций

// Объявляем атомарную переменную для контроля работы демона
std::atomic<bool> daemon_running(true);

// Функция обработки сигналов
void signal_handler(int signal_number) {
    // Проверяем какой сигнал был получен
    switch(signal_number) {
        case SIGTERM:  // Сигнал завершения
        case SIGINT:   // Сигнал прерывания (Ctrl+C)
            // Записываем в системный журнал информацию о завершении
            syslog(LOG_INFO, "Получен сигнал завершения, демон останавливается");
            // Устанавливаем флаг работы в false
            daemon_running = false;
            break;
        case SIGHUP:   // Сигнал перезагрузки конфигурации
            // Записываем в журнал информацию о перезагрузке конфигурации
            syslog(LOG_INFO, "Получен сигнал перезагрузки конфигурации");
            break;
        default:
            // Для неизвестных сигналов записываем предупреждение
            syslog(LOG_WARNING, "Получен неизвестный сигнал: %d", signal_number);
            break;
    }
}

// Функция инициализации обработчиков сигналов
void setup_signal_handlers() {
    // Регистрируем обработчик для сигнала завершения
    signal(SIGTERM, signal_handler);
    // Регистрируем обработчик для сигнала прерывания
    signal(SIGINT, signal_handler);
    // Регистрируем обработчик для сигнала перезагрузки
    signal(SIGHUP, signal_handler);
}

// Функция для превращения процесса в демона
void become_daemon() {
    // Создаем дочерний процесс
    pid_t process_id = fork();
    
    // Проверяем успешность создания процесса
    if (process_id < 0) {
        // В случае ошибки выходим с кодом ошибки
        std::cerr << "Ошибка при создании дочернего процесса" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // Если это родительский процесс - завершаем его
    if (process_id > 0) {
        exit(EXIT_SUCCESS);
    }
    
    // Устанавливаем новый leader для сессии
    pid_t session_id = setsid();
    // Проверяем успешность создания сессии
    if (session_id < 0) {
        std::cerr << "Ошибка при создании сессии" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // Второй fork для предотвращения получения управляющего терминала
    process_id = fork();
    if (process_id < 0) {
        std::cerr << "Ошибка при втором fork" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (process_id > 0) {
        exit(EXIT_SUCCESS);
    }
    
    // Сбрасываем маску прав создания файлов
    umask(0);
    
    // Меняем текущую рабочую директорию на корневую
    chdir("/");
    
    // Закрываем стандартные файловые дескрипторы
    close(STDIN_FILENO);   // Закрываем стандартный ввод
    close(STDOUT_FILENO);  // Закрываем стандартный вывод
    close(STDERR_FILENO);  // Закрываем стандартный вывод ошибок
    
    // Перенаправляем стандартные дескрипторы в /dev/null
    open("/dev/null", O_RDONLY);   // stdin
    open("/dev/null", O_WRONLY);   // stdout
    open("/dev/null", O_WRONLY);   // stderr
}

// Функция для получения информации о загрузке системы
std::string get_system_load() {
    // Создаем объект для чтения файла
    std::ifstream load_file("/proc/loadavg");
    std::string load_info;
    
    // Проверяем успешность открытия файла
    if (load_file.is_open()) {
        // Читаем первую строку с информацией о загрузке
        std::getline(load_file, load_info);
        load_file.close();  // Закрываем файл
    } else {
        load_info = "Не удалось прочитать загрузку системы";
    }
    
    return load_info;  // Возвращаем строку с информацией
}

// Функция для получения информации о свободной памяти
std::string get_memory_info() {
    // Создаем объект для чтения файла информации о памяти
    std::ifstream mem_file("/proc/meminfo");
    std::string line;
    std::string memory_info;
    
    // Проверяем успешность открытия файла
    if (mem_file.is_open()) {
        // Читаем файл построчно
        while (std::getline(mem_file, line)) {
            // Ищем строки с информацией о свободной и общей памяти
            if (line.find("MemFree:") != std::string::npos || 
                line.find("MemTotal:") != std::string::npos) {
                memory_info += line + "\n";  // Добавляем строку к результату
            }
        }
        mem_file.close();  // Закрываем файл
    } else {
        memory_info = "Не удалось прочитать информацию о памяти";
    }
    
    return memory_info;  // Возвращаем строку с информацией
}

// Функция для записи информации в лог-файл
void write_to_log_file(const std::string& message) {
    // Создаем объект для записи в файл в режиме добавления
    std::ofstream log_file("/var/log/system_monitor.log", std::ios::app);
    
    // Получаем текущее время
    auto current_time = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(current_time);
    
    // Проверяем успешность открытия файла
    if (log_file.is_open()) {
        // Записываем время и сообщение в файл
        log_file << "[" << std::ctime(&time_t) << "] " << message << std::endl;
        log_file.close();  // Закрываем файл
    }
}

// Главная функция демона
void run_daemon() {
    // Счетчик циклов для демонстрации работы
    int cycle_count = 0;
    
    // Основной цикл работы демона
    while (daemon_running) {
        // Увеличиваем счетчик циклов
        cycle_count++;
        
        // Получаем информацию о системе
        std::string load_info = get_system_load();
        std::string memory_info = get_memory_info();
        
        // Формируем сообщение для лога
        std::string log_message = "Цикл " + std::to_string(cycle_count) + 
                                 " - Загрузка: " + load_info + 
                                 " Память:\n" + memory_info;
        
        // Записываем информацию в системный журнал
        syslog(LOG_INFO, "Мониторинг системы - цикл %d", cycle_count);
        
        // Записываем подробную информацию в файл лога
        write_to_log_file(log_message);
        
        // Ждем 10 секунд перед следующей итерацией
        for (int i = 0; i < 10 && daemon_running; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

// Точка входа в программу
int main(int argc, char* argv[]) {
    // Открываем соединение с системным журналом
    openlog("system_monitor_daemon", LOG_PID | LOG_NDELAY, LOG_DAEMON);
    
    // Записываем в журнал информацию о запуске демона
    syslog(LOG_INFO, "Демон мониторинга системы запускается");
    
    // Превращаем процесс в демона
    become_daemon();
    
    // Настраиваем обработчики сигналов
    setup_signal_handlers();
    
    // Запускаем основную функцию демона
    run_daemon();
    
    // Записываем в журнал информацию об остановке демона
    syslog(LOG_INFO, "Демон мониторинга системы останавливается");
    
    // Закрываем соединение с системным журналом
    closelog();
    
    // Возвращаем код успешного завершения
    return EXIT_SUCCESS;
}