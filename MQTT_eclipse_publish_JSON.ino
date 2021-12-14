/*
   __   __  _______  _______  _______      _           ___  _______  _______  __    _ 
  |  |_|  ||       ||       ||       |   _| |_        |   ||       ||       ||  |  | |
  |       ||   _   ||_     _||_     _|  |_   _|       |   ||  _____||   _   ||   |_| |
  |       ||  | |  |  |   |    |   |      |_|         |   || |_____ |  | |  ||       |
  |       ||  |_|  |  |   |    |   |               ___|   ||_____  ||  |_|  ||  _    |
  | ||_|| ||      |   |   |    |   |              |       | _____| ||       || | |   |
  |_|   |_||____||_|  |___|    |___|              |_______||_______||_______||_|  |__|

 Name:     ESP8266_MQTT_eclipse_publish_JSON 
 Date:     10th DEC 2021
 Author:   Flavio L Puhl Jr <flavio_puhl@hotmail.com> 
 GIT:      https://github.com/flaviopuhl/MQTT-JSON
 About:    MQTT communication example using JSON payload and eclipse MQTT cloud broker

Update comments                                      
+-----------------------------------------------------+------------------+---------------+
|               Feature added                         |     Version      |      Date     |
+-----------------------------------------------------+------------------+---------------+
| Initial Release                                     |      1.0.0       |   10/DEC/21   |
|                                                     |                  |               |
|                                                     |                  |               |
+-----------------------------------------------------+------------------+---------------+


Library versions                                       
+-----------------------------------------+------------------+-------------------------- +
|       Library                           |     Version      |          Creator          |
+-----------------------------------------+------------------+-------------------------- +
| #include <ESP8266WiFi.h>                |      1.2.7       |     Arduin Community      |
| #include <PubSubClient.h>               |      2.8.0       |     Nick O'Leary          |
| #include <ArduinoJson.h>                |      6.18.0      |     Benoît Blanchon       |
| #include <NTPClient.h>                  |      3.1.0       |     Fabrice Weinberg      |
| #include <WiFiUdp.h>                    |                  |                           |
+-----------------------------------------+------------------+-------------------------- +

Upload settings 
+----------------------------------------------------------------------------------------+
| IDE:  Arduino IDE                                                                      |
| Board: Wemos D1 R1 (ESP8266)                                                           |
| CPU freq: 80MHz                                                                        |
|                                                                                        |
|                                                                                        |
+----------------------------------------------------------------------------------------+
*/

/*+--------------------------------------------------------------------------------------+
 *| Libraries                                                                            |
 *+--------------------------------------------------------------------------------------+ */
// Libraries built into IDE
#ifdef ESP8266
  #include <ESP8266WiFi.h>
#else
  #include <WiFi.h>
#endif
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

/*+--------------------------------------------------------------------------------------+
 *| Constants declaration                                                                |
 *+--------------------------------------------------------------------------------------+ */
 
const char *ssid =  "xxxxxx";                 // name of your WiFi network
const char *password =  "xxxxxx";                   // password of the WiFi network

const char *ID = "xxxxxx";                 // Name of our device, must be unique
const char *TOPIC = "xxxxxx/data";                   // Topic to subcribe to
const char* BROKER_MQTT = "mqtt.eclipseprojects.io";  // MQTT Cloud Broker URL

String swversion = __FILE__;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

WiFiClient wclient;
PubSubClient client(wclient);                         // Setup MQTT client

/*+--------------------------------------------------------------------------------------+
 *| Connect to WiFi network                                                              |
 *+--------------------------------------------------------------------------------------+ */
void setup_wifi() {
  Serial.print("\nConnecting to ");
  Serial.println(ssid);
    WiFi.mode(WIFI_STA);                              // Setup ESP in client mode
    WiFi.setSleepMode(WIFI_NONE_SLEEP);
    WiFi.begin(ssid, password);                       // Connect to network

    int wait_passes = 0;
    while (WiFi.status() != WL_CONNECTED) {           // Wait for connection
      delay(500);
      Serial.print(".");
      if (++wait_passes >= 20) { ESP.restart(); }     // Restart in case of no wifi connection   
    }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

}

 
/*+--------------------------------------------------------------------------------------+
 *| Reconnect to MQTT client                                                             |
 *+--------------------------------------------------------------------------------------+ */
 
void reconnect() {
  
  while (!client.connected()) {                       // Loop until we're reconnected
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(ID)) {
      Serial.println("connected");
      Serial.print("Publishing to: ");
      Serial.println(TOPIC);
      Serial.println('\n');

    } else {
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
      setup_wifi();
    }
  }
}

/*+--------------------------------------------------------------------------------------+
 *| Get Date & Time                                                                      |
 *+--------------------------------------------------------------------------------------+ */
 
String DateAndTime(){

    timeClient.setTimeOffset(-10800);                       // Set offset time in seconds to adjust for your timezone, for example:
                                                            // GMT +1 = 3600
                                                            // GMT +8 = 28800
                                                            // GMT -1 = -3600
                                                            // GMT 0 = 0
    while(!timeClient.update()) {
      timeClient.forceUpdate();
    }

  time_t epochTime = timeClient.getEpochTime();              // The time_t type is just an integer. 
                                                             //It is the number of seconds since the Epoch.
  struct tm * tm = localtime(&epochTime);
  char dts[22];
    strftime(dts, sizeof(dts), "%d%b%Y %H-%M-%S", tm);       // https://www.cplusplus.com/reference/ctime/strftime/
  
  return dts;
 
}

/*+--------------------------------------------------------------------------------------+
 *| Setup                                                                                |
 *+--------------------------------------------------------------------------------------+ */
 
void setup() {
  Serial.begin(115200);                               // Start serial communication at 115200 baud
    delay(100);
  
  swversion = (swversion.substring((swversion.indexOf(".")), (swversion.lastIndexOf("\\")) + 1));   
   Serial.print("SW version: ");
   Serial.println(swversion);
     
  setup_wifi();                                       // Connect to network
    client.setServer(BROKER_MQTT, 1883);              // MQTT port, unsecure

  timeClient.begin();                                 // Initialize a NTPClient to get time
  
}


/*+--------------------------------------------------------------------------------------+
 *| main loop                                                                            |
 *+--------------------------------------------------------------------------------------+ */
 
void loop() {
  if (!client.connected())                            // Reconnect if connection to MQTT is lost
  {    reconnect();      }

  client.loop();                                      // MQTT

  // JSON serialization
  char buffer[256];
  
    StaticJsonDocument<256> doc;                      // See ArduinoJson Assistant V6
    
      doc["Device"] = "Seriema";
      doc["Version"] = swversion;
      doc["RSSI"] = WiFi.RSSI();
      doc["IP"] = WiFi.localIP();
      doc["LastRoll"] = DateAndTime();
      doc["Temp"] = 21.5;
    
    serializeJson(doc, buffer);
      Serial.println("JSON Payload:");
    serializeJsonPretty(doc, Serial);                 // Print JSON payload on Serial port         
      Serial.println("");
                         
      Serial.println("Sending message to MQTT topic");
    client.publish(TOPIC, buffer);                    // Publish data to MQTT Broker
      Serial.println("");

      delay(1000);

  }
