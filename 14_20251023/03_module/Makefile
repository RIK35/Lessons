# Компилятор и флаги
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -fPIC
LDFLAGS := -L. -lstring_operations

# Имена файлов
STATIC_LIB := libmath_operations.a
DYNAMIC_LIB := libstring_operations.so
MAIN_APP := library_demo

# Исходные файлы
MATH_SOURCES := math_operations.cpp
STRING_SOURCES := string_operations.cpp
MAIN_SOURCE := main.cpp

# Объектные файлы
MATH_OBJECTS := $(MATH_SOURCES:.cpp=.o)
STRING_OBJECTS := $(STRING_SOURCES:.cpp=.o)
MAIN_OBJECTS := $(MAIN_SOURCE:.cpp=.o)

# Правило по умолчанию
all: $(STATIC_LIB) $(DYNAMIC_LIB) $(MAIN_APP)

# Сборка статической библиотеки
$(STATIC_LIB): $(MATH_OBJECTS)
	ar rcs $@ $^  # Создаем архив статической библиотеки

# Сборка динамической библиотеки
$(DYNAMIC_LIB): $(STRING_OBJECTS)
	$(CXX) -shared -o $@ $^  # Создаем shared library

# Сборка главного приложения
$(MAIN_APP): $(MAIN_OBJECTS) $(STATIC_LIB) $(DYNAMIC_LIB)
	$(CXX) -o $@ $(MAIN_OBJECTS) -L. -lmath_operations -lstring_operations

# Компиляция объектных файлов для математической библиотеки
math_operations.o: math_operations.cpp math_operations.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Компиляция объектных файлов для строковой библиотеки
string_operations.o: string_operations.cpp string_operations.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Компиляция главного файла
main.o: main.cpp math_operations.h string_operations.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Установка библиотек в систему
install:
	sudo cp $(DYNAMIC_LIB) /usr/local/lib/
	sudo cp string_operations.h /usr/local/include/
	sudo cp math_operations.h /usr/local/include/
	sudo ldconfig  # Обновляем кэш библиотек

# Запуск приложения
run: $(MAIN_APP)
	LD_LIBRARY_PATH=.:$$LD_LIBRARY_PATH ./$(MAIN_APP)

# Очистка
clean:
	rm -f *.o *.a *.so $(MAIN_APP)

# Создание документации
doc:
	doxygen Doxyfile

# Отладочная сборка
debug: CXXFLAGS += -g -DDEBUG
debug: clean all

.PHONY: all clean run install debug doc