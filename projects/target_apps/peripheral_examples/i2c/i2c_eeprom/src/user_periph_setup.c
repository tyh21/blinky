/**
 ****************************************************************************************
 *
 * @file periph_setup.c
 *
 * @brief Peripherals initialization functions
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
#include "gpio.h"
#include "uart.h"
#include "i2c.h"
#include "i2c_eeprom.h"
#include "syscntl.h"

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

static void set_pad_functions(void)
{
/*
    i.e. to set P0_1 as Generic purpose Output:
    GPIO_ConfigurePin(GPIO_PORT_0, GPIO_PIN_1, OUTPUT, PID_GPIO, false);
*/

#if defined (__DA14586__)
    // Disallow spontaneous DA14586 SPI Flash wake-up
    GPIO_ConfigurePin(GPIO_PORT_2, GPIO_PIN_3, OUTPUT, PID_GPIO, true);
#endif

    // Configure UART2 pin functionality
    GPIO_ConfigurePin(UART2_TX_PORT, UART2_TX_PIN, OUTPUT, PID_UART2_TX, false);

    // Configure I2C pin functionality
    GPIO_ConfigurePin(I2C_SCL_PORT, I2C_SCL_PIN, INPUT, PID_I2C_SCL, false);
    GPIO_ConfigurePin(I2C_SDA_PORT, I2C_SDA_PIN, INPUT, PID_I2C_SDA, false);
}

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

// Configuration struct for I2C
static const i2c_cfg_t i2c_cfg = {
    .clock_cfg.ss_hcnt = I2C_SS_SCL_HCNT_REG_RESET,
    .clock_cfg.ss_lcnt = I2C_SS_SCL_LCNT_REG_RESET,
    .clock_cfg.fs_hcnt = I2C_FS_SCL_HCNT_REG_RESET,
    .clock_cfg.fs_lcnt = I2C_FS_SCL_LCNT_REG_RESET,
    .restart_en = I2C_RESTART_ENABLE,
    .speed = I2C_SPEED_MODE,
    .mode = I2C_MODE_MASTER,
    .addr_mode = I2C_ADDRESS_MODE,
    .address = I2C_SLAVE_ADDRESS,
    .tx_fifo_level = 1,
    .rx_fifo_level = 1,
};

// Configuration struct for I2C EEPROM
static const i2c_eeprom_cfg_t i2c_eeprom_cfg = {
    .size = I2C_EEPROM_DEV_SIZE,
    .page_size = I2C_EEPROM_PAGE_SIZE,
    .address_size = I2C_ADDRESS_SIZE,
};

void periph_init(void)
{
#if defined (__DA14531__)
    // Disable HW Reset functionality of P0_0
    GPIO_Disable_HW_Reset();
    // In Boost mode enable the DCDC converter to supply VBAT_HIGH for the used GPIOs
    // Assumption: The connected external peripheral is powered by 3V
    syscntl_dcdc_turn_on_in_boost(SYSCNTL_DCDC_LEVEL_3V0);
#else
    // Power up peripherals' power domain
    SetBits16(PMU_CTRL_REG, PERIPH_SLEEP, 0);
    while (!(GetWord16(SYS_STAT_REG) & PER_IS_UP));
    SetBits16(CLK_16M_REG, XTAL16_BIAS_SH_ENABLE, 1);
#endif

    // Initialize UART2
    uart_initialize(UART, &uart_cfg);

    // Configure I2C EEPROM environment
    i2c_eeprom_configure(&i2c_cfg, &i2c_eeprom_cfg);

    // Initialize I2C
    i2c_eeprom_initialize();

    // Set pad functionality
    set_pad_functions();

    // Enable the pads
    GPIO_set_pad_latch_en(true);
}
