#include "bluetooth.h"

// Inquiry scan: scanning for devices
// Paging scan: acepts connection requests
// Bonding: pairs devices, exchange addresses + names + data, generate keys
// Profiles: e.g. head set, hands free, human interface, serial port
// Protocols: RFCOMM, L2CAP, ACL, SCO
// 16 byte UUID in a format 4-2-2-2-6
// esp_ble_gap_config_adv_data(): sepcifies content of periodic advertisement
// esp_ble_gap_start_advertising(): to initiate periodic advertisement


void bluetooth_init()
{
    // Set up callback function
    esp_ble_gap_register_callback(&bluetooth_event_handler);

    // Set scan parameters
    esp_ble_gap_set_scan_params();
}

void bluetooth_start_scan()
{
    ESP_ERROR_CHECK(esp_ble_gap_start_scanning());
}

void bluetooth_stop_scan()
{
    ESP_ERROR_CHECK(esp_ble_gap_stop_scanning());
}

void bluetooth_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    
}

void config_data()
{
    esp_ble_adv_data_t advertising_data;
    // Set advertising data
    ESP_ERROR_CHECK(esp_ble_gap_config_adv_data(&advertising_data);
}