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

#ifndef STM32H7S3XX_HAL_RCC_H_
#define STM32H7S3XX_HAL_RCC_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ---------------------------------------------------------------- */

#include "stm32h7s3xx_hal_def.h"

/* Exported macros --------------------------------------------------------- */

/* Exported constants ------------------------------------------------------ */

/* Exported types ---------------------------------------------------------- */

/**
 * @brief           Определение структуры данных RCC
 */
typedef RCC_TypeDef rcc_t;


/**
 * @brief           Определение перечисления источника тактирования PLL
 */
enum rcc_pll_clock_source {
    RCC_PLL_CLOCK_SOURCE_HSI,
    RCC_PLL_CLOCK_SOURCE_CSI,
    RCC_PLL_CLOCK_SOURCE_HSE,
    RCC_PLL_NO_CLOCK_SOURCE,
};


/**
 * @brief           Определение перечисления диапазонов PLL VCO
 */
enum rcc_pll_vco {
    RCC_PLL_VCOH,                               /*!< 384MHz - 1672MHz (Fref1_ck = 2MHz - 16MHz) */
    RCC_PLL_VCOL,                               /*!< 150MHz - 420MHz (Fref1_ck = 1MHz - 2MHz) */
};


/**
 * @brief           Определение перечисления диапазонов PLL RGE
 */
enum rcc_pll_rge {
    RCC_PLL_RGE_1_2MHZ,
    RCC_PLL_RGE_2_4MHZ,
    RCC_PLL_RGE_4_8MHZ,
    RCC_PLL_RGE_8_16MHZ,
};


/**
 * @brief           Определение перечисления делителей CPU
 */
enum rcc_cpu_div {
    RCC_CPU_NOT_DIV,
    RCC_CPU_DIV2 = 0x08,
    RCC_CPU_DIV4,
    RCC_CPU_DIV8,
    RCC_CPU_DIV16,
    RCC_CPU_DIV64,
    RCC_CPU_DIV128,
    RCC_CPU_DIV256,
    RCC_CPU_DIV512,
};


/**
 * @brief           Определение перечисления делителей BUS Matrix
 */
enum rcc_bus_div {
    RCC_BUS_NOT_DIV,
    RCC_BUS_DIV2 = 0x08,
    RCC_BUS_DIV4,
    RCC_BUS_DIV8,
    RCC_BUS_DIV16,
    RCC_BUS_DIV64,
    RCC_BUS_DIV128,
    RCC_BUS_DIV256,
    RCC_BUS_DIV512,
};


/**
 * @brief           Определение перечисления делителей APB
 */
enum rcc_apb_div {
    RCC_APB_NOT_DIV,
    RCC_APB_DIV2 = 0x04,
    RCC_APB_DIV4,
    RCC_APB_DIV8,
    RCC_APB_DIV16,
};


/**
 * @brief           Определение перечисления источников тактирования CPU
 */
enum rcc_cpu_clock_source {
    RCC_CPU_CLOCK_SOURCE_HSI,
    RCC_CPU_CLOCK_SOURCE_CSI,
    RCC_CPU_CLOCK_SOURCE_HSE,
    RCC_CPU_CLOCK_SOURCE_PLL1,
};


/**
 * @brief           Определение структуры данных PLL
 */
struct rcc_pll {
    uint8_t         enable;                     /*!< Включить PLL @ref hal_state_t:
                                                    - HAL_ENABLE
                                                    - HAL_DISABLE */

    uint8_t         vco;                        /*!< Диапазон PLL VCO @ref enum rcc_pll_vco:
                                                    - RCC_PLL_VCOH
                                                    - RCC_PLL_VCOL */

    uint8_t         rge;                        /*!< Диапазон PLL RGE @ref enum rcc_pll_rge:
                                                    - RCC_PLL_RGE_1_2MHZ
                                                    - RCC_PLL_RGE_2_4MHZ
                                                    - RCC_PLL_RGE_4_8MHZ
                                                    - RCC_PLL_RGE_8_16MHZ */

    uint16_t        divn;                       /*!< Множитель PLL DIVN: 8 <= DIVN <= 420 */

    uint16_t        divp;                       /*!< Делитель PLL DIVP: 1 <= DIVP 128 */

    uint16_t        divq;                       /*!< Делитель PLL DIVQ: 1 <= DIVQ 128 */

    uint16_t        divr;                       /*!< Делитель PLL DIVR: 1 <= DIVR 128 */

    uint16_t        divs;                       /*!< Делитель PLL DIVS: 1 <= DIVS 8 */

    uint16_t        divt;                       /*!< Делитель PLL DIVR: 1 <= DIVT 8 */

    uint16_t        fracn;                      /*!< Дробная часть коэффициента умножения для PLL VCO */
};


/**
 * @brief           Определение структуры данных инициализации RCC
 */
