/**
 * Copyright (C) 2025 zhmaksim <zhiharev.maxim.alexandrovich@yandex.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/* Includes ---------------------------------------------------------------- */

#include "stm32h7s3xx_hal_gpio.h"

/* Private macros ---------------------------------------------------------- */

/* Private constants ------------------------------------------------------- */

/* Private types ----------------------------------------------------------- */

/* Private variables ------------------------------------------------------- */

/* Private function prototypes --------------------------------------------- */

static void hal_gpio_setup_mode(gpio_t *instance, uint8_t pin, uint8_t mode);

static void hal_gpio_setup_output_type(gpio_t *instance, uint8_t pin, uint8_t otype);

static void hal_gpio_setup_output_speed(gpio_t *instance, uint8_t pin, uint8_t ospeed);

static void hal_gpio_setup_pupd(gpio_t *instance, uint8_t pin, uint8_t pupd);

static void hal_gpio_setup_af(gpio_t *instance, uint8_t pin, uint8_t af);

/* Private user code ------------------------------------------------------- */

/**
 * @brief           Инициализировать GPIO
 *
 * @param[in]       handle: Указатель на структуру данных обработчика GPIO
 */
void hal_gpio_init(struct gpio_handle *handle)
{
    assert(handle != NULL);
    assert(handle->instance != NULL);

    hal_gpio_setup_mode(handle->instance, handle->pin, handle->init.mode);
    hal_gpio_setup_output_type(handle->instance, handle->pin, handle->init.otype);
    hal_gpio_setup_output_speed(handle->instance, handle->pin, handle->init.ospeed);
    hal_gpio_setup_pupd(handle->instance, handle->pin, handle->init.pupd);
    hal_gpio_setup_af(handle->instance, handle->pin, handle->init.af);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить режим работы GPIO
 *
 * @param[in]       instance: Указатель на структуру данных GPIO
 * @param[in]       pin: Номер порта ввода-вывода GPIO @ref enum gpio_pin
 * @param[in]       mode: Режим работы GPIO @ref enum gpio_mode
 */
static void hal_gpio_setup_mode(gpio_t *instance, uint8_t pin, uint8_t mode)
{
    MODIFY_REG(instance->MODER,
               HAL_BITMASK(0x03, pin * 2),
               HAL_BITMASK(mode, pin * 2));
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить тип вывода GPIO
 *
 * @param[in]       instance: Указатель на структуру данных GPIO
 * @param[in]       pin: Номер порта ввода-вывода GPIO @ref enum gpio_pin
 * @param[in]       otype: Тип вывода GPIO @ref enum gpio_output_type
 */
static void hal_gpio_setup_output_type(gpio_t *instance, uint8_t pin, uint8_t otype)
{
    MODIFY_REG(instance->OTYPER,
               HAL_BITMASK(0x01, pin * 1),
               HAL_BITMASK(otype, pin * 1));
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить скорость работы вывода GPIO
 *
 * @param[in]       instance: Указатель на структуру данных GPIO
 * @param[in]       pin: Номер порта ввода-вывода GPIO @ref enum gpio_pin
 * @param[in]       ospeed: Скорость работы вывода GPIO @ref enum gpio_otuput_speed
 */
static void hal_gpio_setup_output_speed(gpio_t *instance, uint8_t pin, uint8_t ospeed)
{
    MODIFY_REG(instance->OSPEEDR,
               HAL_BITMASK(0x03, pin * 2),
               HAL_BITMASK(ospeed, pin * 2));
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить подтяжку сигнала GPIO
 *
 * @param[in]       instance: Указатель на структуру данных GPIO
 * @param[in]       pin: Номер порта ввода-вывода GPIO @ref enum gpio_pin
 * @param[in]       pupd: Подтяжка сигнала GPIO @ref enum gpio_pupd
 */
static void hal_gpio_setup_pupd(gpio_t *instance, uint8_t pin, uint8_t pupd)
{
    MODIFY_REG(instance->PUPDR,
               HAL_BITMASK(0x03, pin * 2),
               HAL_BITMASK(pupd, pin * 2));
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить альтернативную функцию GPIO
 *
 * @param[in]       instance: Указатель на структуру данных GPIO
 * @param[in]       pin: Номер порта ввода-вывода GPIO @ref enum gpio_pin
 * @param[in]       af: Номер альтернативной функции GPIO
 */
static void hal_gpio_setup_af(gpio_t *instance, uint8_t pin, uint8_t af)
{
    if (pin < GPIO_PIN8) {
        MODIFY_REG(instance->AFR[0],
                   HAL_BITMASK(0x0F, pin * 4),
                   HAL_BITMASK(af, pin * 4));
    } else {
        MODIFY_REG(instance->AFR[1],
                   HAL_BITMASK(0x0F, (pin - 8) * 4),
                   HAL_BITMASK(af, (pin - 8) * 4));
    }
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить состояние порта ввода-вывода GPIO
 *
 * @param[in]       handle: Указатель на структуру данных обработчика GPIO
 * @param[in]       state: Состояние порта ввода-вывода GPIO @ref enum gpio_state
 */
void hal_gpio_set_state(struct gpio_handle *handle, uint8_t state)
{
    assert(handle != NULL);
    assert(handle->instance != NULL);

    if (state == GPIO_SET) {
        SET_BIT(handle->instance->BSRR, HAL_BITMASK(0x01, handle->pin));
    } else {
        SET_BIT(handle->instance->BSRR, HAL_BITMASK(0x10000, handle->pin));
    }
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Состояние порта ввода-вывода GPIO
 *
 * @param[in]       handle: Указатель на структуру данных обработчика GPIO
 * @return          Состояние порта ввода-вывода GPIO @ref enum gpio_state
 */
uint8_t hal_gpio_state(struct gpio_handle *handle)
{
    assert(handle != NULL);
    assert(handle->instance != NULL);

    return READ_BIT(handle->instance->IDR, HAL_BITMASK(0x01, handle->pin)) ?
            GPIO_SET : GPIO_RESET;
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Переключить уровень порта ввода-вывода GPIO
 *
 * @param[in]       handle: Указатель на структуру данных обработчика GPIO
 */
void hal_gpio_toggle(struct gpio_handle *handle)
{
    assert(handle != NULL);
    assert(handle->instance != NULL);

    XOR_BIT(handle->instance->ODR, HAL_BITMASK(0x01, handle->pin));
}
/* ------------------------------------------------------------------------- */
