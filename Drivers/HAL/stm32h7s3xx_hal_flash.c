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

#include "stm32h7s3xx_hal_flash.h"

/* Private macros ---------------------------------------------------------- */

/* Private constants ------------------------------------------------------- */

/* Private types ----------------------------------------------------------- */

/* Private variables ------------------------------------------------------- */

/* Private function prototypes --------------------------------------------- */

static void hal_flash_setup_latency(flash_t *instance, uint8_t latency);

static void hal_flash_setup_wrhighfreq(flash_t *instance, uint8_t wrhighfreq);

/* Private user code ------------------------------------------------------- */

/**
 * @brief           Инициализировать FLASH
 *
 * @param[in]       handle: Указатель на структуру данных обработчика FLASH
 */
void hal_flash_init(struct flash_handle *handle)
{
    assert(handle != NULL);
    assert(handle->instance != NULL);

    hal_flash_setup_latency(handle->instance, handle->init.latency);
    hal_flash_setup_wrhighfreq(handle->instance, handle->init.wrhighfreq);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить задержу чтения FLASH
 *
 * @param[in]       instance: Указатель на структуру данных FLASH
 * @param[in]       latency: Задержка чтения FLASH @ref enum flash_latency
 */
static void hal_flash_setup_latency(flash_t *instance, uint8_t latency)
{
    MODIFY_REG(instance->ACR,
               FLASH_ACR_LATENCY_Msk,
               latency << FLASH_ACR_LATENCY_Pos);
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Установить задержку записи FLASH
 *
 * @param[in]       instance: Указатель на структуру данных FLASH
 * @param[in]       wrhighfreq: Задержка записи FLASH @ref enum flash_wrhighfreq
 */
static void hal_flash_setup_wrhighfreq(flash_t *instance, uint8_t wrhighfreq)
{
    MODIFY_REG(instance->ACR,
               FLASH_ACR_WRHIGHFREQ_Msk,
               wrhighfreq << FLASH_ACR_WRHIGHFREQ_Pos);
}
/* ------------------------------------------------------------------------- */
