/**
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

#ifndef MX25UW_H_
#define MX25UW_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Includes ---------------------------------------------------------------- */

#include "main.h"

/* Exported macros --------------------------------------------------------- */

/* Exported constants ------------------------------------------------------ */

#define MX25UW_MANUFACTURER_ID                          ( 0xC2U )

#define MX25UW_FLASH_SIZE                               ( 0x2000000U )
#define MX25UW_BLOCK_SIZE                               ( 0x10000U )
#define MX25UW_SECTOR_SIZE                              ( 0x1000U )
#define MX25UW_PAGE_SIZE                                ( 0x100U )

#define MX25UW_READ_CMD                                 ( 0x03U )       /* Normal Read 3 Byte Address */
#define MX25UW_FAST_READ_CMD                            ( 0x0BU )       /* Fast Read 3 Byte Address */
#define MX25UW_PAGE_PROG_CMD                            ( 0x02U )       /* Page Program 3 Byte Address */
#define MX25UW_SECTOR_ERASE_CMD                         ( 0x20U )       /* SubSector Erase 4KB 3 Byte Address*/
#define MX25UW_BLOCK_ERASE_CMD                          ( 0xD8U )       /* Sector Erase 64KB 3 Byte Address*/
#define MX25UW_CHIP_ERASE_CMD                           ( 0x60U )       /* Bulk Erase */

#define MX25UW_READ_4B_ADDR_CMD                         ( 0x13U )       /* Normal Read 4 Byte address */
#define MX25UW_FAST_READ_4B_ADDR_CMD                    ( 0x0CU )       /* Fast Read 4 Byte address */
#define MX25UW_PAGE_PROG_4B_ADDR_CMD                    ( 0x12U )       /* Page Program 4 Byte Address */
#define MX25UW_SECTOR_ERASE_4B_ADDR_CMD                 ( 0x21U )       /* SubSector Erase 4KB 4 Byte Address */
#define MX25UW_BLOCK_ERASE_4B_ADDR_CMD                  ( 0xDCU )       /* Sector Erase 64KB 4 Byte Address */

#define MX25UW_WRITE_ENABLE_CMD                         ( 0x06U )       /* Write Enable */
#define MX25UW_WRITE_DISABLE_CMD                        ( 0x04U )       /* Write Disable */
#define MX25UW_PROG_ERASE_SUSPEND_CMD                   ( 0xB0U )       /* Program/Erase suspend */
#define MX25UW_PROG_ERASE_RESUME_CMD                    ( 0x30U )       /* Program/Erase resume */
#define MX25UW_ENTER_DEEP_POWER_DOWN_CMD                ( 0xB9U )       /* Enter deep power down */
#define MX25UW_RELEASE_FROM_DEEP_POWER_DOWN_CMD         ( 0xABU )       /* Release from deep power down */
#define MX25UW_SET_BURST_LENGTH_CMD                     ( 0xC0U )       /* Set burst length */
#define MX25UW_ENTER_SECURED_OTP_CMD                    ( 0xB1U )       /* Enter secured OTP) */
#define MX25UW_EXIT_SECURED_OTP_CMD                     ( 0xC1U )       /* Exit secured OTP) */

#define MX25UW_NOP_CMD                                  ( 0x00U )       /* No operation */
#define MX25UW_RESET_ENABLE_CMD                         ( 0x66U )       /* Reset Enable */
#define MX25UW_RESET_MEMORY_CMD                         ( 0x99U )       /* Reset Memory */

#define MX25UW_READ_ID_CMD                              ( 0x9FU )       /* Read IDentification */
#define MX25UW_READ_SERIAL_FLASH_DISCO_PARAM_CMD        ( 0x5AU )       /* Read Serial Flash Discoverable Parameter */
#define MX25UW_READ_STATUS_REG_CMD                      ( 0x05U )       /* Read Status Register */
#define MX25UW_READ_CFG_REG_CMD                         ( 0x15U )       /* Read configuration Register */
#define MX25UW_WRITE_STATUS_REG_CMD                     ( 0x01U )       /* Write Status Register */
#define MX25UW_READ_CFG_REG2_CMD                        ( 0x71U )       /* Read configuration Register2 */
#define MX25UW_WRITE_CFG_REG2_CMD                       ( 0x72U )       /* Write configuration Register2 */
#define MX25UW_READ_FAST_BOOT_REG_CMD                   ( 0x16U )       /* Read fast boot Register */
#define MX25UW_WRITE_FAST_BOOT_REG_CMD                  ( 0x17U )       /* Write fast boot Register */
#define MX25UW_ERASE_FAST_BOOT_REG_CMD                  ( 0x18U )       /* Erase fast boot Register */
#define MX25UW_READ_SECURITY_REG_CMD                    ( 0x2BU )       /* Read security Register */
#define MX25UW_WRITE_SECURITY_REG_CMD                   ( 0x2FU )       /* Write security Register */
#define MX25UW_READ_LOCK_REG_CMD                        ( 0x2DU )       /* Read lock Register */
#define MX25UW_WRITE_LOCK_REG_CMD                       ( 0x2CU )       /* Write lock Register */

