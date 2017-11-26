#include "gpio.hpp"

typedef struct
{
    gpio_num_t    rx_pin;
    gpio_num_t    tx_pin;
    uint32_t      baud_rate;
    uint32_t      rx_buffer_size;
    uint32_t      tx_buffer_size;
    uint32_t      queue_size;
    QueueHandle_t queue;
    int           flags;
} uart_init_S;

typedef struct
{
    uint8_t  sync1;
    uint8_t  sync2;
    uint8_t  msg_class;
    uint8_t  id;
    uint16_t length;
    uint8_t  payload[256];
    uint8_t  checksum_a;
    uint8_t  checksum_b;
} __attribute__((packed)) ublox_packet_S;

typedef enum
{
    NAV = 0x1,
    RXM = 0x2,
    INF = 0x4,
    ACK = 0x5,
    CFG = 0x6,
    MON = 0xA,
    AID = 0xB,
    TIM = 0xD,
    ESF = 0x10
} ublox_msg_class_E;

typedef enum
{
    IDLE,
    HEADER0,
    HEADER1,
    CLASS,
    ID,
    LENGTH,
    PAYLOAD,
    CHECKSUMA,
    CHECKSUMB
} ublox_parser_state_E;

SemaphoreHandle_t GPSsem = xSemaphoreCreateBinary();
extern "C"
{
    void ISR_GPS()
    {
        int HigherPriorityTaskWoken;
        xSemaphoreGiveFromISR(GPSsem, HigherPriorityTaskWoken);
        if (HigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR();
        }
    }
}

class GPS
{
public:

    // @description : Constructor
    GPS(uart_init_S init_info);


private:

    uint8_t RxBuffer[1024];

    ublox_parser_state_E ParserState;
    uint16_t RxBufferPointer;
    uint16_t BytesUnparsed;

    void InitializeGPSUart(uart_init_S init_info);

    uint32_t ReceiveBytes();
};

GPS::GPS(uart_init_S init_info)
{
    memset(RxBuffer, 0, 1024);
    ParserState     = IDLE;
    RxBufferPointer = 0;
    BytesUnparsed   = 0;
    xSemaphoreTake(GPSsem, 0);
    InitializeGPSUart(init_info);
}

void GPS::InitializeGPSUart(uart_init_S init_info)
{
    uart_config_t uart_config = {
        .baud_rate = init_info.baud_rate,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_DISABLE_PARITY,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, init_info.tx_pin, init_info.rx_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_1, 
                        init_info.rx_buffer_size, 
                        init_info.tx_buffer_size, 
                        init_info.queue_size, 
                        init_info.queue, 
                        init_info.flags);

    // Set up ISR handler with low/medium priorit
    uart_isr_register(UART_NUM_1, ISR_GPS, NULL, ESP_INTR_FLAG_LOWMED, NULL);
}

uint32_t GPS::HandleRxBuffer()
{
    // Reset buffer if brand new packet
    if (IDLE == ParserState)
    {
        memset(RxBuffer, 0, 1024);
        ParserState = HEADER;
    }
    
    return uart_read_bytes(UART_NUM_1, RxBuffer, 1024, 20 / portTICK_PERIOD_MS);
}

void GPS::ReadRegister()
{
    const uint8_t address = 0;
    const 
}

void GPS::SetConfiguration()
{

}

