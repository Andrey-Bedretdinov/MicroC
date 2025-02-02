# Объяснение кода: Вывод строки "Hello World" через UART

Этот код реализует отправку строки "Hello World" через интерфейс UART на микроконтроллере ATmega. Для передачи используется скорость 9600 бод.

---

## Настройка частоты и скорости передачи данных

### Определение констант

```c
#define F_CPU 8000000UL // Частота работы микроконтроллера: 8 МГц
#define UART_BAUD 9600  // Скорость передачи: 9600 бод
#define UBRR_VALUE ((F_CPU / (16UL * UART_BAUD)) - 1)
```

1. F_CPU: Частота тактирования микроконтроллера задается в 8 МГц.
2. UART_BAUD: Скорость передачи данных через UART (9600 бод).
3. UBRR_VALUE: Значение регистра скорости UART рассчитывается по формуле:  
   UBRR = (F_CPU / (16 * BAUD)) - 1

Для 8 МГц и 9600 бод:  
UBRR = (8000000 / (16 * 9600)) - 1 = 51

---

## Инициализация UART

### Функция `UART_Init`

```c
void UART_Init(void) {
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8); // Старший байт
    UBRR0L = (uint8_t)UBRR_VALUE;        // Младший байт

    UCSR0B = (1 << TXEN0); // Включаем передатчик UART
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Формат: 8 бит данных, 1 стоп-бит
}
```

1. **Регистр UBRR0H и UBRR0L**: Настраивают скорость передачи данных.
2. **Регистр UCSR0B**:
   - Бит TXEN0: включает передатчик UART.
3. **Регистр UCSR0C**:
   - Биты UCSZ01 и UCSZ00: 8-битные данные.
   - 1 стоп-бит (по умолчанию).
   - Без проверки четности.

---

## Отправка данных через UART

### Функция `UART_TransmitByte`

```c
void UART_TransmitByte(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0))); // Ждем, пока регистр передачи не освободится
    UDR0 = data; // Отправляем данные
}
```

1. Ожидание готовности передатчика:  
   Регистр UCSR0A содержит флаг UDRE0, который указывает, что регистр передачи UDR0 пустой.
2. Передача данных:  
   Запись байта данных в UDR0 инициирует передачу.

---

### Функция `UART_TransmitString`

```c
void UART_TransmitString(const char* str) {
    while (*str) {
        UART_TransmitByte((uint8_t)*str++); // Отправляем строку символ за символом
    }
}
```

- Использует функцию UART_TransmitByte для последовательной отправки каждого символа строки.
- Символы отправляются до тех пор, пока не достигнут символ \0 (конец строки).

---

## Основной цикл

### Главная программа

```c
int main(void) {
    UART_Init(); // Инициализация UART

    while (1) {
        UART_TransmitString("Hello World\r\n\r\n"); // Отправляем строку
        _delay_ms(2000); // Задержка 2 секунды
    }
}
```

1. Инициализация UART: Вызывается функция UART_Init() для настройки UART.
2. Отправка строки:
   - Строка "Hello World" отправляется через UART.
   - \r\n добавляет символы возврата каретки и новой строки.
3. Задержка: Используется _delay_ms(2000) для повторения отправки каждые 2 секунды.

---

## Использованные регистры

1. **UBRR0H и UBRR0L**: Настройка скорости передачи.
2. **UCSR0A**:
   - Бит UDRE0: Указывает, что регистр передачи пуст и готов к записи.
3. **UCSR0B**:
   - Бит TXEN0: Включает передатчик.
4. **UCSR0C**:
   - Биты UCSZ01 и UCSZ00: Настройка длины данных (8 бит).
5. **UDR0**: Регистр передачи данных.

---

## Итоговая схема работы

1. **Инициализация UART**: Настраивается скорость передачи и формат фрейма.
2. **Передача строки**:
   - В цикле строка "Hello World" отправляется через UART.
   - Используется задержка 2 секунды между отправками.
3. **Использование UART**:
   - Позволяет подключить микроконтроллер к ПК или другому устройству для отображения строки через последовательный интерфейс.  