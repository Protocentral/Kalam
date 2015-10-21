/******************************************************************************
Function definitions and pin declaration for ESP8266 defined here
******************************************************************************/



#include <Arduino.h>
#include <SoftwareSerial.h>
#include <IPAddress.h>

//******** Pin Definitions //

#define ESP8266_SW_RX	6	// ESP8266 UART0 RXI goes to Arduino pin 9   //7
#define ESP8266_SW_TX	7	// ESP8266 UART0 TXO goes to Arduino pin 8   //6


// Command Response Timeouts //

#define COMMAND_RESPONSE_TIMEOUT 1000
#define COMMAND_PING_TIMEOUT 3000
#define WIFI_CONNECT_TIMEOUT 30000
#define COMMAND_RESET_TIMEOUT 5000
#define CLIENT_CONNECT_TIMEOUT 5000

#define ESP8266_MAX_SOCK_NUM 5
#define ESP8266_SOCK_NOT_AVAIL 255

#define 	ESP8266_CMD_BAD -5
#define 	ESP8266_RSP_MEMORY_ERR  -4
#define 	ESP8266_RSP_FAIL  -3
#define  	ESP8266_RSP_UNKNOWN  2
#define 	ESP8266_RSP_TIMEOUT -1
#define 	ESP8266_RSP_SUCCESS  0
#define 	ESP8266_ECN_OPEN 0
#define 	ESP8266_ECN_WPA_PSK 	1
#define 	ESP8266_ECN_WPA2_PSK 	2
#define 	ESP8266_ECN_WPA_WPA2_PSK 3



typedef enum esp8266_wifi_mode {
	ESP8266_MODE_STA = 1,
	ESP8266_MODE_AP = 2,
	ESP8266_MODE_STAAP = 3
};

typedef enum esp8266_command_type {
	ESP8266_CMD_QUERY,
	ESP8266_CMD_SETUP,
	ESP8266_CMD_EXECUTE
};


typedef enum esp8266_connect_status {
	ESP8266_STATUS_GOTIP = 2,
	ESP8266_STATUS_CONNECTED = 3,
	ESP8266_STATUS_DISCONNECTED = 4,
	ESP8266_STATUS_NOWIFI = 5	
};


typedef enum esp8266_socket_state {
	AVAILABLE = 0,
	TAKEN = 1,
};

typedef enum esp8266_connection_type {
	ESP8266_TCP,
	ESP8266_UDP,
	ESP8266_TYPE_UNDEFINED
};

struct esp8266_status
{
	esp8266_connect_status stat;

};

class ESP8266Class : public Stream
{
public:
	
	void begin(unsigned long baudRate);

	// Basic AT Commands //

	bool test();
	bool reset();
	int16_t getVersion();
	bool echo(bool enable);
	bool setBaud(unsigned long baud);

	// WiFi Functions //

	int16_t getMode();
	int16_t setMode(esp8266_wifi_mode mode);
	int16_t setMode(int8_t mode);
	int16_t connect(const char * ssid);
	int16_t connect(const char * ssid, const char * pwd);
	int16_t getAP(char * ssid);
	int16_t disconnect();
	IPAddress localIP();
	int16_t ping(IPAddress ip);
	int16_t ping(char * server);
	int16_t setMux(uint8_t mux);
		
	size_t write(uint8_t);
	int available();
	int read();
	int peek();
	void flush();

    int16_t _state[ESP8266_MAX_SOCK_NUM];
	
protected:
 
	unsigned long _baud;
	
private:
	void sendCommand(const char * cmd, enum esp8266_command_type type = ESP8266_CMD_EXECUTE, const char * params = NULL);
	int16_t readForResponse(const char * rsp, unsigned int timeout);
	int16_t readForResponses(const char * pass, const char * fail, unsigned int timeout);
	void clearBuffer();
	/// readByteToBuffer() - Read first byte from UART receive buffer
	/// and store it in rxBuffer.
	unsigned int readByteToBuffer();
	
	char * searchBuffer(const char * test);
	
	esp8266_status _status;
	uint8_t sync();
};

extern ESP8266Class esp8266;

