/***************************************************************************//**
 * @file
 * @brief
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

#include "app/framework/include/af.h"
#include "app/framework/util/attribute-storage.h"
#include "app/util/serial/command-interpreter2.h"

// ZDO
#include "app/util/zigbee-framework/zigbee-device-common.h"
#include "app/util/zigbee-framework/zigbee-device-library.h"

#include "app/util/counters/counters.h"
#include "app/util/security/security.h"
#include "app/util/common/form-and-join.h"

#include "app/framework/util/service-discovery.h"
#include "app/framework/util/af-main.h"
#include "app/framework/util/util.h"

#include "app/framework/security/af-security.h"
#include "app/framework/security/crypto-state.h"

#include "app/framework/plugin/partner-link-key-exchange/partner-link-key-exchange.h"
#include "app/framework/plugin/fragmentation/fragmentation.h"

#include "znet-bookkeeping.h"

#include "stack/include/source-route.h"
#include "stack/include/aes-mmo.h"

#if !defined(EMBER_TEST)
#define EXTENDED_RESET_INFO
#include "hal/micro/cortexm3/diagnostic.h"
#endif //!defined(EMBER_TEST)
// *****************************************************************************
// Globals

// APP_SERIAL is set in the project files
uint8_t serialPort = APP_SERIAL;

#if (EMBER_AF_BAUD_RATE == 300)
  #define BAUD_RATE BAUD_300
#elif (EMBER_AF_BAUD_RATE == 600)
  #define BAUD_RATE BAUD_600
#elif (EMBER_AF_BAUD_RATE == 900)
  #define BAUD_RATE BAUD_900
#elif (EMBER_AF_BAUD_RATE == 1200)
  #define BAUD_RATE BAUD_1200
#elif (EMBER_AF_BAUD_RATE == 2400)
  #define BAUD_RATE BAUD_2400
#elif (EMBER_AF_BAUD_RATE == 4800)
  #define BAUD_RATE BAUD_4800
#elif (EMBER_AF_BAUD_RATE == 9600)
  #define BAUD_RATE BAUD_9600
#elif (EMBER_AF_BAUD_RATE == 14400)
  #define BAUD_RATE BAUD_14400
#elif (EMBER_AF_BAUD_RATE == 19200)
  #define BAUD_RATE BAUD_19200
#elif (EMBER_AF_BAUD_RATE == 28800)
  #define BAUD_RATE BAUD_28800
#elif (EMBER_AF_BAUD_RATE == 38400)
  #define BAUD_RATE BAUD_38400
#elif (EMBER_AF_BAUD_RATE == 50000)
  #define BAUD_RATE BAUD_50000
#elif (EMBER_AF_BAUD_RATE == 57600)
  #define BAUD_RATE BAUD_57600
#elif (EMBER_AF_BAUD_RATE == 76800)
  #define BAUD_RATE BAUD_76800
#elif (EMBER_AF_BAUD_RATE == 100000)
  #define BAUD_RATE BAUD_100000
#elif (EMBER_AF_BAUD_RATE == 115200)
  #define BAUD_RATE BAUD_115200
#elif (EMBER_AF_BAUD_RATE == 230400)
  #define BAUD_RATE BAUD_230400
#elif (EMBER_AF_BAUD_RATE == 460800)
  #define BAUD_RATE BAUD_460800
#elif defined(HAL_CONFIG)
  #define BAUD_RATE HAL_SERIAL_APP_BAUD_RATE
#else
  #error EMBER_AF_BAUD_RATE set to an invalid baud rate
#endif

#if defined(MAIN_FUNCTION_HAS_STANDARD_ARGUMENTS)
  #define APP_FRAMEWORK_MAIN_ARGUMENTS argc, argv
#else
  #define APP_FRAMEWORK_MAIN_ARGUMENTS 0, NULL
#endif

#if defined(__CYGWIN__)
// This is necessary when running simulation on Windows.
const EmberLibraryStatus emNcpLibraryStatus = EMBER_LIBRARY_IS_STUB;
#endif

// *****************************************************************************
// Forward declarations.

#if defined(EMBER_TEST) && defined(EMBER_AF_PLUGIN_OTA_STORAGE_SIMPLE_EEPROM)
void emAfSetupFakeEepromForSimulation(void);
  #define SETUP_FAKE_EEPROM_FOR_SIMULATION() emAfSetupFakeEepromForSimulation()
#else
  #define SETUP_FAKE_EEPROM_FOR_SIMULATION()
#endif

#if defined(ZA_CLI_FULL)
  #define COMMAND_READER_INIT() emberCommandReaderInit()
#else
  #define COMMAND_READER_INIT()
#endif

// *****************************************************************************
// Functions

void emberAfMainInit(void)
{
  SETUP_FAKE_EEPROM_FOR_SIMULATION();

  INTERRUPTS_ON();  // Safe to enable interrupts at this point

  emberSerialInit(APP_SERIAL, BAUD_RATE, PARITY_NONE, 1);

  emberAfCorePrintln("Reset info: 0x%x (%p)",
                     halGetResetInfo(),
                     halGetResetString());

#if defined(EXTENDED_RESET_INFO)
  emberAfCorePrintln("Extended Reset info: 0x%2X (%p)",
                     halGetExtendedResetInfo(),
                     halGetExtendedResetString());

  if (halResetWasCrash()) {
    halPrintCrashSummary(serialPort);
    halPrintCrashDetails(serialPort);
    halPrintCrashData(serialPort);
  }
#endif
}

int emberAfMain(MAIN_FUNCTION_PARAMETERS)
{
  EmberStatus status;

  {
    int returnCode;
    if (emberAfMainStartCallback(&returnCode, APP_FRAMEWORK_MAIN_ARGUMENTS)) {
      return returnCode;
    }
  }

  // Initialize the Ember Stack.
  status = emberInit();

  #ifdef EMBER_AF_PLUGIN_EM4
  // we moved this before emberInit to pre-initialize the tokens
  #if defined(EMBER_TEST)
  uint8_t reset;
  reset = halGetResetInfo();
  if (reset == RESET_2xx_SOFTWARE_EM4) {
    // This can only be called if idle-sleep plugin is enabled, and em4 is OK.
    emberAfPostEm4ResetCallback();
  }
#else
  uint16_t extReset;
  extReset = halGetExtendedResetInfo();
  emberAfDebugPrintln("extReset reason is: %d while RESET_SOFTWARE_EM4 is %d, and RESET_EXTERNAL_EM4PIN is %d",
                      extReset,
                      RESET_SOFTWARE_EM4,
                      RESET_EXTERNAL_EM4PIN);
  if (extReset == RESET_SOFTWARE_EM4 || extReset == RESET_EXTERNAL_EM4PIN) {
    emberAfPostEm4ResetCallback();
  }
#endif // EMBER_TEST
#endif //#ifdef EMBER_AF_PLUGIN_EM4

  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("%pemberInit 0x%x", "ERROR: ", status);

    // The app can choose what to do here.  If the app is running
    // another device then it could stay running and report the
    // error visually for example. This app asserts.
    assert(false);
  } else {
    emberAfDebugPrintln("init pass");
  }

  // This will initialize the stack of networks maintained by the framework,
  // including setting the default network.
  emAfInitializeNetworkIndexStack();

  // Initialize messageSentCallbacks table
  emAfInitializeMessageSentCallbackArray();

  emberAfEndpointConfigure();

  emAfInit();

  // The address cache needs to be initialized and used with the source routing
  // code for the trust center to operate properly.
  securityAddressCacheInit(EMBER_AF_PLUGIN_ADDRESS_TABLE_SIZE,                     // offset
                           EMBER_AF_PLUGIN_ADDRESS_TABLE_TRUST_CENTER_CACHE_SIZE); // size

  emAfNetworkSecurityInit();
  EM_AF_NETWORK_INIT();

  COMMAND_READER_INIT();

  // Set the manufacturing code. This is defined by ZigBee document 053874r10
  // Ember's ID is 0x1002 and is the default, but this can be overridden in App Builder.
  emberSetManufacturerCode(EMBER_AF_MANUFACTURER_CODE);

  emberSetMaximumIncomingTransferSize(EMBER_AF_INCOMING_BUFFER_LENGTH);
  emberSetMaximumOutgoingTransferSize(EMBER_AF_MAXIMUM_SEND_PAYLOAD_LENGTH);
  emberSetTxPowerMode(EMBER_AF_TX_POWER_MODE);

  while (true) {
    halResetWatchdog();   // Periodically reset the watchdog.
    emberTick();          // Allow the stack to run.
    // Allow the ZCL clusters and plugin ticks to run. This should go
    // immediately after emberTick
    // Skip these ticks if a crypto operation is ongoing
    if (0 == emAfIsCryptoOperationInProgress()) {
      emAfTick();
    }

    emberAfRunEvents();

#if defined(ZA_CLI_FULL)
    if (emberProcessCommandInput(APP_SERIAL)) {
      emberAfGuaranteedPrint("%p>", ZA_PROMPT);
    }
#endif

#if defined(EMBER_TEST)
    if (true) {
      // Simulation only
      uint32_t timeToNextEventMax = emberMsToNextStackEvent();
      timeToNextEventMax = emberAfMsToNextEvent(timeToNextEventMax);
      simulatedTimePassesMs(timeToNextEventMax);
    }
#endif

    // After each interation through the main loop, our network index stack
    // should be empty and we should be on the default network index again.
    emAfAssertNetworkIndexStackIsEmpty();

    if (false) {
      break;
    }
  }
  return 0;
}

void emberAfGetMfgString(uint8_t* returnData)
{
  halCommonGetMfgToken(returnData, TOKEN_MFG_STRING);
}

EmberNodeId emberAfGetNodeId(void)
{
  return emberGetNodeId();
}

EmberPanId emberAfGetPanId(void)
{
  return emberGetPanId();
}

EmberNetworkStatus emberAfNetworkState(void)
{
  return emberNetworkState();
}

uint8_t emberAfGetRadioChannel(void)
{
  return emberGetRadioChannel();
}

uint8_t emberAfGetBindingIndex(void)
{
  return emberGetBindingIndex();
}

uint8_t emberAfGetStackProfile(void)
{
  return emberStackProfile();
}

uint8_t emberAfGetAddressIndex(void)
{
  EmberEUI64 longId;
  EmberEUI64 longIdIterator;
  uint8_t i;

  if ((emberGetSenderEui64(longId)) == EMBER_SUCCESS) {
    for (i = 0; i < EMBER_AF_PLUGIN_ADDRESS_TABLE_SIZE; i++) {
      emberGetAddressTableRemoteEui64(i, longIdIterator);
      if (MEMCOMPARE(longIdIterator, longId, EUI64_SIZE) == 0) {
        return i;
      }
    }
  } else {
    EmberNodeId nodeId = emberGetSender();
    if (nodeId == EMBER_NULL_NODE_ID) {
      return EMBER_NULL_ADDRESS_TABLE_INDEX;
    }
    for (i = 0; i < EMBER_AF_PLUGIN_ADDRESS_TABLE_SIZE; i++) {
      if (emberGetAddressTableRemoteNodeId(i) == nodeId) {
        return i;
      }
    }
  }
  return EMBER_NULL_ADDRESS_TABLE_INDEX;
}

// ******************************************************************
// binding
// ******************************************************************
EmberStatus emberAfSendEndDeviceBind(uint8_t endpoint)
{
  EmberStatus status;
  EmberApsOption options = ((EMBER_AF_DEFAULT_APS_OPTIONS
                             | EMBER_APS_OPTION_SOURCE_EUI64)
                            & ~EMBER_APS_OPTION_RETRY);

  status = emberAfPushEndpointNetworkIndex(endpoint);
  if (status != EMBER_SUCCESS) {
    return status;
  }

  emberAfZdoPrintln("send %x %2x", endpoint, options);
  status = emberEndDeviceBindRequest(endpoint, options);
  emberAfZdoPrintln("done: %x.", status);
  emberAfZdoFlush();

  (void) emberAfPopNetworkIndex();
  return status;
}

EmberZdoStatus emberRemoteSetBindingHandler(EmberBindingTableEntry *entry)
{
  EmberStatus setStatus;
  EmberZdoStatus status = EMBER_ZDP_TABLE_FULL;
  EmberBindingTableEntry candidate;
  uint8_t i;

  (void) emberAfPushCallbackNetworkIndex();

  // If we receive a bind request for the Key Establishment cluster and we are
  // not the trust center, then we are doing partner link key exchange.  We
  // don't actually have to create a binding.
  if (emberAfGetNodeId() != EMBER_TRUST_CENTER_NODE_ID
      && entry->clusterId == ZCL_KEY_ESTABLISHMENT_CLUSTER_ID) {
    status = emberAfPartnerLinkKeyExchangeRequestCallback(entry->identifier);
    goto kickout;
  }

  // ask the application if current binding request is allowed or not
  status = emberAfRemoteSetBindingPermissionCallback(entry) == EMBER_SUCCESS ? EMBER_ZDP_SUCCESS : EMBER_ZDP_NOT_AUTHORIZED;
  if (status == EMBER_ZDP_SUCCESS) {
    // For all other requests, we search the binding table for an unused entry
    // and store the new entry there if we find one.
    for (i = 0; i < EMBER_BINDING_TABLE_SIZE; i++) {
      if (emberGetBinding(i, &candidate) == EMBER_SUCCESS
          && candidate.type == EMBER_UNUSED_BINDING) {
        setStatus = emberSetBinding(i, entry);
        status = setStatus == EMBER_SUCCESS ? EMBER_ZDP_SUCCESS // binding set
                 : (setStatus == EMBER_BINDING_IS_ACTIVE ? EMBER_ZDP_NOT_PERMITTED // selected index is active
                    : EMBER_ZDP_TABLE_FULL); // report full for any other failure
        goto kickout;
      }
    }
    // If we get here, we didn't find an empty table slot, so table is full.
    status = EMBER_ZDP_TABLE_FULL;
  }

  kickout:
  (void) emberAfPopNetworkIndex();
  return status;
}

EmberZdoStatus emberRemoteDeleteBindingHandler(uint8_t index)
{
  EmberStatus deleteStatus;
  EmberZdoStatus status;
  (void) emberAfPushCallbackNetworkIndex();

  // ask the application if current binding request is allowed or not
  status = emberAfRemoteDeleteBindingPermissionCallback(index) == EMBER_SUCCESS ? EMBER_ZDP_SUCCESS : EMBER_ZDP_NOT_AUTHORIZED;
  if (status == EMBER_SUCCESS) {
    deleteStatus = emberDeleteBinding(index);
    status = deleteStatus == EMBER_SUCCESS ? EMBER_ZDP_SUCCESS // binding deleted
             : (deleteStatus == EMBER_BINDING_IS_ACTIVE ? EMBER_ZDP_NOT_PERMITTED //selected index is active
                : EMBER_ZDP_NO_ENTRY); // report no entry for any other failure
    emberAfZdoPrintln("delete binding: %x %x", index, status);
  }

  (void) emberAfPopNetworkIndex();
  return status;
}

// ******************************************************************
// setup endpoints and clusters for responding to ZDO requests
// ******************************************************************
uint8_t emberGetEndpoint(uint8_t index)
{
  uint8_t endpoint = EMBER_AF_INVALID_ENDPOINT;
  if (emberAfGetEndpointByIndexCallback(index, &endpoint)) {
    return endpoint;
  }
  return (((emberAfNetworkIndexFromEndpointIndex(index)
            == emberGetCallbackNetwork())
           && emberAfEndpointIndexIsEnabled(index))
          ? emberAfEndpointFromIndex(index)
          : 0xFF);
}

// must return the endpoint desc of the endpoint specified
bool emberGetEndpointDescription(uint8_t endpoint,
                                 EmberEndpointDescription *result)
{
  if (emberAfGetEndpointDescriptionCallback(endpoint, result)) {
    return true;
  }
  uint8_t endpointIndex = emberAfIndexFromEndpoint(endpoint);
  if (endpointIndex == 0xFF
      || (emberAfNetworkIndexFromEndpointIndex(endpointIndex)
          != emberGetCallbackNetwork())) {
    return false;
  }
  result->profileId          = emberAfProfileIdFromIndex(endpointIndex);
  result->deviceId           = emberAfDeviceIdFromIndex(endpointIndex);
  result->deviceVersion      = emberAfDeviceVersionFromIndex(endpointIndex);
  result->inputClusterCount  = emberAfClusterCount(endpoint, true);
  result->outputClusterCount = emberAfClusterCount(endpoint, false);
  return true;
}

// must return the clusterId at listIndex in the list specified for the
// endpoint specified
uint16_t emberGetEndpointCluster(uint8_t endpoint,
                                 EmberClusterListId listId,
                                 uint8_t listIndex)
{
  EmberAfCluster *cluster = NULL;
  uint8_t endpointIndex = emberAfIndexFromEndpoint(endpoint);
  if (endpointIndex == 0xFF
      || (emberAfNetworkIndexFromEndpointIndex(endpointIndex)
          != emberGetCallbackNetwork())) {
    return 0xFFFF;
  } else if (listId == EMBER_INPUT_CLUSTER_LIST) {
    cluster = emberAfGetNthCluster(endpoint, listIndex, true);
  } else if (listId == EMBER_OUTPUT_CLUSTER_LIST) {
    cluster = emberAfGetNthCluster(endpoint, listIndex, false);
  } else {
    // MISRA requires ..else if.. to have terminating else.
  }
  return (cluster == NULL ? 0xFFFF : cluster->clusterId);
}

// *******************************************************************
// Handlers required to use the Ember Stack.

// Called when the stack status changes, usually as a result of an
// attempt to form, join, or leave a network.
void emberStackStatusHandler(EmberStatus status)
{
  (void) emberAfPushCallbackNetworkIndex();
  emAfStackStatusHandler(status);
  (void) emberAfPopNetworkIndex();
}

// Copy the message buffer into a RAM buffer.
//   If message is too large, 0 is returned and no copying is done.
//   Otherwise data is copied, and length of copied data is returned.
uint8_t emAfCopyMessageIntoRamBuffer(EmberMessageBuffer message,
                                     uint8_t *buffer,
                                     uint16_t bufLen)
{
  if (message == EMBER_NULL_MESSAGE_BUFFER) {
    return 0;
  }
  uint8_t length = emberMessageBufferLength(message);
  if (bufLen < length) {
    emberAfAppPrintln("%pmsg too big (%d > %d)",
                      "ERROR: ",
                      length,
                      bufLen);
    return 0;
  }
  emberCopyFromLinkedBuffers(message, 0, buffer, length); // no offset
  return length;
}

void emberIncomingMessageHandler(EmberIncomingMessageType type,
                                 EmberApsFrame *apsFrame,
                                 EmberMessageBuffer message)
{
  uint8_t lastHopLqi;
  int8_t lastHopRssi;
  uint16_t messageLength;
  uint8_t messageContents[EMBER_AF_MAXIMUM_APS_PAYLOAD_LENGTH];

  (void) emberAfPushCallbackNetworkIndex();

  messageLength = emAfCopyMessageIntoRamBuffer(message,
                                               messageContents,
                                               EMBER_AF_MAXIMUM_APS_PAYLOAD_LENGTH);
  if (messageLength == 0) {
    goto kickout;
  }

  emberGetLastHopLqi(&lastHopLqi);
  emberGetLastHopRssi(&lastHopRssi);

  emAfIncomingMessageHandler(type,
                             apsFrame,
                             lastHopLqi,
                             lastHopRssi,
                             messageLength,
                             messageContents);

  kickout:
  (void) emberAfPopNetworkIndex();
}

#define INVALID_MESSAGE_TAG 0x0000
static uint16_t calculateMessageTagHash(uint8_t *messageContents, uint8_t messageLength)
{
  uint8_t temp[EMBER_ENCRYPTION_KEY_SIZE];
  uint16_t hashReturn = 0;
  emberAesHashSimple(messageLength, messageContents, temp);
  for (uint8_t i = 0; i < EMBER_ENCRYPTION_KEY_SIZE; i += 2) {
    hashReturn ^= *((uint16_t *)(temp + i));
  }
  if (hashReturn == INVALID_MESSAGE_TAG) {
    hashReturn = 1;
  }
  return hashReturn;
}

// Called when a message we sent is acked by the destination or when an
// ack fails to arrive after several retransmissions.
void emberMessageSentHandler(EmberOutgoingMessageType type,
                             uint16_t indexOrDestination,
                             EmberApsFrame *apsFrame,
                             EmberMessageBuffer message,
                             EmberStatus status)
{
  uint8_t messageContents[EMBER_AF_MAXIMUM_APS_PAYLOAD_LENGTH];
  uint8_t messageLength;
  uint16_t messageTag;
  (void) emberAfPushCallbackNetworkIndex();

#ifdef EMBER_AF_PLUGIN_FRAGMENTATION
  if (emAfFragmentationMessageSent(apsFrame, status)) {
    goto kickout;
  }
#endif //EMBER_AF_PLUGIN_FRAGMENTATION

  messageLength = emAfCopyMessageIntoRamBuffer(message,
                                               messageContents,
                                               EMBER_AF_MAXIMUM_APS_PAYLOAD_LENGTH);
  if (messageLength == 0) {
    // Message too long.  Error printed by above function.
    goto kickout;
  }
  emberConcentratorNoteDeliveryFailure(type, status);

  messageTag = calculateMessageTagHash(messageContents, messageLength);
  emAfMessageSentHandler(type,
                         indexOrDestination,
                         apsFrame,
                         status,
                         messageLength,
                         messageContents,
                         messageTag);

  kickout:
  (void) emberAfPopNetworkIndex();
}

EmberStatus emAfSend(EmberOutgoingMessageType type,
                     uint16_t indexOrDestination,
                     EmberApsFrame *apsFrame,
                     uint8_t messageLength,
                     uint8_t *message,
                     uint16_t *messageTag,
                     EmberNodeId alias,
                     uint8_t sequence)
{
  EmberMessageBuffer payload = emberFillLinkedBuffers(message, messageLength);
  if (payload == EMBER_NULL_MESSAGE_BUFFER) {
    return EMBER_NO_BUFFERS;
  } else {
    EmberStatus status;

    *messageTag = calculateMessageTagHash(message, messageLength);

    switch (type) {
      case EMBER_OUTGOING_DIRECT:
      case EMBER_OUTGOING_VIA_ADDRESS_TABLE:
      case EMBER_OUTGOING_VIA_BINDING:
        status = emberSendUnicast(type, indexOrDestination, apsFrame, payload);
        break;
      case EMBER_OUTGOING_MULTICAST:
        status = emberSendMulticast(apsFrame,
                                    ZA_MAX_HOPS, // radius
                                    ZA_MAX_HOPS, // nonmember radius
                                    payload);
        break;
      case EMBER_OUTGOING_MULTICAST_WITH_ALIAS:
        status = emberSendMulticastWithAlias(apsFrame,
                                             apsFrame->radius, //radius
                                             apsFrame->radius, //nonmember radius
                                             payload,
                                             alias,
                                             sequence);
        break;
      case EMBER_OUTGOING_BROADCAST:
        status = emberSendBroadcast(indexOrDestination,
                                    apsFrame,
                                    ZA_MAX_HOPS, // radius
                                    payload);
        break;
      case EMBER_OUTGOING_BROADCAST_WITH_ALIAS:
        status = emberProxyBroadcast(alias,
                                     indexOrDestination,
                                     sequence,
                                     apsFrame,
                                     apsFrame->radius, // radius
                                     payload);
        break;
      default:
        status = EMBER_BAD_ARGUMENT;
        break;
    }

    emberReleaseMessageBuffer(payload);

    return status;
  }
}

void emberAfGetEui64(EmberEUI64 returnEui64)
{
  MEMMOVE(returnEui64, emberGetEui64(), EUI64_SIZE);
}

EmberStatus emberAfGetNetworkParameters(EmberNodeType* nodeType,
                                        EmberNetworkParameters* parameters)
{
  emberGetNetworkParameters(parameters);
  return emberGetNodeType(nodeType);
}

EmberStatus emberAfGetNodeType(EmberNodeType *nodeType)
{
  return emberGetNodeType(nodeType);
}

uint8_t emberAfGetSecurityLevel(void)
{
  return emberSecurityLevel();
}

uint8_t emberAfGetKeyTableSize(void)
{
  return EMBER_KEY_TABLE_SIZE;
}

uint8_t emberAfGetBindingTableSize(void)
{
  return EMBER_BINDING_TABLE_SIZE;
}

uint8_t emberAfGetAddressTableSize(void)
{
  return EMBER_ADDRESS_TABLE_SIZE;
}

uint8_t emberAfGetChildTableSize(void)
{
  return EMBER_CHILD_TABLE_SIZE;
}

uint8_t emberAfGetNeighborTableSize(void)
{
  return EMBER_NEIGHBOR_TABLE_SIZE;
}

uint8_t emberAfGetRouteTableSize(void)
{
  return EMBER_ROUTE_TABLE_SIZE;
}

uint8_t emberAfGetSleepyMulticastConfig(void)
{
  return EMBER_SEND_MULTICASTS_TO_SLEEPY_ADDRESS;
}

EmberStatus emberAfGetSourceRouteTableEntry(
  uint8_t index,
  EmberNodeId *destination,
  uint8_t *closerIndex)
{
  return emberGetSourceRouteTableEntry(index,
                                       destination,
                                       closerIndex);
}

uint8_t emberAfGetSourceRouteTableTotalSize(void)
{
  return emberGetSourceRouteTableTotalSize();
}

uint8_t emberAfGetSourceRouteTableFilledSize(void)
{
  return emberGetSourceRouteTableFilledSize();
}

EmberStatus emberAfGetChildData(uint8_t index,
                                EmberChildData *childData)
{
  return emberGetChildData(index,
                           childData);
}

uint8_t emAfGetPacketBufferFreeCount(void)
{
  return emBufferBytesRemaining() / PACKET_BUFFER_SIZE;
}

uint8_t emAfGetPacketBufferTotalCount(void)
{
  return emBufferBytesTotal() / PACKET_BUFFER_SIZE;
}

uint8_t emberAfGetOpenNetworkDurationSec(void)
{
  return sli_zigbee_get_permit_joining_remaining_duration_sec();
}

void emAfCliVersionCommand(void)
{
  emAfParseAndPrintVersion(emberVersion);
}

// TODO: find out a better way to handle these form-and-join scan callbacks.
// Backwards compatibility...ugh.

void emberNetworkFoundHandler(EmberZigbeeNetwork *networkFound)
{
  uint8_t lqi;
  int8_t rssi;
  (void) emberAfPushCallbackNetworkIndex();
  emberGetLastHopLqi(&lqi);
  emberGetLastHopRssi(&rssi);

#ifdef EMBER_AF_PLUGIN_FORM_AND_JOIN
  if (!emberFormAndJoinNetworkFoundHandler(networkFound, lqi, rssi)) {
    emberAfNetworkFoundCallback(networkFound, lqi, rssi);
  }
#else
  emberAfNetworkFoundCallback(networkFound, lqi, rssi);
#endif

  (void) emberAfPopNetworkIndex();
}

void emberScanCompleteHandler(uint8_t channel, EmberStatus status)
{
  (void) emberAfPushCallbackNetworkIndex();

#ifdef EMBER_AF_PLUGIN_FORM_AND_JOIN
  if (!emberFormAndJoinScanCompleteHandler(channel, status)) {
    emberAfScanCompleteCallback(channel, status);
  }
#else
  emberAfScanCompleteCallback(channel, status);
#endif

  (void) emberAfPopNetworkIndex();
}

void emberEnergyScanResultHandler(uint8_t channel, int8_t maxRssiValue)
{
  (void) emberAfPushCallbackNetworkIndex();

#ifdef EMBER_AF_PLUGIN_FORM_AND_JOIN
  if (!emberFormAndJoinEnergyScanResultHandler(channel, maxRssiValue)) {
    emberAfEnergyScanResultCallback(channel, maxRssiValue);
  }
#else
  emberAfEnergyScanResultCallback(channel, maxRssiValue);
#endif

  (void) emberAfPopNetworkIndex();
}

void emberUnusedPanIdFoundHandler(EmberPanId panId, uint8_t channel)
{
  (void) emberAfPushCallbackNetworkIndex();
#ifdef EMBER_AF_PLUGIN_FORM_AND_JOIN
  if (!emberFormAndJoinUnusedPanIdFoundHandler(panId, channel)) {
    emberAfUnusedPanIdFoundCallback(panId, channel);
  }
#else
  emberAfUnusedPanIdFoundCallback(panId, channel);
#endif
  (void) emberAfPopNetworkIndex();
}

void emAfPrintEzspEndpointFlags(uint8_t endpoint)
{
  // Not applicable for SOC
}

void emberZigbeeKeyEstablishmentHandler(EmberEUI64 partner,
                                        EmberKeyStatus status)
{
  // This function is generated.
  emAfZigbeeKeyEstablishment(partner, status);
}

void emberMarkApplicationBuffers(void)
{
  emAfMarkBuffers();
}
