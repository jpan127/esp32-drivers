deps_config := \
	/c/Users/JP/Desktop/ESP32/esp-idf/components/app_trace/Kconfig \
	/c/Users/JP/Desktop/ESP32/esp-idf/components/aws_iot/Kconfig \
	/c/Users/JP/Desktop/ESP32/esp-idf/components/bt/Kconfig \
	/c/Users/JP/Desktop/ESP32/esp-idf/components/esp32/Kconfig \
	/c/Users/JP/Desktop/ESP32/esp-idf/components/ethernet/Kconfig \
	/c/Users/JP/Desktop/ESP32/esp-idf/components/fatfs/Kconfig \
	/c/Users/JP/Desktop/ESP32/esp-idf/components/freertos/Kconfig \
	/c/Users/JP/Desktop/ESP32/esp-idf/components/log/Kconfig \
	/c/Users/JP/Desktop/ESP32/esp-idf/components/lwip/Kconfig \
	/c/Users/JP/Desktop/ESP32/esp-idf/components/mbedtls/Kconfig \
	/c/Users/JP/Desktop/ESP32/esp-idf/components/openssl/Kconfig \
	/c/Users/JP/Desktop/ESP32/esp-idf/components/spi_flash/Kconfig \
	/c/Users/JP/Desktop/ESP32/esp-idf/components/wear_levelling/Kconfig \
	/c/Users/JP/Desktop/ESP32/esp-idf/components/bootloader/Kconfig.projbuild \
	/c/Users/JP/Desktop/ESP32/esp-idf/components/esptool_py/Kconfig.projbuild \
	/c/Users/JP/Desktop/ESP32/esp-idf/components/partition_table/Kconfig.projbuild \
	/c/Users/JP/Desktop/ESP32/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
