/***************************************************************************//**
 * @file
 * @brief Definitions for the Idle Sleep plugin.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#ifndef __IDLE_SLEEP_H__
#define __IDLE_SLEEP_H__

extern bool emAfStayAwakeWhenNotJoined;
extern bool emAfForceEndDeviceToStayAwake;

void emberAfForceEndDeviceToStayAwake(bool stayAwake);
bool emAfOkToIdleOrSleep(void);
bool emAfCheckStayAwakeWhenNotJoined (void);

#endif //#ifndef __IDLE_SLEEP_H__
