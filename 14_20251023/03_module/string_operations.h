#ifndef STRING_OPERATIONS_H
#define STRING_OPERATIONS_H

#include <string>   // Для работы со строками
#include <vector>   // Для работы с векторами

// Макрос для экспорта символов библиотеки
#ifdef _WIN32
    #ifdef BUILDING_DLL
        #define DLL_EXPORT __declspec(dllexport)
    #else
        #define DLL_EXPORT __declspec(dllimport)
    #endif
#else
    #define DLL_EXPORT  // Для Linux ничего не делаем
#endif

// extern "C" для совместимости с C
extern "C" {
    // Функция переворачивания строки
    DLL_EXPORT void reverse_string(char* str);
    
    // Функция проверки палиндрома
    DLL_EXPORT bool is_palindrome(const char* str);
}

// Класс для работы со строками
class DLL_EXPORT StringProcessor {
private:
    std::string data;  // Внутреннее хранилище строки

public:
    // Конструктор из C-строки
    StringProcessor(const char* str = "");
    
    // Метод преобразования к верхнему регистру
    void to_uppercase();
    
    // Метод преобразования к нижнему регистру
    void to_lowercase();
    
    // Метод получения текущей строки
    std::string get_string() const;
    
    // Метод разбиения строки по разделителю
    std::vector<std::string> split(char delimiter) const;
    
    // Статический метод объединения строк
    static std::string join(const std::vector<std::string>& strings, 
                           const std::string& delimiter);
};

#endif // STRING_OPERATIONS_H