/******************************************************************************
SparkFunESP8266WiFi.cpp
ESP8266 WiFi Shield Library Main Source File
******************************************************************************/

#include <protoESP8266WiFiShield.h>
#include <Arduino.h>
#include "ESP8266_AT.h"

#define ESP8266_DISABLE_ECHO
SoftwareSerial ESP8266Serial(ESP8266_SW_TX, ESP8266_SW_RX);

#define ESP8266_RX_BUFFER_LEN 128 // Number of bytes in the serial receive buffer
char esp8266RxBuffer[ESP8266_RX_BUFFER_LEN];
unsigned int bufferHead; 

void ESP8266Class::begin(unsigned long baudRate)
{
   ESP8266Serial.begin(9600);

}

// Basic AT Commands //

// Test AT startup
bool ESP8266Class::test()
{
	sendCommand(ESP8266_TEST); // Send AT

	if (readForResponse(RESPONSE_OK, COMMAND_RESPONSE_TIMEOUT) > 0)
	{//	return true;
	if (!setMux(1))
	return false;
#ifdef ESP8266_DISABLE_ECHO
		if (!echo(false))
			return false;
#endif	
	return true;
	}
	return false;
}

// Restart the module
bool ESP8266Class::reset()
{
	sendCommand(ESP8266_RESET); // Send AT+RST
	
	if (readForResponse(RESPONSE_READY, COMMAND_RESET_TIMEOUT) > 0)
		return true;
	
	return false;
}

// enable or disable the echo on esp
bool ESP8266Class::echo(bool enable)
{
	if (enable)
		sendCommand(ESP8266_ECHO_ENABLE);
	else
		sendCommand(ESP8266_ECHO_DISABLE);
	
	if (readForResponse(RESPONSE_OK, COMMAND_RESPONSE_TIMEOUT) > 0)
		return true;
	
	return false;
}

//set the baud rate

bool ESP8266Class::setBaud(unsigned long baud)
{
	char parameters[16];
	
	// Send AT+UART=baud,databits,stopbits,parity,flowcontrol
	sendCommand(ESP8266_UART, ESP8266_CMD_SETUP, parameters);
	
	if (readForResponse(RESPONSE_OK, COMMAND_RESPONSE_TIMEOUT) > 0)
		return true;
	
	return false;
}

// Get the current firmware  version on the device
int16_t ESP8266Class::getVersion()
{
	sendCommand(ESP8266_VERSION); // Send AT+GMR
	// Example Response: AT version:0.30.0.0(Jul  3 2015 19:35:49)\r\n (43 chars)
	int16_t rsp = (readForResponse(RESPONSE_OK, COMMAND_RESPONSE_TIMEOUT) > 0);
	
	return rsp;
}


// WiFi Functions //
// getMode()
//    - Success: 1, 2, 3 (ESP8266_MODE_STA, ESP8266_MODE_AP, ESP8266_MODE_STAAP)

int16_t ESP8266Class::getMode()
{
	sendCommand(ESP8266_WIFI_MODE, ESP8266_CMD_QUERY);
	
    int16_t rsp = readForResponse(RESPONSE_OK, COMMAND_RESPONSE_TIMEOUT);
	if (rsp > 0)
	{
		// get the number
		char * p = strchr(esp8266RxBuffer, ':');
		if (p != NULL)
		{
			char mode = *(p+1);
			if ((mode >= '1') && (mode <= '3'))
				return (mode - 48); // Convert ASCII to decimal
		}
		
		return ESP8266_RSP_UNKNOWN;
	}
	
	return rsp;
}

// setMode()
// Input: 1, 2, 3 (ESP8266_MODE_STA, ESP8266_MODE_AP, ESP8266_MODE_STAAP)
int16_t ESP8266Class::setMode(esp8266_wifi_mode mode)
{
	char modeChar[2] = {0, 0};
	sprintf(modeChar, "%d", mode);
	sendCommand(ESP8266_WIFI_MODE, ESP8266_CMD_SETUP, modeChar);
	
	return readForResponse(RESPONSE_OK, COMMAND_RESPONSE_TIMEOUT);
}

int16_t ESP8266Class::connect(const char * ssid)
{
	connect(ssid, "");
}

// connect to the wifi
// Input: ssid and pwd const char's

