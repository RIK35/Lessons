// Подключаем заголовочные файлы GTK+ 4
#include <gtk/gtk.h>  // Основной заголовочный файл GTK+

// Объявляем глобальные переменные для виджетов
static GtkWidget *entry_display;    // Поле для отображения ввода и результата
static gdouble current_value = 0;   // Текущее значение калькулятора
static gchar current_operator = 0;  // Текущий оператор (+, -, *, /)
static gboolean new_input = TRUE;   // Флаг нового ввода

// Функция обновления дисплея калькулятора
static void update_display(const gchar *text) {
    // Устанавливаем текст в поле ввода
    gtk_editable_set_text(GTK_EDITABLE(entry_display), text);
}

// Функция обработки нажатия цифровых кнопок
static void digit_button_clicked(GtkWidget *widget, gpointer data) {
    // Получаем текст с кнопки, которая была нажата
    const gchar *digit = gtk_button_get_label(GTK_BUTTON(widget));
    
    // Получаем текущий текст с дисплея
    const gchar *current_text = gtk_editable_get_text(GTK_EDITABLE(entry_display));
    
    // Если начат новый ввод, сбрасываем дисплей
    if (new_input) {
        update_display(digit);      // Устанавливаем новую цифру
        new_input = FALSE;          // Сбрасываем флаг нового ввода
    } else {
        // Иначе добавляем цифру к существующему тексту
        gchar *new_text = g_strdup_printf("%s%s", current_text, digit);
        update_display(new_text);   // Обновляем дисплей
        g_free(new_text);           // Освобождаем память
    }
}

// Функция обработки нажатия кнопок операторов
static void operator_button_clicked(GtkWidget *widget, gpointer data) {
    // Получаем текущий текст с дисплея
    const gchar *current_text = gtk_editable_get_text(GTK_EDITABLE(entry_display));
    
    // Преобразуем текст в число
    gdouble operand = g_ascii_strtod(current_text, NULL);
    
    // Если есть предыдущий оператор, выполняем вычисление
    if (current_operator != 0) {
        // Выполняем операцию в зависимости от оператора
        switch (current_operator) {
            case '+': current_value += operand; break;  // Сложение
            case '-': current_value -= operand; break;  // Вычитание
            case '*': current_value *= operand; break;  // Умножение
            case '/': 
                // Проверяем деление на ноль
                if (operand != 0) {
                    current_value /= operand;           // Деление
                } else {
                    update_display("Error");            // Ошибка деления на ноль
                    current_value = 0;                  // Сбрасываем значение
                    current_operator = 0;               // Сбрасываем оператор
                    return;
                }
                break;
        }
    } else {
        // Если оператора не было, устанавливаем текущее значение
        current_value = operand;
    }
    
    // Получаем символ оператора с кнопки
    const gchar *op_label = gtk_button_get_label(GTK_BUTTON(widget));
    current_operator = op_label[0];  // Берем первый символ метки
    
    // Обновляем дисплей с текущим значением
    gchar *result_text = g_strdup_printf("%g", current_value);
    update_display(result_text);     // Показываем результат
    g_free(result_text);             // Освобождаем память
    
    new_input = TRUE;                // Устанавливаем флаг нового ввода
}

// Функция обработки нажатия кнопки "равно"
static void equals_button_clicked(GtkWidget *widget, gpointer data) {
    // Получаем текущий текст с дисплея
    const gchar *current_text = gtk_editable_get_text(GTK_EDITABLE(entry_display));
    
    // Преобразуем текст в число
    gdouble operand = g_ascii_strtod(current_text, NULL);
    
    // Выполняем операцию, если есть оператор
    if (current_operator != 0) {
        switch (current_operator) {
            case '+': current_value += operand; break;  // Сложение
            case '-': current_value -= operand; break;  // Вычитание
            case '*': current_value *= operand; break;  // Умножение
            case '/': 
                // Проверяем деление на ноль
                if (operand != 0) {
                    current_value /= operand;           // Деление
                } else {
                    update_display("Error");            // Ошибка деления на ноль
                    current_value = 0;                  // Сбрасываем значение
                    current_operator = 0;               // Сбрасываем оператор
                    return;
                }
                break;
        }
    }
    
    // Обновляем дисплей с результатом
    gchar *result_text = g_strdup_printf("%g", current_value);
    update_display(result_text);     // Показываем результат
    g_free(result_text);             // Освобождаем память
    
    // Сбрасываем оператор и устанавливаем флаг нового ввода
    current_operator = 0;            // Сбрасываем оператор
    new_input = TRUE;                // Устанавливаем флаг нового ввода
}

