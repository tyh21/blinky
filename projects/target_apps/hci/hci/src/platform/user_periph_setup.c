/**
 ****************************************************************************************
 *
 * @file periph_setup.c
 *
 * @brief Peripherals setup and initialization.
 *
 * Copyright (C) 2012-2019 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "user_periph_setup.h"
#include "datasheet.h"
#include "system_library.h"
#include "rwip_config.h"
#include "gpio.h"
#include "uart.h"
#include "syscntl.h"

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

#if DEVELOPMENT_DEBUG

void GPIO_reservations(void)
{
/*
    i.e. to reserve P0_1 as Generic Purpose I/O:
    RESERVE_GPIO(DESCRIPTIVE_NAME, GPIO_PORT_0, GPIO_PIN_1, PID_GPIO);
*/

    RESERVE_GPIO(UART1_TX, UART1_TX_PORT, UART1_TX_PIN, PID_UART1_TX);
    RESERVE_GPIO(UART1_RX, UART1_RX_PORT, UART1_RX_PIN, PID_UART1_RX);
    RESERVE_GPIO(UART1_RTS, UART1_RTSN_PORT, UART1_RTSN_PIN, PID_UART1_RTSN);
    RESERVE_GPIO(UART1_CTS, UART1_CTSN_PORT, UART1_CTSN_PIN, PID_UART1_CTSN);

#if defined (CFG_PRINTF_UART2)
    RESERVE_GPIO(UART2_TX,        UART2_TX_PORT,   UART2_TX_PIN,   PID_UART2_TX);
#endif

#if defined (CFG_WAKEUP_EXT_PROCESSOR)
    // external MCU wakeup GPIO
    RESERVE_GPIO(EXT_WAKEUP_GPIO, EXT_WAKEUP_PORT,      EXT_WAKEUP_PIN,      PID_GPIO);
#endif
}
#endif

void set_pad_functions(void)
{
/*
    i.e. to set P0_1 as Generic purpose Output:
    GPIO_ConfigurePin(GPIO_PORT_0, GPIO_PIN_1, OUTPUT, PID_GPIO, false);
*/

#if defined (__DA14586__)
    // Disallow spontaneous DA14586 SPI Flash wake-up
    GPIO_ConfigurePin(GPIO_PORT_2, GPIO_PIN_3, OUTPUT, PID_GPIO, true);
#endif

    GPIO_ConfigurePin(UART1_TX_PORT, UART1_TX_PIN, OUTPUT, PID_UART1_TX, false);
    GPIO_ConfigurePin(UART1_RX_PORT, UART1_RX_PIN, INPUT, PID_UART1_RX, false);
    GPIO_ConfigurePin(UART1_RTSN_PORT, UART1_RTSN_PIN, OUTPUT, PID_UART1_RTSN, false);
    GPIO_ConfigurePin(UART1_CTSN_PORT, UART1_CTSN_PIN, INPUT, PID_UART1_CTSN, false);

#if defined (CFG_PRINTF_UART2)
    GPIO_ConfigurePin(UART2_TX_PORT, UART2_TX_PIN, OUTPUT, PID_UART2_TX, false);
#endif

#if defined (CFG_WAKEUP_EXT_PROCESSOR)
    // External MCU wakeup GPIO
    GPIO_ConfigurePin(EXT_WAKEUP_PORT, EXT_WAKEUP_PIN, OUTPUT, PID_GPIO, false);
#endif
}

#if defined (CFG_PRINTF_UART2)
// Configuration struct for UART2
static const uart_cfg_t uart_cfg = {
    .baud_rate = UART2_BAUDRATE,
    .data_bits = UART2_DATABITS,
    .parity = UART2_PARITY,
    .stop_bits = UART2_STOPBITS,
    .auto_flow_control = UART2_AFCE,
    .use_fifo = UART2_FIFO,
    .tx_fifo_tr_lvl = UART2_TX_FIFO_LEVEL,
    .rx_fifo_tr_lvl = UART2_RX_FIFO_LEVEL,
    .intr_priority = 2,
};
#endif

void periph_init(void)
{
#if defined (__DA14531__)
    // Disable HW RST on P0_0
    GPIO_Disable_HW_Reset();

    // In Boost mode enable the DCDC converter to supply VBAT_HIGH for the used GPIOs
    syscntl_dcdc_turn_on_in_boost(SYSCNTL_DCDC_LEVEL_3V0);
#else
    // Power up peripherals' power domain
    SetBits16(PMU_CTRL_REG, PERIPH_SLEEP, 0);
    while (!(GetWord16(SYS_STAT_REG) & PER_IS_UP));
    SetBits16(CLK_16M_REG, XTAL16_BIAS_SH_ENABLE, 1);
#endif

    // ROM patch
    patch_func();

    // Initialize UART1 ROM driver
    uart_init(BAUD_RATE_DIV(UART1_BAUDRATE), BAUD_RATE_FRAC(UART1_BAUDRATE), UART1_DATABITS);

    // Initialize peripherals
#if defined (CFG_PRINTF_UART2)
    // Initialize UART2
    uart_initialize(UART2, &uart_cfg);
#endif

    // Set pad functionality
    set_pad_functions();

    // Enable the pads
    GPIO_set_pad_latch_en(true);
}