#define MX25UW_READ_DPB_REG_CMD                         ( 0xE0U )       /* Read DPB register */
#define MX25UW_WRITE_DPB_REG_CMD                        ( 0xE1U )       /* Write DPB register */
#define MX25UW_READ_SPB_STATUS_CMD                      ( 0xE2U )       /* Read SPB status */
#define MX25UW_WRITE_SPB_BIT_CMD                        ( 0xE3U )       /* SPB bit program */
#define MX25UW_ERASE_ALL_SPB_CMD                        ( 0xE4U )       /* Erase all SPB bit */
#define MX25UW_WRITE_PROTECT_SEL_CMD                    ( 0x68U )       /* Write Protect selection */
#define MX25UW_GANG_BLOCK_LOCK_CMD                      ( 0x7EU )       /* Gang block lock: whole chip write protect */
#define MX25UW_GANG_BLOCK_UNLOCK_CMD                    ( 0x98U )       /* Gang block unlock: whole chip write unprotect */
#define MX25UW_READ_PASSWORD_REGISTER_CMD               ( 0x27U )       /* Read Password */
#define MX25UW_WRITE_PASSWORD_REGISTER_CMD              ( 0x28U )       /* Write Password */
#define MX25UW_PASSWORD_UNLOCK_CMD                      ( 0x29U )       /* Unlock Password */
#define MX25UW_PAGE_BUFFER_READ                         ( 0x25U )       /* Page Buffer Read */
#define MX25UW_WRITE_BUFFER_INITIAL                     ( 0x22U )       /* Write Buffer Initial */
#define MX25UW_WRITE_BUFFER_CONTINUE                    ( 0x24U )       /* Write Buffer Continue */
#define MX25UW_WRITE_BUFFER_CONFIRM                     ( 0x31U )       /* Write Buffer Confirm */

#define MX25UW_OPI_READ_CMD                             ( 0xEC13U )     /* OPI IO Read */
#define MX25UW_OPI_READ_DTR_CMD                         ( 0xEE11U )     /* OPI IO Read DTR */
#define MX25UW_OPI_PAGE_PROG_CMD                        ( 0x12EDU )     /* OPI Page Program */
#define MX25UW_OPI_SECTOR_ERASE_CMD                     ( 0x21DEU )     /* OPI SubSector Erase 4KB */
#define MX25UW_OPI_BLOCK_ERASE_CMD                      ( 0xDC23U )     /* OPI Sector Erase 64KB */
#define MX25UW_OPI_CHIP_ERASE_CMD                       ( 0x609FU )     /* OPI Bulk Erase */

#define MX25UW_OPI_WRITE_ENABLE_CMD                     ( 0x06F9U )     /* OPI Write Enable */
#define MX25UW_OPI_WRITE_DISABLE_CMD                    ( 0x04FBU )     /* OPI Write Disable */
#define MX25UW_OPI_PROG_ERASE_SUSPEND_CMD               ( 0xB04FU )     /* OPI Program/Erase suspend */
#define MX25UW_OPI_PROG_ERASE_RESUME_CMD                ( 0x30CFU )     /* OPI Program/Erase resume */
#define MX25UW_OPI_ENTER_DEEP_POWER_DOWN_CMD            ( 0xB946U )     /* OPI Enter deep power down */
#define MX25UW_OPI_RELEASE_FROM_DEEP_POWER_DOWN_CMD     ( 0xAB54U )     /* OPI Release from deep power down */
#define MX25UW_OPI_SET_BURST_LENGTH_CMD                 ( 0xC03FU )     /* OPI Set burst length */
#define MX25UW_OPI_ENTER_SECURED_OTP_CMD                ( 0xB14EU )     /* OPI Enter secured OTP) */
#define MX25UW_OPI_EXIT_SECURED_OTP_CMD                 ( 0xC13EU )     /* OPI Exit secured OTP) */

#define MX25UW_OPI_NOP_CMD                              ( 0x00FFU )     /* OPI No operation */
#define MX25UW_OPI_RESET_ENABLE_CMD                     ( 0x6699U )     /* OPI Reset Enable */
#define MX25UW_OPI_RESET_MEMORY_CMD                     ( 0x9966U )     /* OPI Reset Memory */

