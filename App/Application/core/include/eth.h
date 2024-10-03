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

#ifndef ETH_H_
#define ETH_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ---------------------------------------------------------------- */

#include "main.h"

/* Exported macros --------------------------------------------------------- */

/* Exported constants ------------------------------------------------------ */

#define ETH_DMA_DESC_TRANSMIT_COUNT     4

#define ETH_DMA_DESC_RECEIVE_COUNT      4

/* Exported types ---------------------------------------------------------- */

/**
  * @brief          Определение структуры данных дескриптора ETH DMA
  */
struct eth_dma_desc {
    volatile uint32_t       DES0;

    volatile uint32_t       DES1;

    volatile uint32_t       DES2;

    volatile uint32_t       DES3;

    uint32_t                addr_buff1;

    uint32_t                addr_buff2;
};


/**
  * @brief          Определение структуры данных буфера ETH DMA
  */
struct eth_buff {
    uint8_t            *data;                   /*<! Указатель на данные буфера */

    size_t              size;                   /*<! Размер данных буфера */

    struct eth_buff    *next;                   /*<! Указатель на следующий буфера в списке */
};


/**
  * @brief          Определение структуры данных списка дескрипторов передачи
  */
struct eth_dma_desc_transmit_list {
    uint32_t    addr_desc[ETH_DMA_DESC_TRANSMIT_COUNT];         /*<! Адрес дескриптора */

    uint32_t    curr_desc;                                      /*<! Текущий индекс дескриптора */
};


/**
  * @brief          Определение структуры данных списка дескрипторов приема
  */
struct eth_dma_desc_receive_list {
  uint32_t      addr_desc[ETH_DMA_DESC_RECEIVE_COUNT];          /*<! Адрес дескриптора */

  uint32_t      curr_desc;                                      /*<! Текущий индекс дескриптора */

  uint32_t      desc_count;                                     /*<! Количество дескрипторов */

  uint32_t      data_size;                                      /*<! Размер данных */

  uint32_t      build_desc;                                     /*<! Текущий индекс для построения дескриптора */

  uint32_t      build_desc_count;                               /*<! Количество дескрипторов ожидающих построения */

  uint32_t      it_mode;                                        /*<! Если 1 - DMA сгенерирует прерывание завершения приема */
};

/* Exported variables ------------------------------------------------------ */

/* Exported function prototypes -------------------------------------------- */

void eth_init(void);

/* Exported callback function prototypes ----------------------------------- */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ETH_H_ */
