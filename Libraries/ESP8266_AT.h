/*******************************************************************************
ESP8266 AT Command Definitions
*****************************************************************************/



// AT command response

const char RESPONSE_OK[] = "OK\r\n";
const char RESPONSE_ERROR[] = "ERROR\r\n";
const char RESPONSE_FAIL[] = "FAIL";
const char RESPONSE_READY[] = "READY!";

const char ESP8266_TEST[] = "";	// Test AT startup
const char ESP8266_RESET[] = "+RST"; // Reset the module
const char ESP8266_VERSION[] = "+GMR"; // check version info
const char ESP8266_ECHO_ENABLE[] = "E1"; // AT  enable commands echo
const char ESP8266_ECHO_DISABLE[] = "E0"; // AT disable commands echo
const char ESP8266_UART[] = "+UART"; // UART configuration


// WiFi Functions //

const char ESP8266_WIFI_MODE[] = "+CWMODE"; // WiFi mode (sta/AP/sta+AP)
const char ESP8266_CONNECT_AP[] = "+CWJAP"; // Connect to AP
const char ESP8266_DISCONNECT[] = "+CWQAP"; // Disconnect from AP


const char ESP8266_GET_LOCAL_IP[] = "+CIFSR"; // Get local IP address
const char ESP8266_TCP_MULTIPLE[] = "+CIPMUX"; // Set multiple connections mode
const char ESP8266_TRANSMISSION_MODE[] = "+CIPMODE"; // Set transmission mode
const char ESP8266_PING[] = "+PING"; // Function Ping


