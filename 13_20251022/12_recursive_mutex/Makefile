# Компилятор и флаги
CC = g++
CFLAGS = -Wall -Wextra -std=c++20 -pthread -Iinclude
LDFLAGS = -lm

# Директории
SRC_DIR = .
OBJ_DIR = obj
INC_DIR = include

# Цели
TARGET = recursive_mutex

# Поиск исходных файлов
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Основное правило
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)
	@echo "Программа $(TARGET) успешно собрана!"

# Правило для объектных файлов
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Создание директории для объектных файлов
$(OBJ_DIR):
	mkdir -p $@

# Очистка
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
	@echo "Очистка завершена"

# Пересборка
rebuild: clean $(TARGET)

# Запуск программы
run: $(TARGET)
	@echo "Запуск программы..."
	./$(TARGET)

# Отладка
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

# Показать переменные
vars:
	@echo "CC: $(CC)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "SOURCES: $(SOURCES)"
	@echo "OBJECTS: $(OBJECTS)"
	@echo "TARGET: $(TARGET)"

# Справка
help:
	@echo "Доступные команды:"
	@echo "  make           - сборка программы"
	@echo "  make run       - сборка и запуск"
	@echo "  make debug     - сборка с отладкой"
	@echo "  make clean     - очистка проекта"
	@echo "  make rebuild   - полная пересборка"
	@echo "  make vars      - показать переменные"
	@echo "  make help      - эта справка"

.PHONY: clean rebuild run debug vars help