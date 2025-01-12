#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_PORT_B PORTB
#define LED_PORT_C PORTC
#define LED_DDR_B DDRB
#define LED_DDR_C DDRC
#define F_CPU 8000000UL

// Настройки таймера
#define TIMER_PRESCALER 1024
#define TIMER_TOP_VALUE 233 // Задержка около 300ms при 16 МГц

// Глобальная переменная для управления состоянием светодиодов
volatile uint8_t current_led = 0;

// Функция инициализации портов
void init_led_ports() {
    // Настраиваем все пины PORTB как выходы
    LED_DDR_B = 0xFF;
    // Настраиваем пины PC0 и PC1 как выходы
    LED_DDR_C = 0x03;

    // Гасим все светодиоды на старте
    LED_PORT_B = 0x00;
    LED_PORT_C = 0x00;
}

// Функция инициализации таймера в режиме CTC
void init_timer_ctc() {
    // Настройка таймера 0 в режиме CTC
    TCCR0A = 0x00; // Очищаем регистр TCCR0A
    TCCR0B = (1 << WGM02) | (1 << CS02) | (1 << CS00); // Режим CTC, предделитель 1024
    OCR0A = TIMER_TOP_VALUE; // Устанавливаем значение сравнения

    // Включаем прерывание по совпадению
    TIMSK0 = (1 << OCIE0A);

    // Разрешаем глобальные прерывания
    sei();
}

// Обработчик прерывания таймера
ISR(TIMER0_COMPA_vect) {
    // Очищаем все светодиоды
    LED_PORT_B = 0x00;
    LED_PORT_C = 0x00;

    // Включаем соответствующий светодиод
    if (current_led < 8) {
        // Включаем светодиод на PORTB
        LED_PORT_B = (1 << current_led);
    } else {
        // Включаем светодиод на PORTC
        LED_PORT_C = (1 << (current_led - 8));
    }

    // Переходим к следующему светодиоду
    current_led = (current_led + 1) % 10; // Цикличный переход
}

// Главная функция
int main() {
    // Инициализация портов и таймера
    init_led_ports();
    init_timer_ctc();

    // Бесконечный цикл (основная программа не делает ничего)
    while (1) {
        // Все действия выполняются в обработчике прерывания
    }
}
