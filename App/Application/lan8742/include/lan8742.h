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

#ifndef LAN8742_H_
#define LAN8742_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ---------------------------------------------------------------- */

#include "main.h"

/* Exported macros --------------------------------------------------------- */

/* Exported constants ------------------------------------------------------ */

/* Registers Mapping */
#define LAN8742_BCR                           0x0000
#define LAN8742_BSR                           0x0001
#define LAN8742_PHYI1R                        0x0002
#define LAN8742_PHYI2R                        0x0003
#define LAN8742_ANAR                          0x0004
#define LAN8742_ANLPAR                        0x0005
#define LAN8742_ANER                          0x0006
#define LAN8742_ANNPTR                        0x0007
#define LAN8742_ANNPRR                        0x0008
#define LAN8742_MMDACR                        0x000D
#define LAN8742_MMDAADR                       0x000E
#define LAN8742_ENCTR                         0x0010
#define LAN8742_MCSR                          0x0011
#define LAN8742_SMR                           0x0012
#define LAN8742_TPDCR                         0x0018
#define LAN8742_TCSR                          0x0019
#define LAN8742_SECR                          0x001A
#define LAN8742_SCSIR                         0x001B
#define LAN8742_CLR                           0x001C
#define LAN8742_ISFR                          0x001D
#define LAN8742_IMR                           0x001E
#define LAN8742_PHYSCSR                       0x001F

/* LAN8742 BCR Bit Definition */
#define LAN8742_BCR_SOFT_RESET                0x8000
#define LAN8742_BCR_LOOPBACK                  0x4000
#define LAN8742_BCR_SPEED_SELECT              0x2000
#define LAN8742_BCR_AUTONEGO_EN               0x1000
#define LAN8742_BCR_POWER_DOWN                0x0800
#define LAN8742_BCR_ISOLATE                   0x0400
#define LAN8742_BCR_RESTART_AUTONEGO          0x0200
#define LAN8742_BCR_DUPLEX_MODE               0x0100

/* LAN8742 BSR Bit Definition */
#define LAN8742_BSR_100BASE_T4                0x8000
#define LAN8742_BSR_100BASE_TX_FD             0x4000
#define LAN8742_BSR_100BASE_TX_HD             0x2000
#define LAN8742_BSR_10BASE_T_FD               0x1000
#define LAN8742_BSR_10BASE_T_HD               0x0800
#define LAN8742_BSR_100BASE_T2_FD             0x0400
#define LAN8742_BSR_100BASE_T2_HD             0x0200
#define LAN8742_BSR_EXTENDED_STATUS           0x0100
#define LAN8742_BSR_AUTONEGO_CPLT             0x0020
#define LAN8742_BSR_REMOTE_FAULT              0x0010
#define LAN8742_BSR_AUTONEGO_ABILITY          0x0008
#define LAN8742_BSR_LINK_STATUS               0x0004
#define LAN8742_BSR_JABBER_DETECT             0x0002
#define LAN8742_BSR_EXTENDED_CAP              0x0001

/* LAN8742 PHYI1R Bit Definition */
#define LAN8742_PHYI1R_OUI_3_18               0xFFFF

/* LAN8742 PHYI2R Bit Definition */
#define LAN8742_PHYI2R_OUI_19_24              0xFC00
#define LAN8742_PHYI2R_MODEL_NBR              0x03F0
#define LAN8742_PHYI2R_REVISION_NBR           0x000F

/* LAN8742 ANAR Bit Definition */
#define LAN8742_ANAR_NEXT_PAGE                0x8000
#define LAN8742_ANAR_REMOTE_FAULT             0x2000
#define LAN8742_ANAR_PAUSE_OPERATION          0x0C00
#define LAN8742_ANAR_PO_NOPAUSE               0x0000
#define LAN8742_ANAR_PO_SYMMETRIC_PAUSE       0x0400
#define LAN8742_ANAR_PO_ASYMMETRIC_PAUSE      0x0800
#define LAN8742_ANAR_PO_ADVERTISE_SUPPORT     0x0C00
#define LAN8742_ANAR_100BASE_TX_FD            0x0100
#define LAN8742_ANAR_100BASE_TX               0x0080
#define LAN8742_ANAR_10BASE_T_FD              0x0040
#define LAN8742_ANAR_10BASE_T                 0x0020
#define LAN8742_ANAR_SELECTOR_FIELD           0x000F

