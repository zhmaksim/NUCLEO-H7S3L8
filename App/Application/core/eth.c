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

#include "eth.h"
#include "systick.h"

/* Private macros ---------------------------------------------------------- */

/* Private constants ------------------------------------------------------- */

#define ETH_CSR_CLOCK               300000000

#define ETH_SWRESET_TIMEOUT         500

#define ETH_DMAMR_Msk               0x00007802
#define ETH_DMASBMR_Msk             0x0000D001
#define ETH_DMACCR_Msk              0x00013FFF
#define ETH_DMACTCR_Msk             0x003F1010
#define ETH_DMACRCR_Msk             0x803F0000

#define ETH_MTLTQOMR_Msk            0x00000072
#define ETH_MTLRQOMR_Msk            0x0000007B

#define ETH_MACCR_Msk               0xFFFB7F7C
#define ETH_MACECR_Msk              0x3F077FFF
#define ETH_MACPFR_Msk              0x800007FF
#define ETH_MACWTR_Msk              0x0000010F
#define ETH_MACTFCR_Msk             0xFFFF00F2
#define ETH_MACRFCR_Msk             0x00000003

/* Private types ----------------------------------------------------------- */

/* Private variables ------------------------------------------------------- */

/* TODO ETH linker section and Rx Pool */
static struct eth_dma_desc eth_dma_desc_transmit[ETH_DMA_DESC_TRANSMIT_COUNT];
static struct eth_dma_desc_transmit_list eth_dma_desc_transmit_list;

static struct eth_dma_desc eth_dma_desc_receive[ETH_DMA_DESC_RECEIVE_COUNT];
static struct eth_dma_desc_receive_list eth_dma_desc_receive_list;

static const uint8_t ETH_MAC_ADDRESS[6] = {
    0x00, 0x80, 0xE1, 0x00, 0x00, 0x00,
};

/* Private function prototypes --------------------------------------------- */

/* Private user code ------------------------------------------------------- */

/**
 * @brief           Инициализировать ETH
 */
