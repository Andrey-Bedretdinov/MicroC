#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL

// Определения для сегментов семисегментного индикатора
#define SEG_LOW_MASK  0x0F // Маска для младших 4 бит
#define SEG_HIGH_MASK 0xF0 // Маска для старших 4 бит

// Таблица отображения цифр на семисегментный индикатор
const uint8_t segment_map[] = {
    0b0000, // 0
    0b0001, // 1
    0b0010, // 2
    0b0011, // 3
    0b0100, // 4
    0b0101, // 5
    0b0110, // 6
    0b0111, // 7
    0b1000, // 8
    0b1001, // 9
};

// Инициализация UART
void UART_Init() {
    uint16_t ubrr_value = 51; // Конфигурация для 9600 бод на частоте 8 МГц
    UBRR0H = (ubrr_value >> 8);
    UBRR0L = ubrr_value;

    // Включение передатчика и приёмника
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    // Формат данных: 8 бит, 1 стоп-бит
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// Чтение символа из UART
char UART_ReadChar() {
    while (!(UCSR0A & (1 << RXC0))); // Ожидание данных
    return UDR0;
}

// Вывод цифры на младший семисегментный индикатор
void DisplayDigitLow(uint8_t digit) {
    if (digit >= 10) return;  // Проверка корректности значения
    DDRB |= SEG_LOW_MASK;     // Настройка младших битов порта B как выходы
    PORTB = (PORTB & SEG_HIGH_MASK) | segment_map[digit]; // Обновление младших бит
}

// Вывод цифры на старший семисегментный индикатор
void DisplayDigitHigh(uint8_t digit) {
    if (digit >= 10) return;  // Проверка корректности значения
    DDRB |= SEG_HIGH_MASK;    // Настройка старших битов порта B как выходы
    PORTB = (PORTB & SEG_LOW_MASK) | (segment_map[digit] << 4); // Обновление старших бит
}

int main(void) {
    UART_Init(); // Инициализация UART

    char input_buffer[3]; // Буфер для хранения введённых символов
    uint8_t buffer_index = 0;

    while (1) {
        char received_char = UART_ReadChar();

        // Обработка завершения ввода строки
        if (received_char == '\r' || received_char == '\n') {
            input_buffer[buffer_index] = '\0'; // Завершение строки

            if (buffer_index > 0) {
                // Преобразование символов в цифры и отображение
                uint8_t first_digit = (input_buffer[0] >= '0' && input_buffer[0] <= '9')
                                        ? input_buffer[0] - '0' : 0;
                uint8_t second_digit = (buffer_index > 1 && input_buffer[1] >= '0' && input_buffer[1] <= '9')
                                         ? input_buffer[1] - '0' : 0;

                DisplayDigitLow(second_digit);
                DisplayDigitHigh(first_digit);
            }

            buffer_index = 0; // Сброс индекса буфера
        }
        // Добавление цифры в буфер, если символ допустим
        else if (received_char >= '0' && received_char <= '9' && buffer_index < 2) {
            input_buffer[buffer_index++] = received_char;
        }
    }

    return 0;
}
