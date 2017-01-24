/* 

 * This is a test application to verify how the
 * ESP8266 responds to different AT commands.
 * Connect ESP8266 TX and RX to software Serial pins
 * on Arduino 6&7.
 * Modify which command will be sent on first
 * run. The output will be sent to PC one
 * byte at a time.
 */
 
#include <protoESP8266WiFiShield.h>
#include <SoftwareSerial.h>

#define ESP8266_SW_RX 7 // ESP8266 UART0 RXI goes to Arduino pin 9   //7
#define ESP8266_SW_TX 6 // ESP8266 UART0 TXO goes to Arduino pin 8   //6

SoftwareSerial ESP8266Serial1(ESP8266_SW_TX, ESP8266_SW_RX);
const char AT_RESTART[] =  "AT+RST";
const char AT_MODE[] =     "AT+CWMODE";
const char AT_JOIN_AP[] =  "AT+CWJAP";
const char AT_LIST_AP[] =  "AT+CWLAP";
const char AT_QUIT_AP[] =  "AT+CWQAP";
const char AT_AP_MODE[] =  "AT+CWSAP";
ESP8266Class esp8266;

void setup() {
  // initialize serial for PC COM
  Serial.begin(9600);
  // Initialize software serial for ESP8266 COM
  ESP8266Serial1.begin(9600);
  esp8266.test();
  
}

void loop() {
  
  
   // scan for nearby networks: send AT command
  Serial.println("** Scan Networks **");
  ESP8266Serial1.println(AT_LIST_AP);
  
  while(1){

    // if there's any serial available, read it:
    while (ESP8266Serial1.available() > 0) {
      char c=ESP8266Serial1.read();
        Serial.print(c);
    }
  }
}
