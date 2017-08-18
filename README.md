# esp32-drivers
Series of drivers written for ESP32 based on ESP-IDF's FreeRTOS framework.

## Instructions to flash

Assuming mys32 and esp-idf is installed correctly.

    1. Launch mingw32.exe
    2. Change directory to the project
    3. Run <make menuconfig>
    4. Change setting under <Serial flasher config> <Default serial port> to the device's serial port
    5. Save and exit
    6. Run <make flash monitor>
    7. Run <Ctrl + ]> to exit
