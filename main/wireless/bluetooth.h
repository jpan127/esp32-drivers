#pragma once
#include <esp_log.h>                    // Logging
#include <freertos/FreeRTOS.h>          // FreeRTOS
#include <freertos/task.h>              // Create tasks
#include <bt.h>
#include <bt_trace.h>
#include <bt_types.h>
#include <esp_gap_ble_api.h>

// GAP: Generic Access Profile
// Scanning:
//      1. Register a callback function: esp_ble_gap_register_callback
//      2. Scan request: esp_ble_gap_start_scanning()
// GATT: General Attributes Protocol
// Client:
//      1. Register a callback function: esp_ble_gattc_register_callback
//      2. Call esp_ble_gattc_app_register()
//      3. Open a connection to the GATT server: esp_ble_gattc_open()
//      4. Execute a search for open events: esp_ble_gattc_search_service()
// Server:
//      esp_bt_controller_init()
//      esp_bt_controller_enable()
//      esp_bluedroid_init()
//      esp_bluedroid_enable()
//      esp_ble_gatts_register_callback()
//      esp_ble_gap_register_callback()
//      Register application: esp_ble_gatts_app_register()
//      esp_ble_gap_set_device_name()
//      esp_ble_gap_config_adv_data()
//      esp_ble_gatts_create_service()