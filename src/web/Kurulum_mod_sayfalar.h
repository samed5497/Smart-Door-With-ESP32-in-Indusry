#include <web/Web_Server_Fonksiyonlar.h>

void Kurulum_mod_sayfalar()
{

  Serial.println("[Yerel Sunucu]: Bağlantı başlatılıyor.");
  //////////////settings  //////////////////////////////////////////////////////////////////////////////////////////////////////////////

  server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request)
            {
        String s = generateSettingsForm();
        
        request->send(200, "text/html", s); });

  /////////////setap      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  server.on("/setap", HTTP_POST, [](AsyncWebServerRequest *request)
            {
    Serial.println("[HAFIZA]: Eeprom Sıfırlanıyor. ");

    for (int i = 0; i < last_eeprom; ++i) {
      EEPROM.write(i, 0);
    }

    if (request->hasParam("ssid_STA", true) &&
        request->hasParam("pass_STA", true) /*&&
        request->hasParam("APP_KEY", true) &&
        request->hasParam("APP_SECRET", true) &&
        request->hasParam("DEVICE_ID", true)*/
    )
    {
      String ssid_STA = request->getParam("ssid_STA", true)->value();
      String pass_STA = request->getParam("pass_STA", true)->value();
      String APP_KEY = request->getParam("APP_KEY", true)->value();
      String APP_SECRET = request->getParam("APP_SECRET", true)->value();
      String DEVICE_ID = request->getParam("DEVICE_ID", true)->value();

      Serial.print("SSID: ");
      Serial.println(ssid_STA);
      Serial.print("Şifre: ");
      Serial.println(pass_STA);
      Serial.println("Ağ bilgileri kaydediliyor...");

      // EEPROM'a yaz
      int ssidLen = ssid_STA.length();
      int passLen = pass_STA.length();
      int APP_KEYLen = APP_KEY.length();
      int APP_SECRETLen = APP_SECRET.length();
      int DEVICE_IDLen = DEVICE_ID.length();

      for (int i = 0; i < ssidLen; ++i) {
        EEPROM.write(i, ssid_STA[i]);
      }

      for (int i = 0; i < passLen; ++i) {
        EEPROM.write(32 + i, pass_STA[i]);
      }

      for (int i = 0; i < APP_KEYLen; ++i) {
        EEPROM.write(96 + i, APP_KEY[i]);
      }

      for (int i = 0; i < APP_SECRETLen; ++i) {
        EEPROM.write(140 + i, APP_SECRET[i]);
      }

      for (int i = 0; i < DEVICE_IDLen; ++i) {
        EEPROM.write(250 + i, DEVICE_ID[i]);
      }

      EEPROM.commit();
      Serial.println("Kayıt işlemi başarılı!");

      String s = generateSettingsFinishForm();
                     request->send(200, "text/html", s);
      ESP.restart();
    } 
    else 
    {
      request->send(400, "text/plain", "Eksik Bilgiler Mevcut!");
    } });

  /////////////onNotFound   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  server.onNotFound([](AsyncWebServerRequest *request)
                    {
    String s = generateFirstPageForm();
    request->send(200, "text/html", s); });
}