#pragma once
#include <uart.h>

#define DEFAULT_BAUD (9600)

class Uart
{
public:

    // Constructor without RTS/CTS pins
    Uart(uart_port_t port, uint8_t tx_pin, uint8_t rx_pin, int baud_rate=DEFAULT_BAUD);

    // Constructor with RTS/CTS pins
    Uart(uart_port_t port, uint8_t tx_pin, uint8_t rx_pin, uint8_t rts_pin, uint8_t cts_pin, int baud_rate=DEFAULT_BAUD);

    // Initialize UART
    Initialize();

    // Send string
    bool SendString(char *string, size_t size);
    bool SendString(const char *string, size_t size);

    // Send a single byte
    bool SendByte(char *byte);

    // Receive a string
    char* ReceiveString();

private:

    uint8_t     Tx;
    uint8_t     Rx;
    uart_port_t Port;
};

void Uart::Uart(uart_port_t port, uint8_t tx_pin, uint8_t rx_pin, int baud_rate)
{
    Tx = tx_pin;
    Rx = rx_pin;
    Port = port;

    Initialize(port, pin, false);
}

void Uart::Uart(uart_port_t port, uint8_t tx_pin, uint8_t rx_pin, uint8_t rts_pin, uint8_t cts_pin, int baud_rate)
{
    Tx = tx_pin;
    Rx = rx_pin;
    Rts = rts_pin;
    Cts = cts_pin;
    Port = port;

    Initialize(baud_rate, true);
}

void Uart::Initialize(int baud_rate, bool flow_control)
{
    // Configure
    uart_config_t config;
    config.baud_rate    = baud_rate                 // Baud rate
    config.data_bits    = UART_DATA_8_BITS;         // 8 bit data
    config.parity       = UART_PARITY_DISABLE;      // No parity
    config.stop_bits    = UART_STOP_BITS_1;         // 1 Stop bit
    config.flow_ctrl    = (flow_control) ?          // Hardware flow control
                          (UART_HW_FLOWCTRL_CTS_RTS) : 
                          (UART_HW_FLOWCTRL_DISABLE);
    config.rx_flow_ctrl_thresh = 122;               // Not sure what this threshold does
    ESP_ERROR_CHECK(uart_param_config(Port, &config));

    // Set pins
    ESP_ERROR_CHECK(uart_set_pin(Port, Tx, Rx, Rts, Cts));

    // Install uart driver
    int queue_length = 32;
    int buffer_size  = 1024;
    QueueHandle_t *uart_queue;
    uart_driver_install(Port, buffer_size, buffer_size, queue_length, uart_queue, 0);
}

bool Uart::SendString(char *string, size_t size)
{
    if (uart_write_bytes(Port, (const char *)string, size) < 0) {
        ESP_LOGE("Uart::SendString", "Error writing byte to Uart.");
        return false;
    }
    else return true;
}

bool Uart::SendString(const char *string, size_t size)
{
    if (uart_write_bytes(Port, string, size) < 0) {
        ESP_LOGE("Uart::SendString", "Error writing byte to Uart.");
        return false;
    }
    else return true;
}

bool Uart::SendByte(char *byte)
{
    if (uart_write_bytes(Port, byte, 1) < 0) {
        ESP_LOGE("Uart::SendByte", "Error writing byte to Uart.");
        return false;
    }
    else return true;
}

char* Uart::ReceiveString(int timeout)
{
    int buffer_length = 1024;
    char *buffer = new char[buffer_length];
    if (uart_read_bytes(Port, buffer, buffer_length, (TickType_t)timeout) < 0) {
        ESP_LOGE("Uart::ReceiveString", "Timed out receiving bytes.");
        return NULL;
    }
    else {
        return buffer;
    }
}