/* LAN8742 ANLPAR Bit Definition */
#define LAN8742_ANLPAR_NEXT_PAGE              0x8000
#define LAN8742_ANLPAR_REMOTE_FAULT           0x2000
#define LAN8742_ANLPAR_PAUSE_OPERATION        0x0C00
#define LAN8742_ANLPAR_PO_NOPAUSE             0x0000
#define LAN8742_ANLPAR_PO_SYMMETRIC_PAUSE     0x0400
#define LAN8742_ANLPAR_PO_ASYMMETRIC_PAUSE    0x0800
#define LAN8742_ANLPAR_PO_ADVERTISE_SUPPORT   0x0C00
#define LAN8742_ANLPAR_100BASE_TX_FD          0x0100
#define LAN8742_ANLPAR_100BASE_TX             0x0080
#define LAN8742_ANLPAR_10BASE_T_FD            0x0040
#define LAN8742_ANLPAR_10BASE_T               0x0020
#define LAN8742_ANLPAR_SELECTOR_FIELD         0x000F

/* LAN8742 ANER Bit Definition */
#define LAN8742_ANER_RX_NP_LOCATION_ABLE      0x0040
#define LAN8742_ANER_RX_NP_STORAGE_LOCATION   0x0020
#define LAN8742_ANER_PARALLEL_DETECT_FAULT    0x0010
#define LAN8742_ANER_LP_NP_ABLE               0x0008
#define LAN8742_ANER_NP_ABLE                  0x0004
#define LAN8742_ANER_PAGE_RECEIVED            0x0002
#define LAN8742_ANER_LP_AUTONEG_ABLE          0x0001

/* LAN8742 ANNPTR Bit Definition */
#define LAN8742_ANNPTR_NEXT_PAGE              0x8000
#define LAN8742_ANNPTR_MESSAGE_PAGE           0x2000
#define LAN8742_ANNPTR_ACK2                   0x1000
#define LAN8742_ANNPTR_TOGGLE                 0x0800
#define LAN8742_ANNPTR_MESSAGGE_CODE          0x07FF

/* LAN8742 ANNPTR Bit Definition */
#define LAN8742_ANNPTR_NEXT_PAGE              0x8000
#define LAN8742_ANNPRR_ACK                    0x4000
#define LAN8742_ANNPRR_MESSAGE_PAGE           0x2000
#define LAN8742_ANNPRR_ACK2                   0x1000
#define LAN8742_ANNPRR_TOGGLE                 0x0800
#define LAN8742_ANNPRR_MESSAGGE_CODE          0x07FF

/* LAN8742 MMDACR Bit Definition */
#define LAN8742_MMDACR_MMD_FUNCTION           0xC000
#define LAN8742_MMDACR_MMD_FUNCTION_ADDR      0x0000
#define LAN8742_MMDACR_MMD_FUNCTION_DATA      0x4000
#define LAN8742_MMDACR_MMD_DEV_ADDR           0x001F

/* LAN8742 ENCTR Bit Definition */
#define LAN8742_ENCTR_TX_ENABLE               0x8000
#define LAN8742_ENCTR_TX_TIMER                0x6000
#define LAN8742_ENCTR_TX_TIMER_1S             0x0000
#define LAN8742_ENCTR_TX_TIMER_768MS          0x2000
#define LAN8742_ENCTR_TX_TIMER_512MS          0x4000
#define LAN8742_ENCTR_TX_TIMER_265MS          0x6000
#define LAN8742_ENCTR_RX_ENABLE               0x1000
#define LAN8742_ENCTR_RX_MAX_INTERVAL         0x0C00
#define LAN8742_ENCTR_RX_MAX_INTERVAL_64MS    0x0000
#define LAN8742_ENCTR_RX_MAX_INTERVAL_256MS   0x0400
#define LAN8742_ENCTR_RX_MAX_INTERVAL_512MS   0x0800
#define LAN8742_ENCTR_RX_MAX_INTERVAL_1S      0x0C00
#define LAN8742_ENCTR_EX_CROSS_OVER           0x0002
#define LAN8742_ENCTR_EX_MANUAL_CROSS_OVER    0x0001

/* LAN8742 MCSR Bit Definition */
#define LAN8742_MCSR_EDPWRDOWN                0x2000
#define LAN8742_MCSR_FARLOOPBACK              0x0200
#define LAN8742_MCSR_ALTINT                   0x0040
#define LAN8742_MCSR_ENERGYON                 0x0002

