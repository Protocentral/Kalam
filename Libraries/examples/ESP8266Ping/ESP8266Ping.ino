/************************************************************
ESP8266_Ping.h
this example pinging to the domain and returns the response time
************************************************************/

#include <SoftwareSerial.h>
#include <protoESP8266WiFiShield.h>

// Replace these two character strings with the name and
const char mySSID[] = "circuitects";   // your SSID
const char myPSK[] = "open1234";       // password

char destServer[] = "protocentral.com";
ESP8266Class esp8266;


void setup() 
{
  Serial.begin(9600);       // Serial print UART
  esp8266.begin(9600);      // software UART for ESP , the default baudrate of the module is 9600, if you have burned latest firmware on ESP makesure the baudrate is 9600.
  delay(2000);
  while (esp8266.test() != true)  // Check ESP shield/Kalam is present by sending AT and wait for 'OK' to receive
  {
    Serial.println("Error connecting to ESP8266."); // exit the loop once you receive 'OK'
    Serial.println("Make sure the Jumper and Switch position on the board if still error exists reset the ESP module alone"); // exit the loop once you receive 'OK'
    delay(1000);
  }
  Serial.println("ESP8266 Sheild is present");
  delay(1000);

  Serial.println("Setting up the WiFi the connection ...");
  delay(1000);
  Serial.println("Make sure you have Entered the SSID and Password........");
  while (esp8266.connect(mySSID, myPSK) < 0)  // Connect to the network
  {
     Serial.print("Connecting to the ");
     Serial.print(mySSID );
     Serial.println(".....");
     delay(1000); 
  }
  Serial.print("Connected to ");
  Serial.println(mySSID );
  delay(1000);
  Serial.println("------------------------------------");
  Serial.print("IP adress is :");
  Serial.println(esp8266.localIP());
  Serial.println("------------------------------------");
  Serial.print("Pinging ");
  Serial.println(destServer);
}

void loop() 
{
  Serial.print("Response Time ");
  Serial.print(esp8266.ping(destServer ));
  Serial.println("ms");
  delay(1000);
}
