// Определяем макрос для экспорта
#define BUILDING_DLL

// Подключаем заголовочный файл
#include "string_operations.h"

// Подключаем необходимые библиотеки
#include <algorithm>  // Для алгоритмов STL
#include <cctype>     // Для функций работы с символами
#include <cstring>    // Для C-строковых функций
#include <sstream>

// Реализация C-функции переворота строки
void reverse_string(char* str) {
    // Проверяем валидность указателя
    if (str == nullptr) return;
    
    // Находим длину строки
    int length = std::strlen(str);
    
    // Переворачиваем строку с двух концов
    for (int i = 0; i < length / 2; ++i) {
        // Меняем симметричные символы местами
        std::swap(str[i], str[length - i - 1]);
    }
}

// Реализация C-функции проверки палиндрома
bool is_palindrome(const char* str) {
    // Проверяем валидность указателя
    if (str == nullptr) return false;
    
    // Находим длину строки
    int length = std::strlen(str);
    
    // Сравниваем симметричные символы
    for (int i = 0; i < length / 2; ++i) {
        if (str[i] != str[length - i - 1]) {
            return false;  // Найдено несоответствие - не палиндром
        }
    }
    
    return true;  // Все символы совпали - палиндром
}

// Реализация методов класса StringProcessor

// Конструктор класса
StringProcessor::StringProcessor(const char* str) : data(str ? str : "") {}

// Метод преобразования к верхнему регистру
void StringProcessor::to_uppercase() {
    // Применяем преобразование к каждому символу
    std::transform(data.begin(), data.end(), data.begin(),
                  [](unsigned char c) { return std::toupper(c); });
}

// Метод преобразования к нижнему регистру
void StringProcessor::to_lowercase() {
    // Применяем преобразование к каждому символу
    std::transform(data.begin(), data.end(), data.begin(),
                  [](unsigned char c) { return std::tolower(c); });
}

// Метод получения текущей строки
std::string StringProcessor::get_string() const {
    return data;  // Возвращаем копию внутренней строки
}

// Метод разбиения строки по разделителю
std::vector<std::string> StringProcessor::split(char delimiter) const {
    std::vector<std::string> result;  // Вектор для результата
    std::string token;               // Временная строка для токена
    std::istringstream stream(data); // Создаем поток из строки
    
    // Читаем токены до конца строки
    while (std::getline(stream, token, delimiter)) {
        result.push_back(token);  // Добавляем токен в результат
    }
    
    return result;  // Возвращаем вектор токенов
}

// Статический метод объединения строк
std::string StringProcessor::join(const std::vector<std::string>& strings, 
                                 const std::string& delimiter) {
    std::string result;  // Результирующая строка
    
    // Объединяем все строки через разделитель
    for (size_t i = 0; i < strings.size(); ++i) {
        result += strings[i];  // Добавляем текущую строку
        
        // Добавляем разделитель, если не последняя строка
        if (i != strings.size() - 1) {
            result += delimiter;
        }
    }
    
    return result;  // Возвращаем объединенную строку
}