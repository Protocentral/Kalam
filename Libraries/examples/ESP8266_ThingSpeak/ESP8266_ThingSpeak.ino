/* 
 * This is test example uses Arduino and ESP8266 
 * to update a remote server (https://thingspeak.com/)
 * ESP8266 TX and RX connects to software Serial pins
 * on Arduino 6&7.
 * Assign the SSID and the Passkey of your network
 * View and analyze the data on Thingspeak channel 
 * https://thingspeak.com/channels/59428
 * To create a Thingspeak account and tutorials 
 * Refer https://thingspeak.com/docs/tutorials/
 */
#include<stdlib.h>
#include <SoftwareSerial.h>
#include <protoESP8266WiFiShield.h>

#define SSID "circuitects"   // your network ID
#define PASSKEY "open1234"   // Password
#define IP "184.106.153.149" // thingspeak.com

String GET = "GET /update?key=QHN8ZQHFV4HTDRX4&field1="; // GET /update?key=APIkey&field1=

float tempN1,tempN2;   
char buffer[10];
SoftwareSerial monitor(6, 7); // RX, TX

void setup()
{
  Serial.begin(9600);
  monitor.begin(9600);
  sendDebug("AT");
  delay(5000);
  if(monitor.find("OK")){
    Serial.println("RECEIVED: OK");
    connectWiFi();
  }
   Serial.println("****Example to publish the random data on Thingspeak****");
   Serial.println("*********************************************************************************");
}

void loop(){
  

  // generates a random number from 0 to 299
  tempN1 = random(300);
  // generates a random number from 10 to 19
  tempN2 = random(10, 20);

  Serial.print("Random Numbers generated are ");
  Serial.print(tempN1);Serial.print(" and "); Serial.print(tempN2);
  Serial.println(".This will be posted on to the Thingspeak ");
  
  String tempStr1 = dtostrf(tempN1, 4, 1, buffer);
  String tempStr2 = dtostrf(tempN2, 4, 1, buffer);
  updateTemp(tempStr1,tempStr2);
  delay(5000);
}

void updateTemp(String str1,String str2){
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  sendDebug(cmd);
  delay(2000);
  if(monitor.find("Error")){
    Serial.print("RECEIVED: Error");
    return;
  }
  cmd = GET;
  cmd += str1;
  cmd += "&field2=";
  cmd += str2;
  cmd += "\r\n";
  
  Serial.print("Posting Random numbers ");
  Serial.print(tempN1);Serial.print("and "); Serial.print(tempN2);
  Serial.println("  on Thingspeak ");
  monitor.print("AT+CIPSEND=");
  monitor.println(cmd.length());
  if(monitor.find(">")){
    Serial.print(">");
    Serial.print(cmd);
    monitor.print(cmd);
  }else{
    sendDebug("AT+CIPCLOSE");
  }
  if(monitor.find("OK")){
    Serial.println("RECEIVED: OK");
    Serial.println("Successfully published on Thingspeak, check https://thingspeak.com/channels/59428");
    Serial.println("**********************************************************************************");
  }else{
    Serial.println("RECEIVED: Error");
  }
}
void sendDebug(String cmd){
  Serial.print("SEND: ");
  Serial.println(cmd);
  monitor.println(cmd);
} 

 
boolean connectWiFi(){
   Serial.print("Connecting to "); Serial.println(SSID );
   monitor.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASSKEY;
  cmd+="\"";
  sendDebug(cmd);
  delay(5000);
  if(monitor.find("OK")){
    Serial.print("Connected to ");Serial.println(SSID);
    return true;
  }else{
    Serial.println("RECEIVED: Error");
    return false;
  }
    delay(2000);
}
