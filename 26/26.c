#include <avr/io.h>

// Функция инициализации портов ввода-вывода
void initPorts(void) {
    // Настройка PORTD (PD0 - PD7) как входов с подтягивающими резисторами
    DDRD = 0x00;       // Все пины PORTD настроены как входы
    PORTD = 0xFF;      // Включены подтягивающие резисторы на всех пинах PORTD

    // Настройка PORTB (PB0 - PB7) как выходов для семисегментных индикаторов
    DDRB = 0xFF;       // Все пины PORTB настроены как выходы
    PORTB = 0x00;      // Все выходы PORTB установлены в низкий уровень
}

// Функция чтения состояния кнопок из PORTD
uint8_t readButtonGroup(uint8_t mask, uint8_t shift) {
    return (~PIND & mask) >> shift;
}

// Функция обновления состояния семисегментного индикатора
void updateSegmentDisplay(uint8_t lowerNibble, uint8_t upperNibble) {
    // Обновление младших 4 бит (PB0 - PB3)
    PORTB = (PORTB & 0xF0) | lowerNibble;

    // Обновление старших 4 бит (PB4 - PB7)
    PORTB = (PORTB & 0x0F) | (upperNibble << 4);
}

int main(void) {
    initPorts(); // Инициализация портов

    while (1) {
        // Считывание состояния кнопок
        uint8_t lowerButtons = readButtonGroup(0x0F, 0); // Чтение младших кнопок (PD0 - PD3)
        uint8_t upperButtons = readButtonGroup(0xF0, 4); // Чтение старших кнопок (PD4 - PD7)

        // Обновление семисегментного индикатора
        updateSegmentDisplay(lowerButtons, upperButtons);
    }

    return 0;
}
