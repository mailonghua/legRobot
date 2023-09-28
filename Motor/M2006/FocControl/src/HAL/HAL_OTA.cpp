#include "HAL.h"
// #include <ESP32WiFi.h>
// #include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = STASSID;
const char* password = STAPSK;
String Current_IP_Address = "NUll";//当前IP地址
void HAL::OTA_Init()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        ERR("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
     }
    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
        } else {  // U_FS
        type = "filesystem";
        }

        // NOTE: if updating FS this would be the place to unmount FS using FS.end()
        INFO("Start updating %s\r\n",type.c_str());
   });
    ArduinoOTA.onEnd([]() {
        INFO("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        INFO("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        ERR("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
        ERR("Auth Failed\r\n");
        } else if (error == OTA_BEGIN_ERROR) {
        ERR("Begin Failed\r\n");
        } else if (error == OTA_CONNECT_ERROR) {
        ERR("Connect Failed\r\n");
        } else if (error == OTA_RECEIVE_ERROR) {
        ERR("Receive Failed\r\n");
        } else if (error == OTA_END_ERROR) {
        ERR("End Failed\r\n");
        }
    });
    ArduinoOTA.begin();
    INFOLN("Ready...");
    INFOLN("IP address: ");
    INFOLN(WiFi.localIP());
    Current_IP_Address = WiFi.localIP().toString();
}
void HAL::OTA_Update()
{
    ArduinoOTA.handle();
}