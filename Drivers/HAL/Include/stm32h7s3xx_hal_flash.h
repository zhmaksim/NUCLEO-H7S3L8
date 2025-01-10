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

#ifndef STM32H7S3XX_HAL_FLASH_H_
#define STM32H7S3XX_HAL_FLASH_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ---------------------------------------------------------------- */

#include "stm32h7s3xx_hal_def.h"

/* Exported macros --------------------------------------------------------- */

/* Exported constants ------------------------------------------------------ */

/* Exported types ---------------------------------------------------------- */

/**
 * @brief           Определение структуры данных FLASH
 */
typedef FLASH_TypeDef flash_t;


/**
 * @brief           Определение перечисления задержки чтения FLASH
 */
enum flash_latency {
    FLASH_LATENCY_0WS,
    FLASH_LATENCY_1WS,
    FLASH_LATENCY_2WS,
    FLASH_LATENCY_3WS,
    FLASH_LATENCY_4WS,
    FLASH_LATENCY_5WS,
    FLASH_LATENCY_6WS,
    FLASH_LATENCY_7WS,
};


/**
 * @brief           Определение перечисления задержки записи FLASH
 */
enum flash_wrhighfreq {
    FLASH_WRHIGHFREQ0,
    FLASH_WRHIGHFREQ1,
    FLASH_WRHIGHFREQ2,
    FLASH_WRHIGHFREQ3,
};


/**
 * @brief           Определение структуры данных инициализации FLASH
 */
struct flash_init {
    uint8_t         latency;                    /*!< Задержка чтения @ref enum flash_latency:
                                                    - FLASH_LATENCY_0WS
                                                    - FLASH_LATENCY_1WS
                                                    ...
                                                    - FLASH_LATENCY_7WS */

    uint8_t         wrhighfreq;                 /*!< Задержка записи @ref enum flash_wrhighfreq:
                                                    - FLASH_WRHIGHFREQ0
                                                    - FLASH_WRHIGHFREQ1
                                                    - FLASH_WRHIGHFREQ2
                                                    - FLASH_WRHIGHFREQ3 */
};


/**
 * @brief           Определение структуры данных обработчика FLASH
 */
struct flash_handle {
    flash_t                *instance;           /*!< Указатель на структуру данных FLASH */

    struct flash_init       init;               /*!< Настройки FLASH */
};

/* Exported variables ------------------------------------------------------ */

/* Exported function prototypes -------------------------------------------- */

void hal_flash_init(struct flash_handle *handle);

/* Exported callback function prototypes ----------------------------------- */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STM32H7S3XX_HAL_FLASH_H_ */
