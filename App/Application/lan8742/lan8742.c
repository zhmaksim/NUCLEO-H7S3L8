/*
 * Copyright (C) 2024 zhmaksim <zhiharev.maxim.alexandrovich@yandex.ru>
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

#include "lan8742.h"
#include "systick.h"

/* Private macros ---------------------------------------------------------- */

/* Private constants ------------------------------------------------------- */

#define LAN8742_PHY_MAX_ADDR                31

#define LAN8742_SW_RESET_DELAY              500

#define LAN8742_INIT_DELAY                  2000

#define LAN8742_ETH_MDIO_BUS_TIMEOUT        1000

/* Private types ----------------------------------------------------------- */

/* Private variables ------------------------------------------------------- */

struct lan8742 lan8742 = {
    .eth = ETH,
};

/* Private function prototypes --------------------------------------------- */

static int32_t lan8742_find_device(void);

static int32_t lan8742_sw_reset(void);

static int32_t lan8742_eth_read_register(uint32_t phy_address,
                                         uint32_t phy_register,
                                         uint16_t *data);

static int32_t lan8742_eth_write_register(uint32_t phy_address,
                                          uint32_t phy_register,
                                          uint16_t data);

static uint32_t lan8742_tick(void);

/* Private user code ------------------------------------------------------- */

/**
 * @brief           Инициализировать LAN8742
 *
 * @return          Статус:
 *                      - LAN8742_STATUS_READ_ERROR
 *                      - LAN8742_STATUS_WRITE_ERROR
 *                      - LAN8742_STATUS_ADDRESS_ERROR
 *                      - LAN8742_STATUS_RESET_TIMEOUT
 *                      - LAN8742_STATUS_ERROR
 *                      - LAN8742_STATUS_OK
 */
