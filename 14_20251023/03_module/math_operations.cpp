// Подключаем собственный заголовочный файл
#include "math_operations.h"

// Подключаем стандартные библиотеки
#include <stdexcept>  // Для исключений
#include <sstream>    // Для работы со строками
#include <cmath>      // Для математических функций

// Реализация функции сложения
int add(int a, int b) {
    return a + b;  // Возвращаем сумму параметров
}

// Реализация функции вычитания
int subtract(int a, int b) {
    return a - b;  // Возвращаем разность параметров
}

// Реализация функции умножения
int multiply(int a, int b) {
    return a * b;  // Возвращаем произведение параметров
}

// Реализация функции деления
double divide(double a, double b) {
    // Проверяем деление на ноль
    if (b == 0.0) {
        // Генерируем исключение при делении на ноль
        throw std::runtime_error("Division by zero!");
    }
    return a / b;  // Возвращаем частное параметров
}

// Реализация функции вычисления факториала
unsigned long long factorial(int n) {
    // Проверяем корректность входного параметра
    if (n < 0) {
        throw std::invalid_argument("Factorial is not defined for negative numbers");
    }
    
    // Инициализируем результат
    unsigned long long result = 1;
    
    // Вычисляем факториал в цикле
    for (int i = 2; i <= n; ++i) {
        result *= i;  // Умножаем результат на текущее число
    }
    
    return result;  // Возвращаем вычисленный факториал
}

// Реализация функции проверки числа на простоту
bool is_prime(int number) {
    // Обрабатываем специальные случаи
    if (number <= 1) return false;    // Числа меньше 2 не простые
    if (number == 2) return true;     // 2 - простое число
    if (number % 2 == 0) return false; // Четные числа больше 2 не простые
    
    // Проверяем нечетные делители до квадратного корня
    for (int i = 3; i * i <= number; i += 2) {
        if (number % i == 0) {
            return false;  // Найден делитель - число не простое
        }
    }
    
    return true;  // Делителей не найдено - число простое
}

// Реализация методов класса ComplexNumber

// Конструктор класса
ComplexNumber::ComplexNumber(double real, double imag) 
    : real_part(real), imag_part(imag) {}  // Инициализация членов класса

// Метод получения действительной части
double ComplexNumber::get_real() const {
    return real_part;  // Возвращаем действительную часть
}

// Метод получения мнимой части
double ComplexNumber::get_imag() const {
    return imag_part;  // Возвращаем мнимую часть
}

// Метод сложения комплексных чисел
ComplexNumber ComplexNumber::add(const ComplexNumber& other) const {
    // Создаем новый объект с суммой соответствующих частей
    return ComplexNumber(real_part + other.real_part, 
                        imag_part + other.imag_part);
}

// Метод умножения комплексных чисел
ComplexNumber ComplexNumber::multiply(const ComplexNumber& other) const {
    // Вычисляем новую действительную часть
    double new_real = real_part * other.real_part - imag_part * other.imag_part;
    // Вычисляем новую мнимую часть
    double new_imag = real_part * other.imag_part + imag_part * other.real_part;
    // Возвращаем новый объект комплексного числа
    return ComplexNumber(new_real, new_imag);
}

// Метод преобразования в строку
std::string ComplexNumber::to_string() const {
    std::ostringstream oss;  // Создаем строковый поток
    oss << real_part;        // Добавляем действительную часть
    
    // Добавляем мнимую часть с соответствующим знаком
    if (imag_part >= 0) {
        oss << " + " << imag_part << "i";  // Положительная мнимая часть
    } else {
        oss << " - " << -imag_part << "i"; // Отрицательная мнимая часть
    }
    
    return oss.str();  // Возвращаем результирующую строку
}