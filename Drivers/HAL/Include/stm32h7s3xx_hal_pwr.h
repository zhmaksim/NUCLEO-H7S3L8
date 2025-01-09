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

#ifndef STM32H7S3XX_HAL_PWR_H_
#define STM32H7S3XX_HAL_PWR_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ---------------------------------------------------------------- */

#include "stm32h7s3xx_hal_def.h"

/* Exported macros --------------------------------------------------------- */

/* Exported constants ------------------------------------------------------ */

/* Exported types ---------------------------------------------------------- */

/**
 * @brief           Определение структуры данных PWR
 */
typedef PWR_TypeDef pwr_t;


/**
 * @brief           Определение перечисления конфигурации источника питания PWR
 */
enum pwr_supply {
    PWR_DEFAULT_SUPPLY = 0x06,
    PWR_LDO = 0x02,
    PWR_DIRECT_SMPS = 0x04,
    PWR_SMPS_EXTERNAL_LDO_EXTERNAL = 0x1E,
    PWR_SMPS_EXTERNAL_LDO_BYPASS = 0x1D,
    PWR_SMPS_DISABLED_LDO_BYPASS = 0x01,
};


/**
 * @brief           Определение перечисления масштабирования напряжения PWR
 */
enum pwr_vos {
    PWR_VOS_LOW,
    PWR_VOS_HIGH,
};


/**
 * @brief           Определение структуры данных инициализации PWR
 */
struct pwr_init {
    uint8_t         supply;                     /*!< Конфигурация источника питания @ref enum pwr_supply:
                                                    - PWR_DEFAULT_SUPPLY
                                                    - PWR_LDO
                                                    - PWR_DIRECT_SMPS
                                                    - PWR_SMPS_EXTERNAL_LDO_EXTERNAL
                                                    - PWR_SMPS_EXTERNAL_LDO_BYPASS
                                                    - PWR_SMPS_DISABLED_LDO_BYPASS */

    uint8_t         vos;                        /*!< Масштабирование напряжения @ref enum pwr_vos:
                                                    - PWR_VOS_LOW
                                                    - PWR_VOS_HIGH */
};


/**
 * @brief           Определение структуры данных обработчика PWR
 */
struct pwr_handle {
    pwr_t                  *instance;           /*!< Указатель на структуру данных PWR */

    struct pwr_init         init;               /*!< Настройки PWR */
};

/* Exported variables ------------------------------------------------------ */

/* Exported function prototypes -------------------------------------------- */

void hal_pwr_init(struct pwr_handle *handle);

/* Exported callback function prototypes ----------------------------------- */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STM32H7S3XX_HAL_PWR_H_ */