int32_t lan8742_init(void)
{
    int32_t status = LAN8742_STATUS_OK;

    status = lan8742_find_device();
    if (status < 0) return status;

    status = lan8742_sw_reset();
    if (status < 0) return status;

    uint32_t tickstart = lan8742_tick();
    while (lan8742_tick() - tickstart <= LAN8742_INIT_DELAY) {}

    return status;
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Поиск устройства LAN8742
 *
 * @return          Статус:
 *                      - LAN8742_STATUS_READ_ERROR
 *                      - LAN8742_STATUS_WRITE_ERROR
 *                      - LAN8742_STATUS_ADDRESS_ERROR
 *                      - LAN8742_STATUS_ERROR
 *                      - LAN8742_STATUS_OK
 */
static int32_t lan8742_find_device(void)
{
    uint16_t regvalue;

    for (uint32_t addr = 0; addr < LAN8742_PHY_MAX_ADDR; addr++) {
        if (lan8742_eth_read_register(addr, LAN8742_SMR, &regvalue) < 0) {
            return LAN8742_STATUS_READ_ERROR;
        } else if ((READ_BIT(regvalue, LAN8742_SMR_PHY_ADDR) == addr) && (addr <= LAN8742_PHY_MAX_ADDR)) {
            lan8742.phy_address = addr;
            return LAN8742_STATUS_OK;
        }
    }

    return LAN8742_STATUS_ADDRESS_ERROR;
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Программный сброс LAN8742
 *
 * @return          Статус:
 *                      - LAN8742_STATUS_READ_ERROR
 *                      - LAN8742_STATUS_WRITE_ERROR
 *                      - LAN8742_STATUS_RESET_TIMEOUT
 *                      - LAN8742_STATUS_ERROR
 *                      - LAN8742_STATUS_OK
 */
static int32_t lan8742_sw_reset(void)
{
    uint32_t tickstart;
    uint16_t regvalue;

    if (lan8742_eth_write_register(lan8742.phy_address, LAN8742_BCR, LAN8742_BCR_SOFT_RESET) < 0) {
        return LAN8742_STATUS_WRITE_ERROR;
    } else if (lan8742_eth_read_register(lan8742.phy_address, LAN8742_BCR, &regvalue) < 0) {
        return LAN8742_STATUS_READ_ERROR;
    }

    tickstart = lan8742_tick();

    while (READ_BIT(regvalue, LAN8742_BCR_SOFT_RESET)) {
        if (lan8742_eth_read_register(lan8742.phy_address, LAN8742_BCR, &regvalue) < 0) {
            return LAN8742_STATUS_READ_ERROR;
        } else if (lan8742_tick() - tickstart >= LAN8742_SW_RESET_DELAY) {
            return LAN8742_STATUS_RESET_TIMEOUT;
        }
    }

    return LAN8742_STATUS_OK;
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Включить Auto Nego
 *
 * @return          Статус:
 *                      - LAN8742_STATUS_READ_ERROR
 *                      - LAN8742_STATUS_WRITE_ERROR
 *                      - LAN8742_STATUS_ERROR
 *                      - LAN8742_STATUS_OK
 */
int32_t lan8742_enable_auto_nego(void)
{
    uint16_t regvalue;

    if (lan8742_eth_read_register(lan8742.phy_address, LAN8742_BCR, &regvalue) < 0) {
        return LAN8742_STATUS_READ_ERROR;
    }

    SET_BIT(regvalue, LAN8742_BCR_AUTONEGO_EN);

    if (lan8742_eth_write_register(lan8742.phy_address, LAN8742_BCR, regvalue) < 0) {
        return LAN8742_STATUS_WRITE_ERROR;
    }

    return LAN8742_STATUS_OK;
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Получить состояние подключения
 *
 * @return          Статус:
 *                      - LAN8742_STATUS_READ_ERROR
 *                      - LAN8742_STATUS_WRITE_ERROR
 *                      - LAN8742_STATUS_ERROR
 *                      - LAN8742_STATUS_OK
 *                      - LAN8742_STATUS_LINK_DOWN
 *                      - LAN8742_STATUS_100MBITS_FULLDUPLEX
 *                      - LAN8742_STATUS_100MBITS_HALFDUPLEX
 *                      - LAN8742_STATUS_10MBITS_FULLDUPLEX
 *                      - LAN8742_STATUS_10MBITS_HALFDUPLEX
 *                      - LAN8742_STATUS_AUTONEGO_NOT_DONE
 */
int32_t lan8742_link_state(void)
{
    uint16_t regvalue;

    if (lan8742_eth_read_register(lan8742.phy_address, LAN8742_BSR, &regvalue) < 0) {
        return LAN8742_STATUS_READ_ERROR;
    }

    if (!READ_BIT(regvalue, LAN8742_BSR_LINK_STATUS)) {
        return LAN8742_STATUS_LINK_DOWN;
    }

    if (lan8742_eth_read_register(lan8742.phy_address, LAN8742_BCR, &regvalue) < 0) {
        return LAN8742_STATUS_READ_ERROR;
    }

    if (READ_BIT(regvalue, LAN8742_BCR_AUTONEGO_EN)) {
        if (lan8742_eth_read_register(lan8742.phy_address, LAN8742_PHYSCSR, &regvalue) < 0) {
            return LAN8742_STATUS_READ_ERROR;
        }

        if (!READ_BIT(regvalue, LAN8742_PHYSCSR_AUTONEGO_DONE)) {
          return LAN8742_STATUS_AUTONEGO_NOT_DONE;
        }

        if (READ_BIT(regvalue, LAN8742_PHYSCSR_HCDSPEEDMASK) == LAN8742_PHYSCSR_100BTX_FD) {
          return LAN8742_STATUS_100MBITS_FULLDUPLEX;
        } else if (READ_BIT(regvalue, LAN8742_PHYSCSR_HCDSPEEDMASK) == LAN8742_PHYSCSR_100BTX_HD) {
          return LAN8742_STATUS_100MBITS_HALFDUPLEX;
        } else if (READ_BIT(regvalue, LAN8742_PHYSCSR_HCDSPEEDMASK) == LAN8742_PHYSCSR_10BT_FD) {
          return LAN8742_STATUS_10MBITS_FULLDUPLEX;
        } else {
          return LAN8742_STATUS_10MBITS_HALFDUPLEX;
        }
    } else {
        if (READ_BIT(regvalue, LAN8742_BCR_SPEED_SELECT) && READ_BIT(regvalue, LAN8742_BCR_DUPLEX_MODE)) {
          return LAN8742_STATUS_100MBITS_FULLDUPLEX;
        } else if (READ_BIT(regvalue, LAN8742_BCR_SPEED_SELECT)) {
          return LAN8742_STATUS_100MBITS_HALFDUPLEX;
        } else if (READ_BIT(regvalue, LAN8742_BCR_DUPLEX_MODE)) {
          return LAN8742_STATUS_10MBITS_FULLDUPLEX;
        } else {
          return LAN8742_STATUS_10MBITS_HALFDUPLEX;
        }
    }
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Прочитать регистр LAN8742
 *
 * @param[in]       phy_address: Адрес устройства
 * @param[in]       phy_regsiter: Регистр устройства
 * @param[out]      data: Данные
 * @return          Статус:
 *                      - LAN8742_STATUS_ERROR
 *                      - LAN8742_STATUS_OK
 */
static int32_t lan8742_eth_read_register(uint32_t phy_address,
                                         uint32_t phy_register,
                                         uint16_t *data)
{
    uint32_t tickstart;
    uint32_t tmpreg;

    /* Проверить готовность MDIO */
    if (READ_BIT(lan8742.eth->MACMDIOAR, ETH_MACMDIOAR_MB_Msk)) {
        return LAN8742_STATUS_ERROR;
    }

    /* Получить значение MDIO */
    tmpreg = READ_REG(lan8742.eth->MACMDIOAR);

    /*
     * Подготовить значение MDIO
     *     - Установить адрес устройства PHY
     *     - Установить адрес регистра PHY
     *     - Установить режим чтения
     *     - Установить бит занятости MII
     */

    MODIFY_REG(tmpreg,
               ETH_MACMDIOAR_PA_Msk
             | ETH_MACMDIOAR_RDA_Msk
             | ETH_MACMDIOAR_MOC_Msk,
               phy_address << ETH_MACMDIOAR_PA_Pos         /* PHY Address */
             | phy_register << ETH_MACMDIOAR_RDA_Pos       /* PHY Register */
             | 0x03U << ETH_MACMDIOAR_MOC_Pos              /* MII Read */
             | ETH_MACMDIOAR_MB_Msk);                      /* Busy */

    /* Записть значение MDIO */
    WRITE_REG(lan8742.eth->MACMDIOAR, tmpreg);

    /* Ожидание завершения операции */
    tickstart = lan8742_tick();

    while (READ_BIT(lan8742.eth->MACMDIOAR, ETH_MACMDIOAR_MB_Msk)) {
        if (lan8742_tick() - tickstart >= LAN8742_ETH_MDIO_BUS_TIMEOUT) {
            return LAN8742_STATUS_ERROR;
        }
    }

    /* Получить значение данных */
    *data = (uint16_t) READ_REG(lan8742.eth->MACMDIODR);

    return LAN8742_STATUS_OK;
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Записать регистр LAN8742
 *
 * @param[in]       phy_address: Адрес устройства
 * @param[in]       phy_regsiter: Регистр устройства
 * @param[in]       data: Данные
 * @return          Статус:
 *                      - LAN8742_STATUS_ERROR
 *                      - LAN8742_STATUS_OK
 */
static int32_t lan8742_eth_write_register(uint32_t phy_address,
                                          uint32_t phy_register,
                                          uint16_t data)
{
    uint32_t tickstart;
    uint32_t tmpreg;

    /* Проверить готовность MDIO */
    if (READ_BIT(lan8742.eth->MACMDIOAR, ETH_MACMDIOAR_MB_Msk)) {
        return LAN8742_STATUS_ERROR;
    }

    /* Получить значение MDIO */
    tmpreg = READ_REG(lan8742.eth->MACMDIOAR);

    /*
     * Подготовить значение MDIO
     *     - Установить адрес устройства PHY
     *     - Установить адрес регистра PHY
     *     - Установить режим записи
     *     - Установить бит занятости MII
     */

    MODIFY_REG(tmpreg,
               ETH_MACMDIOAR_PA_Msk
             | ETH_MACMDIOAR_RDA_Msk
             | ETH_MACMDIOAR_MOC_Msk,
               phy_address << ETH_MACMDIOAR_PA_Pos         /* PHY Address */
             | phy_register << ETH_MACMDIOAR_RDA_Pos       /* PHY Register */
             | 0x01U << ETH_MACMDIOAR_MOC_Pos              /* MII Write */
             | ETH_MACMDIOAR_MB_Msk);                      /* Busy */

    /* Записать значение */
    WRITE_REG(lan8742.eth->MACMDIODR, (uint16_t) data);

    /* Записть значение MDIO */
    WRITE_REG(lan8742.eth->MACMDIOAR, tmpreg);

    /* Ожидание завершения операции */
    tickstart = lan8742_tick();

    while (READ_BIT(lan8742.eth->MACMDIOAR, ETH_MACMDIOAR_MB_Msk)) {
        if (lan8742_tick() - tickstart >= LAN8742_ETH_MDIO_BUS_TIMEOUT) {
            return LAN8742_STATUS_ERROR;
        }
    }

    return LAN8742_STATUS_OK;
}
/* ------------------------------------------------------------------------- */

/**
 * @brief           Получить значение таймера (мс)
 *
 * @return          Значение таймера
 */
static inline uint32_t lan8742_tick(void)
{
    return systick_get_tick();
}
/* ------------------------------------------------------------------------- */