#define MX25UW_OPI_READ_ID_CMD                          ( 0x9F60U )     /* OPI Read IDentification */
#define MX25UW_OPI_READ_SERIAL_FLASH_DISCO_PARAM_CMD    ( 0x5AA5U )     /* OPI Read Serial Flash Discoverable Parameter */
#define MX25UW_OPI_READ_STATUS_REG_CMD                  ( 0x05FAU )     /* OPI Read Status Register */
#define MX25UW_OPI_READ_CFG_REG_CMD                     ( 0x15EAU )     /* OPI Read configuration Register */
#define MX25UW_OPI_WRITE_STATUS_REG_CMD                 ( 0x01FEU )     /* OPI Write Status Register */
#define MX25UW_OPI_READ_CFG_REG2_CMD                    ( 0x718EU )     /* OPI Read configuration Register2 */
#define MX25UW_OPI_WRITE_CFG_REG2_CMD                   ( 0x728DU )     /* OPI Write configuration Register2 */
#define MX25UW_OPI_READ_FAST_BOOT_REG_CMD               ( 0x16E9U )     /* OPI Read fast boot Register */
#define MX25UW_OPI_WRITE_FAST_BOOT_REG_CMD              ( 0x17E8U )     /* OPI Write fast boot Register */
#define MX25UW_OPI_ERASE_FAST_BOOT_REG_CMD              ( 0x18E7U )     /* OPI Erase fast boot Register */
#define MX25UW_OPI_READ_SECURITY_REG_CMD                ( 0x2BD4U )     /* OPI Read security Register */
#define MX25UW_OPI_WRITE_SECURITY_REG_CMD               ( 0x2FD0U )     /* OPI Write security Register */
#define MX25UW_OPI_READ_LOCK_REG_CMD                    ( 0x2DD2U )     /* OPI Read lock Register */
#define MX25UW_OPI_WRITE_LOCK_REG_CMD                   ( 0x2CD3U )     /* OPI Write lock Register */
#define MX25UW_OPI_READ_DPB_REG_CMD                     ( 0xE01FU )     /* OPI Read DPB register */
#define MX25UW_OPI_WRITE_DPB_REG_CMD                    ( 0xE11EU )     /* OPI Write DPB register */
#define MX25UW_OPI_READ_SPB_STATUS_CMD                  ( 0xE21DU )     /* OPI Read SPB status */
#define MX25UW_OPI_WRITE_SPB_BIT_CMD                    ( 0xE31CU )     /* OPI SPB bit program */
#define MX25UW_OPI_ERASE_ALL_SPB_CMD                    ( 0xE41BU )     /* OPI Erase all SPB bit */
#define MX25UW_OPI_WRITE_PROTECT_SEL_CMD                ( 0x6897U )     /* OPI Write Protect selection */
#define MX25UW_OPI_GANG_BLOCK_LOCK_CMD                  ( 0x7E81U )     /* OPI Gang block lock: whole chip write protect */
#define MX25UW_OPI_GANG_BLOCK_UNLOCK_CMD                ( 0x9867U )     /* OPI Gang block unlock: whole chip write unprote */
#define MX25UW_OPI_READ_PASSWORD_REGISTER_CMD           ( 0x27D8U )     /* OPI Read Password */
#define MX25UW_OPI_WRITE_PASSWORD_REGISTER_CMD          ( 0x28D7U )     /* OPI Write Password */
#define MX25UW_OPI_PASSWORD_UNLOCK_CMD                  ( 0x29D6U )     /* OPI Unlock Password */
#define MX25UW_OPI_PAGE_BUFFER_READ                     ( 0x25DAU )     /* OPI Page Buffer Read */
#define MX25UW_OPI_WRITE_BUFFER_INITIAL                 ( 0x22DDU )     /* OPI Write Buffer Initial */
#define MX25UW_OPI_WRITE_BUFFER_CONTINUE                ( 0x24DBU )     /* OPI Write Buffer Continue */

/* Exported types ---------------------------------------------------------- */

/**
 * Определение перечисления интерфейсов MX25UW.
 */
enum mx25uw_interface
{
    MX25UW_SPI,
    MX25UW_OPI_STR,
    MX25UW_OPI_DTR,
};


/**
 * Определение структуры данных MX25UW.
 */
struct mx25uw
{
    XSPI_TypeDef *  xspi;           /*!< Указатель на структуру данных XSPI */

    uint8_t         interface;      /*!< Интерфейс:
                                         - MX25UW_SPI
                                         - MX25UW_OPI_STR
                                         - MX25UW_OPI_DTR */

    uint8_t         id[ 3 ];        /*!< Идентификатор */
};

/* Exported variables ------------------------------------------------------ */

/* Exported function prototypes -------------------------------------------- */

uint8_t mx25uw_init( void );

uint8_t mx25uw_setup_opi_dtr( void );

uint8_t mx25uw_setup_memory_mapped_mode( void );

/* Exported callback function prototypes ----------------------------------- */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX25UW_H_ */
