/**
 ****************************************************************************************
 *
 * @file user_custs_config.c
 *
 * @brief Custom1/2 Server (CUSTS1/2) profile database structure and initialization.
 *
 * Copyright (C) 2016-2019 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @defgroup USER_CONFIG
 * @ingroup USER
 * @brief Custom1/2 Server (CUSTS1/2) profile database structure and initialization.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "app_prf_types.h"
#include "app_customs.h"
#include "user_custs1_def.h"

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Custom1/2 server function callback table
const struct cust_prf_func_callbacks cust_prf_funcs[] =
{
#if (BLE_CUSTOM1_SERVER)
    {   TASK_ID_CUSTS1,
        custs1_att_db,
        CUST1_IDX_NB,
        #if (BLE_APP_PRESENT)
        app_custs1_create_db, app_custs1_enable,
        #else
        NULL, NULL,
        #endif
        custs1_init, NULL
    },
#endif
#if (BLE_CUSTOM2_SERVER)
    {   TASK_ID_CUSTS2,
        NULL,
        0,
        #if (BLE_APP_PRESENT)
        app_custs2_create_db, app_custs2_enable,
        #else
        NULL, NULL,
        #endif
        custs2_init, NULL
    },
#endif
    {TASK_ID_INVALID, NULL, 0, NULL, NULL, NULL, NULL},   // DO NOT MOVE. Must always be last
};

/// @} USER_CONFIG
