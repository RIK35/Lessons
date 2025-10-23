# Компилятор C++
CXX = g++

# Флаги компиляции
CXXFLAGS = -std=c++11 -Wall -Wextra -O2 -pthread

# Имя исполняемого файла
TARGET = dining_philosophers

# Исходные файлы
SOURCES = dining_philosophers.cpp

# Правило по умолчанию
all: $(TARGET)

# Правило компиляции
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

# Запуск программы
run: $(TARGET)
	./$(TARGET)

# Отладочная сборка
debug: CXXFLAGS += -g -DDEBUG
debug: clean $(TARGET)

# Очистка
clean:
	rm -f $(TARGET)

# Статический анализ кода
check:
	cppcheck --enable=all $(SOURCES)

.PHONY: all run debug clean check