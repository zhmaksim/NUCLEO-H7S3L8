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

/* Includes ---------------------------------------------------------------- */

#include "mx25uw.h"
#include "systick.h"

/* Private macros ---------------------------------------------------------- */

/* Private constants ------------------------------------------------------- */

#define MX25UW_XSPI_TIMEOUT     ( 5000U )

/* Private types ----------------------------------------------------------- */

/* Private variables ------------------------------------------------------- */

struct mx25uw mx25uw;

/* Private function prototypes --------------------------------------------- */

static uint8_t mx25uw_read_id( void );

static uint8_t mx25uw_write_enable( void );

static uint8_t mx25uw_write_cfg_reg2( uint32_t address, uint8_t value );

/* Private user code ------------------------------------------------------- */

/**
 * Инициализировать MX25UW.
 *
 * @return Статус:
 *      - SUCCESS;
 *      - ERROR.
 */
uint8_t mx25uw_init( void )
{
    mx25uw.xspi = XSPI2;
    mx25uw.interface = MX25UW_SPI;

    if( mx25uw_read_id() != SUCCESS )
    {
        return ERROR;
    }
    else if( mx25uw.id[ 0 ] != MX25UW_MANUFACTURER_ID )
    {
        return ERROR;
    }
    else return SUCCESS;
}
/* ------------------------------------------------------------------------- */

/**
 * Прочитать идентификационные данные.
 *
 * @return Статус:
 *      - SUCCESS;
 *      - ERROR.
 */
static uint8_t mx25uw_read_id( void )
{
    uint32_t tickstart = systick_tick();

    /* Размер данных идентификатора */
    uint32_t data_size = sizeof( mx25uw.id );
    /* Указатель на данные идентификатора */
    uint8_t *pdata = ( uint8_t * ) &mx25uw.id;
    /* Указатель на регистр данных XSPI */
    uint8_t *DR = ( uint8_t * ) &mx25uw.xspi->DR;

    /* Ожидание готовности XSPI */
    while( READ_BIT( mx25uw.xspi->SR, XSPI_SR_BUSY_Msk ) )
    {
        if( systick_tick() - tickstart >= MX25UW_XSPI_TIMEOUT ) return ERROR;
    }

    /* Настроить Functional Mode */
    MODIFY_REG( mx25uw.xspi->CR,
                XSPI_CR_FMODE_Msk,
                0x01 << XSPI_CR_FMODE_Pos );

    /* Настоить DLR */
    WRITE_REG( mx25uw.xspi->DLR, data_size - 1 );

    if( mx25uw.interface == MX25UW_SPI )
    {
        /* Настоить TCR */
        CLEAR_REG( mx25uw.xspi->TCR );

        /* Настроить CCR */
        WRITE_REG( mx25uw.xspi->CCR,
                   0x01 << XSPI_CCR_IMODE_Pos
                 | 0x01 << XSPI_CCR_DMODE_Pos );

        /* Настроить IR */
        WRITE_REG( mx25uw.xspi->IR, MX25UW_READ_ID_CMD );
    }
    else if( mx25uw.interface == MX25UW_OPI_STR )
    {
        /* Настоить TCR */
        WRITE_REG( mx25uw.xspi->TCR, 0x04 << XSPI_TCR_DCYC_Pos );

        /* Настроить CCR */
        WRITE_REG( mx25uw.xspi->CCR,
                   0x04 << XSPI_CCR_IMODE_Pos
                 | 0x01 << XSPI_CCR_ISIZE_Pos
                 | 0x04 << XSPI_CCR_ADMODE_Pos
                 | 0x03 << XSPI_CCR_ADSIZE_Pos
                 | 0x04 << XSPI_CCR_DMODE_Pos );

        /* Настроить IR */
        WRITE_REG( mx25uw.xspi->IR, MX25UW_OPI_READ_ID_CMD );

        /* Настроить AR */
        WRITE_REG( mx25uw.xspi->AR, 0x00000000 );
    }
    else if( mx25uw.interface == MX25UW_OPI_DTR )
    {
        /* Настоить TCR */
        WRITE_REG( mx25uw.xspi->TCR,
                   0x04 << XSPI_TCR_DCYC_Pos
                 | XSPI_TCR_DHQC_Msk );

        /* Настроить CCR */
        WRITE_REG( mx25uw.xspi->CCR,
                   0x04 << XSPI_CCR_IMODE_Pos
                 | XSPI_CCR_IDTR_Msk
                 | 0x01 << XSPI_CCR_ISIZE_Pos
                 | 0x04 << XSPI_CCR_ADMODE_Pos
                 | XSPI_CCR_ADDTR_Msk
                 | 0x03 << XSPI_CCR_ADSIZE_Pos
                 | 0x04 << XSPI_CCR_DMODE_Pos
                 | XSPI_CCR_DQSE_Msk );

        /* Настроить IR */
        WRITE_REG( mx25uw.xspi->IR, MX25UW_OPI_READ_ID_CMD );

        /* Настроить AR */
        WRITE_REG( mx25uw.xspi->AR, 0x00000000 );
    }
    else return ERROR;

    /* Принять данные */
    while( data_size > 0 )
    {
        /* Ожидание возможности приема данных */
        while( !READ_BIT( mx25uw.xspi->SR,
                          XSPI_SR_FTF_Msk
                        | XSPI_SR_TCF_Msk ) )
        {
            if( systick_tick() - tickstart >= MX25UW_XSPI_TIMEOUT ) return ERROR;
        }

        /* Записать принятые данные */
        *pdata = *DR;

        pdata++;
        data_size--;
    }

    /* Ожидание завершения операции */
    while( !READ_BIT( mx25uw.xspi->SR, XSPI_SR_TCF_Msk ) )
    {
        if( systick_tick() - tickstart >= MX25UW_XSPI_TIMEOUT ) return ERROR;
    }

    SET_BIT( mx25uw.xspi->FCR, XSPI_FCR_CTCF_Msk );

    return SUCCESS;
}
/* ------------------------------------------------------------------------- */

