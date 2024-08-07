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

#include "main.h"
#include "systick.h"

/* Private macros ---------------------------------------------------------- */

/* Private constants ------------------------------------------------------- */

/* Private types ----------------------------------------------------------- */

/* Private variables ------------------------------------------------------- */

/* Private function prototypes --------------------------------------------- */

static void setup_hardware( void );

static void setup_vector_table( void );

static void setup_fpu( void );

static void app_main( void );

/* Private user code ------------------------------------------------------- */

int main( void )
{
    setup_hardware();
    app_main();
}
/* ------------------------------------------------------------------------- */

void error( void )
{
    __disable_irq();

    for( ;; )
        ;
}
/* ------------------------------------------------------------------------- */

static void app_main( void )
{
    for( ;; )
        ;
}
/* ------------------------------------------------------------------------- */

static void setup_hardware( void )
{
    setup_vector_table();
    setup_fpu();

    systick_init( 64000000 );
}
/* ------------------------------------------------------------------------- */

static void setup_vector_table( void  )
{
    __set_PRIMASK( 1 );
    {
        WRITE_REG( SCB->VTOR, 0x08000000 );
    }
    __set_PRIMASK( 0 );

    __enable_irq();
}
/* ------------------------------------------------------------------------- */

static void setup_fpu( void )
{
    SET_BIT( SCB->CPACR, ( 0x03 << 20 ) | ( 0x03 << 22 ) );
}
/* ------------------------------------------------------------------------- */
