/**
 ****************************************************************************************
 *
 * @file main.c
 *
 * @brief Blinky example
 *
 * Copyright (C) 2012-2019 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 ****************************************************************************************
 */
#include <stdio.h>
#include "arch_system.h"
#include "uart.h"
#include "uart_utils.h"
#include "user_periph_setup.h"
#include "gpio.h"
#include "systick.h"
#include "EPD_2in13_V2.h"
 #include "ImageData.h"
#define LED_OFF_THRESHOLD           10000
#define LED_ON_THRESHOLD            400000
#define SYSTICK_PERIOD_US   1000000     // period for systick timer in us, so 1000000ticks = 1second
#define SYSTICK_EXCEPTION   0           // generate systick exceptions
/**
 ****************************************************************************************
 * @brief Blinky test function
 ****************************************************************************************
 */
void blinky_test(void);

/**
 ****************************************************************************************
 * @brief Main routine of the Blinky example
 ****************************************************************************************
 */
int main (void)
{    
    system_init();
    EPD_GPIO_init();
    EPD_2IN13_V2_Init_RED();
//    EPD_2IN13_V2_Init(0);
    EPD_2IN13_V2_Clear_RED();
//    EPD_2IN13_V2_Clear();
    EPD_2IN13_V2_TurnOnDisplay_RED();
//    EPD_2IN13_V2_TurnOnDisplay();
    DEV_Delay_ms(100);
    EPD_2IN13_V2_DisplayPartBaseImage((UBYTE *)gImage_2in13d);
    EPD_2IN13_V2_TurnOnDisplay_RED();
//    EPD_2IN13_V2_TurnOnDisplay();
    EPD_2IN13_V2_Sleep();
    //blinky_test();
    while(1);
}

void blinky_test(void)
{
    volatile int i=0;
    

    while(1)
    {
        i++;

         
         GPIO_SetActive(LED_PORT, LED_PIN);
         //printf_string(UART, "\n\r *LED ON* ");
				 
         //systick_wait (1000);

       
         //GPIO_SetInactive(LED_PORT, LED_PIN);
         //printf_string(UART, "\n\r *LED OFF* ");
				 
         //systick_wait (1000);

        if (i == 2 * LED_ON_THRESHOLD)
        {
            i = 0;
        }
    }
}