void eth_init(void)
{
    /* Включить тактирование SBS */
    SET_BIT(RCC->APB4ENR, RCC_APB4ENR_SBSEN_Msk);

    /* Настроить ETH RMII */
    MODIFY_REG(SBS->PMCR,
               SBS_PMCR_ETH_PHYSEL_Msk,
               0x04 << SBS_PMCR_ETH_PHYSEL_Pos);
    READ_REG(SBS->PMCR);

    /* Включить тактирование ETH */
    SET_BIT(RCC->AHB1ENR,
            RCC_AHB1ENR_ETH1MACEN_Msk
          | RCC_AHB1ENR_ETH1TXEN_Msk
          | RCC_AHB1ENR_ETH1RXEN_Msk);

    /* Настроить источник тактирования ETH = PLL3S */
    SET_BIT(RCC->CCIPR1, RCC_CCIPR1_ETH1PHYCKSEL_Msk);


    /* Инициализация DMA --------------------------------------------------- */
    /* Программный сброс */
    SET_BIT(ETH->DMAMR, ETH_DMAMR_SWR_Msk);

    uint32_t tickstart = systick_get_tick();

    while (READ_BIT(ETH->DMAMR, ETH_DMAMR_SWR_Msk)) {
        if (systick_get_tick() - tickstart >= ETH_SWRESET_TIMEOUT) error();
    }


    /* Настроить DMAMR */
    CLEAR_REG(ETH->DMAMR);


    /* Настроить DMASBMR */
    MODIFY_REG(ETH->DMASBMR,
               ETH_DMASBMR_Msk,
               ETH_DMASBMR_AAL_Msk              /* Пакетная передача данных с выравниванием по адресу по каналам чтения и записи */
             | ETH_DMASBMR_FB_Msk);             /* Пакетная передача фиксированной длины */


    /* Инициализировать дескрипторы передачи */
    for (uint32_t i = 0; i < ETH_DMA_DESC_TRANSMIT_COUNT; i++) {
        CLEAR_REG(eth_dma_desc_transmit[i].DES0);
        CLEAR_REG(eth_dma_desc_transmit[i].DES1);
        CLEAR_REG(eth_dma_desc_transmit[i].DES2);
        CLEAR_REG(eth_dma_desc_transmit[i].DES3);

        eth_dma_desc_transmit_list.addr_desc[i] = (uint32_t) &eth_dma_desc_transmit[i];
    }

    eth_dma_desc_transmit_list.curr_desc = 0;

    /* Установить длину кольца дескрипторов передачи */
    WRITE_REG(ETH->DMACTDRLR, ETH_DMA_DESC_TRANSMIT_COUNT - 1);

    /* Установить адрес дескриптора передачи */
    WRITE_REG(ETH->DMACTDLAR, (uint32_t) &eth_dma_desc_transmit[0]);

    /* Установить адрес хвостового дескриптора передачи */
    WRITE_REG(ETH->DMACTDTPR, (uint32_t) &eth_dma_desc_transmit[0]);


    /* Инициализировать дескрипторы приема */
    for (uint32_t i = 0; i < ETH_DMA_DESC_RECEIVE_COUNT; i++) {
        CLEAR_REG(eth_dma_desc_receive[i].DES0);
        CLEAR_REG(eth_dma_desc_receive[i].DES1);
        CLEAR_REG(eth_dma_desc_receive[i].DES2);
        CLEAR_REG(eth_dma_desc_receive[i].DES3);
        CLEAR_REG(eth_dma_desc_receive[i].addr_buff1);
        CLEAR_REG(eth_dma_desc_receive[i].addr_buff2);

        eth_dma_desc_receive_list.addr_desc[i] = (uint32_t) &eth_dma_desc_receive[i];
    }

    eth_dma_desc_receive_list.curr_desc = 0;
    eth_dma_desc_receive_list.desc_count = 0;
    eth_dma_desc_receive_list.data_size = 0;
    eth_dma_desc_receive_list.build_desc = 0;
    eth_dma_desc_receive_list.build_desc_count = 0;
    eth_dma_desc_receive_list.it_mode = 0;

    /* Установить длину кольца дескрипторов приема */
    WRITE_REG(ETH->DMACRDRLR, ETH_DMA_DESC_RECEIVE_COUNT - 1);

    /* Установить адрес дескриптора приема */
    WRITE_REG(ETH->DMACRDLAR, (uint32_t) &eth_dma_desc_receive[0]);

    /* Установить адрес хвостового дескриптора приема */
    WRITE_REG(ETH->DMACRDTPR, (uint32_t) (&eth_dma_desc_receive[0] + (ETH_DMA_DESC_RECEIVE_COUNT - 1)));


    /* Настроить DMACCR, DMACTCR, DMACRCR */
    MODIFY_REG(ETH->DMACCR,
               ETH_DMACCR_Msk,
               0x02 << ETH_DMACCR_DSL_Pos);             /* Длина пропуска адреса дескриптора = 64 бит */

    MODIFY_REG(ETH->DMACTCR,
               ETH_DMACTCR_Msk,
               0x20 << ETH_DMACTCR_TPBL_Pos);           /* Длина пакета передачи = 32 бита */

    MODIFY_REG(ETH->DMACRCR,
               ETH_DMACRCR_Msk,
               0x800 << ETH_DMACRCR_RBSZ_Pos            /* Размер буфера приема данных */
             | 0x20 << ETH_DMACRCR_RPBL_Pos);           /* Длина пакета приема = 32 бита */


    /* Инициализировать MTL ------------------------------------------------ */
    MODIFY_REG(ETH->MTLTQOMR,
               ETH_MTLTQOMR_Msk,
               ETH_MTLTQOMR_TSF_Msk);                   /* Начало передачи, когда в очереди находится полный пакет */

    MODIFY_REG(ETH->MTLRQOMR,
               ETH_MTLRQOMR_Msk,
               ETH_MTLRQOMR_RSF_Msk);                   /* Устройство ждет весь пакет */

    /* Инициализировать MAC ------------------------------------------------ */
    /* Установить MAC адрес */
    WRITE_REG(ETH->MACA0HR,
              ETH_MAC_ADDRESS[5] << 8
            | ETH_MAC_ADDRESS[4]);
    WRITE_REG(ETH->MACA0LR,
              ETH_MAC_ADDRESS[3] << 24
            | ETH_MAC_ADDRESS[2] << 16
            | ETH_MAC_ADDRESS[1] << 8
            | ETH_MAC_ADDRESS[0]);

    /*
     * Настройка часов MDIO CSR (от 1MHz до 2.5MHz)
     *      hclk = 300MHz => 300MHz / 124 = 2.4MHz
     */
    MODIFY_REG(ETH->MACMDIOAR,
               ETH_MACMDIOAR_CR_Msk,
               0x05 << ETH_MACMDIOAR_CR_Pos);

    /* Настройка 1 мс таймера MAC */
    WRITE_REG(ETH->MAC1USTCR, (ETH_CSR_CLOCK / 1000000) - 1);

    /* Настройка режима работы */
    MODIFY_REG(ETH->MACCR,
               ETH_MACCR_Msk,
               0x03 << ETH_MACCR_SARC_Pos           /* Замена содержимого MAC-адреса 0 */
             | ETH_MACCR_IPC_Msk                    /* Проверка CRC заголовка */
             | ETH_MACCR_CST_Msk                    /* Расшифровка CRC */
             | ETH_MACCR_ACS_Msk                    /* Автоматическое удаление CRC */
             | ETH_MACCR_FES_Msk                    /* 100Mbps */
             | ETH_MACCR_DM_Msk);                   /* Full-Duplex */


    /* Настройка расширенного режима работы */
    MODIFY_REG(ETH->MACECR,
               ETH_MACECR_Msk,
               0x618 << ETH_MACECR_GPSL_Pos);       /* Размер "гигантского" пакета = 1560 */

    /* Настройка тайм-аута сторожевого таймера */
    CLEAR_BIT(ETH->MACWTR, ETH_MACWTR_Msk);

    /* Настройка управления потоком передачи */
    CLEAR_BIT(ETH->MACTFCR, ETH_MACTFCR_Msk);

    /* Настройка управления потоком приема */
    CLEAR_BIT(ETH->MACRFCR, ETH_MACRFCR_Msk);


    /* Выключить прерывания RX MMC */
    SET_BIT(ETH->MMCRIMR,
            ETH_MMCRIMR_RXLPITRCIM_Msk
          | ETH_MMCRIMR_RXLPIUSCIM_Msk
          | ETH_MMCRIMR_RXUCGPIM_Msk
          | ETH_MMCRIMR_RXALGNERPIM_Msk
          | ETH_MMCRIMR_RXCRCERPIM);

    /* Выключить прерывания TX MMC */
    SET_BIT(ETH->MMCTIMR,
            ETH_MMCTIMR_TXLPITRCIM_Msk
          | ETH_MMCTIMR_TXLPIUSCIM_Msk
          | ETH_MMCTIMR_TXGPKTIM_Msk
          | ETH_MMCTIMR_TXMCOLGPIM_Msk
          | ETH_MMCTIMR_TXSCOLGPIM_Msk);
}
/* ------------------------------------------------------------------------- */
