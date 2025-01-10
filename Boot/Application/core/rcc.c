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

#include "rcc.h"

/* Private macros ---------------------------------------------------------- */

/* Private constants ------------------------------------------------------- */

/* Private types ----------------------------------------------------------- */

/* Private variables ------------------------------------------------------- */

/* Обработчик RCC */
struct rcc_handle rcc = {
    .instance = RCC,
};

/* Private function prototypes --------------------------------------------- */

/* Private user code ------------------------------------------------------- */

/**
 * @brief           Инициализировать RCC
 */
void rcc_init(void)
{
    rcc.init.hse_enable = HAL_ENABLE;
    rcc.init.hse_css_enable = HAL_ENABLE;

    rcc.init.pll_clksource = RCC_PLL_CLOCK_SOURCE_HSE;      /* HSE = 24MHz */
    rcc.init.pll_divm1 = 12;                                /* DIVM1 = 24MHz / 12 = 2MHz */
    rcc.init.pll_divm2 = 4;                                 /* DIVM2 = 24MHz / 4 = 6MHz */

    rcc.init.pll1.enable = HAL_ENABLE;
    rcc.init.pll1.vco = RCC_PLL_VCOH;
    rcc.init.pll1.rge = RCC_PLL_RGE_1_2MHZ;
    rcc.init.pll1.divn = 300;                               /* DIVN = 2MHz * 300 = 600MHz */
    rcc.init.pll1.divp = 1;                                 /* DIVP = 600MHz / 1 = 600MHz */

    rcc.init.pll2.enable = HAL_ENABLE;
    rcc.init.pll2.vco = RCC_PLL_VCOH;
    rcc.init.pll2.rge = RCC_PLL_RGE_4_8MHZ;
    rcc.init.pll2.divn = 100;                               /* DIVN = 6MHz * 100 = 600MHz */
    rcc.init.pll2.divt = 3;                                 /* DIVT = 600MHz / 3 = 200MHz */

    rcc.init.cpu_div = RCC_CPU_NOT_DIV;
    rcc.init.bus_div = RCC_BUS_DIV2;
    rcc.init.apb1_div = RCC_APB_DIV2;
    rcc.init.apb2_div = RCC_APB_DIV2;
    rcc.init.apb4_div = RCC_APB_DIV2;
    rcc.init.apb5_div = RCC_APB_DIV2;
    rcc.init.cpu_clksource = RCC_CPU_CLOCK_SOURCE_PLL1;

    rcc.cpu_clock = 600000000;
    rcc.bus_clock = 300000000;
    rcc.apb1_clock = 150000000;
    rcc.apb2_clock = 150000000;
    rcc.apb4_clock = 150000000;
    rcc.apb5_clock = 150000000;

    hal_rcc_init(&rcc);
}
/* ------------------------------------------------------------------------- */