/**
 * Включить разрешение на запись.
 *
 * @return Статус:
 *      - SUCCESS;
 *      - ERROR.
 */
static uint8_t mx25uw_write_enable( void )
{
    uint32_t tickstart = systick_tick();

    /* Ожидание готовности XSPI */
    while( READ_BIT( mx25uw.xspi->SR, XSPI_SR_BUSY_Msk ) )
    {
        if( systick_tick() - tickstart >= MX25UW_XSPI_TIMEOUT ) return ERROR;
    }

    /* Настроить Functional Mode */
    CLEAR_BIT( mx25uw.xspi->CR, XSPI_CR_FMODE_Msk );

    /* Настоить DLR */
    CLEAR_REG( mx25uw.xspi->DLR );

    if( mx25uw.interface == MX25UW_SPI )
    {
        /* Настоить TCR */
        CLEAR_REG( mx25uw.xspi->TCR );

        /* Настроить CCR */
        WRITE_REG( mx25uw.xspi->CCR,
                   0x01 << XSPI_CCR_IMODE_Pos );

        /* Настроить IR */
        WRITE_REG( mx25uw.xspi->IR, MX25UW_WRITE_ENABLE_CMD );
    }
    else if( mx25uw.interface == MX25UW_OPI_STR )
    {
        /* Настоить TCR */
        CLEAR_REG( mx25uw.xspi->TCR );

        /* Настроить CCR */
        WRITE_REG( mx25uw.xspi->CCR,
                   0x04 << XSPI_CCR_IMODE_Pos
                 | 0x01 << XSPI_CCR_ISIZE_Pos );

        /* Настроить IR */
        WRITE_REG( mx25uw.xspi->IR, MX25UW_OPI_WRITE_ENABLE_CMD );
    }
    else if( mx25uw.interface == MX25UW_OPI_DTR )
    {
        /* Настоить TCR */
        WRITE_REG( mx25uw.xspi->TCR, XSPI_TCR_DHQC_Msk );

        /* Настроить CCR */
        WRITE_REG( mx25uw.xspi->CCR,
                   0x04 << XSPI_CCR_IMODE_Pos
                 | XSPI_CCR_IDTR_Msk
                 | 0x01 << XSPI_CCR_ISIZE_Pos );

        /* Настроить IR */
        WRITE_REG( mx25uw.xspi->IR, MX25UW_OPI_WRITE_ENABLE_CMD );
    }
    else return ERROR;

    /* Ожидание завершения операции */
    while( !READ_BIT( mx25uw.xspi->SR, XSPI_SR_TCF_Msk ) )
    {
        if( systick_tick() - tickstart >= MX25UW_XSPI_TIMEOUT ) return ERROR;
    }

    SET_BIT( mx25uw.xspi->FCR, XSPI_FCR_CTCF_Msk );

    return SUCCESS;
}
/* ------------------------------------------------------------------------- */

