#include <Arduino.h>
#include <EEPROM.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <AsyncWebSocket.h>

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#include <Update.h>
#define U_PART U_SPIFFS
String CPU = "ESP32 - DualCore / ";

#ifdef __cplusplus
extern "C"
{
#endif
    uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();

#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <Updater.h>
#define U_PART U_FS
String CPU = "ESP8266 / ";

#else
#error "Bu kod yalnızca ESP32 ve ESP8266 platformlarında derlenebilir."
#endif