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

#include "stm32h7s3xx_hal_pwr.h"

/* Private macros ---------------------------------------------------------- */

/* Private constants ------------------------------------------------------- */

/* Private types ----------------------------------------------------------- */

/* Private variables ------------------------------------------------------- */

/* Private function prototypes --------------------------------------------- */

static void hal_pwr_setup_supply(pwr_t *instance, uint8_t supply);

static void hal_pwr_setup_vos(pwr_t *instance, uint8_t vos);

/* Private user code ------------------------------------------------------- */

/**
 * @brief           Инициализировать PWR
 *
 * @param[in]       handle: Указатель на структуру данных обработчика PWR
 */
void hal_pwr_init(struct pwr_handle *handle)
{
    assert(handle != NULL);
    assert(handle->instance != NULL);

    hal_pwr_setup_supply(handle->instance, handle->init.supply);
    hal_pwr_setup_vos(handle->instance, handle->init.vos);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить конфигурацию источника питания PWR
 *
 * @param[in]       instance: Указатель на структуру данных PWR
 * @param[in]       supply: Конфигурация источника питания PWR @ref enum pwr_supply
 */
static void hal_pwr_setup_supply(pwr_t *instance, uint8_t supply)
{
    WRITE_REG(instance->CSR2, supply);

    while (!READ_BIT(instance->SR1, PWR_SR1_ACTVOSRDY_Msk)) {
        continue;
    }
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить масштабирование напряжения PWR
 *
 * @param[in]       instance: Указатель на структуру данных PWR
 * @param[in]       vos: Масштабирование напряжения PWR @ref enum pwr_vos
 */
static void hal_pwr_setup_vos(pwr_t *instance, uint8_t vos)
{
    MODIFY_REG(instance->CSR4,
               PWR_CSR4_VOS_Msk,
               vos << PWR_CSR4_VOS_Pos);

    while (!READ_BIT(instance->CSR4, PWR_CSR4_VOSRDY_Msk)) {
        continue;
    }
}
/* ------------------------------------------------------------------------- */
