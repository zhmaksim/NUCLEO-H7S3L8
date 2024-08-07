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

#include "systick.h"

/* Private macros ---------------------------------------------------------- */

/* Private constants ------------------------------------------------------- */

/* Private types ----------------------------------------------------------- */

/* Private variables ------------------------------------------------------- */

static volatile uint32_t tick;

/* Private function prototypes --------------------------------------------- */

/* Private user code ------------------------------------------------------- */

/**
 * Инициализировать SysTick.
 *
 * @param[in] frequency Частота тактирования (Гц).
 */
void systick_init( const uint32_t frequency )
{
    /* Сбросить регистр управления таймера */
    CLEAR_REG( SysTick->CTRL );

    /* Настроить значение перезагрузки счетчика таймера */
    WRITE_REG( SysTick->LOAD, frequency / 1000 - 1 );

    /* Сбросить текущее значение счетчика таймера */
    CLEAR_REG( SysTick->VAL );

    /* Настроить и запустить таймер */
    WRITE_REG( SysTick->CTRL,
               SysTick_CTRL_CLKSOURCE_Msk       /* Источник тактирования = CPU */
             | SysTick_CTRL_TICKINT_Msk         /* Включить прерывания таймера */
             | SysTick_CTRL_ENABLE_Msk );       /* Включить таймер */
}
/* ------------------------------------------------------------------------- */

/**
 * Обработать прерывания SysTick.
 */
inline void systick_it_handler( void )
{
    /* Если счетчик SysTick достиг нулевого значения -
     * изменяем значение системного таймера,
     * вызываем функцию обратного вызова systick_period_elapsed_callback() */
    if( READ_BIT( SysTick->CTRL, SysTick_CTRL_COUNTFLAG_Msk ) )
    {
        tick++;

        systick_period_elapsed_callback();
    }
}
/* ------------------------------------------------------------------------- */

/**
 * Получить значение системного таймера (1 мс).
 *
 * @return Значение системного таймера.
 */
inline uint32_t systick_tick( void )
{
    return tick;
}
/* ------------------------------------------------------------------------- */

/**
 * Функция обратного вызова - Истек период счетчика SysTick.
 */
__WEAK void systick_period_elapsed_callback( void )
{

}
/* ------------------------------------------------------------------------- */
