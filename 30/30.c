#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t value = 0;  // Значение для отображения на индикаторах

// Обработчик прерывания INT0 (PD2) — увеличение значения
ISR(INT0_vect) {
    if (value < 99) value++;
}

// Обработчик прерывания INT1 (PD3) — уменьшение значения
ISR(INT1_vect) {
    if (value > 0) value--;
}

void display_number(uint8_t number) {
    uint8_t digit1 = number / 10;  // Десятки (для первого индикатора)
    uint8_t digit2 = number % 10;  // Единицы (для второго индикатора)

    // Вывод числа на первый индикатор (PB4-PB7)
    PORTB = (PORTB & 0x0F) | (digit1 << 4);

    // Вывод числа на второй индикатор (PB0-PB3)
    PORTB = (PORTB & 0xF0) | (digit2 & 0x0F);
}

int main(void) {
    // Настройка портов
    DDRB = 0xFF;  // Все порты PB настроены как выходы
    PORTB = 0x00; // Очищаем порты PB

    // Настройка прерываний INT0 и INT1
    EICRA |= (1 << ISC01) | (1 << ISC11); // Прерывания по спадающему фронту
    EIMSK |= (1 << INT0) | (1 << INT1);   // Разрешить прерывания INT0 и INT1

    sei(); // Включаем глобальные прерывания

    while (1) {
        display_number(value);  // Отображение текущего значения
        _delay_ms(10);          // Минимальная задержка для стабильности
    }
}