/**
 * Записать значение в конфигурационный регистр 2.
 *
 * @param[in] address Адрес.
 * @param[in] value Значение.
 * @return Статус:
 *      - SUCCESS;
 *      - ERROR.
 */
static uint8_t mx25uw_write_cfg_reg2( uint32_t address, uint8_t value )
{
    uint32_t tickstart = systick_tick();

    /* Размер данных регистра */
    uint32_t data_size = sizeof( value );
    /* Указатель на данные регистра */
    uint8_t *pdata = ( uint8_t * ) &value;
    /* Указатель на регистр данных XSPI */
    uint8_t *DR = ( uint8_t * ) &mx25uw.xspi->DR;

    /* Ожидание готовности XSPI */
    while( READ_BIT( mx25uw.xspi->SR, XSPI_SR_BUSY_Msk ) )
    {
        if( systick_tick() - tickstart >= MX25UW_XSPI_TIMEOUT ) return ERROR;
    }

    /* Настроить Functional Mode */
    CLEAR_BIT( mx25uw.xspi->CR, XSPI_CR_FMODE_Msk );

    /* Настоить DLR */
    WRITE_REG( mx25uw.xspi->DLR, data_size - 1 );

    if( mx25uw.interface == MX25UW_SPI )
    {
        /* Настоить TCR */
        CLEAR_REG( mx25uw.xspi->TCR );

        /* Настроить CCR */
        WRITE_REG( mx25uw.xspi->CCR,
                   0x01 << XSPI_CCR_IMODE_Pos
                 | 0x01 << XSPI_CCR_ADMODE_Pos
                 | 0x03 << XSPI_CCR_ADSIZE_Pos
                 | 0x01 << XSPI_CCR_DMODE_Pos );

        /* Настроить IR */
        WRITE_REG( mx25uw.xspi->IR, MX25UW_WRITE_CFG_REG2_CMD );

        /* Настроить AR */
        WRITE_REG( mx25uw.xspi->AR, address );
    }
    else if( mx25uw.interface == MX25UW_OPI_STR )
    {
        /* Настоить TCR */
        CLEAR_REG( mx25uw.xspi->TCR );

        /* Настроить CCR */
        WRITE_REG( mx25uw.xspi->CCR,
                   0x04 << XSPI_CCR_IMODE_Pos
                 | 0x01 << XSPI_CCR_ISIZE_Pos
                 | 0x04 << XSPI_CCR_ADMODE_Pos
                 | 0x03 << XSPI_CCR_ADSIZE_Pos
                 | 0x04 << XSPI_CCR_DMODE_Pos );

        /* Настроить IR */
        WRITE_REG( mx25uw.xspi->IR, MX25UW_OPI_WRITE_CFG_REG2_CMD );

        /* Настроить AR */
        WRITE_REG( mx25uw.xspi->AR, address );
    }
    else if( mx25uw.interface == MX25UW_OPI_DTR )
    {
        /* Настоить TCR */
        CLEAR_REG( mx25uw.xspi->TCR );

        /* Настроить CCR */
        WRITE_REG( mx25uw.xspi->CCR,
                   0x04 << XSPI_CCR_IMODE_Pos
                 | XSPI_CCR_IDTR_Msk
                 | 0x01 << XSPI_CCR_ISIZE_Pos
                 | 0x04 << XSPI_CCR_ADMODE_Pos
                 | XSPI_CCR_ADDTR_Msk
                 | 0x03 << XSPI_CCR_ADSIZE_Pos
                 | 0x04 << XSPI_CCR_DMODE_Pos
                 | XSPI_CCR_DDTR_Msk );

        /* Настроить IR */
        WRITE_REG( mx25uw.xspi->IR, MX25UW_OPI_WRITE_CFG_REG2_CMD );

        /* Настроить AR */
        WRITE_REG( mx25uw.xspi->AR, address );
    }
    else return ERROR;

    /* Передать данные */
    while( data_size > 0 )
    {
        /* Ожидание возможности передачи данных */
        while( !READ_BIT( mx25uw.xspi->SR, XSPI_SR_FTF_Msk ) )
        {
            if( systick_tick() - tickstart >= MX25UW_XSPI_TIMEOUT ) return ERROR;
        }

        /* Записать передаваемые данные */
        *DR = *pdata;

        pdata++;
        data_size--;
    }

    /* Ожидание завершения операции */
    while( !READ_BIT( mx25uw.xspi->SR, XSPI_SR_TCF_Msk ) )
    {
        if( systick_tick() - tickstart >= MX25UW_XSPI_TIMEOUT ) return ERROR;
    }

    SET_BIT( mx25uw.xspi->FCR, XSPI_FCR_CTCF_Msk );

    return SUCCESS;
}
/* ------------------------------------------------------------------------- */

