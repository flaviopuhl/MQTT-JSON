/*
   __   __  _______  _______  _______      _           ___  _______  _______  __    _ 
  |  |_|  ||       ||       ||       |   _| |_        |   ||       ||       ||  |  | |
  |       ||   _   ||_     _||_     _|  |_   _|       |   ||  _____||   _   ||   |_| |
  |       ||  | |  |  |   |    |   |      |_|         |   || |_____ |  | |  ||       |
  |       ||  |_|  |  |   |    |   |               ___|   ||_____  ||  |_|  ||  _    |
  | ||_|| ||      |   |   |    |   |              |       | _____| ||       || | |   |
  |_|   |_||____||_|  |___|    |___|              |_______||_______||_______||_|  |__|

 Name:     MQTT_eclipse_subscribe_JSON 
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

/*+--------------------------------------------------------------------------------------+
 *| Constants declaration                                                                |
 *+--------------------------------------------------------------------------------------+ */
 
const char *ssid = "xxxxxx";                  // name of your WiFi network
const char *password = "xxxxxx";                    // password of the WiFi network

const char *ID = "xxxxxx";                       // Name of our device, must be unique
const char *TOPIC = "xxxxxx/data";                   // Topic to subcribe to

const char* BROKER_MQTT = "mqtt.eclipseprojects.io";  //MQTT Cloud Broker URL

String swversion = __FILE__;

WiFiClient wclient;
PubSubClient client(wclient);                         // Setup MQTT client

/*+--------------------------------------------------------------------------------------+
 *| MQTT callback                                                                        |
 *+--------------------------------------------------------------------------------------+ */

// Handle incomming messages from the broker
void callback(char* topic, byte* payload, unsigned int length) {

  char str[length+1];
    Serial.print("Message arrived [");
      Serial.print(topic);
        Serial.print("] ");
  int i=0;
  for (i=0;i<length;i++) {
    //Serial.print((char)payload[i]);                               // print raw json data, debug only
    str[i]=(char)payload[i];
  }
  
  Serial.println();       
  str[i] = 0; // Null termination

 // JSON Deserialization
  StaticJsonDocument <256> doc;
  
  DeserializationError error = deserializeJson(doc, payload);
  
    // Test if parsing succeeds.
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      //Serial.println(error.f_str()); //https://github.com/bblanchon/ArduinoJson/issues/1525
      return;
    }
  
  String device_ = doc["Device"];
  String swversion = doc["Version"];
  float RSSI_ = doc["RSSI"];
  String IP_ = doc["IP"];
  String LastRoll = doc["LastRoll"];
  float Temp = doc["Temp"];

    Serial.println("Listening to :");
    Serial.print(" Device = "); Serial.println(device_);
    Serial.print(" Version = "); Serial.println(swversion);
    Serial.print(" RSSI = "); Serial.println(RSSI_);
    Serial.print(" IP = "); Serial.println(IP_);
    Serial.print(" LastRoll = "); Serial.println(LastRoll);
    Serial.print(" Temp = "); Serial.println(Temp);
    Serial.println();Serial.println();
    

}

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
      client.subscribe(TOPIC);                        // Subscribe to MQTT
      Serial.println("connected");
      Serial.print("Subscribing to: ");
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
    client.setCallback(callback);                     // Initialize the callback routine
}

/*+--------------------------------------------------------------------------------------+
 *| main loop                                                                            |
 *+--------------------------------------------------------------------------------------+ */
 
void loop() {
  if (!client.connected())                            // Reconnect if connection to MQTT is lost
  {    reconnect();      }

  
  client.loop();                                      // MQTT
  delay(1000);
  
  
  
}