/* LAN8742 SMR Bit Definition */
#define LAN8742_SMR_MODE                      0x00E0
#define LAN8742_SMR_PHY_ADDR                  0x001F

/* LAN8742 TPDCR Bit Definition */
#define LAN8742_TPDCR_DELAY_IN                0x8000
#define LAN8742_TPDCR_LINE_BREAK_COUNTER      0x7000
#define LAN8742_TPDCR_PATTERN_HIGH            0x0FC0
#define LAN8742_TPDCR_PATTERN_LOW             0x003F

/* LAN8742 TCSR Bit Definition */
#define LAN8742_TCSR_TDR_ENABLE               0x8000
#define LAN8742_TCSR_TDR_AD_FILTER_ENABLE     0x4000
#define LAN8742_TCSR_TDR_CH_CABLE_TYPE        0x0600
#define LAN8742_TCSR_TDR_CH_CABLE_DEFAULT     0x0000
#define LAN8742_TCSR_TDR_CH_CABLE_SHORTED     0x0200
#define LAN8742_TCSR_TDR_CH_CABLE_OPEN        0x0400
#define LAN8742_TCSR_TDR_CH_CABLE_MATCH       0x0600
#define LAN8742_TCSR_TDR_CH_STATUS            0x0100
#define LAN8742_TCSR_TDR_CH_LENGTH            0x00FF

/* LAN8742 SCSIR Bit Definition */
#define LAN8742_SCSIR_AUTO_MDIX_ENABLE        0x8000
#define LAN8742_SCSIR_CHANNEL_SELECT          0x2000
#define LAN8742_SCSIR_SQE_DISABLE             0x0800
#define LAN8742_SCSIR_XPOLALITY               0x0010

/* LAN8742 CLR Bit Definition */
#define LAN8742_CLR_CABLE_LENGTH              0xF000

/* LAN8742 IMR_ISFR Bit Definition */
#define LAN8742_IMR_ISFR_INT_8                0x0100
#define LAN8742_IMR_ISFR_INT_7                0x0080
#define LAN8742_IMR_ISFR_INT_6                0x0040
#define LAN8742_IMR_ISFR_INT_5                0x0020
#define LAN8742_IMR_ISFR_INT_4                0x0010
#define LAN8742_IMR_ISFR_INT_3                0x0008
#define LAN8742_IMR_ISFR_INT_2                0x0004
#define LAN8742_IMR_ISFR_INT_1                0x0002

/* LAN8742 PHYSCSR Bit Definition */
#define LAN8742_PHYSCSR_AUTONEGO_DONE         0x1000
#define LAN8742_PHYSCSR_HCDSPEEDMASK          0x001C
#define LAN8742_PHYSCSR_10BT_HD               0x0004
#define LAN8742_PHYSCSR_10BT_FD               0x0014
#define LAN8742_PHYSCSR_100BTX_HD             0x0008
#define LAN8742_PHYSCSR_100BTX_FD             0x0018

/* LAN8742 Status */
#define  LAN8742_STATUS_READ_ERROR            -5
#define  LAN8742_STATUS_WRITE_ERROR           -4
#define  LAN8742_STATUS_ADDRESS_ERROR         -3
#define  LAN8742_STATUS_RESET_TIMEOUT         -2
#define  LAN8742_STATUS_ERROR                 -1
#define  LAN8742_STATUS_OK                     0
#define  LAN8742_STATUS_LINK_DOWN              1
#define  LAN8742_STATUS_100MBITS_FULLDUPLEX    2
#define  LAN8742_STATUS_100MBITS_HALFDUPLEX    3
#define  LAN8742_STATUS_10MBITS_FULLDUPLEX     4
#define  LAN8742_STATUS_10MBITS_HALFDUPLEX     5
#define  LAN8742_STATUS_AUTONEGO_NOT_DONE      6

/* Exported types ---------------------------------------------------------- */

/**
 * @brief           Определение структуры данных LAN8742
 */
struct lan8742 {
    ETH_TypeDef    *eth;                /*!< Указатель на структуру данных ETH */

    uint8_t         phy_address;        /*!< Адрес PHY */
};

/* Exported variables ------------------------------------------------------ */

/* Exported function prototypes -------------------------------------------- */

int32_t lan8742_init(void);

int32_t lan8742_enable_auto_nego(void);

int32_t lan8742_link_state(void);

/* Exported callback function prototypes ----------------------------------- */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LAN8742_H_ */