int16_t ESP8266Class::connect(const char * ssid, const char * pwd)
{
	ESP8266Serial.print("AT");
	ESP8266Serial.print(ESP8266_CONNECT_AP);
	ESP8266Serial.print("=\"");
	ESP8266Serial.print(ssid);
	ESP8266Serial.print("\"");
	if (pwd != NULL)
	{
		ESP8266Serial.print(',');
		ESP8266Serial.print("\"");
		ESP8266Serial.print(pwd);
		ESP8266Serial.print("\"");
	}
	ESP8266Serial.print("\r\n");
	
	return readForResponses(RESPONSE_OK, RESPONSE_FAIL, WIFI_CONNECT_TIMEOUT);
}

int16_t ESP8266Class::getAP(char * ssid)
{
	sendCommand(ESP8266_CONNECT_AP, ESP8266_CMD_QUERY); // Send "AT+CWJAP?"
	
	int16_t rsp = readForResponse(RESPONSE_OK, COMMAND_RESPONSE_TIMEOUT);
	// Example Responses: No AP\r\n\r\nOK\r\n
	// - or -
	// +CWJAP:"WiFiSSID","00:aa:bb:cc:dd:ee",6,-45\r\n\r\nOK\r\n
	if (rsp > 0)
	{
		// Look for "No AP"
		if (strstr(esp8266RxBuffer, "No AP") != NULL)
			return 0;
		
		// Look for "+CWJAP"
		char * p = strstr(esp8266RxBuffer, ESP8266_CONNECT_AP);
		if (p != NULL)
		{
			p += strlen(ESP8266_CONNECT_AP) + 2;
			char * q = strchr(p, '"');
			if (q == NULL) return ESP8266_RSP_UNKNOWN;
			strncpy(ssid, p, q-p); // Copy string to temp char array:
			return 1;
		}
	}
	
	return rsp;
}

int16_t ESP8266Class::disconnect()
{
	sendCommand(ESP8266_DISCONNECT); // Send AT+CWQAP
	// Example response: \r\n\r\nOK\r\nWIFI DISCONNECT\r\n
	// "WIFI DISCONNECT" comes up to 500ms _after_ OK. 
	int16_t rsp = readForResponse(RESPONSE_OK, COMMAND_RESPONSE_TIMEOUT);
	if (rsp > 0)
	{
		rsp = readForResponse("WIFI DISCONNECT", COMMAND_RESPONSE_TIMEOUT);
		if (rsp > 0)
			return rsp;
		return 1;
	}
	
	return rsp;
}


IPAddress ESP8266Class::localIP()
{
	sendCommand(ESP8266_GET_LOCAL_IP); // Send AT+CIFSR\r\n
	// Example Response: +CIFSR:STAIP,"192.168.0.114"\r\n
	//                   +CIFSR:STAMAC,"18:fe:34:9d:b7:d9"\r\n
	int16_t rsp = readForResponse(RESPONSE_OK, COMMAND_RESPONSE_TIMEOUT);
	if (rsp > 0)
	{
		// Look for "STAIP" in the rxBuffer
		char * p = strstr(esp8266RxBuffer, "STAIP");
		if (p != NULL)
		{
			IPAddress returnIP;
			
			p += 7; // Move p seven places. (skip STAIP,")
			for (uint8_t i = 0; i < 4; i++)
			{
				char tempOctet[4];
				memset(tempOctet, 0, 4); 
				
				size_t octetLength = strspn(p, "0123456789"); // Find length of numerical string:
				if (octetLength >= 4) // If it's too big, return an error
					return ESP8266_RSP_UNKNOWN;
				
				strncpy(tempOctet, p, octetLength); // Copy string to temp char array:
				returnIP[i] = atoi(tempOctet); 
				
				p += (octetLength + 1); 
			}
			
			return returnIP;
		}
	}
	
	return rsp;
}



