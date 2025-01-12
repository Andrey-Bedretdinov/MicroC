#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL // Установка тактовой частоты микроконтроллера

// Коэффициент перевода значений АЦП в напряжение (вольты)
#define ADC_TO_VOLTAGE_COEFF (5.0 / 1023.0)

// Глобальная переменная для хранения текущего значения напряжения
float current_voltage;

// Инициализация портов и модуля АЦП
void initialize_system() {
    DDRB = 0xFF; // Устанавливаем все пины порта B как выходы
    ADMUX |= (1 << REFS0); // Опорное напряжение: внутреннее (AVcc)
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1); // Включаем АЦП, предделитель 64
}

// Чтение значения АЦП и преобразование его в напряжение
void read_adc_voltage() {
    // Запуск преобразования
    ADCSRA |= (1 << ADSC);
    // Ожидание завершения преобразования
    while (!(ADCSRA & (1 << ADIF)));
    // Считываем результат и преобразуем в напряжение
    uint16_t adc_value = ADC;
    current_voltage = adc_value * ADC_TO_VOLTAGE_COEFF;
    _delay_ms(1); // Небольшая задержка для стабилизации
}

// Преобразование числа в двоичный код для 7-сегментного индикатора
uint8_t convert_to_binary(uint8_t value) {
    uint8_t binary_result = 0;
    uint8_t i;
    for (i = 0; i < 4; ++i) {
        if (value % 2 == 1) {
            binary_result |= (1 << i);
        }
        value /= 2;
    }
    return binary_result;
}

// Отображение текущего напряжения на 7-сегментном индикаторе
void display_voltage_on_indicator() {
    // Разделяем напряжение на целую и дробную части
    uint8_t integer_part = (uint8_t)current_voltage; // Целая часть
    uint8_t fractional_part = (uint8_t)((current_voltage - integer_part) * 10); // Первая цифра после запятой

    // Преобразуем целую и дробную части в двоичный код
    uint8_t integer_binary = convert_to_binary(integer_part);
    uint8_t fractional_binary = convert_to_binary(fractional_part);

    // Объединяем двоичные коды и отправляем их на порт B
    PORTB = (integer_binary << 4) | fractional_binary;
}

// Главная функция
int main() {
    // Инициализация системы
    initialize_system();

    // Бесконечный цикл
    while (1) {
        read_adc_voltage(); // Считываем текущее значение напряжения
        display_voltage_on_indicator(); // Отображаем напряжение на индикаторе
    }
}