struct rcc_init {
    uint8_t                 hse_enable;         /*!< Включить HSE @ref hal_state_t:
                                                    - HAL_ENABLE
                                                    - HAL_DISABLE */

    uint8_t                 hse_css_enable;     /*!< Включить HSE CSS @ref hal_state_t:
                                                    - HAL_ENABLE
                                                    - HAL_DISABLE */

    uint8_t                 pll_clksource;      /*!< Источник тактирования PLL @ref enum rcc_pll_clock_source:
                                                    - RCC_PLL_CLOCK_SOURCE_HSI
                                                    - RCC_PLL_CLOCK_SOURCE_CSI
                                                    - RCC_PLL_CLOCK_SOURCE_HSE
                                                    - RCC_PLL_NO_CLOCK_SOURCE (если не используется) */

    uint8_t                 pll_divm1;          /*!< Делитель PLL DIVM1:
                                                    0 - Выключен
                                                    1 - Bypass
                                                    2 - /2
                                                    3 - /3
                                                    ...
                                                    63 - /63 */

    uint8_t                 pll_divm2;          /*!< Делитель PLL DIVM2:
                                                    0 - Выключен
                                                    1 - Bypass
                                                    2 - /2
                                                    3 - /3
                                                    ...
                                                    63 - /63 */

    uint8_t                 pll_divm3;          /*!< Делитель PLL DIVM3:
                                                    0 - Выключен
                                                    1 - Bypass
                                                    2 - /2
                                                    3 - /3
                                                    ...
                                                    63 - /63 */

    struct rcc_pll          pll1;               /*!< Данные PLL1 */

    struct rcc_pll          pll2;               /*!< Данные PLL2 */

    struct rcc_pll          pll3;               /*!< Данные PLL3 */

    uint8_t                 cpu_div;            /*!< Делитель CPU @ref enum rcc_cpu_div:
                                                    - RCC_CPU_NOT_DIV
                                                    - RCC_CPU_DIV2
                                                    ...
                                                    - RCC_CPU_DIV512 */

    uint8_t                 bus_div;            /*!< Делитель BUS Matrix @ref enum rcc_bus_div:
                                                    - RCC_BUS_NOT_DIV
                                                    - RCC_BUS_DIV2
                                                    ...
                                                    - RCC_BUS_DIV512 */

    uint8_t                 apb1_div;           /*!< Делитель APB1 @ref enum rcc_apb_div:
                                                    - RCC_APB_NOT_DIV
                                                    - RCC_APB_DIV2
                                                    ...
                                                    - RCC_APB_DIV16 */

    uint8_t                 apb2_div;           /*!< Делитель APB2 @ref enum rcc_apb_div:
                                                    - RCC_APB_NOT_DIV
                                                    - RCC_APB_DIV2
                                                    ...
                                                    - RCC_APB_DIV16 */

    uint8_t                 apb4_div;           /*!< Делитель APB4 @ref enum rcc_apb_div:
                                                    - RCC_APB_NOT_DIV
                                                    - RCC_APB_DIV2
                                                    ...
                                                    - RCC_APB_DIV16 */

    uint8_t                 apb5_div;           /*!< Делитель APB5 @ref enum rcc_apb_div:
                                                    - RCC_APB_NOT_DIV
                                                    - RCC_APB_DIV2
                                                    ...
                                                    - RCC_APB_DIV16 */

    uint8_t                 cpu_clksource;      /*!< Источник тактирования CPU @ref enum rcc_cpu_clock_source:
                                                    - RCC_CPU_CLOCK_SOURCE_HSI
                                                    - RCC_CPU_CLOCK_SOURCE_CSI
                                                    - RCC_CPU_CLOCK_SOURCE_HSE
                                                    - RCC_CPU_CLOCK_SOURCE_PLL1 */
};


/**
 * @brief           Определение структуры данных обработчика RCC
 */
struct rcc_handle {
    rcc_t                  *instance;           /*!< Указатель на структуру данных обработчика RCC */

    struct rcc_init         init;               /*!< Настройки RCC */

    uint32_t                cpu_clock;          /*!< Частота тактирования CPU */

    uint32_t                bus_clock;          /*!< Частота тактирования BUS Matrix */

    uint32_t                apb1_clock;         /*!< Частота тактирования APB1 */

    uint32_t                apb2_clock;         /*!< Частота тактирования APB2 */

    uint32_t                apb4_clock;         /*!< Частота тактирования APB4 */

    uint32_t                apb5_clock;         /*!< Частота тактирования APB5 */
};


/* Exported variables ------------------------------------------------------ */

/* Exported function prototypes -------------------------------------------- */

void hal_rcc_init(struct rcc_handle *handle);

/* Exported callback function prototypes ----------------------------------- */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STM32H7S3XX_HAL_RCC_H_ */
