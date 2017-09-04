#include <driver/i2c.h>

typedef unsigned char byte;

// Configure SCL and SDA pins for I2C
void i2c_config(i2c_port_t port, i2c_mode_t mode, gpio_num_t sda, gpio_num_t scl,
                gpio_pullup_t scl_en, gpio_pullup_t sda_en)
{
    i2c_config_t config;
    config.mode = mode;
    config.sda_io_num = sda;
    config.scl_io_num = scl;
    config.sda_pullup_en = sda_en;
    config.scl_pullup_en = scl_en;

    ESP_ERROR_CHECK(i2c_param_config(port, &config));
}

// Installing the driver for the I2C pins
void i2c_install(i2c_port_t port, i2c_mode_t mode)
{
    ESP_ERROR_CHECK(i2c_driver_install(port, mode, 0, 0, 0));
}

// Scan for I2C slaves
void i2c_master_scanner()
{
    static const char *TAG = "I2C_SCANNER";

    ESP_LOGD(TAG, "Starting I2C scan.");

    i2c_port_t port;

    i2c_config_t config;
    config.mode = I2C_MODE_MASTER;
    config.sda_io_num = sda;
    config.scl_io_num = scl;
    config.sda_pullup_en = GPIO_PULLUP_ENABLE;
    config.scl_pullup_en = GPIO_PULLUP_ENABLE;
    config.master.clk_speed = 100000;
    i2c_param_config(port, &config);

    i2c_driver_install(port, I2C_MODE_MASTER, 0, 0, 0);

    esp_error_t error;
    printf("     0 1 2 3 4 5 6 7 8 9 a b c d e f\n");
    printf("00:       ");
    for (int i=3; i < 0x78; i++) {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (i<<1) | I2C_MASTER_WRITE, 1);
        i2c_master_stop(cmd);

        error = i2c_master_cmd_begin(port, cmd, 10 / portTICK_PERIOD_MS);

        if (i%16  == 0) printf("\n%.2x:", i);
        if (error == 0) printf(" %.2x", i);
        else            printf(" --");

        i2c_cmd_link_delete(cmd);
    }

    printf("\n");
}

// FreeRTOs task for running as an I2C master
void i2c_master_task(void *p)
{
    // Scan for slave first
    i2c_master_scanner();

    // Create command handle and start as master
    i2c_cmd_handle_t cmd_handle = i2c_cmd_link_create();
    i2c_master_start(cmd_handle);

    i2c_port_t port;

    while (1)
    {
        // Write
        byte addr = 0x00;
        byte data = 0xFF;
        bool ack_en = true;
        i2c_master_write(cmd_handle, addr, data, ack_en);

        // Read
        byte data;
        byte addr;
        int ack;
        i2c_master_read(cmd_handle, addr, data, ack);
    }
}

void i2c_master_read(i2c_cmd_handle_t cmd_handle, byte addr, byte &data, int &ack)
{
    // Add read bit
    addr = (addr << 1) | I2C_MASTER_READ;
    ESP_ERROR_CHECK(i2c_master_read_byte(cmd_handle, &data, ack));
    // Stop
    i2c_master_stop(cmd_handle);
    // Perform command
    ESP_ERROR_CHECK(i2c_master_cmd_begin(port, cmd_handle, 1000 / portTICK_PERIOD_MS));
    // Delete
    i2c_cmd_link_delete(cmd_handle);
}

void i2c_master_write(i2c_cmd_handle_t cmd_handle, byte addr, byte data, bool ack_en)
{
    // Add write bit
    addr = (addr << 1) | I2C_MASTER_WRITE;
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd_handle, data, ack_en));
    // Stop
    i2c_master_stop(cmd_handle);
    // Perform command
    ESP_ERROR_CHECK(i2c_master_cmd_begin(port, cmd_handle, 1000 / portTICK_PERIOD_MS));
    // Delete
    i2c_cmd_link_delete(cmd_handle);
}