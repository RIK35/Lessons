# Компилятор C++
CXX = g++

# Флаги компиляции
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -pthread

# Имя исполняемого файла
TARGET = system_monitor_daemon

# Исходные файлы
SOURCES = system_monitor_daemon.cpp

# Правило по умолчанию
all: $(TARGET)

# Правило компиляции
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

# Установка демона (требует прав root)
install:
	sudo cp $(TARGET) /usr/local/bin/
	sudo chmod +x /usr/local/bin/$(TARGET)
	sudo cp system-monitor.service /etc/systemd/system/
	sudo systemctl daemon-reload

# Запуск демона
start:
	sudo systemctl start system-monitor

# Остановка демона
stop:
	sudo systemctl stop system-monitor

# Просмотр статуса
status:
	sudo systemctl status system-monitor

# Просмотр логов
logs:
	sudo journalctl -u system-monitor -f

# Очистка
clean:
	rm -f $(TARGET)

.PHONY: all install start stop status logs clean