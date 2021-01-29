/**
 ****************************************************************************************
 *
 * @file main.c
 *
 * @brief Blinky example
 *
 * Copyright (c) 2012-2019 Dialog Semiconductor. All rights reserved.
 *
 * This software ("Software") is owned by Dialog Semiconductor.
 *
 * By using this Software you agree that Dialog Semiconductor retains all
 * intellectual property and proprietary rights in and to this Software and any
 * use, reproduction, disclosure or distribution of the Software without express
 * written permission or a license agreement from Dialog Semiconductor is
 * strictly prohibited. This Software is solely for use on or in conjunction
 * with Dialog Semiconductor products.
 *
 * EXCEPT AS OTHERWISE PROVIDED IN A LICENSE AGREEMENT BETWEEN THE PARTIES, THE
 * SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. EXCEPT AS OTHERWISE
 * PROVIDED IN A LICENSE AGREEMENT BETWEEN THE PARTIES, IN NO EVENT SHALL
 * DIALOG SEMICONDUCTOR BE LIABLE FOR ANY DIRECT, SPECIAL, INDIRECT, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THE SOFTWARE.
 *
 ****************************************************************************************
 */
#include <stdio.h>
#include "arch_system.h"
#include "uart.h"
#include "uart_utils.h"
#include "user_periph_setup.h"
#include "gpio.h"



#define LED_OFF_THRESHOLD           10000
#define LED_ON_THRESHOLD            400000

/**
 ****************************************************************************************
 * @brief Blinky test function
 * @return void
 ****************************************************************************************
 */
void blinky_test(void);

/**
 ****************************************************************************************
 * @brief Main routine of the Blinky example
 * @return void
 ****************************************************************************************
 */
int main (void)
{
    system_init();
    periph_init();
    blinky_test();
    while(1);
}

void blinky_test(void)
{
    volatile int i=0;

    printf_string(UART, "\n\r\n\r");
    printf_string(UART, "***************\n\r");
    printf_string(UART, "* BLINKY DEMO *\n\r");
    printf_string(UART, "***************\n\r");

    while(1)
    {
        i++;

        if (LED_OFF_THRESHOLD == i)
        {
            GPIO_SetActive(LED_PORT, LED_PIN);
            printf_string(UART, "\n\r *LED ON* ");
        }

        if (LED_ON_THRESHOLD == i)
        {
            GPIO_SetInactive(LED_PORT, LED_PIN);
            printf_string(UART, "\n\r *LED OFF* ");
        }

        if (i == 2 * LED_ON_THRESHOLD)
        {
            i = 0;
        }
    }
}
