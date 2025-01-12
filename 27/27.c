#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL // Частота работы микроконтроллера: 8 МГц
#define UART_BAUD 9600  // Скорость передачи: 9600 бод

// Рассчитываем значение UBRR для заданной частоты и скорости
#define UBRR_VALUE ((F_CPU / (16UL * UART_BAUD)) - 1)

// Инициализация UART
void UART_Init(void) {
    // Установка значения регистра UBRR
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8); // Старший байт
    UBRR0L = (uint8_t)UBRR_VALUE;        // Младший байт

    // Включаем передатчик UART
    UCSR0B = (1 << TXEN0);

    // Настраиваем формат фрейма: 8 бит данных, 1 стоп-бит, без проверки четности
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// Отправка одного байта через UART
void UART_TransmitByte(uint8_t data) {
    // Ждем, пока регистр передачи не станет пустым
    while (!(UCSR0A & (1 << UDRE0)));

    // Записываем данные в регистр передачи
    UDR0 = data;
}

// Отправка строки через UART
void UART_TransmitString(const char* str) {
    while (*str) {
        UART_TransmitByte((uint8_t)*str++); // Отправляем символ за символом
    }
}

int main(void) {
    UART_Init(); // Инициализация UART

    while (1) {
        // Отправка строки "Hello World" через UART
        UART_TransmitString("Hello World\r\n\r\n");

        // Задержка перед повторной отправкой
        _delay_ms(2000);
    }
}
