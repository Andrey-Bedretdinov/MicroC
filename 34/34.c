// 34. Широтно-импульсная модуляция на таймере (PWM)
// Реализация генератора ШИМ-сигнала с коэффициентом заполнения 0,25
// Вывод сигнала: PB1/OC1A (pin 13)

#include <avr/io.h>

#define F_CPU 8000000UL // Частота тактового генератора (8 МГц)

// Константы для настройки ШИМ
#define PWM_DUTY_CYCLE 64   // Коэффициент заполнения (64/255 ≈ 0.25)
#define PWM_FAST_MODE (1 << WGM10) | (1 << WGM12) // Режим Fast PWM (8 бит)
#define PWM_NON_INVERTING (1 << COM1A1) // Неинвертирующий режим
#define TIMER_NO_PRESCALER (1 << CS10) // Без деления частоты таймера

void pwm_init(void);

int main(void) {
    pwm_init(); // Инициализация ШИМ
    while (1);  // Бесконечный цикл
}

// Функция для инициализации ШИМ на выводе PB1
void pwm_init(void) {
    DDRB |= (1 << PB1); // Настройка порта PB1 как выход
    TCCR1A = PWM_NON_INVERTING | PWM_FAST_MODE; // Настройка режима работы таймера
    TCCR1B = TIMER_NO_PRESCALER | PWM_FAST_MODE; // Настройка делителя частоты и режима
    OCR1A = PWM_DUTY_CYCLE; // Установка коэффициента заполнения
}
