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

#include "gpio.h"

/* Private macros ---------------------------------------------------------- */

/* Private constants ------------------------------------------------------- */

/* Private types ----------------------------------------------------------- */

/* Private variables ------------------------------------------------------- */

/* Private function prototypes --------------------------------------------- */

static void gpio_octospi_init(void);

/* Private user code ------------------------------------------------------- */

/**
 * @brief           Инициализировать GPIO
 */
void gpio_init(void)
{
    HAL_GPIOA_ENABLE_CLOCK();
    HAL_GPION_ENABLE_CLOCK();

    gpio_octospi_init();
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Инициализировать GPOI OCTOSPI
 */
static void gpio_octospi_init(void)
{
    /*
     * NCS GPION1
     * DQS GPION0
     * CLK GPION6
     * IO0 GPION2
     * IO1 GPION3
     * IO2 GPION4
     * IO3 GPION5
     * IO4 GPION8
     * IO5 GPION9
     * IO6 GPION10
     * IO7 GPION11
     */

    static const struct gpio_init init = {
         .mode = GPIO_AF,
         .otype = GPIO_PUSH_PULL,
         .ospeed = GPIO_VERY_HIGH_SPEED,
         .pupd = GPIO_NO_PULL,
         .af = 9,
    };

    struct gpio_handle gpio_octospi_ncs = { .instance = GPION, .pin = GPIO_PIN1  };
    struct gpio_handle gpio_octospi_dqs = { .instance = GPION, .pin = GPIO_PIN0  };
    struct gpio_handle gpio_octospi_clk = { .instance = GPION, .pin = GPIO_PIN6  };
    struct gpio_handle gpio_octospi_io0 = { .instance = GPION, .pin = GPIO_PIN2  };
    struct gpio_handle gpio_octospi_io1 = { .instance = GPION, .pin = GPIO_PIN3  };
    struct gpio_handle gpio_octospi_io2 = { .instance = GPION, .pin = GPIO_PIN4  };
    struct gpio_handle gpio_octospi_io3 = { .instance = GPION, .pin = GPIO_PIN5  };
    struct gpio_handle gpio_octospi_io4 = { .instance = GPION, .pin = GPIO_PIN8  };
    struct gpio_handle gpio_octospi_io5 = { .instance = GPION, .pin = GPIO_PIN9  };
    struct gpio_handle gpio_octospi_io6 = { .instance = GPION, .pin = GPIO_PIN10 };
    struct gpio_handle gpio_octospi_io7 = { .instance = GPION, .pin = GPIO_PIN11 };

    gpio_octospi_ncs.init = init;
    gpio_octospi_dqs.init = init;
    gpio_octospi_clk.init = init;
    gpio_octospi_io0.init = init;
    gpio_octospi_io1.init = init;
    gpio_octospi_io2.init = init;
    gpio_octospi_io3.init = init;
    gpio_octospi_io4.init = init;
    gpio_octospi_io5.init = init;
    gpio_octospi_io6.init = init;
    gpio_octospi_io7.init = init;

    hal_gpio_init(&gpio_octospi_ncs);
    hal_gpio_init(&gpio_octospi_dqs);
    hal_gpio_init(&gpio_octospi_clk);
    hal_gpio_init(&gpio_octospi_io0);
    hal_gpio_init(&gpio_octospi_io1);
    hal_gpio_init(&gpio_octospi_io2);
    hal_gpio_init(&gpio_octospi_io3);
    hal_gpio_init(&gpio_octospi_io4);
    hal_gpio_init(&gpio_octospi_io5);
    hal_gpio_init(&gpio_octospi_io6);
    hal_gpio_init(&gpio_octospi_io7);
}
/* ------------------------------------------------------------------------- */