/**
 * Настроить интерфейс OPI DTR.
 *
 * @return Статус:
 *      - SUCCESS;
 *      - ERROR.
 */
uint8_t mx25uw_setup_opi_dtr( void )
{
    if( mx25uw_write_enable() != SUCCESS )
    {
        return ERROR;
    }
    else if( mx25uw_write_cfg_reg2( 0x00000000, MX25UW_OPI_DTR ) != SUCCESS )
    {
        return ERROR;
    }
    else
    {
        mx25uw.interface = MX25UW_OPI_DTR;
        return SUCCESS;
    }
}
/* ------------------------------------------------------------------------- */

/**
 * Настроить Memory Mapped Mode.
 *
 * @return Статус:
 *      - SUCCESS;
 *      - ERROR.
 */
uint8_t mx25uw_setup_memory_mapped_mode( void )
{
    uint32_t tickstart = systick_tick();

    /* Ожидание готовности XSPI */
    while( READ_BIT( mx25uw.xspi->SR, XSPI_SR_BUSY_Msk ) )
    {
        if( systick_tick() - tickstart >= MX25UW_XSPI_TIMEOUT ) return ERROR;
    }

    /* Сбросить Functional Mode */
    CLEAR_BIT( mx25uw.xspi->CR, XSPI_CR_FMODE_Msk );

    if( mx25uw.interface == MX25UW_SPI )
    {
        /* Настоить TCR */
        WRITE_REG( mx25uw.xspi->TCR, 0x08 << XSPI_TCR_DCYC_Pos );

        /* Настроить CCR */
        WRITE_REG( mx25uw.xspi->CCR,
                   0x01 << XSPI_CCR_IMODE_Pos
                 | 0x01 << XSPI_CCR_ADMODE_Pos
                 | 0x02 << XSPI_CCR_ADSIZE_Pos
                 | 0x01 << XSPI_CCR_DMODE_Pos );

        /* Настроить IR */
        WRITE_REG( mx25uw.xspi->IR, MX25UW_FAST_READ_CMD );

        /* Настоить WTCR */
        CLEAR_REG( mx25uw.xspi->WTCR );

        /* Настроить WCCR */
        WRITE_REG( mx25uw.xspi->WCCR,
                   0x01 << XSPI_WCCR_IMODE_Pos
                 | 0x01 << XSPI_WCCR_ADMODE_Pos
                 | 0x02 << XSPI_WCCR_ADSIZE_Pos
                 | 0x01 << XSPI_WCCR_DMODE_Pos );

        /* Настроить WIR */
        WRITE_REG( mx25uw.xspi->WIR, MX25UW_PAGE_PROG_CMD );
    }
    else if( mx25uw.interface == MX25UW_OPI_STR )
    {
        /* Настоить TCR */
        WRITE_REG( mx25uw.xspi->TCR, 0x14 << XSPI_TCR_DCYC_Pos );

        /* Настроить CCR */
        WRITE_REG( mx25uw.xspi->CCR,
                   0x04 << XSPI_CCR_IMODE_Pos
                 | 0x01 << XSPI_CCR_ISIZE_Pos
                 | 0x04 << XSPI_CCR_ADMODE_Pos
                 | 0x03 << XSPI_CCR_ADSIZE_Pos
                 | 0x04 << XSPI_CCR_DMODE_Pos
                 | XSPI_CCR_DQSE_Msk );

        /* Настроить IR */
        WRITE_REG( mx25uw.xspi->IR, MX25UW_OPI_READ_CMD );

        /* Настоить WTCR */
        CLEAR_REG( mx25uw.xspi->WTCR );

        /* Настроить WCCR */
        WRITE_REG( mx25uw.xspi->WCCR,
                   0x04 << XSPI_WCCR_IMODE_Pos
                 | 0x01 << XSPI_WCCR_ISIZE_Pos
                 | 0x04 << XSPI_WCCR_ADMODE_Pos
                 | 0x03 << XSPI_WCCR_ADSIZE_Pos
                 | 0x04 << XSPI_WCCR_DMODE_Pos );

        /* Настроить WIR */
        WRITE_REG( mx25uw.xspi->WIR, MX25UW_OPI_PAGE_PROG_CMD );
    }
    else if( mx25uw.interface == MX25UW_OPI_DTR )
    {
        /* Настоить TCR */
        WRITE_REG( mx25uw.xspi->TCR,
                   0x14 << XSPI_TCR_DCYC_Pos
                 | XSPI_TCR_DHQC_Msk );

        /* Настроить CCR */
        WRITE_REG( mx25uw.xspi->CCR,
                   0x04 << XSPI_CCR_IMODE_Pos
                 | XSPI_CCR_IDTR_Msk
                 | 0x01 << XSPI_CCR_ISIZE_Pos
                 | 0x04 << XSPI_CCR_ADMODE_Pos
                 | XSPI_CCR_ADDTR_Msk
                 | 0x03 << XSPI_CCR_ADSIZE_Pos
                 | 0x04 << XSPI_CCR_DMODE_Pos
                 | XSPI_CCR_DDTR_Msk
                 | XSPI_CCR_DQSE_Msk );

        /* Настроить IR */
        WRITE_REG( mx25uw.xspi->IR, MX25UW_OPI_READ_DTR_CMD );

        /* Настоить WTCR */
        CLEAR_REG( mx25uw.xspi->WTCR );

        /* Настроить WCCR */
        WRITE_REG( mx25uw.xspi->WCCR,
                   0x04 << XSPI_WCCR_IMODE_Pos
                 | XSPI_WCCR_IDTR_Msk
                 | 0x01 << XSPI_WCCR_ISIZE_Pos
                 | 0x04 << XSPI_WCCR_ADMODE_Pos
                 | XSPI_WCCR_ADDTR_Msk
                 | 0x03 << XSPI_WCCR_ADSIZE_Pos
                 | 0x04 << XSPI_WCCR_DMODE_Pos
                 | XSPI_WCCR_DDTR_Msk );

        /* Настроить WIR */
        WRITE_REG( mx25uw.xspi->WIR, MX25UW_OPI_PAGE_PROG_CMD );
    }
    else return ERROR;

    /* Ожидание готовности XSPI */
    while( READ_BIT( mx25uw.xspi->SR, XSPI_SR_BUSY_Msk ) )
    {
        if( systick_tick() - tickstart >= MX25UW_XSPI_TIMEOUT ) return ERROR;
    }

    /* Настроить Memory Mapped Mode */
    MODIFY_REG( mx25uw.xspi->CR,
                XSPI_CR_FMODE_Msk,
                0x03 << XSPI_CR_FMODE_Pos );

    return SUCCESS;
}
/* ------------------------------------------------------------------------- */
