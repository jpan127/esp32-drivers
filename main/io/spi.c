#include <driver/spi_master.h>

void spi_initialize(spi_device_handle_t *spi, int cs, int miso, int mosi, int sclk, int ssel)
{
    static const char *TAG = "spi_initialize";
    ESP_LOGI(TAG, "Starting initialization.");

    // SPI bus initialize
    int max_transfer_size = 5;
    spi_bus_config_t bus_config = spi_initialize_bus_config(mosi, miso, sclk, -1, -1, max_transfer_size);
    ESP_ERROR_CHECK(spi_bus_initialize(HSPI_HOST, bus_config, 1));

    // Device interface configuration
    spi_device_interface_config_t device_config;
    device_config.address_bits      = 0;
    device_config.command_bits      = 0;
    device_config.dummy_bits        = 0;
    device_config.mode              = 0;
    device_config.cs_ena_posttrans  = 0;
    device_config.cs_ena_pretrans   = 0;
    device_config.clock_speed_hz    = 10000;
    device_config.spics_io_num      = PIN_CS;
    device_config.flags             = 0;
    device_config.queue_size        = 1;
    device_config.pre_cb            = NULL;
    device_config.post_cb           = NULL;
    device_config.device_duty_cycle_pos = 0;
    // Add external SPI device
    ESP_ERROR_CHECK(spi_bus_add_device(HSPI_HOST, spi));

    // Make a transaction
    // spi_transaction_t transaction;
    // transacton.address
    // transaction.command
    // ESP_ERROR_CHECK(spi_device_transmit(spi, transaction));

    // Remove device
    ESP_ERROR_CHECK(spi_bus_remove_device(handle));

    // Free bus
    ESP_ERROR_CHECK(spi_bus_free(HSPI_HOST));

    ESP_LOGI(TAG, "Finished initialization.");
}

spi_config_t spi_initialize_config(int mosi, int miso, int sclk, int wp, int hold)
{
    spi_config_t config;
    config.mosi_io_num = mosi;
    config.miso_io_num = miso;
    config.sclk_io_num = sclk;
    config.quadwp_io_num = wp;      // write protect
    config.quadhd_io_num = hold;    // hold

    return config;
}

spi_bus_config_t spi_initialize_bus_config(int mosi, int miso, int sclk, int wp, int hold, int max_size)
{
    spi_bus_config_t bus_config;
    bus_config.mosi_io_num = mosi;
    bus_config.miso_io_num = miso;
    bus_config.sclk_io_num = sclk;
    bus_config.quadwp_io_num = wp;
    bus_config.quadhd_io_num = hold;
    bus_config.max_transfer_sz = max_size;

    return bus_config;
}