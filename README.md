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

## Change $HOME of msys32 Windows esp-idf shell

    1. Navigate to msys32/home/<USER>
    2. Open .bash_profile
    3. Add this at the very top
        - export HOME=<PATH_TO_PROJECT>
        - cd $HOME
