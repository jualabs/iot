deps_config := \
	/Users/victormedeiros/esp/esp-idf/components/app_trace/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/aws_iot/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/bt/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/driver/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/esp32/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/esp_event/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/esp_http_client/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/esp_http_server/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/ethernet/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/fatfs/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/freemodbus/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/freertos/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/heap/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/libsodium/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/log/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/lwip/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/mbedtls/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/mdns/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/mqtt/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/nvs_flash/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/openssl/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/pthread/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/spi_flash/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/spiffs/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/vfs/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/wear_levelling/Kconfig \
	/Users/victormedeiros/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/Users/victormedeiros/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/Users/victormedeiros/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/Users/victormedeiros/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
