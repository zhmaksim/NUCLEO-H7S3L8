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

#ifndef STM32H7S3XX_HAL_GPIO_H_
#define STM32H7S3XX_HAL_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ---------------------------------------------------------------- */

#include "stm32h7s3xx_hal_def.h"

/* Exported macros --------------------------------------------------------- */

/**
 * @brief           Включить тактирование GPIOA
 */
#define HAL_GPIOA_ENABLE_CLOCK() \
    SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOAEN_Msk)

/**
 * @brief           Включить тактирование GPIOB
 */
#define HAL_GPIOB_ENABLE_CLOCK() \
    SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOBEN_Msk)

/**
 * @brief           Включить тактирование GPIOC
 */
#define HAL_GPIOC_ENABLE_CLOCK() \
    SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOCEN_Msk)

/**
 * @brief           Включить тактирование GPIOD
 */
#define HAL_GPIOD_ENABLE_CLOCK() \
    SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIODEN_Msk)

/**
 * @brief           Включить тактирование GPIOE
 */
#define HAL_GPIOE_ENABLE_CLOCK() \
    SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOEEN_Msk)

/**
 * @brief           Включить тактирование GPIOF
 */
#define HAL_GPIOF_ENABLE_CLOCK() \
    SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOFEN_Msk)

/**
 * @brief           Включить тактирование GPIOG
 */
#define HAL_GPIOG_ENABLE_CLOCK() \
    SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOGEN_Msk)

/**
 * @brief           Включить тактирование GPIOH
 */
#define HAL_GPIOH_ENABLE_CLOCK() \
    SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOHEN_Msk)

/**
 * @brief           Включить тактирование GPIOM
 */
#define HAL_GPIOM_ENABLE_CLOCK() \
    SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOMEN_Msk)

/**
 * @brief           Включить тактирование GPION
 */
#define HAL_GPION_ENABLE_CLOCK() \
    SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIONEN_Msk)

/**
 * @brief           Включить тактирование GPIOO
 */
#define HAL_GPIOO_ENABLE_CLOCK() \
    SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOOEN_Msk)

/**
 * @brief           Включить тактирование GPIOP
 */
#define HAL_GPIOP_ENABLE_CLOCK() \
    SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOPEN_Msk)

/* Exported constants ------------------------------------------------------ */

/* Exported types ---------------------------------------------------------- */

/**
 * @brief           Определение структуры данных GPIO
 */
typedef GPIO_TypeDef gpio_t;


/**
 * @brief           Определение перечисления портов ввода-вывода GPIO
 */
enum gpio_pin {
    GPIO_PIN0,
    GPIO_PIN1,
    GPIO_PIN2,
    GPIO_PIN3,
    GPIO_PIN4,
    GPIO_PIN5,
    GPIO_PIN6,
    GPIO_PIN7,
    GPIO_PIN8,
    GPIO_PIN9,
    GPIO_PIN10,
    GPIO_PIN11,
    GPIO_PIN12,
    GPIO_PIN13,
    GPIO_PIN14,
    GPIO_PIN15,
};


/**
 * @brief           Определение перечисления состояний порта ввода-вывода GPIO
 */
enum gpio_state {
    GPIO_RESET,
    GPIO_SET,
};


/**
 * @brief           Определение перечисления режмов работы порта ввода-вывода GPIO
 */
enum gpio_mode {
    GPIO_INPUT,
    GPIO_OUTPUT,
    GPIO_AF,
    GPIO_ANALOG,
};


/**
 * @brief           Определение перечисления типов порта вывода GPIO
 */
enum gpio_output_type {
    GPIO_PUSH_PULL,
    GPIO_OPEN_DRAIN,
};


/**
 * @brief           Определение перечисления скорости работы порта вывода GPIO
 */
enum gpio_output_speed {
    GPIO_LOW_SPEED,
    GPIO_MEDIUM_SPEED,
    GPIO_HIGH_SPEED,
    GPIO_VERY_HIGH_SPEED,
};


/**
 * @brief           Определение перечисления подтяжки сигнала порта ввода-вывода GPIO
 */
enum gpio_pupd {
    GPIO_NO_PULL,
    GPIO_PULL_UP,
    GPIO_PULL_DOWN,
};


/**
 * @brief           Определение структуры данных инициализации GPIO
 */
struct gpio_init {
    uint8_t         mode;                       /*!< Режим работы @ref enum gpio_mode:
                                                    - GPIO_INPUT
                                                    - GPIO_OUTPUT
                                                    - GPIO_AF
                                                    - GPIO_ANALOG */

    uint8_t         otype;                      /*!< Тип вывода @ref enum gpio_output_type:
                                                    - GPIO_PUSH_PULL
                                                    - GPIO_OPEN_DRAIN */

    uint8_t         ospeed;                     /*!< Скорость работы вывода @ref enum gpio_output_speed:
                                                    - GPIO_LOW_SPEED
                                                    - GPIO_MEDIUM_SPEED
                                                    - GPIO_HIGH_SPEED
                                                    - GPIO_VERY_HIGH_SPEED */

    uint8_t         pupd;                       /*!< Подтяжка сигнала @ref enum gpio_pupd:
                                                    - GPIO_NO_PULL
                                                    - GPIO_PULL_UP
                                                    - GPIO_PULL_DOWN */

    uint8_t         af;                         /*!< Альтернативная функция: 0-15 */
};


/**
 * @brief           Определение структуры данных обработчика GPIO
 */
struct gpio_handle {
    gpio_t                 *instance;       /*!< Указатель на структуру данных GPIO */

    uint8_t                 pin;            /*!< Номер порта ввода-вывода GPIO @ref enum gpio_pin:
                                                 - GPIO_PIN0
                                                 - GPIO_PIN1
                                                 ...
                                                 - GPIO_PIN15 */

    struct gpio_init        init;         /*!< Настройки GPIO */
};

/* Exported variables ------------------------------------------------------ */

/* Exported function prototypes -------------------------------------------- */

void hal_gpio_init(struct gpio_handle *handle);

void hal_gpio_set_state(struct gpio_handle *handle, uint8_t state);

uint8_t hal_gpio_state(struct gpio_handle *handle);

void hal_gpio_toggle(struct gpio_handle *handle);

/* Exported callback function prototypes ----------------------------------- */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STM32H7S3XX_HAL_GPIO_H_ */