int16_t ESP8266Class::ping(IPAddress ip)
{
	char ipStr[17];
	sprintf(ipStr, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
	return ping(ipStr);
}

int16_t ESP8266Class::ping(char * server)
{
	char params[strlen(server) + 3];
	sprintf(params, "\"%s\"", server);
	// Send AT+Ping=<server>
	sendCommand(ESP8266_PING, ESP8266_CMD_SETUP, params); 
	int16_t rsp = readForResponses(RESPONSE_OK, RESPONSE_ERROR, COMMAND_PING_TIMEOUT);
	if (rsp > 0)
	{
		char * p = searchBuffer("+");
		p += 1; // Move p forward 1 space
		char * q = strchr(p, '\r'); // Find the first \r
		if (q == NULL)
			return ESP8266_RSP_UNKNOWN;
		char tempRsp[10];
		strncpy(tempRsp, p, q - p);
		return atoi(tempRsp);
	}
	else
	{
		if (searchBuffer("timeout") != NULL)
			return 0;
	}
	
	return rsp;
}


//////////////////////////////
// Stream Virtual Functions //
//////////////////////////////

size_t ESP8266Class::write(uint8_t c)
{
	ESP8266Serial.write(c);
}

int ESP8266Class::available()
{
	return ESP8266Serial.available();
}

int ESP8266Class::read()
{
	return ESP8266Serial.read();
}

int ESP8266Class::peek()
{
	return ESP8266Serial.peek();
}

void ESP8266Class::flush()
{
	ESP8266Serial.flush();
}


void ESP8266Class::sendCommand(const char * cmd, enum esp8266_command_type type, const char * params)
{
	ESP8266Serial.print("AT");
	ESP8266Serial.print(cmd);
	if (type == ESP8266_CMD_QUERY)
		ESP8266Serial.print('?');
	else if (type == ESP8266_CMD_SETUP)
	{
		ESP8266Serial.print("=");
		ESP8266Serial.print(params);		
	}
	ESP8266Serial.print("\r\n");
}

int16_t ESP8266Class::readForResponse(const char * rsp, unsigned int timeout)
{
	unsigned long timeIn = millis();	
	unsigned int received = 0; 
	
	clearBuffer();	// Clear the class receive buffer (esp8266RxBuffer)
	while (timeIn + timeout > millis()) 
	{
		if (ESP8266Serial.available()) // If data is available on UART RX
		{
			received += readByteToBuffer();
			if (searchBuffer(rsp))	// Search the buffer for goodRsp
				return received;	// Return how number of chars read
		}
	}
	
	if (received > 0) // If we received any characters
		return ESP8266_RSP_UNKNOWN; // Return unkown response error code
	else 
		return ESP8266_RSP_TIMEOUT; 
}

int16_t ESP8266Class::readForResponses(const char * pass, const char * fail, unsigned int timeout)
{
	unsigned long timeIn = millis();	// Timestamp coming into function
	unsigned int received = 0; 
	
	clearBuffer();	// Clear the class receive buffer (esp8266RxBuffer)
	while (timeIn + timeout > millis()) 
	{
		if (ESP8266Serial.available()) // If data is available on UART RX
		{
			received += readByteToBuffer();
			if (searchBuffer(pass))	// Search the buffer for goodRsp
				return received;	// Return how number of chars read
			if (searchBuffer(fail))
				return ESP8266_RSP_FAIL;
		}
	}
	
	if (received > 0) // If we received any characters
		return ESP8266_RSP_UNKNOWN; 
	else 
		return ESP8266_RSP_TIMEOUT; // Return the timeout error code
}


// Buffer Stuff //

void ESP8266Class::clearBuffer()
{
	memset(esp8266RxBuffer, '\0', ESP8266_RX_BUFFER_LEN);
	bufferHead = 0;
}	

unsigned int ESP8266Class::readByteToBuffer()
{
	// Read the data in
	char c = ESP8266Serial.read();
	
	// Store the data in the buffer
	esp8266RxBuffer[bufferHead] = c;
	bufferHead = (bufferHead + 1) % ESP8266_RX_BUFFER_LEN;
	
	return 1;
}

char * ESP8266Class::searchBuffer(const char * test)
{
	int bufferLen = strlen((const char *)esp8266RxBuffer);
	
	if (bufferLen < ESP8266_RX_BUFFER_LEN)
		return strstr((const char *)esp8266RxBuffer, test);
	else
	{	
		int testLen = strlen(test);
		for (int i=0; i<ESP8266_RX_BUFFER_LEN; i++)
		{
			
		}
	}
}

int16_t ESP8266Class::setMux(uint8_t mux)
{
	char params[2] = {0, 0};
	params[0] = (mux > 0) ? '1' : '0';
	sendCommand(ESP8266_TCP_MULTIPLE, ESP8266_CMD_SETUP, params);
	
	return readForResponse(RESPONSE_OK, COMMAND_RESPONSE_TIMEOUT);
}