// Функция обработки нажатия кнопки очистки
static void clear_button_clicked(GtkWidget *widget, gpointer data) {
    update_display("0");             // Сбрасываем дисплей на "0"
    current_value = 0;               // Сбрасываем текущее значение
    current_operator = 0;            // Сбрасываем оператор
    new_input = TRUE;                // Устанавливаем флаг нового ввода
}

// Функция создания кнопки калькулятора
static GtkWidget* create_button(const gchar *label, GCallback callback) {
    // Создаем новую кнопку с указанной меткой
    GtkWidget *button = gtk_button_new_with_label(label);
    
    // Подключаем сигнал "clicked" к указанной функции обратного вызова
    g_signal_connect(button, "clicked", callback, NULL);
    
    // Устанавливаем размер кнопки
    gtk_widget_set_size_request(button, 60, 50);
    
    return button;  // Возвращаем созданную кнопку
}

// Основная функция создания интерфейса приложения
static void activate(GtkApplication *app, gpointer user_data) {
    // Создаем главное окно приложения
    GtkWidget *window = gtk_application_window_new(app);
    
    // Устанавливаем заголовок окна
    gtk_window_set_title(GTK_WINDOW(window), "GTK+ 4 Калькулятор");
    
    // Устанавливаем размер окна
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 400);
    
    // Создаем основной контейнер с вертикальным расположением
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    
    // Устанавливаем отступы вокруг контейнера
    gtk_widget_set_margin_top(vbox, 10);
    gtk_widget_set_margin_bottom(vbox, 10);
    gtk_widget_set_margin_start(vbox, 10);
    gtk_widget_set_margin_end(vbox, 10);
    
    // Создаем поле для ввода/вывода (дисплей калькулятора)
    entry_display = gtk_entry_new();
    
    // Устанавливаем выравнивание текста по правому краю
    gtk_entry_set_alignment(GTK_ENTRY(entry_display), 1.0);
    
    // Устанавливаем текст по умолчанию
    gtk_editable_set_text(GTK_EDITABLE(entry_display), "0");
    
    // Делаем поле только для чтения
    gtk_editable_set_editable(GTK_EDITABLE(entry_display), FALSE);
    
    // Добавляем дисплей в вертикальный контейнер
    gtk_box_append(GTK_BOX(vbox), entry_display);
    
    // Создаем контейнер для кнопок с сеточным расположением
    GtkWidget *grid = gtk_grid_new();
    
    // Устанавливаем отступы и промежутки между кнопками
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    
    // Массив меток для кнопок калькулятора
    const gchar *button_labels[16] = {
        "7", "8", "9", "/",
        "4", "5", "6", "*", 
        "1", "2", "3", "-",
        "C", "0", "=", "+"
    };
    
    // Создаем кнопки и размещаем их в сетке
    for (gint i = 0; i < 16; i++) {
        // Создаем кнопку в зависимости от ее назначения
        GtkWidget *button;
        if (g_ascii_isdigit(button_labels[i][0])) {
            // Создаем цифровую кнопку
            button = create_button(button_labels[i], 
                                 G_CALLBACK(digit_button_clicked));
        } else if (button_labels[i][0] == 'C') {
            // Создаем кнопку очистки
            button = create_button(button_labels[i], 
                                 G_CALLBACK(clear_button_clicked));
        } else if (button_labels[i][0] == '=') {
            // Создаем кнопку равенства
            button = create_button(button_labels[i], 
                                 G_CALLBACK(equals_button_clicked));
        } else {
            // Создаем кнопку оператора
            button = create_button(button_labels[i], 
                                 G_CALLBACK(operator_button_clicked));
        }
        
        // Вычисляем позицию кнопки в сетке (4x4)
        gint row = i / 4;     // Номер строки
        gint column = i % 4;  // Номер столбца
        
        // Добавляем кнопку в сетку на вычисленную позицию
        gtk_grid_attach(GTK_GRID(grid), button, column, row, 1, 1);
    }
    
    // Добавляем сетку с кнопками в вертикальный контейнер
    gtk_box_append(GTK_BOX(vbox), grid);
    
    // Добавляем вертикальный контейнер в главное окно
    gtk_window_set_child(GTK_WINDOW(window), vbox);
    
    // Показываем все виджеты окна
    gtk_widget_show(window);
}

// Главная функция программы
int main(int argc, char **argv) {
    // Создаем GTK приложение с уникальным идентификатором
    GtkApplication *app = gtk_application_new(
        "org.example.calculator",    // Уникальный идентификатор приложения
        G_APPLICATION_FLAGS_NONE  // Флаги по умолчанию
    );
    
    // Подключаем сигнал "activate" к нашей функции
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    
    // Запускаем приложение
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    
    // Освобождаем память, занятую приложением
    g_object_unref(app);
    
    // Возвращаем статус выхода
    return status;
}