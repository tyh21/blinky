/**
 ****************************************************************************************
 *
 * @file user_all_in_one.h
 *
 * @brief All in one project header file.
 *
 * Copyright (C) 2015-2019 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef _USER_ALL_IN_ONE_H_
#define _USER_ALL_IN_ONE_H_

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

#include "gapc_task.h"
#include "suotar.h"
#include "ke_msg.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/* Duration of timer for connection parameter update request */
#define APP_PARAM_UPDATE_REQUEST_TO         (1000)   // 1000*10ms = 10sec, The maximum allowed value is 41943sec (4194300 * 10ms)

/* Advertising data update timer */
#define APP_ADV_DATA_UPDATE_TO              (3000)   // 3000*10ms = 30sec, The maximum allowed value is 41943sec (4194300 * 10ms)

/* Manufacturer specific data constants */
#define APP_AD_MSD_COMPANY_ID              (0xABCD)
#define APP_AD_MSD_COMPANY_ID_LEN          (2)
#define APP_AD_MSD_DATA_LEN                (sizeof(uint16_t))

#define APP_PERIPHERAL_CTRL_TIMER_DELAY    (100)

/// Passkey that is presented to the user and is entered on the peer device (MITM)
#define APP_SECURITY_MITM_PASSKEY_VAL      (123456)

/// OOB data
#define APP_SECURITY_OOB_TK_VAL            {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,\
                                            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f}

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Application initialization function.
 ****************************************************************************************
*/
void user_app_init(void);

/**
 ****************************************************************************************
 * @brief Advertising function.
 ****************************************************************************************
*/
void user_app_adv_start(void);

/**
 ****************************************************************************************
 * @brief Connection function.
 * @param[in] conidx    Connection Id index
 * @param[in] param     Pointer to GAPC_CONNECTION_REQ_IND message
 ****************************************************************************************
*/
void user_app_connection(uint8_t conidx,
                         struct gapc_connection_req_ind const *param);

/**
 ****************************************************************************************
 * @brief Undirect advertising completion function.
 * @param[in] status    Command complete event message status
 ****************************************************************************************
*/
void user_app_adv_undirect_complete(uint8_t status);

#if BLE_APP_SEC

/**
 ****************************************************************************************
 * @brief Function called on term key exchange event.
 * @param[in] conidx    Connection Id number
 * @param[in] param     Pointer to GAPC_BOND_REQ_IND message
 ****************************************************************************************
 */
void user_app_on_tk_exch(uint8_t conidx,
                         struct gapc_bond_req_ind const *param);

#endif // (BLE_APP_SEC)

/**
 ****************************************************************************************
 * @brief Disconnection function.
 * @param[in] param    Pointer to GAPC_DISCONNECT_IND message
 ****************************************************************************************
*/
void user_app_disconnect(struct gapc_disconnect_ind const *param);

/**
 ****************************************************************************************
 * @brief Handles the messages that are not handled by the SDK internal mechanisms.
 * @param[in] msgid      Id of the message received.
 * @param[in] param      Pointer to the parameters of the message.
 * @param[in] dest_id    ID of the receiving task instance.
 * @param[in] src_id     ID of the sending task instance.
 ****************************************************************************************
*/
void user_catch_rest_hndl(ke_msg_id_t const msgid,
                          void const *param,
                          ke_task_id_t const dest_id,
                          ke_task_id_t const src_id);

#if (BLE_SUOTA_RECEIVER)
/**
 ****************************************************************************************
 * @brief Function called when the SUOTAR status changes.
 * @param[in] suotar_event SUOTAR_START or SUOTAR_STOP
 ****************************************************************************************
 */
void on_suotar_status_change(const uint8_t suotar_event);
#endif

/// @} APP

#endif // _USER_ALL_IN_ONE_H_
