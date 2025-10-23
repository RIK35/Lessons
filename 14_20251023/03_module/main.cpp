// Подключаем стандартные библиотеки
#include <iostream>   // Для ввода-вывода
#include <iomanip>    // Для форматирования вывода

// Подключаем наши библиотеки
#include "math_operations.h"   // Статическая библиотека
#include "string_operations.h" // Динамическая библиотека

// Функция демонстрации математических операций
void demonstrate_math_operations() {
    std::cout << "=== ДЕМОНСТРАЦИЯ МАТЕМАТИЧЕСКИХ ОПЕРАЦИЙ ===" << std::endl;
    
    // Демонстрация базовых операций
    std::cout << "Базовые операции:" << std::endl;
    std::cout << "5 + 3 = " << add(5, 3) << std::endl;
    std::cout << "10 - 4 = " << subtract(10, 4) << std::endl;
    std::cout << "6 * 7 = " << multiply(6, 7) << std::endl;
    
    // Демонстрация деления с обработкой исключения
    try {
        std::cout << "15.0 / 3.0 = " << divide(15.0, 3.0) << std::endl;
        std::cout << "10.0 / 0.0 = " << divide(10.0, 0.0) << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << std::endl;
    }
    
    // Демонстрация факториала
    std::cout << "\nФакториалы:" << std::endl;
    for (int i = 0; i <= 10; ++i) {
        std::cout << i << "! = " << factorial(i) << std::endl;
    }
    
    // Демонстрация проверки простых чисел
    std::cout << "\nПростые числа до 20:" << std::endl;
    for (int i = 1; i <= 20; ++i) {
        if (is_prime(i)) {
            std::cout << i << " - простое" << std::endl;
        }
    }
    
    // Демонстрация работы с комплексными числами
    std::cout << "\nКомплексные числа:" << std::endl;
    ComplexNumber num1(3.0, 4.0);   // Создаем первое комплексное число
    ComplexNumber num2(1.0, -2.0);  // Создаем второе комплексное число
    
    std::cout << "num1 = " << num1.to_string() << std::endl;
    std::cout << "num2 = " << num2.to_string() << std::endl;
    
    ComplexNumber sum = num1.add(num2);           // Сложение
    ComplexNumber product = num1.multiply(num2);  // Умножение
    
    std::cout << "Сумма: " << sum.to_string() << std::endl;
    std::cout << "Произведение: " << product.to_string() << std::endl;
}

// Функция демонстрации строковых операций
void demonstrate_string_operations() {
    std::cout << "\n=== ДЕМОНСТРАЦИЯ СТРОКОВЫХ ОПЕРАЦИЙ ===" << std::endl;
    
    // Демонстрация C-функций
    char test_str[] = "Hello World";  // Тестовая строка
    std::cout << "Исходная строка: " << test_str << std::endl;
    
    reverse_string(test_str);  // Переворачиваем строку
    std::cout << "Перевернутая строка: " << test_str << std::endl;
    
    // Проверка палиндромов
    const char* palindrome1 = "radar";
    const char* palindrome2 = "hello";
    
    std::cout << "\nПроверка палиндромов:" << std::endl;
    std::cout << palindrome1 << " - " 
              << (is_palindrome(palindrome1) ? "палиндром" : "не палиндром") 
              << std::endl;
    std::cout << palindrome2 << " - " 
              << (is_palindrome(palindrome2) ? "палиндром" : "не палиндром") 
              << std::endl;
    
    // Демонстрация класса StringProcessor
    std::cout << "\nРабота с классом StringProcessor:" << std::endl;
    StringProcessor processor("Hello,World,Test,String");  // Создаем процессор
    
    std::cout << "Исходная строка: " << processor.get_string() << std::endl;
    
    // Преобразование к верхнему регистру
    processor.to_uppercase();
    std::cout << "В верхнем регистре: " << processor.get_string() << std::endl;
    
    // Преобразование к нижнему регистру
    processor.to_lowercase();
    std::cout << "В нижнем регистре: " << processor.get_string() << std::endl;
    
    // Разбиение строки
    auto parts = processor.split(',');  // Разбиваем по запятой
    std::cout << "Разбиение строки:" << std::endl;
    for (size_t i = 0; i < parts.size(); ++i) {
        std::cout << "  Часть " << i << ": " << parts[i] << std::endl;
    }
    
    // Объединение строк
    std::vector<std::string> strings = {"Apple", "Banana", "Orange"};
    std::string joined = StringProcessor::join(strings, " | ");
    std::cout << "Объединенная строка: " << joined << std::endl;
}

// Главная функция программы
int main() {
    std::cout << "ПРИМЕР РАБОТЫ С БИБЛИОТЕКАМИ И МОДУЛЯМИ НА C++" << std::endl;
    std::cout << "=============================================" << std::endl;
    
    try {
        // Демонстрируем математические операции
        demonstrate_math_operations();
        
        // Демонстрируем строковые операции
        demonstrate_string_operations();
        
    } catch (const std::exception& e) {
        // Обрабатываем исключения
        std::cerr << "Произошла ошибка: " << e.what() << std::endl;
        return 1;  // Возвращаем код ошибки
    }
    
    std::cout << "\nПрограмма завершена успешно!" << std::endl;
    return 0;  // Успешное завершение
}