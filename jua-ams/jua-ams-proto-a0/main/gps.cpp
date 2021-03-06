#include "esp_log.h"
#include "driver/uart.h"
#include "minmea.h"
#include "esp_timer.h"
#include "time.h"

#define GPS_TX_PIN (GPIO_NUM_12)

static char tag[] = "gps";

char *line;


char *readLine(uart_port_t uart) {
	static char line[256];
	int size;
	char *ptr = line;

	while(1) {
		size = uart_read_bytes(uart, (unsigned char *)ptr, 1, portMAX_DELAY);
		if (size == 1) {
			if (*ptr == '\n') {
				ptr++;
				*ptr = 0;
				return line;
			}
			ptr++;
		} // End of read a character
	} // End of loop
} // End of readLine


void initGPS() {
	ESP_LOGI(tag, ">> GPS init...");
	uart_config_t myUartConfig;
	myUartConfig.baud_rate           = 9600;
	myUartConfig.data_bits           = UART_DATA_8_BITS;
	myUartConfig.parity              = UART_PARITY_DISABLE;
	myUartConfig.stop_bits           = UART_STOP_BITS_1;
	myUartConfig.flow_ctrl           = UART_HW_FLOWCTRL_DISABLE;
	myUartConfig.rx_flow_ctrl_thresh = 120;

	uart_param_config(UART_NUM_1, &myUartConfig);

	uart_set_pin(UART_NUM_1,
			UART_PIN_NO_CHANGE, // TX
			GPS_TX_PIN,         // RX
			UART_PIN_NO_CHANGE, // RTS
			UART_PIN_NO_CHANGE  // CTS
  	);
	uart_driver_install(UART_NUM_1, 2048, 2048, 0, NULL, 0);
}

void readGPS() {
	//ESP_LOGD(tag, "%s", line);
	switch(minmea_sentence_id(line, false)) {
		case MINMEA_SENTENCE_RMC:
			ESP_LOGD(tag, "Sentence - MINMEA_SENTENCE_RMC");
      		struct minmea_sentence_rmc frame;
      		if (minmea_parse_rmc(&frame, line)) {
          		ESP_LOGD(tag, "$xxRMC: raw coordinates and speed: (%d/%d,%d/%d) %d/%d",
                  			  frame.latitude.value, frame.latitude.scale,
                  			  frame.longitude.value, frame.longitude.scale,
                  			  frame.speed.value, frame.speed.scale);
          		ESP_LOGD(tag, "$xxRMC fixed-point coordinates and speed scaled to three decimal places: (%d,%d) %d",
                  minmea_rescale(&frame.latitude, 1000),
                  minmea_rescale(&frame.longitude, 1000),
                  minmea_rescale(&frame.speed, 1000));
          		ESP_LOGD(tag, "$xxRMC floating point degree coordinates and speed: (%f,%f) %f",
                  minmea_tocoord(&frame.latitude),
                  minmea_tocoord(&frame.longitude),
                  minmea_tofloat(&frame.speed));
				
				struct timespec ts;
				minmea_gettime(&ts, &frame.date, &frame.time);
				ESP_LOGD(tag, "$xxRMC time: %ld", ts.tv_sec);
				// Use POSIX and C standard library functions to work with files.
				// First create a file.
				ESP_LOGI(tag, "Opening file");
				FILE* f = fopen("/sdcard/gps.txt", "a");
				if (f == NULL) {
					ESP_LOGE(tag, "Failed to open file for writing");
					return;
				}
				fprintf(f, "%ld;%f;%f\n", ts.tv_sec,
										 minmea_tocoord(&frame.latitude),
										 minmea_tocoord(&frame.longitude));
				fclose(f);
				ESP_LOGI(tag, "File written");
      		}
      		else {
      			ESP_LOGD(tag, "$xxRMC sentence is not parsed\n");
      		}
			break;
		case MINMEA_SENTENCE_GGA:
			//ESP_LOGD(tag, "Sentence - MINMEA_SENTENCE_GGA");
			break;
		case MINMEA_SENTENCE_GSV:
			//ESP_LOGD(tag, "Sentence - MINMEA_SENTENCE_GSV");
			break;
		default:
			//ESP_LOGD(tag, "Sentence - other");
			break;
	}
}

void smartDelay(int64_t us) {
  int64_t start = esp_timer_get_time();
  do {
	line = readLine(UART_NUM_1);
  } while (esp_timer_get_time() - start < us);
}