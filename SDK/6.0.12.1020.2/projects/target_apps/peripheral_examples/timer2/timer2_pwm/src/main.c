/**
 ****************************************************************************************
 *
 * @file main.c
 *
 * @brief DA14585/DA14586 Timer2 example.
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
#include <stdint.h>
#include "arch_system.h"
#include "uart.h"
#include "uart_utils.h"
#include "user_periph_setup.h"
#include "timer0_2.h"
#include "timer2.h"

/**
 ****************************************************************************************
 * @brief Timer2 test function
 * @return void
 ****************************************************************************************
 */
void timer2_test(void);

/**
 ****************************************************************************************
 * @brief Simple delay function
 * @return void
 ****************************************************************************************
 */
void simple_delay(void);


static tim0_2_clk_div_config_t clk_div_config =
{
    .clk_div  = TIM0_2_CLK_DIV_8
};

static tim2_config_t config =
{
    .hw_pause = TIM2_HW_PAUSE_OFF
};

static tim2_pwm_config_t pwm_config;

/**
 ****************************************************************************************
 * @brief Main routine of the timer2 example
 * @return void
 ****************************************************************************************
 */
int main (void)
{
    system_init();
    periph_init();
    timer2_test();
    while(1);
}

void timer2_test(void)
{
    uint8_t i;
    printf_string(UART, "\n\r\n\r***************");
    printf_string(UART, "\n\r* TIMER2 TEST *\n\r");
    printf_string(UART, "***************\n\r");

    // Enable the Timer0/Timer2 input clock
    timer0_2_clk_enable();
    // Set the Timer0/Timer2 input clock division factor
    timer0_2_clk_div_set(&clk_div_config);

    timer2_config(&config);

    // System clock, divided by 8, is the Timer2 input clock source (according
    // to the clk_div_config struct above).
    timer2_pwm_freq_set(PWM_FREQUENCY, 16000000 / 8);

    timer2_start();

    printf_string(UART, "\n\rTIMER2 started!");
    for (i = 0; i < 100; i += 10)
    {
        // Set PWM2 duty cycle
        pwm_config.pwm_signal = TIM2_PWM_2;
        pwm_config.pwm_dc     = i;
        timer2_pwm_signal_config(&pwm_config);
        
        // Set PWM3 duty cycle
        pwm_config.pwm_signal = TIM2_PWM_3;
        pwm_config.pwm_dc     = i + 5;
        timer2_pwm_signal_config(&pwm_config);
        
        // Set PWM4 duty cycle
        pwm_config.pwm_signal = TIM2_PWM_4;
        pwm_config.pwm_dc     = i + 10;
        timer2_pwm_signal_config(&pwm_config);
        
        // Release sw pause to let PWM2, PWM3, and PWM4 run
        timer2_resume();

        // Delay approx. 1 second to observe duty cycle change
        simple_delay();

        timer2_pause();
    }

    timer2_stop();

    // Disable the Timer0/Timer2 input clock
    timer0_2_clk_disable();

    printf_string(UART, "\n\rTIMER2 stopped!\n\r");
    printf_string(UART, "\n\rEnd of test\n\r");
}

void simple_delay(void)
{
    uint32_t i;

    for (i = 0xFFFFF; i != 0; --i)
        ;
}
