/*
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

#include "systick.h"
#include "stm32h7s3xx_it.h"

/* Private macros ---------------------------------------------------------- */

/* Private constants ------------------------------------------------------- */

/* Private types ----------------------------------------------------------- */

/* Private variables ------------------------------------------------------- */

/* Обработчик SysTick */
struct systick_handle systick = {
    .instance = SysTick,
};

/* Private function prototypes --------------------------------------------- */

/* Private user code ------------------------------------------------------- */

/**
 * @brief           Инициализировать SysTick
 *
 * @param[in]       frequency: Частота тактирования (Гц)
 */
void systick_init(const uint32_t frequency)
{
    systick.init.frequency = frequency;
    systick.init.clksource = SYSTICK_CPU_CLOCK;

    hal_systick_register_callback(&systick,
                                  SYSTICK_PERIOD_ELAPSED_CALLBACK,
                                  SysTick_PeriodElapsedCallback);

    hal_systick_init(&systick);
    hal_systick_start_it(&systick);
}
/* ------------------------------------------------------------------------- */