void GPS::ParseMessages()
{
    const  uint8_t header[2]            = { 0xB5, 0x62 };
    static uint8_t correct_checksum[2]  = { 0 };

    // Packet
    static ublox_packet_S packet = { 0 };

    // Counts how many bytes left in the current state before switching to next state
    static int counter = -1;
    static uint16_t payload_pointer = 0;

    // State machine for every byte unparsed in the buffer
    for (int i=RxBufferPointer; i < (RxBufferPointer + BytesUnparsed); i++)
    {
        switch (ParserState)
        {
            case HEADER:
                // Reset counter
                if (-1 == counter)
                {
                    counter = 2;
                }
                // First byte
                if (2 == counter)
                {
                    packet.sync1 = RxBuffer[i];
                    if (packet.sync1 != header[0])
                    {
                        ESP_LOGE("GPS::ParseMessages", "Header0 incorrect! Expected: { 0xB5, } Actual: %0.2f\n", packet.sync1);
                        // Should fail gracefully here
                    }
                    --counter;
                }
                // Second byte
                else if (1 == counter)
                {
                    packet.sync2 = RxBuffer[i];
                    if (packet.sync2 != header[1])
                    {
                        ESP_LOGE("GPS::ParseMessages", "Header1 incorrect! Expected: { , 0x62} Actual: %0.2f\n", packet.sync2);
                        // Should fail gracefully here
                    }
                    ParserState = CLASS;
                    counter = -1;
                }
                break;
            case CLASS:
                packet.msg_class = RxBuffer[i];
                ParserState = ID;
                break;
            case ID:
                packet.id = RxBuffer[i];
                ParserState = LENGTH;
                break;
            case LENGTH:
                // Reset counter
                if (-1 == counter)
                {
                    counter = 2;
                }
                // First byte
                if (2 == counter)
                {
                    // LSB first
                    packet.length |= RxBuffer[i];
                    --counter;
                }
                // Second byte
                else if (1 == counter)
                {
                    packet.length |= RxBuffer[i] << 8;
                    counter = -1;
                    ParserState = PAYLOAD;
                }
                break;
            case PAYLOAD:
                // Reset counter to payload length
                if (-1 == counter)
                {
                    counter = length;
                }
                // Load into payload buffer
                if (counter > 0)
                {
                    packet.payload[payload_pointer++] = RxBuffer[i];
                    --counter;
                }
                // When counter reaches 0 switch to checksum
                if (0 == counter)
                {
                    counter = -1;
                    ParserState = CHECKSUM;
                }
                break;
            case CHECKSUM:
                if (-1 == counter)
                {
                    counter = 2;
                }
                if (2 == counter)
                {
                    packet.checksum_a = RxBuffer[i];
                    --counter;
                }
                else if (1 == counter)
                {
                    packet.checksum_b = RxBuffer[i];
                    counter = -1;
                    ParserState = IDLE;
                }
                break;
        }
    }

    CalculateChecksum(&packet, correct_checksum[0], correct_checksum[1]);
    if (correct_checksum[0] != checksum[0] || correct_checksum[1] != checksum[1])
    {
        printf("Checksum check failed!\n");
    }
}

void GPS::CalculateChecksum(ublox_packet_S *packet, uint8_t *checksum_a, uint8_t *checksum_b)
{
    *checksum_a += packet.class;
    *checksum_b += *checksum_a;

    *checksum_a += packet.id;
    *checksum_b += *checksum_a;

    *checksum_a += packet.length & 0xFF;
    *checksum_b += *checksum_a;

    *checksum_a += packet.length >> 8;
    *checksum_b += *checksum_a;

    for (int i=0; i<packet.length; i++)
    {
        *checksum_a += packet.payload[i];
        *checksum_b += *checksum_a;
    }
}

void GPS::PollUbloxUartConfiguration()
{
    uint8_t checksum[2] = { 0x00, 0x00 };
    ublox_packet_S packet = {
        .sync1      = 0xB5,
        .sync2      = 0x62,
        .class      = 0x6,
        .id         = 0x0,
        .length     = 0x0,
        .payload    = { 0 },
        .checksum_a = 0,
        .checksum_b = 0,
    };

    // Calculate checksum and replace it in the packet
    CalculateChecksum(&packet, checksum[0], checksum[1]);
    packet.checksum_a = checksum[0];
    packet.checksum_b = checksum[1];

    // Send to device
    uint16_t array_size = 8 + packet.length;
    uint8_t *array = ConvertUbloxPacketToCharArray(&packet);
    {
        uart_write_bytes(UART_NUM_1, 
                         array, 
                         sizeof(uint8_t) * array_size, 
                         100 / portTICK_PERIOD_MS);
    }
    free(array);

    // Receive from device
    memset(RxBuffer, 0, 1024);
    uart_read_bytes(UART_NUM_1, RxBuffer, 1024, 20 / portTICK_PERIOD_MS);
    ParseMessages();
}

uint8_t* GPS::ConvertUbloxPacketToCharArray(ublox_packet_S *packet)
{
    // Packets always have 8 bytes excluding payload
    uint8_t *array = malloc(sizeof(uint8_t) * (8 + packet.length));
    array[0] = packet.sync1;
    array[1] = packet.sync2;
    array[2] = packet.class;
    array[3] = packet.id;
    array[4] = packet.length & 0xFF;
    array[5] = packet.length >> 8;
    memcpy(array + 6, packet.payload, packet.length);

    // Remember to free after using
    return array;
}