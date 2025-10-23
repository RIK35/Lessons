// Защита от множественного включения файла
#ifndef MATH_OPERATIONS_H
#define MATH_OPERATIONS_H

#include <string>

// Объявления функций математических операций

// Функция сложения двух целых чисел
int add(int a, int b);

// Функция вычитания двух целых чисел
int subtract(int a, int b);

// Функция умножения двух целых чисел
int multiply(int a, int b);

// Функция деления двух чисел с плавающей точкой
double divide(double a, double b);

// Функция вычисления факториала числа
unsigned long long factorial(int n);

// Функция проверки числа на простоту
bool is_prime(int number);

// Класс для работы с комплексными числами
class ComplexNumber {
private:
    double real_part;  // Действительная часть комплексного числа
    double imag_part;  // Мнимая часть комплексного числа

public:
    // Конструктор класса с параметрами по умолчанию
    ComplexNumber(double real = 0.0, double imag = 0.0);
    
    // Метод получения действительной части
    double get_real() const;
    
    // Метод получения мнимой части
    double get_imag() const;
    
    // Метод сложения двух комплексных чисел
    ComplexNumber add(const ComplexNumber& other) const;
    
    // Метод умножения двух комплексных чисел
    ComplexNumber multiply(const ComplexNumber& other) const;
    
    // Метод вывода комплексного числа в строку
    std::string to_string() const;
};

#endif // MATH_OPERATIONS_H