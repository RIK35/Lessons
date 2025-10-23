#!/bin/bash

# Скрипт управления демоном мониторинга системы

DAEMON_NAME="system_monitor_daemon"
SERVICE_NAME="system-monitor"

# Функция показа справки
show_help() {
    echo "Использование: $0 {start|stop|restart|status|install|uninstall}"
    echo "  start    - запустить демона"
    echo "  stop     - остановить демона"
    echo "  restart  - перезапустить демона"
    echo "  status   - показать статус демона"
    echo "  install  - установить демона как системную службу"
    echo "  uninstall - удалить демона"
}

# Функция компиляции демона
compile_daemon() {
    echo "Компиляция демона..."
    make clean
    make
    if [ $? -eq 0 ]; then
        echo "Демон успешно скомпилирован"
    else
        echo "Ошибка компиляции демона"
        exit 1
    fi
}

# Функция установки
install_daemon() {
    echo "Установка демона..."
    
    # Компилируем демона
    compile_daemon
    
    # Копируем исполняемый файл
    sudo cp $DAEMON_NAME /usr/local/bin/
    sudo chmod +x /usr/local/bin/$DAEMON_NAME
    
    # Копируем service файл
    sudo cp system-monitor.service /etc/systemd/system/
    
    # Перезагружаем systemd
    sudo systemctl daemon-reload
    
    # Включаем автозагрузку
    sudo systemctl enable $SERVICE_NAME
    
    echo "Демон установлен и настроен для автозагрузки"
}

# Функция удаления
uninstall_daemon() {
    echo "Удаление демона..."
    
    # Останавливаем службу
    sudo systemctl stop $SERVICE_NAME 2>/dev/null
    
    # Отключаем автозагрузку
    sudo systemctl disable $SERVICE_NAME 2>/dev/null
    
    # Удаляем файлы
    sudo rm -f /usr/local/bin/$DAEMON_NAME
    sudo rm -f /etc/systemd/system/$SERVICE_NAME.service
    
    # Перезагружаем systemd
    sudo systemctl daemon-reload
    
    echo "Демон удален"
}

# Проверка прав доступа
check_root() {
    if [ "$EUID" -ne 0 ]; then
        echo "Это действие требует прав root. Используйте sudo."
        exit 1
    fi
}

# Основная логика скрипта
case "$1" in
    start)
        check_root
        echo "Запуск демона..."
        sudo systemctl start $SERVICE_NAME
        ;;
    stop)
        check_root
        echo "Остановка демона..."
        sudo systemctl stop $SERVICE_NAME
        ;;
    restart)
        check_root
        echo "Перезапуск демона..."
        sudo systemctl restart $SERVICE_NAME
        ;;
    status)
        sudo systemctl status $SERVICE_NAME
        ;;
    install)
        check_root
        install_daemon
        ;;
    uninstall)
        check_root
        uninstall_daemon
        ;;
    compile)
        compile_daemon
        ;;
    *)
        show_help
        exit 1
        ;;
esac

exit 0