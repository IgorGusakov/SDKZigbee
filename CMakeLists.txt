 cmake_minimum_required(VERSION 3.13)

project(host)

#set(CMAKE_C_COMPILER /usr/bin/clang)
#set(CMAKE_CXX_COMPILER /usr/bin/clang++)


set(CMAKE_C_FLAGS_INIT "-std=gnu99 -g -ggdb -O0  -DNO_READLINE")
set(CMAKE_CXX_FLAGS_INIT "-std=c++11 -Wno-pointer-sign")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-gc-sections, -T, -Wl,--allow-multiple-definition, -rdynamic")

set(SDK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2)

set(PLATFORM_HEADER_FILE \"${SDK_PATH}/platform/base/hal/micro/unix/compiler/gcc.h\")
add_definitions(${COMPILER_DEFINES})

set(APP_BUILDER_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(APP_BUILDER_CONFIG_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/Z3GatewayHost.h)
set(APP_BUILDER_STORAGE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Z3GatewayHost_endpoint_config.h)

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    add_definitions(-Wno-pointer-sign)
endif()

add_definitions(
        -DUNIX
        -DOTA_DEBUG
        -DUNIX_HOST
        -DPHY_NULL
        -DCONFIGURATION_HEADER=\"${SDK_PATH}/protocol/zigbee/app/framework/util/config.h\"
        -DEZSP_HOST
        -DGATEWAY_APP
        -DZA_GENERATED_HEADER=\"${APP_BUILDER_CONFIG_HEADER}\"
        -DATTRIBUTE_STORAGE_CONFIGURATION=\"${APP_BUILDER_STORAGE_FILE}\"
        -DPLATFORM_HEADER=${PLATFORM_HEADER_FILE}
        -DBOARD_HOST
        -DBOARD_HEADER=\"${CMAKE_CURRENT_SOURCE_DIR}/Z3GatewayHost_board.h\"
        -DEM_AF_TEST_HARNESS_CODE
        -DEM_AF_LINK_M
        -DEM_AF_LINK_PTHREAD
        -DEMBER_AF_API_EMBER_TYPES=\"${SDK_PATH}/protocol/zigbee/stack/include/ember-types.h\"
        -DEMBER_AF_API_DEBUG_PRINT=\"${SDK_PATH}/protocol/zigbee/app/framework/util/print.h\"
        -DEMBER_AF_API_AF_HEADER=\"${SDK_PATH}/protocol/zigbee/app/framework/include/af.h\"
        -DEMBER_AF_API_AF_SECURITY_HEADER=\"${SDK_PATH}/protocol/zigbee/app/framework/security/af-security.h\"
        -DEMBER_AF_API_NEIGHBOR_HEADER=\"${SDK_PATH}/protocol/zigbee/stack/include/stack-info.h\"
        -DEMBER_STACK_ZIGBEE
        -DEZSP_ASH
        -DMBEDTLS_CONFIG_FILE=\"${CMAKE_CURRENT_SOURCE_DIR}/mbedtls-config-generated.h\")


        set(SDK_SRC
        "${CMAKE_CURRENT_SOURCE_DIR}/znet-bookkeeping.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/call-command-handler.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/callback-stub.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/stack-handler-stub.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/znet-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/Z3GatewayHost_callbacks.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/../util/common/library.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/../util/serial/command-interpreter2.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/../util/zigbee-framework/zigbee-device-common.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/../util/zigbee-framework/zigbee-device-host.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/../../../../util/silicon_labs/silabs_core/event_control/event-control.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/platform/base/hal/micro/generic/led-stub.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/platform/base/hal/micro/generic/mem-util.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/platform/base/hal/plugin/antenna-stub/antenna-stub.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/platform/base/hal/plugin/buzzer-stub/buzzer-stub.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/address-table/address-table.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/address-table/address-table-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/basic/basic.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/color-control-server/color-control-server.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/concentrator/concentrator-support.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/concentrator/concentrator-support-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/counters/counters-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/counters/counters-ota-host.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/counters/counters-host.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/ezmode-commissioning/ez-mode.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/ezmode-commissioning/ez-mode-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/util/ezsp/ezsp-callbacks.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/util/ezsp/ezsp-enum-decode.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/util/ezsp/ezsp-frame-utilities.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/util/ezsp/ezsp.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/ezsp-host/ezsp-host-io.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/ezsp-host/ezsp-host-queues.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/ezsp-host/ezsp-host-ui.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/util/ezsp/serial-interface-uart.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/ezsp-host/ash/ash-host-ui.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/ezsp-host/ash/ash-host.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/platform/base/hal/micro/generic/ash-common.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin-host/file-descriptor-dispatch/file-descriptor-dispatch.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/form-and-join/form-and-join-afv2.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/util/common/form-and-join.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/util/common/form-and-join-host-adapter.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin-host/gateway/gateway-support.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin-host/gateway/backchannel-support.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin-host/gateway/gateway-support-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/green-power-client/green-power-client.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/green-power-client/green-power-client-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/green-power-common/green-power-common.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/stack/gp/gp-util.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/heartbeat/heartbeat.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/ias-zone-client/ias-zone-client.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/ias-zone-client/ias-zone-client-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/identify/identify.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/identify/identify-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/identify-feedback/identify-feedback.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/level-control/level-control.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/plugin/plugin-common/linked-list/linked-list.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/aes.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/asn1parse.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/asn1write.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/base64.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/bignum.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/ccm.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/chacha20.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/chachapoly.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/cipher.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/cipher_wrap.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/cmac.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/ctr_drbg.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/des.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/ecdh.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/ecdsa.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/ecjpake.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/ecp.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/ecp_curves.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/entropy.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/error.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/gcm.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/md.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/md5.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/oid.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/pem.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/pk.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/pk_wrap.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/pkparse.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/platform_util.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/poly1305.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/psa_crypto.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/psa_crypto_cipher.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/psa_crypto_client.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/psa_crypto_driver_wrappers.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/psa_crypto_ecp.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/psa_crypto_hash.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/psa_crypto_mac.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/psa_crypto_slot_management.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/psa_crypto_storage.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/rsa.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/rsa_internal.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/sha1.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/sha256.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/sha512.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/ssl_cache.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/ssl_ciphersuites.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/ssl_cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/ssl_cookie.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/ssl_msg.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/ssl_srv.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/ssl_ticket.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/ssl_tls.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/threading.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/x509.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/x509_create.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/x509_crl.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/x509_crt.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/x509_csr.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/x509write_crt.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library/x509write_csr.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin-host/ncp-configuration/ncp-configuration.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/network-creator/network-creator.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/network-creator/network-creator-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/network-creator-security/network-creator-security.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/network-creator-security/network-creator-security-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/network-find/network-find.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/network-find/network-find-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/network-steering/network-steering.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/network-steering/network-steering-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/network-steering/network-steering-v2.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/network-steering/network-steering-host.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/on-off/on-off.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/ota-common/ota-common.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/ota-server/ota-server.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/ota-server/ota-server-page-request.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/ota-server/ota-server-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/ota-server/ota-server-dynamic-block-period.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/ota-server-policy/ota-server-policy.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/ota-storage-common/ota-storage-common.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/ota-storage-common/ota-storage-common-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/ota-storage-posix-filesystem/ota-storage-linux.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/poll-control-client/poll-control-client.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/poll-control-client/poll-control-client-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/reporting/reporting.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/reporting/reporting-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/reporting/reporting-default-configuration.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/scan-dispatch/scan-dispatch.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/util/ezsp/secure-ezsp-stub.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/simple-main/simple-main.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/simple-metering-client/simple-metering-client.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/simple-metering-client/simple-metering-client-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/stack-diagnostics/stack-diagnostics.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/test-harness/test-harness.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/test-harness/read-write-attributes.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/test-harness/test-harness-host.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/time-server/time-server.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/trust-center-backup/trust-center-backup.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/trust-center-backup/trust-center-backup-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/trust-center-backup/trust-center-backup-cli-posix.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/trust-center-backup/trust-center-backup-posix.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/trust-center-keepalive/trust-center-keepalive.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/trust-center-nwk-key-update-broadcast/trust-center-nwk-key-update-broadcast.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/trust-center-nwk-key-update-periodic/trust-center-nwk-key-update-periodic.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/trust-center-nwk-key-update-unicast/trust-center-nwk-key-update-unicast.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/platform/base/hal/micro/generic/crc.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/platform/base/hal/micro/generic/endian.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/platform/base/hal/micro/generic/random.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/platform/base/hal/micro/generic/system-timer.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/platform/base/hal/micro/unix/host/micro.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/platform/base/hal/micro/unix/host/token-def-unix.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/platform/base/hal/micro/unix/host/token.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/platform/base/hal/plugin/serial/ember-printf-convert.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/util/serial/linux-serial.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/update-tc-link-key/update-tc-link-key.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/plugin/update-tc-link-key/update-tc-link-key-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/cli/core-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/cli/network-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/cli/option-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/cli/plugin-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/cli/security-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/cli/zcl-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/cli/zdo-cli.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/security/af-node.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/security/af-security-common.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/security/af-trust-center.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/security/crypto-state.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/util/af-event.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/util/af-main-common.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/util/attribute-size.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/util/attribute-storage.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/util/attribute-table.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/util/client-api.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/util/message.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/util/multi-network.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/util/print.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/util/print-formatter.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/util/process-cluster-message.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/util/process-global-message.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/util/service-discovery-common.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/util/time-util.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/util/util.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/util/af-main-host.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/util/service-discovery-host.c"
        )


set(SDK_INC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/../..
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/../../stack
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/../util
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/../util/common
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/../util/ezsp
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/../util/serial
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/../util/zigbee-framework
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/cli
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/include
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/security
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/protocol/zigbee/app/framework/util
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/platform/base/hal
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/platform/base/hal/plugin
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/platform/base/hal/..
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/platform/base/hal/micro/generic
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/platform/base/hal/micro/unix/host
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/platform/base/hal/../../../platform/common/inc/
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/platform/base/hal/../../../platform/radio/mac/
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/platform/base/hal/../../../util/silicon_labs/silabs_core/
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/platform/base/hal/../../../util/silicon_labs/silabs_core/buffer_manager
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/platform/base/hal/../../../util/silicon_labs/silabs_core/event_control
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/platform/base/hal/micro/unix/host/board
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/silicon_labs/silabs_core
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/plugin/plugin-common/mbedtls
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/include
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/include/mbedtls
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/include/psa
        ${CMAKE_CURRENT_SOURCE_DIR}/developer/sdks/gecko_sdk_suite/v3.2/util/third_party/crypto/mbedtls/library
        )

include_directories(${SDK_INC})

add_library(host STATIC ${SDK_SRC} ${SDK_INC})



