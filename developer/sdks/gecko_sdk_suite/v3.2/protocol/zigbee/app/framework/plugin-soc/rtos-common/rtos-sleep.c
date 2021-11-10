/***************************************************************************//**
 * @file
 * @brief Code related to letting the chip sleep when using RTOS.
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
// Define module name for Power Manager debuging feature.
#define CURRENT_MODULE_NAME    "ZIGBEE_PLUGIN_SOC_RTOS"

#include PLATFORM_HEADER
#ifdef EMBER_AF_NCP
#include "app/framework/include/af-ncp.h"
#else // !EMBER_AF_NCP
#include "app/framework/include/af.h"
#endif // EMBER_AF_NCP
#include "sl_sleeptimer.h"

#include "cmsis_os2.h"
#include "sl_cmsis_os2_common.h"

#include "rtos-zigbee.h"
#include "sl_power_manager.h"

#if (defined(EMBER_AF_PLUGIN_RTOS_COMMON_POLL_CLI) && defined(ZA_CLI_FULL))
#define ENABLE_POLL_CLI
#define POLL_CLI_TASK_PERIOD_MS                 250
#endif // EMBER_AF_PLUGIN_RTOS_COMMON_POLL_CLI && ZA_CLI_FULL

// Power manager transition events of interest. List of events that we subscribe for.
#define POWER_MANAGER_EVENTS_OF_INTEREST            \
  (SL_POWER_MANAGER_EVENT_TRANSITION_ENTERING_EM0   \
   | SL_POWER_MANAGER_EVENT_TRANSITION_LEAVING_EM0  \
   | SL_POWER_MANAGER_EVENT_TRANSITION_ENTERING_EM1 \
   | SL_POWER_MANAGER_EVENT_TRANSITION_LEAVING_EM1  \
   | SL_POWER_MANAGER_EVENT_TRANSITION_ENTERING_EM2 \
   | SL_POWER_MANAGER_EVENT_TRANSITION_LEAVING_EM2)

//------------------------------------------------------------------------------
// Extern and forward declarations.

//------------------------------------------------------------------------------
// Static functions
static void energy_mode_transition_callback(sl_power_manager_em_t from,
                                            sl_power_manager_em_t to)
{
  if (from == SL_POWER_MANAGER_EM0) { // Leaving EM0
    halPowerDown();
  } else if (to == SL_POWER_MANAGER_EM0) { // Coming back to EM0
    halPowerUp();
  }
}

//------------------------------------------------------------------------------
// Static variables.
#ifdef EMBER_AF_PLUGIN_RTOS_COMMON_SLEEP_ENABLE
static bool pm_em1_requirement_set = false;
#endif //EMBER_AF_PLUGIN_RTOS_COMMON_SLEEP_ENABLE
static sl_power_manager_em_transition_event_handle_t pm_handle;
static sl_power_manager_em_transition_event_info_t pm_event_info =
{ POWER_MANAGER_EVENTS_OF_INTEREST, energy_mode_transition_callback };

void emAfPluginRtosSleepInit(void)
{
  sl_power_manager_subscribe_em_transition_event(&pm_handle, &pm_event_info);

  // Set initial power requirement to EM1
  sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
  #ifdef EMBER_AF_PLUGIN_RTOS_COMMON_SLEEP_ENABLE
  pm_em1_requirement_set = true;
  #endif //EMBER_AF_PLUGIN_RTOS_COMMON_SLEEP_ENABLE
}

//------------------------------------------------------------------------------
// Idle/sleep rtos callback - invoked from the idle/sleep plugin whenever the
// Zigbee stack can idle or sleep.
bool emberAfPluginIdleSleepRtosCallback(uint32_t *duration_ms, bool sleep_ok)
{
  uint32_t actual_duration_ms = *duration_ms;
  uint32_t attempted_duration_ms = actual_duration_ms;
  uint32_t delta_ms;
  uint32_t start_ticks = sl_sleeptimer_get_tick_count();
  uint32_t yield_time_ticks;

#if defined(ENABLE_POLL_CLI)
  if (actual_duration_ms > POLL_CLI_TASK_PERIOD_MS) {
    actual_duration_ms = POLL_CLI_TASK_PERIOD_MS;
  }
#endif // ENABLE_POLL_CLI

  yield_time_ticks = (osKernelGetTickFreq() * actual_duration_ms) / 1000;

// Do not let removing the EM1 requirement if sleep is not allowed.
#if defined(EMBER_AF_PLUGIN_RTOS_COMMON_SLEEP_ENABLE)
  // Depending on stateOk, we can either sleep or idle here.
  if (sleep_ok && pm_em1_requirement_set) {
    // Remove requirement for EM1 in case we can sleep and
    // EM1 requirement is set.
    sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
    pm_em1_requirement_set = false;
  } else if (!sleep_ok && !pm_em1_requirement_set) {
    // Add requirement for EM1 in case we cannot sleep and
    // EM1 requirement is not set already.
    sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
    pm_em1_requirement_set = true;
  }
#endif // EMBER_AF_PLUGIN_RTOS_COMMON_SLEEP_ENABLE

  INTERRUPTS_ON();

#ifdef EMBER_AF_NCP
  // Enable the UART interrupt(s) to active zigbee stack task
  // if there is serial port activity. This is mainly to process
  // ezsp frames from zigbee host if something comes up while zigbee
  // task is suspended. UART irq handler post the semaphore to run
  // the zigbee stack task.
  COM_InternalPowerDown(true);
#endif  // EMBER_AF_NCP

  //
  rtos_zigbee_pend_on_event(yield_time_ticks);

#ifdef EMBER_AF_NCP
  // Disable back the UART interrupt(s).
  COM_InternalPowerUp(true);
#endif // EMBER_AF_NCP

  delta_ms = sl_sleeptimer_tick_to_ms(sl_sleeptimer_get_tick_count() - start_ticks);

  if (delta_ms <= attempted_duration_ms) {
    *duration_ms = attempted_duration_ms - delta_ms;
  } else {
    *duration_ms = 0;
  }

  return true;
}
