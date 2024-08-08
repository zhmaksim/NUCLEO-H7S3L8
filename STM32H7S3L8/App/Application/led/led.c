/**
 * Copyright (C) 2024 zhmaksim <zhiharev.maxim.alexandrovich@yandex.ru>
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

#include "led.h"

/* Private macros ---------------------------------------------------------- */

/* Private constants ------------------------------------------------------- */

/* Private types ----------------------------------------------------------- */

/* Private variables ------------------------------------------------------- */

struct led led[ LED_COUNT ];

/* Private function prototypes --------------------------------------------- */

/* Private user code ------------------------------------------------------- */

/**
 * Инициализировать светодиоды.
 */
void led_init( void )
{
    /* Green */
    led[ LED_GREEN ].gpio = GPIOD;
    led[ LED_GREEN ].pin = GPIO_ODR_OD10;

    /* Yellow */
    led[ LED_YELLOW ].gpio = GPIOD;
    led[ LED_YELLOW ].pin = GPIO_ODR_OD13;

    /* Red */
    led[ LED_RED ].gpio = GPIOB;
    led[ LED_RED ].pin = GPIO_ODR_OD7;
}
/* ------------------------------------------------------------------------- */

/**
 * Включить светодиод.
 *
 * @param[in] id Идентификатор светодиода.
 */
void led_on( uint8_t id )
{
    assert( id < LED_COUNT );

    SET_BIT( led[ id ].gpio->BSRR, led[ id ].pin );
}
/* ------------------------------------------------------------------------- */

/**
 * Выключить светодиод.
 *
 * @param[in] id Идентификатор светодиода.
 */
void led_off( uint8_t id )
{
    assert( id < LED_COUNT );

    SET_BIT( led[ id ].gpio->BSRR, led[ id ].pin << 16 );
}
/* ------------------------------------------------------------------------- */

/**
 * Переключить состояние светодиода.
 *
 * @param[in] id Идентификатор светодиода.
 */
void led_toggle( uint8_t id )
{
    assert( id < LED_COUNT );

    XOR_BIT( led[ id ].gpio->ODR, led[ id ].pin );
}
/* ------------------------------------------------------------------------- */
