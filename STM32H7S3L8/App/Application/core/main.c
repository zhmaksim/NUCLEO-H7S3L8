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

/* Private macros ---------------------------------------------------------- */

/* Private constants ------------------------------------------------------- */

/* Private types ----------------------------------------------------------- */

/* Private variables ------------------------------------------------------- */

/* Private function prototypes --------------------------------------------- */

static void setup_vector_table( void  );

static void app_main( void );

/* Private user code ------------------------------------------------------- */

int main( void )
{
    setup_vector_table();
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

static void setup_vector_table( void  )
{
    __set_PRIMASK( 1 );
    {
        WRITE_REG( SCB->VTOR, 0x70000000 );
    }
    __set_PRIMASK( 0 );

    __enable_irq();
}
/* ------------------------------------------------------------------------- */
