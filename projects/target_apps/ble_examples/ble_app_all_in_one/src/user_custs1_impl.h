/**
 ****************************************************************************************
 *
 * @file user_custs1_impl.h
 *
 * @brief All in one project Custom1 Server implementation header file.
 *
 * Copyright (C) 2015-2019 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef _USER_CUSTS1_IMPL_H_
#define _USER_CUSTS1_IMPL_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "custs1_task.h"
#include "gapc_task.h"                 // gap functions and messages
#include "gapm_task.h"                 // gap functions and messages

/*
 * DEFINES
 ****************************************************************************************
 */

enum
{
    CUSTS1_BTN_STATE_RELEASED = 0,
    CUSTS1_BTN_STATE_PRESSED,
};

enum
{
    CUSTS1_CP_CMD_ADC_VAL1_DISABLE = 0,
    CUSTS1_CP_CMD_ADC_VAL1_ENABLE,
    CUSTS1_CP_CMD_PWM_DISABLE,
    CUSTS1_CP_CMD_PWM_ENABLE,
    CUSTS1_CP_CMD_ADC_VAL_2_DISABLE,
    CUSTS1_CP_CMD_ADC_VAL_2_ENABLE,
};

enum
{
    CUSTS1_LED_OFF = 0,
    CUSTS1_LED_ON,
};

struct app_proj_env_tag
{
    uint8_t custs1_pwm_enabled;
    uint8_t custs1_adcval2_enabled;
    uint8_t custs1_btn_state;
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Control point write indication handler.
 * @param[in] msgid   Id of the message received.
 * @param[in] param   Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance.
 * @param[in] src_id  ID of the sending task instance.
 ****************************************************************************************
*/
void user_svc1_ctrl_wr_ind_handler(ke_msg_id_t const msgid,
                                   struct custs1_val_write_ind const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief Led state value write indication handler.
 * @param[in] msgid   Id of the message received.
 * @param[in] param   Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance.
 * @param[in] src_id  ID of the sending task instance.
 ****************************************************************************************
*/
void user_svc1_led_wr_ind_handler(ke_msg_id_t const msgid,
                                  struct custs1_val_write_ind const *param,
                                  ke_task_id_t const dest_id,
                                  ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief Long value configuration indication handler.
 * @param[in] msgid   Id of the message received.
 * @param[in] param   Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance.
 * @param[in] src_id  ID of the sending task instance.
 ****************************************************************************************
*/
void user_svc1_long_val_cfg_ind_handler(ke_msg_id_t const msgid,
                                        struct custs1_val_write_ind const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief Long value write indication handler.
 * @param[in] msgid   Id of the message received.
 * @param[in] param   Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance.
 * @param[in] src_id  ID of the sending task instance.
 ****************************************************************************************
*/
void user_svc1_long_val_wr_ind_handler(ke_msg_id_t const msgid,
                                       struct custs1_val_write_ind const *param,
                                       ke_task_id_t const dest_id,
                                       ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief ADC value 1 configuration indication handler.
 * @param[in] msgid   Id of the message received.
 * @param[in] param   Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance.
 * @param[in] src_id  ID of the sending task instance.
 ****************************************************************************************
*/
void user_svc1_adc_val_1_cfg_ind_handler(ke_msg_id_t const msgid,
                                         struct custs1_val_write_ind const *param,
                                         ke_task_id_t const dest_id,
                                         ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief Button configuration indication handler.
 * @param[in] msgid   Id of the message received.
 * @param[in] param   Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance.
 * @param[in] src_id  ID of the sending task instance.
 ****************************************************************************************
*/
void user_svc1_button_cfg_ind_handler(ke_msg_id_t const msgid,
                                      struct custs1_val_write_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief Long value attribute info request handler.
 * @param[in] msgid   Id of the message received.
 * @param[in] param   Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance.
 * @param[in] src_id  ID of the sending task instance.
 ****************************************************************************************
*/
void user_svc1_long_val_att_info_req_handler(ke_msg_id_t const msgid,
                                             struct custs1_att_info_req const *param,
                                             ke_task_id_t const dest_id,
                                             ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief Remaining attribute info request handler.
 * @param[in] msgid   Id of the message received.
 * @param[in] param   Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance.
 * @param[in] src_id  ID of the sending task instance.
 ****************************************************************************************
*/
void user_svc1_rest_att_info_req_handler(ke_msg_id_t const msgid,
                                         struct custs1_att_info_req const *param,
                                         ke_task_id_t const dest_id,
                                         ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief ADC sampling timer callback handler.
 ****************************************************************************************
 */
void user_app_adcval1_timer_cb_handler(void);

/**
 ****************************************************************************************
 * @brief Enable peripherals used by application.
 ****************************************************************************************
*/
void user_app_enable_periphs(void);


/**
 ****************************************************************************************
 * @brief Disable peripherals used by application.
 ****************************************************************************************
*/
void user_app_disable_periphs(void);

/// @} APP

#endif // _USER_CUSTS1_IMPL_H_
