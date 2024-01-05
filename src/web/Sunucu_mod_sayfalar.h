#include <web/Kurulum_mod_sayfalar.h>

void Sunucu_mod_sayfalar()
{
    Serial.println("[Yerel Sunucu]: İnternet bağlantısı başlatılıyor.");

    //////////////Sayfa güncelleme scripleri için gerekli fonksiyonlar     ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

    server.on("/getCPUTemperature", HTTP_GET, [](AsyncWebServerRequest *request)
              {
#if defined(ESP32)
        float temperature = ((temprature_sens_read() - 32) / 1.8);
#else
        String temperature ="Desteklenmiyor.";
#endif
        request->send(200, "text/plain", String(temperature)); });

    server.on("/getRAMUsage", HTTP_GET, [](AsyncWebServerRequest *request)
              {
#if defined(ESP32)
                int RAM = (ESP.getHeapSize()/1024);
#elif defined(ESP8266)
                int RAM =80;
#endif
                int RAMFree = (ESP.getFreeHeap()/1024);
                float  RAMUsage = RAM - RAMFree;
                float  RAMOran = ((RAMUsage / RAM) * 100);
                String RAMMesaj = String(RAM) +  " Kb / % " + String(RAMOran);
                request->send(200, "text/plain", RAMMesaj); });

    server.on("/getROMUsage", HTTP_GET, [](AsyncWebServerRequest *request)
              {
#if defined(ESP32)
                int ROM = 1280; //  KB
#elif defined(ESP8266)
                int ROM = 1024; //  KB
#endif
                //int ROM = (ESP.getFlashChipSize()/1024)/4;
                float ROMUsage = (ESP.getSketchSize()/1024);
                //float  ROMFree = ROM - ROMUsage;
                float  ROMOran = ((ROMUsage/ROM)* 100);
                String ROMMesaj = String(ROM) +  " Kb / % " + String(ROMOran);
                request->send(200, "text/plain", ROMMesaj); });

    server.on("/updateOtoSayacStatus", HTTP_GET, [](AsyncWebServerRequest *request)
              { 
                    String data;
                    if ((Kapi_Durum == 6 or Kapi_Durum == 5 or Kapi_Durum == 2) and Oto_Algoritma_Sayac > ((Kapi_Oto_Kapanma_Sure * 60) - 1))
                    {
                        data = String(Oto_Algoritma_Sayac) + " sn " + String("(Oto Mod Kapalı)");
                    }
                    else
                    {
                        data = String(Oto_Algoritma_Sayac) +" sn";
                    }
                    request->send(200, "text/plain", data); });

    server.on("/updateDoorStatus", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", Kapi_Durum_String); });

    server.on("/updateWorkingTime", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", Sistem_Calisma_Zamani); });

    server.on("/updateminSwitchStatus", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", String(digitalRead(IN_1_SW_MIN_35))); });

    server.on("/updatemaxSwitchStatus", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", String(digitalRead(IN_2_SW_MAX_34))); });

    server.on("/updateobjdetectStatus", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", String(digitalRead(IN_3_Cisim_39))); });

    //////////////Kısa Linkler, sadece veri gonderiliyor.      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

    server.on("/kapiac", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  Serial.println();
                  Serial.println("[BILGI]: Web üzerinden Kapı Açılıyor.");
                  Kapi_Komut = 1;
                  request->send(200); // Başarılı bir istek durumunda 200 kodunu gönderir
              });

    server.on("/kapikapat", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  Serial.println();
                  Serial.println("[BILGI]: Web üzerinden Kapı Kapatılıyor");
                  Kapi_Komut = 2;
                  request->send(200); // Başarılı bir istek durumunda 200 kodunu gönderir
              });

    server.on("/isikac", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  Serial.println();
                  Serial.println("[BILGI]: Web üzerinden Aydınlatma Devreye Alındı.");
                  digitalWrite(ROLE_1_ISIK, HIGH); // ISIK_PIN yerine uygun pin numarasını belirtin
                  Isik_Durumu = 3;
                  request->send(200); // Başarılı bir istek durumunda 200 kodunu gönderir
              });

    server.on("/isikkapat", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  Serial.println();
                  Serial.println("[BILGI]: Web üzerinden Aydınlatma Devre Dışı Bırakıldı.");
                  digitalWrite(ROLE_1_ISIK, LOW); // ISIK_PIN yerine uygun pin numarasını belirtin
                  Isik_Durumu = 0;
                  request->send(200); // Başarılı bir istek durumunda 200 kodunu gönderir
              });
    server.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  Serial.println();
                  Serial.println("[BILGI]: Sistem yeniden başlatılıyor..");
                  Serial.println();
                  Serial.println();
                  Serial.println();

                  Serial.flush();
                  ESP.restart();
                  request->send(200); // Başarılı bir istek durumunda 200 kodunu gönderir
              });

    //////////////main      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { String s = generateMainPageForm();
                request->send(200, "text/html", s); });

    //////////////update    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
    server.on(
        "/update", HTTP_POST, [](AsyncWebServerRequest *request)
        {

                // Güncelleme işlemleri
                AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", generateUpdateInProgressForm());
                request->send(response); },
        [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
        {
            static size_t totalSize = 0;

            if (!index)
            {
                Update_Mod = 1;

                totalSize = request->contentLength();
                Serial.println();
                Serial.println("[Bilgi]: Güncelleme başladı.");
                Serial.print("[Bilgi]: Dosya Boyutu: ");

                if (totalSize > 1023 and totalSize < 1048577)
                {
                    Serial.print(totalSize / 1024);
                    Serial.println(" kb");
                }
                else if (totalSize > 1048576)
                {
                    Serial.print(totalSize / 1024);
                    Serial.println(" mb");
                }
                Serial.printf("[Bilgi]: Dosya Boyutu: %d byte\n", totalSize);
                int cmd = (filename.indexOf("spiffs") > -1) ? U_PART : U_FLASH;
#ifdef ESP32
                if (!Update.begin(UPDATE_SIZE_UNKNOWN))
                {
                    Update.printError(Serial);
                }

#elif defined(ESP8266)
                Update.runAsync(true);
                if (!Update.begin(totalSize, cmd))
                {
                    Update.printError(Serial);
                }
#endif
            }

            if (Update.write(data, len) != len)
            {
                Update.printError(Serial);
            }
            else
            {
                Serial.printf("Durum:  %d%%\n", (Update.progress() * 100) / Update.size());
            }

            if (final)
            {
                if (Update.end(true))
                {
                    Serial.println();
                    Serial.println("[Bilgi]: Güncelleme tamamlandı!");

                    // Güncelleme başarılı olduysa başarı sayfası göster
                    AsyncWebServerResponse *response = request->beginResponse(200, "text/html", generateUpdateSuccessForm());
                    request->send(response);
                    Serial.println("[Bilgi]: Cihaz Baştan Başlatılıyor..");
                    Serial.print("[Bilgi]: 3..");
                    delay(1000); // 1 saniye bekle
                    Serial.print(" 2..");
                    delay(1000); // 1 saniye bekle
                    Serial.println(" 1.. ");
                    delay(1000); // 1 saniye bekle
                    Serial.println();
                    Serial.println("[Bilgi]: Görüşürüz bb .ss :-) ");
                    Serial.println();
                    Serial.println();
                    Serial.flush();

                    Update_Mod = 2;
                    lastTime_Update = millis();
                }
                else
                {
                    Serial.println();
                    Serial.println("[HATA]: Güncelleme başarısız!");
                    Update.printError(Serial);
                    Update_Mod = 0;
                    AsyncWebServerResponse *response = request->beginResponse(200, "text/html", generateUpdateFailForm(String(Update.getError())));
                    request->send(response);
                }
            }
        });

    /////////////reset      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        for (int i = 0; i < last_eeprom; ++i) {
            EEPROM.write(i, 0);
        }
        EEPROM.commit();
        Serial.println("");
        Serial.println("[Bilgi]: Ayarlar Sıfırlandı. Yeniden yapılandırmak için cihazı yeniden başlatın. ");
        Serial.println("");

        String s = generateResetForm();
                request->send(200, "text/html", s); });
}