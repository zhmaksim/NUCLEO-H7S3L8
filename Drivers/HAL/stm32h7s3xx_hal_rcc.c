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

#include "stm32h7s3xx_hal_rcc.h"
#include "stm32h7s3xx_hal_systick.h"

/* Private macros ---------------------------------------------------------- */

/* Private constants ------------------------------------------------------- */

#define HAL_RCC_HSERDY_TIMEOUT      100

#define HAL_RCC_LSERDY_TIMEOUT      5000

/* Private types ----------------------------------------------------------- */

/* Private variables ------------------------------------------------------- */

/* Обработчик SysTick */
extern struct systick_handle systick;

/* Private function prototypes --------------------------------------------- */

static void hal_rcc_setup_hse(rcc_t *instance, uint8_t state);

static void hal_rcc_setup_hse_css(rcc_t *instance, uint8_t state);

static void hal_rcc_setup_pll1(rcc_t *instance, uint8_t state);

static void hal_rcc_setup_pll2(rcc_t *instance, uint8_t state);

static void hal_rcc_setup_pll3(rcc_t *instance, uint8_t state);

static void hal_rcc_setup_pll_clock_source(rcc_t *instance, uint8_t clksource);

static void hal_rcc_setup_pll_divm1(rcc_t *instance, uint8_t div);

static void hal_rcc_setup_pll_divm2(rcc_t *instance, uint8_t div);

static void hal_rcc_setup_pll_divm3(rcc_t *instance, uint8_t div);

static void hal_rcc_pll1_init(rcc_t *instance, const struct rcc_pll *pll);

static void hal_rcc_pll1_setup_vco(rcc_t *instance, uint8_t vco);

static void hal_rcc_pll1_setup_rge(rcc_t *instance, uint8_t rge);

static void hal_rcc_pll1_setup_divn(rcc_t *instance, uint16_t div);

static void hal_rcc_pll1_setup_divp(rcc_t *instance, uint16_t div);

static void hal_rcc_pll1_setup_divq(rcc_t *instance, uint16_t div);

static void hal_rcc_pll1_setup_divr(rcc_t *instance, uint16_t div);

static void hal_rcc_pll1_setup_divs(rcc_t *instance, uint16_t div);

static void hal_rcc_pll1_setup_fractional(rcc_t *instance, uint16_t fracn);

static void hal_rcc_pll2_init(rcc_t *instance, const struct rcc_pll *pll);

static void hal_rcc_pll2_setup_vco(rcc_t *instance, uint8_t vco);

static void hal_rcc_pll2_setup_rge(rcc_t *instance, uint8_t rge);

static void hal_rcc_pll2_setup_divn(rcc_t *instance, uint16_t div);

static void hal_rcc_pll2_setup_divp(rcc_t *instance, uint16_t div);

static void hal_rcc_pll2_setup_divq(rcc_t *instance, uint16_t div);

static void hal_rcc_pll2_setup_divr(rcc_t *instance, uint16_t div);

static void hal_rcc_pll2_setup_divs(rcc_t *instance, uint16_t div);

static void hal_rcc_pll2_setup_divt(rcc_t *instance, uint16_t div);

static void hal_rcc_pll2_setup_fractional(rcc_t *instance, uint16_t fracn);

static void hal_rcc_pll3_init(rcc_t *instance, const struct rcc_pll *pll);

static void hal_rcc_pll3_setup_vco(rcc_t *instance, uint8_t vco);

static void hal_rcc_pll3_setup_rge(rcc_t *instance, uint8_t rge);

static void hal_rcc_pll3_setup_divn(rcc_t *instance, uint16_t div);

static void hal_rcc_pll3_setup_divp(rcc_t *instance, uint16_t div);

static void hal_rcc_pll3_setup_divq(rcc_t *instance, uint16_t div);

static void hal_rcc_pll3_setup_divr(rcc_t *instance, uint16_t div);

static void hal_rcc_pll3_setup_divs(rcc_t *instance, uint16_t div);

static void hal_rcc_pll3_setup_fractional(rcc_t *instance, uint16_t fracn);

static void hal_rcc_setup_cpu_div(rcc_t *instance, uint8_t div);

