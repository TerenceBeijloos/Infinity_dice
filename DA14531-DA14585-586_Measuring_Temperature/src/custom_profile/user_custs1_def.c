/**
 ****************************************************************************************
 *
 * @file user_custs1_def.c
 *
 * @brief Custom Server 1 (CUSTS1) profile database definitions.
 *
 * Copyright (c) 2016-2019 Dialog Semiconductor. All rights reserved.
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

/**
 ****************************************************************************************
 * @defgroup USER_CONFIG
 * @ingroup USER
 * @brief Custom server 1 (CUSTS1) profile database definitions.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 *****************************************************
 ***********************************
 */

#include <stdint.h>
#include "co_utils.h"
#include "prf_types.h"
#include "attm_db_128.h"
#include "user_custs1_def.h"
#include "dice_communication.h"

/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

// Service 1 of the custom server 1
static const att_svc_desc128_t custs1_svc1	= DEF_SVC1_UUID_128;

// Attribute specifications
static const uint16_t att_decl_svc       		= ATT_DECL_PRIMARY_SERVICE;
static const uint16_t att_decl_char      		= ATT_DECL_CHARACTERISTIC;
static const uint16_t att_desc_cfg       		= ATT_DESC_CLIENT_CHAR_CFG;

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
static const uint8_t u8aDICE_TRANSMIT_MESSAGE_UUID_128[ATT_UUID_128_LEN] = DICE_TRANSMIT_MESSAGE_UUID_128;

const uint8_t custs1_services[]  		= {SVC1_IDX_SVC,CUSTS1_IDX_NB};
const uint8_t custs1_services_size 	= ARRAY_LEN(custs1_services) - 1;
const uint16_t custs1_att_max_nb 		= CUSTS1_IDX_NB;


/// Full CUSTS1 Database Description - Used to add attributes into the database
const struct attm_desc_128 custs1_att_db[CUSTS1_IDX_NB] =
{
//    /*************************
//     * Service 1 configuration
//     *************************
//     */

    // Service 1 Declaration
    [SVC1_IDX_SVC]                     			 = {(uint8_t*)&att_decl_svc, ATT_UUID_128_LEN, PERM(RD, ENABLE),
                                            sizeof(custs1_svc1), sizeof(custs1_svc1), (uint8_t*)&custs1_svc1},

		// Temperature Value Characteristic Declaration
    [DICE_CHANGE_CHANCE_DECLARATION]         = {(uint8_t*)&att_decl_char, ATT_UUID_16_LEN, PERM(RD, ENABLE),
                                            0, 0, NULL},

    // Temperature Characteristic Value
    [DICE_CHANGE_CHANCE_VAL]          			 = {u8aDICE_TRANSMIT_MESSAGE_UUID_128, ATT_UUID_128_LEN, PERM(RD, ENABLE) | PERM(WR, ENABLE) | PERM(NTF, ENABLE)| PERM(WRITE_REQ, ENABLE),
                                            NTF_STRING_SIZE, 0, NULL},

    // Temperature Client Characteristic Configuration Descriptor
    [DICE_CHANGE_CHANCE_CFG]       					 = {(uint8_t*)&att_desc_cfg, ATT_UUID_16_LEN, PERM(RD, ENABLE) | PERM(WR, ENABLE) | PERM(WRITE_REQ, ENABLE),
                                            sizeof(uint16_t), 0, NULL},


};

/// @} USER_CONFIG
