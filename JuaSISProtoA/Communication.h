#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#define BLYNK_PRINT Serial

#include <Arduino.h>
#include <ESP8266WiFi.h>
/* set #define FTP_DEBUG in ESP8266FtpServer.h to see ftp verbose on serial */
#include <ESP8266FtpServer.h>


class Communication {
	public:
		static Communication* getInstance();
		enum class CommunicationState {DISCONNECTED, CONNECTING, CONNECTED};
		void enable();
		void disable();
		bool isEnabled();
		void startFTPServer();
		void checkCommunication();

	private:
		Communication();  // private so that it can  not be called
		Communication(const Communication&) = delete;
		Communication& operator=(const Communication&) = delete;
		static Communication* pInstance;
		const char *ssid = "juasis";
		const char *pass = "";
		const char *ftpUser = "juasis";
		const char *ftpPass = "juasis";
		bool enableCommFlag;
		CommunicationState commState;
		FtpServer ftpSrv;
		uint32_t lastVerificationTS;
};

#endif
