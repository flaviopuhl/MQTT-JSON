# MQTT-JSON
Simple examples about use of MQTT + JSON on ESP8266 or ESP32

Instructions:

On PUBLISH file:
    * Under constant declarations, change the SSID and Password for your wifi network credentials
    * Change ID for a unique name that refers your device
    * Change TOPIC for something like "ID"/Data.

    /*+--------------------------------------------------------------------------------------+
     *| Constants declaration                                                                |
     *+--------------------------------------------------------------------------------------+ */

    const char *ssid =  "xxxxxx";                 // name of your WiFi network
    const char *password =  "xxxxxx";                   // password of the WiFi network

    const char *ID = "xxxxxx";                 // Name of our device, must be unique
    const char *TOPIC = "xxxxxx/data";                   // Topic to publish to
    const char* BROKER_MQTT = "mqtt.eclipseprojects.io";  // MQTT Cloud Broker URL

On SUBSCRIBE file:
    * Under constant declarations, change the SSID and Password for your wifi network credentials
    * Change ID for a unique name that refers your device, 
    * Change TOPIC to match the same topic on publish device;
    
    /*+--------------------------------------------------------------------------------------+
     *| Constants declaration                                                                |
     *+--------------------------------------------------------------------------------------+ */

    const char *ssid =  "xxxxxx";                 // name of your WiFi network
    const char *password =  "xxxxxx";                   // password of the WiFi network

    const char *ID = "xxxxxx";                 // Name of our device, must be unique
    const char *TOPIC = "xxxxxx/data";                   // Topic to subcribe to
    const char* BROKER_MQTT = "mqtt.eclipseprojects.io";  // MQTT Cloud Broker URL
    
    
The result shall be:

![image](https://user-images.githubusercontent.com/33373207/145825556-2daa3015-d764-4020-96c6-385fc83eb3d7.png)






Usefull links:
http://www.steves-internet-guide.com/arduino-sending-receiving-json-mqtt/
https://arduinojson.org/v6/how-to/use-arduinojson-with-pubsubclient/
https://www.digikey.com.br/en/maker/projects/send-and-receive-messages-to-your-iot-devices-using-mqtt/39ed5690cc46473abe8904c8f960341f
http://kio4.com/arduino/117C_Wemos_IOT_Eclipse.htm