static void hal_rcc_setup_bus_div(rcc_t *instance, uint8_t div);

static void hal_rcc_setup_apb1_div(rcc_t *instance, uint8_t div);

static void hal_rcc_setup_apb2_div(rcc_t *instance, uint8_t div);

static void hal_rcc_setup_apb4_div(rcc_t *instance, uint8_t div);

static void hal_rcc_setup_apb5_div(rcc_t *instance, uint8_t div);

static void hal_rcc_setup_cpu_clock_source(rcc_t *instance, uint8_t clksource);

/* Private user code ------------------------------------------------------- */

/**
 * @brief           Инициализировать RCC
 *
 * @param[in]       handle: Указатель на структуру данных обработчика RCC
 */
void hal_rcc_init(struct rcc_handle *handle)
{
    assert(handle != NULL);
    assert(handle->instance != NULL);

    /* Настроить HSE */
    hal_rcc_setup_hse(handle->instance, handle->init.hse_enable);
    hal_rcc_setup_hse_css(handle->instance, handle->init.hse_css_enable);

    /* Настроить PLL */
    hal_rcc_setup_pll1(handle->instance, DISABLE);
    hal_rcc_setup_pll2(handle->instance, DISABLE);
    hal_rcc_setup_pll3(handle->instance, DISABLE);
    hal_rcc_setup_pll_clock_source(handle->instance, handle->init.pll_clksource);
    hal_rcc_setup_pll_divm1(handle->instance, handle->init.pll_divm1);
    hal_rcc_setup_pll_divm2(handle->instance, handle->init.pll_divm2);
    hal_rcc_setup_pll_divm3(handle->instance, handle->init.pll_divm3);
    hal_rcc_pll1_init(handle->instance, (struct rcc_pll *) &handle->init.pll1);
    hal_rcc_pll2_init(handle->instance, (struct rcc_pll *) &handle->init.pll2);
    hal_rcc_pll3_init(handle->instance, (struct rcc_pll *) &handle->init.pll3);

    /* Настроить тактирование CPU, BUS, CPU CLKSOURCE */
    hal_rcc_setup_cpu_div(handle->instance, handle->init.cpu_div);
    hal_rcc_setup_bus_div(handle->instance, handle->init.bus_div);
    hal_rcc_setup_apb1_div(handle->instance, handle->init.apb1_div);
    hal_rcc_setup_apb2_div(handle->instance, handle->init.apb2_div);
    hal_rcc_setup_apb4_div(handle->instance, handle->init.apb4_div);
    hal_rcc_setup_apb5_div(handle->instance, handle->init.apb5_div);
    hal_rcc_setup_cpu_clock_source(handle->instance, handle->init.cpu_clksource);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить состояние HSE
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       state: Состояние @ref hal_state_t
 */
static void hal_rcc_setup_hse(rcc_t *instance, uint8_t state)
{
    MODIFY_REG(instance->CR,
               RCC_CR_HSEON_Msk,
               state << RCC_CR_HSEON_Pos);

    if (state == HAL_ENABLE) {
        uint32_t tickstart = hal_systick_tick(&systick);

        while (!READ_BIT(instance->CR, RCC_CR_HSERDY_Msk)) {
            if (hal_systick_tick(&systick) - tickstart >= HAL_RCC_HSERDY_TIMEOUT) {
                hal_error();
            }
        }
    }
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить состояние HSE CSS
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       state: Состояние @ref hal_state_t
 */
static void hal_rcc_setup_hse_css(rcc_t *instance, uint8_t state)
{
    MODIFY_REG(instance->CR,
               RCC_CR_HSECSSON_Msk,
               state << RCC_CR_HSECSSON_Pos);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить состояние PLL1
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       state: Состояние @ref hal_state_t
 */
static void hal_rcc_setup_pll1(rcc_t *instance, uint8_t state)
{
    MODIFY_REG(instance->CR,
               RCC_CR_PLL1ON_Msk,
               state << RCC_CR_PLL1ON_Pos);

    if (state == HAL_ENABLE) {
        while (!READ_BIT(instance->CR, RCC_CR_PLL1RDY_Msk)) {
            continue;
        }
    }
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить состояние PLL2
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       state: Состояние @ref hal_state_t
 */
static void hal_rcc_setup_pll2(rcc_t *instance, uint8_t state)
{
    MODIFY_REG(instance->CR,
               RCC_CR_PLL2ON_Msk,
               state << RCC_CR_PLL2ON_Pos);

    if (state == HAL_ENABLE) {
        while (!READ_BIT(instance->CR, RCC_CR_PLL2RDY_Msk)) {
            continue;
        }
    }
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить состояние PLL3
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       state: Состояние @ref hal_state_t
 */
static void hal_rcc_setup_pll3(rcc_t *instance, uint8_t state)
{
    MODIFY_REG(instance->CR,
               RCC_CR_PLL3ON_Msk,
               state << RCC_CR_PLL3ON_Pos);

    if (state == HAL_ENABLE) {
        while (!READ_BIT(instance->CR, RCC_CR_PLL3RDY_Msk)) {
            continue;
        }
    }
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить источник тактирования PLL
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       clksource: Источник тактирования PLL @ref enum rcc_pll_clock_source
 */
static void hal_rcc_setup_pll_clock_source(rcc_t *instance, uint8_t clksource)
{
    MODIFY_REG(instance->PLLCKSELR,
               RCC_PLLCKSELR_PLLSRC_Msk,
               clksource << RCC_PLLCKSELR_PLLSRC_Pos);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить значение делителя DIVM1
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       div: Делитель
 */
static void hal_rcc_setup_pll_divm1(rcc_t *instance, uint8_t div)
{
    MODIFY_REG(instance->PLLCKSELR,
               RCC_PLLCKSELR_DIVM1_Msk,
               div << RCC_PLLCKSELR_DIVM1_Pos);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить значение делителя DIVM2
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       div: Делитель
 */
static void hal_rcc_setup_pll_divm2(rcc_t *instance, uint8_t div)
{
    MODIFY_REG(instance->PLLCKSELR,
               RCC_PLLCKSELR_DIVM2_Msk,
               div << RCC_PLLCKSELR_DIVM2_Pos);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить значение делителя DIVM3
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       div: Делитель
 */
static void hal_rcc_setup_pll_divm3(rcc_t *instance, uint8_t div)
{
    MODIFY_REG(instance->PLLCKSELR,
               RCC_PLLCKSELR_DIVM3_Msk,
               div << RCC_PLLCKSELR_DIVM3_Pos);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Инициализировать PLL1
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       pll: Указатель на структуру данных PLL
 */
static void hal_rcc_pll1_init(rcc_t *instance, const struct rcc_pll *pll)
{
    if (pll->enable == HAL_DISABLE) {
        return;
    }

    hal_rcc_pll1_setup_vco(instance, pll->vco);
    hal_rcc_pll1_setup_rge(instance, pll->rge);
    hal_rcc_pll1_setup_divn(instance, pll->divn);
    hal_rcc_pll1_setup_divp(instance, pll->divp);
    hal_rcc_pll1_setup_divq(instance, pll->divq);
    hal_rcc_pll1_setup_divr(instance, pll->divr);
    hal_rcc_pll1_setup_divs(instance, pll->divs);
    hal_rcc_pll1_setup_fractional(instance, pll->fracn);

    hal_rcc_setup_pll1(instance, HAL_ENABLE);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить PLL1 VCO
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       vco: Диапазон VCO @ref enum rcc_pll_vco
 */
static void hal_rcc_pll1_setup_vco(rcc_t *instance, uint8_t vco)
{
    MODIFY_REG(instance->PLLCFGR,
               RCC_PLLCFGR_PLL1VCOSEL_Msk,
               vco << RCC_PLLCFGR_PLL1VCOSEL_Pos);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить PLL1 RGE
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       rge: Диапазон RGE @ref enum rcc_pll_rge
 */
static void hal_rcc_pll1_setup_rge(rcc_t *instance, uint8_t rge)
{
    MODIFY_REG(instance->PLLCFGR,
               RCC_PLLCFGR_PLL1RGE_Msk,
               rge << RCC_PLLCFGR_PLL1RGE_Pos);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить PLL1 DIVN
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       div: Значение делителя
 */
static void hal_rcc_pll1_setup_divn(rcc_t *instance, uint16_t div)
{
    if (div < 8 || div > 420) {
        return;
    }

    MODIFY_REG(instance->PLL1DIVR1,
               RCC_PLL1DIVR1_DIVN_Msk,
               (div - 1) << RCC_PLL1DIVR1_DIVN_Pos);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить PLL1 DIVP
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       div: Значение делителя
 */
static void hal_rcc_pll1_setup_divp(rcc_t *instance, uint16_t div)
{
    if (div == 0 || div == 2 || div > 128) {
        CLEAR_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL1PEN_Msk);
    } else {
        SET_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL1PEN_Msk);

        MODIFY_REG(instance->PLL1DIVR1,
                   RCC_PLL1DIVR1_DIVP_Msk,
                   (div - 1) << RCC_PLL1DIVR1_DIVP_Pos);
    }
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить PLL1 DIVQ
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       div: Значение делителя
 */
static void hal_rcc_pll1_setup_divq(rcc_t *instance, uint16_t div)
{
    if (div == 0 || div > 128) {
        CLEAR_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL1QEN_Msk);
    } else {
        SET_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL1QEN_Msk);

        MODIFY_REG(instance->PLL1DIVR1,
                   RCC_PLL1DIVR1_DIVQ_Msk,
                   (div - 1) << RCC_PLL1DIVR1_DIVQ_Pos);
    }
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить PLL1 DIVR
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       div: Значение делителя
 */
static void hal_rcc_pll1_setup_divr(rcc_t *instance, uint16_t div)
{
    if (div == 0 || div > 128) {
        CLEAR_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL1REN_Msk);
    } else {
        SET_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL1REN_Msk);

        MODIFY_REG(instance->PLL1DIVR1,
                   RCC_PLL1DIVR1_DIVR_Msk,
                   (div - 1) << RCC_PLL1DIVR1_DIVR_Pos);
    }
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить PLL1 DIVS
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       div: Значение делителя
 */
static void hal_rcc_pll1_setup_divs(rcc_t *instance, uint16_t div)
{
    if (div == 0 || div > 8) {
        CLEAR_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL1SEN_Msk);
    } else {
        SET_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL1SEN_Msk);

        MODIFY_REG(instance->PLL1DIVR2,
                   RCC_PLL1DIVR2_DIVS_Msk,
                   (div - 1) << RCC_PLL1DIVR2_DIVS_Pos);
    }
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить PLL1 fractional
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       fracn: Значение fractional
 */
static void hal_rcc_pll1_setup_fractional(rcc_t *instance, uint16_t fracn)
{
    CLEAR_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL1FRACEN_Msk);

    MODIFY_REG(instance->PLL1FRACR,
               RCC_PLL1FRACR_FRACN_Msk,
               fracn << RCC_PLL1FRACR_FRACN_Pos);

    SET_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL1FRACEN_Msk);

    uint32_t tickstart = hal_systick_tick(&systick);
    while (hal_systick_tick(&systick) - tickstart < 1) {
        continue;
    }
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Инициализировать PLL2
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       pll: Указатель на структуру данных PLL
 */
static void hal_rcc_pll2_init(rcc_t *instance, const struct rcc_pll *pll)
{
    if (pll->enable == HAL_DISABLE) {
        return;
    }

    hal_rcc_pll2_setup_vco(instance, pll->vco);
    hal_rcc_pll2_setup_rge(instance, pll->rge);
    hal_rcc_pll2_setup_divn(instance, pll->divn);
    hal_rcc_pll2_setup_divp(instance, pll->divp);
    hal_rcc_pll2_setup_divq(instance, pll->divq);
    hal_rcc_pll2_setup_divr(instance, pll->divr);
    hal_rcc_pll2_setup_divs(instance, pll->divs);
    hal_rcc_pll2_setup_divt(instance, pll->divt);
    hal_rcc_pll2_setup_fractional(instance, pll->fracn);

    hal_rcc_setup_pll2(instance, HAL_ENABLE);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить PLL2 VCO
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       vco: Диапазон VCO @ref enum rcc_pll_vco
 */
static void hal_rcc_pll2_setup_vco(rcc_t *instance, uint8_t vco)
{
    MODIFY_REG(instance->PLLCFGR,
               RCC_PLLCFGR_PLL2VCOSEL_Msk,
               vco << RCC_PLLCFGR_PLL2VCOSEL_Pos);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить PLL2 RGE
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       rge: Диапазон RGE @ref enum rcc_pll_rge
 */
static void hal_rcc_pll2_setup_rge(rcc_t *instance, uint8_t rge)
{
    MODIFY_REG(instance->PLLCFGR,
               RCC_PLLCFGR_PLL2RGE_Msk,
               rge << RCC_PLLCFGR_PLL2RGE_Pos);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить PLL2 DIVN
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       div: Значение делителя
 */
static void hal_rcc_pll2_setup_divn(rcc_t *instance, uint16_t div)
{
    if (div < 8 || div > 420) {
        return;
    }

    MODIFY_REG(instance->PLL2DIVR1,
               RCC_PLL2DIVR1_DIVN_Msk,
               (div - 1) << RCC_PLL2DIVR1_DIVN_Pos);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить PLL2 DIVP
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       div: Значение делителя
 */
static void hal_rcc_pll2_setup_divp(rcc_t *instance, uint16_t div)
{
    if (div == 0 || div == 2 || div > 128) {
        CLEAR_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL2PEN_Msk);
    } else {
        SET_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL2PEN_Msk);

        MODIFY_REG(instance->PLL2DIVR1,
                   RCC_PLL2DIVR1_DIVP_Msk,
                   (div - 1) << RCC_PLL2DIVR1_DIVP_Pos);
    }
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить PLL2 DIVQ
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       div: Значение делителя
 */
static void hal_rcc_pll2_setup_divq(rcc_t *instance, uint16_t div)
{
    if (div == 0 || div > 128) {
        CLEAR_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL2QEN_Msk);
    } else {
        SET_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL2QEN_Msk);

        MODIFY_REG(instance->PLL2DIVR1,
                   RCC_PLL2DIVR1_DIVQ_Msk,
                   (div - 1) << RCC_PLL2DIVR1_DIVQ_Pos);
    }
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить PLL2 DIVR
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       div: Значение делителя
 */
static void hal_rcc_pll2_setup_divr(rcc_t *instance, uint16_t div)
{
    if (div == 0 || div > 128) {
        CLEAR_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL2REN_Msk);
    } else {
        SET_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL2REN_Msk);

        MODIFY_REG(instance->PLL2DIVR1,
                   RCC_PLL2DIVR1_DIVR_Msk,
                   (div - 1) << RCC_PLL2DIVR1_DIVR_Pos);
    }
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить PLL2 DIVS
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       div: Значение делителя
 */
static void hal_rcc_pll2_setup_divs(rcc_t *instance, uint16_t div)
{
    if (div == 0 || div > 8) {
        CLEAR_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL2SEN_Msk);
    } else {
        SET_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL2SEN_Msk);

        MODIFY_REG(instance->PLL2DIVR2,
                   RCC_PLL2DIVR2_DIVS_Msk,
                   (div - 1) << RCC_PLL2DIVR2_DIVS_Pos);
    }
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить PLL2 DIVT
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       div: Значение делителя
 */
static void hal_rcc_pll2_setup_divt(rcc_t *instance, uint16_t div)
{
    if (div == 0 || div > 8) {
        CLEAR_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL2TEN_Msk);
    } else {
        SET_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL2TEN_Msk);

        MODIFY_REG(instance->PLL2DIVR2,
                   RCC_PLL2DIVR2_DIVT_Msk,
                   (div - 1) << RCC_PLL2DIVR2_DIVT_Pos);
    }
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить PLL2 fractional
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       fracn: Значение fractional
 */
static void hal_rcc_pll2_setup_fractional(rcc_t *instance, uint16_t fracn)
{
    CLEAR_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL2FRACEN_Msk);

    MODIFY_REG(instance->PLL2FRACR,
               RCC_PLL2FRACR_FRACN_Msk,
               fracn << RCC_PLL2FRACR_FRACN_Pos);

    SET_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL2FRACEN_Msk);

    uint32_t tickstart = hal_systick_tick(&systick);
    while (hal_systick_tick(&systick) - tickstart < 1) {
        continue;
    }
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Инициализировать PLL3
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       pll: Указатель на структуру данных PLL
 */
static void hal_rcc_pll3_init(rcc_t *instance, const struct rcc_pll *pll)
{
    if (pll->enable == HAL_DISABLE) {
        return;
    }

    hal_rcc_pll3_setup_vco(instance, pll->vco);
    hal_rcc_pll3_setup_rge(instance, pll->rge);
    hal_rcc_pll3_setup_divn(instance, pll->divn);
    hal_rcc_pll3_setup_divp(instance, pll->divp);
    hal_rcc_pll3_setup_divq(instance, pll->divq);
    hal_rcc_pll3_setup_divr(instance, pll->divr);
    hal_rcc_pll3_setup_divs(instance, pll->divs);
    hal_rcc_pll3_setup_fractional(instance, pll->fracn);

    hal_rcc_setup_pll3(instance, HAL_ENABLE);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить PLL3 VCO
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       vco: Диапазон VCO @ref enum rcc_pll_vco
 */
static void hal_rcc_pll3_setup_vco(rcc_t *instance, uint8_t vco)
{
    MODIFY_REG(instance->PLLCFGR,
               RCC_PLLCFGR_PLL3VCOSEL_Msk,
               vco << RCC_PLLCFGR_PLL3VCOSEL_Pos);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить PLL3 RGE
 *
 * @param[in]       instance Указатель на структуру данных RCC
 * @param[in]       rge: Диапазон RGE @ref enum rcc_pll_rge
 */
static void hal_rcc_pll3_setup_rge(rcc_t *instance, uint8_t rge)
{
    MODIFY_REG(instance->PLLCFGR,
               RCC_PLLCFGR_PLL3RGE_Msk,
               rge << RCC_PLLCFGR_PLL3RGE_Pos);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить PLL3 DIVN
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       div: Значение делителя
 */
static void hal_rcc_pll3_setup_divn(rcc_t *instance, uint16_t div)
{
    if (div < 8 || div > 420) {
        return;
    }

    MODIFY_REG(instance->PLL3DIVR1,
               RCC_PLL3DIVR1_DIVN_Msk,
               (div - 1) << RCC_PLL3DIVR1_DIVN_Pos);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить PLL3 DIVP
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       div: Значение делителя
 */
static void hal_rcc_pll3_setup_divp(rcc_t *instance, uint16_t div)
{
    if (div == 0 || div == 2 || div > 128) {
        CLEAR_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL3PEN_Msk);
    } else {
        SET_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL3PEN_Msk);

        MODIFY_REG(instance->PLL3DIVR1,
                   RCC_PLL3DIVR1_DIVP_Msk,
                   (div - 1) << RCC_PLL3DIVR1_DIVP_Pos);
    }
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить PLL3 DIVQ
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       div: Значение делителя
 */
static void hal_rcc_pll3_setup_divq(rcc_t *instance, uint16_t div)
{
    if (div == 0 || div > 128) {
        CLEAR_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL3QEN_Msk);
    } else {
        SET_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL3QEN_Msk);

        MODIFY_REG(instance->PLL3DIVR1,
                   RCC_PLL3DIVR1_DIVQ_Msk,
                   (div - 1) << RCC_PLL3DIVR1_DIVQ_Pos);
    }
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить PLL3 DIVR
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       div: Значение делителя
 */
static void hal_rcc_pll3_setup_divr(rcc_t *instance, uint16_t div)
{
    if (div == 0 || div > 128) {
        CLEAR_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL3REN_Msk);
    } else {
        SET_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL3REN_Msk);

        MODIFY_REG(instance->PLL3DIVR1,
                   RCC_PLL3DIVR1_DIVR_Msk,
                   (div - 1) << RCC_PLL3DIVR1_DIVR_Pos);
    }
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить PLL3 DIVS
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       div: Значение делителя
 */
static void hal_rcc_pll3_setup_divs(rcc_t *instance, uint16_t div)
{
    if (div == 0 || div > 8) {
        CLEAR_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL3SEN_Msk);
    } else {
        SET_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL3SEN_Msk);

        MODIFY_REG(instance->PLL3DIVR2,
                   RCC_PLL3DIVR2_DIVS_Msk,
                   (div - 1) << RCC_PLL3DIVR2_DIVS_Pos);
    }
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить PLL3 fractional
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       fracn: Значение fractional
 */
static void hal_rcc_pll3_setup_fractional(rcc_t *instance, uint16_t fracn)
{
    CLEAR_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL3FRACEN_Msk);

    MODIFY_REG(instance->PLL3FRACR,
               RCC_PLL3FRACR_FRACN_Msk,
               fracn << RCC_PLL3FRACR_FRACN_Pos);

    SET_BIT(instance->PLLCFGR, RCC_PLLCFGR_PLL3FRACEN_Msk);

    uint32_t tickstart = hal_systick_tick(&systick);
    while (hal_systick_tick(&systick) - tickstart < 1) {
        continue;
    }
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить делитель CPU
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       div: Значение делителя @ref enum rcc_cpu_div
 */
static void hal_rcc_setup_cpu_div(rcc_t *instance, uint8_t div)
{
    MODIFY_REG(instance->CDCFGR,
               RCC_CDCFGR_CPRE_Msk,
               div << RCC_CDCFGR_CPRE_Pos);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить делитель BUS
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       div: Значение делителя @ref enum rcc_bus_div
 */
static void hal_rcc_setup_bus_div(rcc_t *instance, uint8_t div)
{
    MODIFY_REG(instance->BMCFGR,
               RCC_BMCFGR_BMPRE_Msk,
               div << RCC_BMCFGR_BMPRE_Pos);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить делитель APB1
 *
 * @param[in]       instance Указатель на структуру данных RCC
 * @param[in]       div: Значение делителя @ref enum rcc_apb_div
 */
static void hal_rcc_setup_apb1_div(rcc_t *instance, uint8_t div)
{
    MODIFY_REG(instance->APBCFGR,
               RCC_APBCFGR_PPRE1_Msk,
               div << RCC_APBCFGR_PPRE1_Pos);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить делитель APB2
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       div: Значение делителя @ref enum rcc_apb_div
 */
static void hal_rcc_setup_apb2_div(rcc_t *instance, uint8_t div)
{
    MODIFY_REG(instance->APBCFGR,
               RCC_APBCFGR_PPRE2_Msk,
               div << RCC_APBCFGR_PPRE2_Pos);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить делитель APB4
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       div: Значение делителя @ref enum rcc_apb_div
 */
static void hal_rcc_setup_apb4_div(rcc_t *instance, uint8_t div)
{
    MODIFY_REG(instance->APBCFGR,
               RCC_APBCFGR_PPRE4_Msk,
               div << RCC_APBCFGR_PPRE4_Pos);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить делитель APB5
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       div: Значение делителя @ref enum rcc_apb_div
 */
static void hal_rcc_setup_apb5_div(rcc_t *instance, uint8_t div)
{
    MODIFY_REG(instance->APBCFGR,
               RCC_APBCFGR_PPRE5_Msk,
               div << RCC_APBCFGR_PPRE5_Pos);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить источник тактирования CPU
 *
 * @param[in]       instance: Указатель на структуру данных RCC
 * @param[in]       clksource: Источник тактирования @ref enum rcc_cpu_clock_source
 */
static void hal_rcc_setup_cpu_clock_source(rcc_t *instance, uint8_t clksource)
{
    MODIFY_REG(instance->CFGR,
               RCC_CFGR_SW_Msk,
               clksource << RCC_CFGR_SW_Pos);

    while (READ_BIT(instance->CFGR, RCC_CFGR_SWS_Msk) !=
            (clksource << RCC_CFGR_SWS_Pos)) {
        continue;
    }
}
/* ------------------------------------------------------------------------- */
