#include <functions/Temel_Fonksiyonlar.h>

void Seriport_Rapor() // sure = ms
{
    Durum_Guncellemeleri();

    if (millis() - lastTime_DONGU > (1000*seriport_sure) and Update_Mod == 0 and SeriPort_Durum == 1)
    {
        Sistem_Saat_Guncelleme();
        // Her türlü ekrana yazılacak bilgiler.

        Serial.print(" ~ Kapı Durumu        = ");
        Serial.println(Kapi_Durum_String);

        Serial.print(" ~ Oto Kapanma Sayac  = ");
        if (Oto_Mod_Durum)
        {
            Serial.println(Oto_Algoritma_Sayac);
        }
        else
        {
            Serial.println("Devre Dışı");
        }

        Serial.print(" ~ INPUT 4 Durumu     = ");
        Serial.println(IN4_deger_String);

        Serial.println();

        if (Kurulum_Modu == true) // Kurulum modu devredeyse ekrana yazılacak bilgiler.

        {
            Serial.println("[HAFIZA]: Cihaz kurulum modunda. Lütfen Wi-Fi kalibrasyonu yapınız. ");
            Serial.print(" ~ AP Mod Wi-Fi Adı   = ");
            Serial.println(ssid_AP);
            Serial.print(" ~ AP Mod Şifre       = ");
            Serial.println(pass_AP);
        }

        if (Kurulum_Modu == false) // Kurulum mod devre dışıysa ekrana yazılacak bilgiler.

        {
            Serial.print(" ~ Bağlı Wi-Fi Ağı    = ");
            Serial.println(ssid_STA);

            Serial.print(" ~ IP Adresi          = ");
            Serial.println(WiFi.localIP().toString());
        }

        // Her türlü ekrana yazılacak bilgiler2.

        Serial.print(" ~ MAC Adresi         = ");
        Serial.println(WiFi.macAddress());
#if defined(ESP32)
        Serial.print(" ~ CPU Sıcaklığı      = ");
        Serial.print((temprature_sens_read() - 32) / 1.8);
        Serial.println(" C");
#endif
        Serial.println();
        Serial.print(" ~ Min Switch         = ");
        Serial.println(digitalRead(IN_1_SW_MIN_35));
        Serial.print(" ~ Max Switch         = ");
        Serial.println(digitalRead(IN_2_SW_MAX_34));
        Serial.print(" ~ Cisim Sensoru      = ");
        Serial.println(digitalRead(IN_3_Cisim_39));
        Serial.println();

        lastTime_DONGU = millis();
    }

    if (digitalRead(IN_BUTON_0) == LOW and SeriPort_Durum == 0)
    {
        Serial.begin(115200);
        Serial.println("");
        Serial.println("[BILGI]: Seri Port Tekrardan Devreye Alındı.");
        Serial.println("");
        SeriPort_Durum = 1;
        while (digitalRead(IN_BUTON_0) == LOW)
        {
        } // Butonun serbest bırakılmasını bekle
    }
    if (digitalRead(IN_BUTON_0) == LOW and SeriPort_Durum == 1)
    {
        Serial.println("");
        Serial.println("[BILGI]: Seri Port Devre Dışı Bırakılıyor..");
        Serial.println("");
        Serial.flush();
        Serial.end();
        SeriPort_Durum = 0;
        while (digitalRead(IN_BUTON_0) == LOW)
        {
        } // Butonun serbest bırakılmasını bekle
    }
}

void Kapi_Fonksiyonlari() // Kapı açma fonk calısırken
{

    if (Kapi_Komut == 1 or Kapi_Durum == 7) // cihaz ilk açılışta komut alırsa her türlü açılacak.
    {
        if ((digitalRead(IN_1_SW_MIN_35) == HIGH and digitalRead(IN_2_SW_MAX_34) == HIGH) or (digitalRead(IN_1_SW_MIN_35) == LOW and digitalRead(IN_2_SW_MAX_34) == HIGH))
        {
            if (SeriPort_Durum == 1)
            {
                Serial.println("[BILGI]: Kapı Açılıyor, Aydınlatma Devreye Alınıyor..");
            }
            lasttime_Buzzer = 0;

            while ((digitalRead(IN_1_SW_MIN_35) == HIGH and digitalRead(IN_2_SW_MAX_34) == HIGH) or (digitalRead(IN_1_SW_MIN_35) == LOW and digitalRead(IN_2_SW_MAX_34) == HIGH)) // max limit swici tetiklenene kadar motoru çalıştır.
            {
                digitalWrite(ROLE_1_ISIK, HIGH);
                if (Isik_Durumu != 3)
                {
                    Isik_Durumu = 1; // Isik_Durumu devrede
                }
                lasttime_ISIK = millis();
                digitalWrite(M1_OUT, HIGH); // kapıyı acmak için motoru çalıştır.
                digitalWrite(M2_OUT, LOW);
                Kapi_Durum = 4;

                Buzzer_Ton();

                if (Kapi_Komut == 2) // eger webden komut gelirse dur
                {
                    Manuel_KOMUT = 1;
                    RF_KOMUT = 1;
                    lasttime_ManuelButon = millis();
                    lasttime_RF = millis();
                    digitalWrite(ROLE_2_Buzzer, LOW);
                    lasttime_Buzzer = 0;

                    if (SeriPort_Durum == 1)
                    {
                        Serial.println();
                        Serial.println("***[HATA]: Webden Müdehale Edildi. Kapı Kapanamadı!");
                        Serial.println();
                    }

                    Kapi_Komut = 0;
                    Kapi_Durum = 5;
                    Motor_Dur(); // dur ve döngüden çık.
                    break;
                }

                if (digitalRead(IN_5_RF_23) == HIGH and RF_KOMUT == 0) // eger RF ten komut gelirse dur
                {
                    Manuel_KOMUT = 1;
                    RF_KOMUT = 1;
                    lasttime_ManuelButon = millis();
                    lasttime_RF = millis();
                    digitalWrite(ROLE_2_Buzzer, LOW);
                    lasttime_Buzzer = 0;

                    if (SeriPort_Durum == 1)
                    {
                        Serial.println();
                        Serial.println("***[HATA]: RF ile  Müdehale Edildi. Kapı Kapanamadı!");
                        Serial.println();
                    }

                    Kapi_Komut = 0;
                    Kapi_Durum = 5;
                    Motor_Dur();
                    break;
                }

                if (digitalRead(IN_BUTON_16) == HIGH and Manuel_KOMUT == 0) // eger butondan komut gelirse dur
                {
                    Manuel_KOMUT = 1;
                    RF_KOMUT = 1;
                    lasttime_ManuelButon = millis();
                    lasttime_RF = millis();
                    digitalWrite(ROLE_2_Buzzer, LOW);
                    lasttime_Buzzer = 0;

                    if (SeriPort_Durum == 1)
                    {
                        Serial.println();
                        Serial.println("***[HATA]: Manuel Buton Müdehale Edildi. Kapı Kapanamadı!");
                        Serial.println();
                    }

                    Kapi_Komut = 0;
                    Kapi_Durum = 5;
                    Motor_Dur();
                    while (digitalRead(IN_BUTON_16) == HIGH)
                    {
                    } // Butonun serbest bırakılmasını bekle
                    break;
                }

                if (digitalRead(IN_2_SW_MAX_34) == LOW and digitalRead(IN_1_SW_MIN_35) == LOW)
                {
                    digitalWrite(ROLE_2_Buzzer, LOW);
                    lasttime_Buzzer = 0;

                    if (SeriPort_Durum == 1)
                    {
                        Serial.println("[BILGI]: Kapı Açık Durumda.");
                    }
                    Oto_Algoritma_Sayac = Kapi_Oto_Kapanma_Sure * 60;
                    Kapi_Komut = 0;
                    Kapi_Durum = 6;
                    Motor_Dur();
                    break;
                }
                Zaman_Guncellemesi();
            }
        }
        else
        {
            if (SeriPort_Durum == 1)
            {
                Serial.println("[BILGI]: Kapı Zaten Açık Durumda.");
            }
            Oto_Algoritma_Sayac = Kapi_Oto_Kapanma_Sure * 60;
            Kapi_Komut = 0;
            Kapi_Durum = 6;
            Motor_Dur();
        }
    }

    if (Kapi_Komut == 2)
    {
        if ((digitalRead(IN_1_SW_MIN_35) == HIGH and digitalRead(IN_2_SW_MAX_34) == HIGH) or (digitalRead(IN_1_SW_MIN_35) == LOW and digitalRead(IN_2_SW_MAX_34) == LOW))
        {
            if (SeriPort_Durum == 1)
            {
                Serial.println("[BILGI]: Kapı Kapatılıyor, Aydınlatma Devreye Alınıyor..");
            }
            lasttime_Buzzer = 0;

            while ((digitalRead(IN_1_SW_MIN_35) == HIGH and digitalRead(IN_2_SW_MAX_34) == HIGH) or (digitalRead(IN_1_SW_MIN_35) == LOW and digitalRead(IN_2_SW_MAX_34) == LOW)) // max limit swici tetiklenene kadar motoru çalıştır.
            {
                digitalWrite(ROLE_1_ISIK, HIGH);
                if (Isik_Durumu != 3)
                {
                    Isik_Durumu = 1; // Isik_Durumu devrede
                }
                lasttime_ISIK = millis();
                digitalWrite(M1_OUT, HIGH); // kapıyı acmak için motoru çalıştır.
                digitalWrite(M2_OUT, HIGH);
                Kapi_Durum = 1;

                Buzzer_Ton();

                if (digitalRead(IN_3_Cisim_39) == HIGH)
                {
                    Manuel_KOMUT = 1;
                    RF_KOMUT = 1;
                    lasttime_ManuelButon = millis();
                    lasttime_RF = millis();
                    digitalWrite(ROLE_2_Buzzer, LOW);
                    lasttime_Buzzer = 0;

                    if (SeriPort_Durum == 1)
                    {
                        Serial.println();
                        Serial.println("***[HATA]: Cisim Algılandı. Kapı Kapanamadı!");
                        Serial.println();
                    }

                    Kapi_Komut = 0;
                    Kapi_Durum = 2;
                    Motor_Dur(); // dur ve döngüden çık.
                    break;
                }

                if (Kapi_Komut == 1) // eger kapı çalışırken cisim araya girerse
                {
                    Manuel_KOMUT = 1;
                    RF_KOMUT = 1;
                    lasttime_ManuelButon = millis();
                    lasttime_RF = millis();
                    digitalWrite(ROLE_2_Buzzer, LOW);
                    lasttime_Buzzer = 0;

                    if (SeriPort_Durum == 1)
                    {
                        Serial.println();
                        Serial.println("***[HATA]: Manuel Olarak Iptal Edildi. Kapı Kapanamadı!");
                        Serial.println();
                    }

                    Kapi_Komut = 0;
                    Kapi_Durum = 2;
                    Motor_Dur(); // dur ve döngüden çık.
                    break;
                }

                if (digitalRead(IN_5_RF_23) == HIGH and RF_KOMUT == 0) // eger kapı çalışırken cisim araya girerse
                {
                    Manuel_KOMUT = 1;
                    RF_KOMUT = 1;
                    lasttime_ManuelButon = millis();
                    lasttime_RF = millis();
                    digitalWrite(ROLE_2_Buzzer, LOW);
                    lasttime_Buzzer = 0;

                    if (SeriPort_Durum == 1)
                    {
                        Serial.println();
                        Serial.println("***[HATA]: RF ile  Müdehale Edildi. Kapı Açılamadı!");
                        Serial.println();
                    }

                    Kapi_Komut = 0;
                    Kapi_Durum = 2;
                    Motor_Dur();
                    break;
                }
                if (digitalRead(IN_BUTON_16) == HIGH and Manuel_KOMUT == 0) // eger butondan komut gelirse dur
                {
                    Manuel_KOMUT = 1;
                    RF_KOMUT = 1;
                    lasttime_ManuelButon = millis();
                    lasttime_RF = millis();
                    digitalWrite(ROLE_2_Buzzer, LOW);
                    lasttime_Buzzer = 0;

                    if (SeriPort_Durum == 1)
                    {
                        Serial.println();
                        Serial.println("***[HATA]: Manuel Buton ile Müdehale Edildi. Kapı Açılamadı!");
                        Serial.println();
                    }

                    Kapi_Komut = 0;
                    Kapi_Durum = 2;
                    Motor_Dur();
                    while (digitalRead(IN_BUTON_16) == HIGH)
                    {
                    } // Butonun serbest bırakılmasını bekle
                    break;
                }

                if (digitalRead(IN_1_SW_MIN_35) == LOW and digitalRead(IN_2_SW_MAX_34) == HIGH)
                {
                    digitalWrite(ROLE_2_Buzzer, LOW);
                    lasttime_Buzzer = 0;

                    if (SeriPort_Durum == 1)
                    {
                        Serial.println("[BILGI]: Kapı Kapalı Durumda.");
                    }

                    Kapi_Komut = 0;
                    Kapi_Durum = 3;
                    Motor_Dur();
                    break;
                }
                Zaman_Guncellemesi();
            }
        }
        else
        {
            if (SeriPort_Durum == 1)
            {
                Serial.println("[BILGI]: Kapı Zaten Kapalı Durumda.");
            }

            Kapi_Komut = 0;
            Kapi_Durum = 3;
            Motor_Dur();
        }
    }
    else
    {
        digitalWrite(ROLE_2_Buzzer, LOW);
    }

    if (Kapi_Durum == 3 and (digitalRead(IN_1_SW_MIN_35) == HIGH and digitalRead(IN_2_SW_MAX_34) == HIGH))
    {
        Kapi_Komut = 2;
    }

    if (Kapi_Durum == 6 and (digitalRead(IN_1_SW_MIN_35) == HIGH and digitalRead(IN_2_SW_MAX_34) == LOW))
    {
        Kapi_Komut = 1;
    }
}

void RF_Kontrol() // RF'ten gelen sinyalleri kontrol eder.
{
    if (digitalRead(IN_5_RF_23) == HIGH and RF_KOMUT == 0) // RF ten sinyal gelirse ve son sinyal üzerinden 1 saniye geçmişse
    {
        RF_KOMUT = 1;
        lasttime_RF = millis();
        if (SeriPort_Durum == 1)
        {
            Serial.println();
            Serial.println("[BILGI]: RF Sinyali algılandı!");
        }

        if (Kapi_Durum == 0 or Kapi_Durum == 1 or Kapi_Durum == 2 or Kapi_Durum == 3) // kapı kapalı durumdaysa aç
        {
            Kapi_Komut = 1;
        }
        if (Kapi_Durum == 4 or Kapi_Durum == 5 or Kapi_Durum == 6) // kapı açık yada belirsiz durumdaysa kapat
        {
            Kapi_Komut = 2;
        }
    }
}

void Buton_Kontrol() // Kapıyı açma foknsiyonu
{
    if (digitalRead(IN_BUTON_16) == HIGH and Manuel_KOMUT == 0 or digitalRead(IN_4_ManuelButon_36) == HIGH and Manuel_KOMUT == 0) // RF ten sinyal gelirse ve son sinyal üzerinden 1 saniye geçmişse
    {
        Manuel_KOMUT = 1;
        lasttime_ManuelButon = millis();
        if (SeriPort_Durum == 1)
        {
            Serial.println();
            Serial.println("[BILGI]: Manuel Butona Basıldı.");
        }

        if (Kapi_Durum == 0 or Kapi_Durum == 1 or Kapi_Durum == 2 or Kapi_Durum == 3) // kapı kapalı durumdaysa aç
        {
            Kapi_Komut = 1;
        }
        if (Kapi_Durum == 4 or Kapi_Durum == 5 or Kapi_Durum == 6) // kapı açık yada belirsiz durumdaysa kapat
        {
            Kapi_Komut = 2;
        }
        while (digitalRead(IN_BUTON_16) == HIGH or digitalRead(IN_4_ManuelButon_36) == HIGH)
        {
        } // Butonun serbest bırakılmasını bekle
    }
}

void Oto_Kontrol_Algoritmasi()
{
    if (Oto_Mod_Durum)
    {
        if (Kapi_Durum == 6)
        {
            if (digitalRead(IN_3_Cisim_39) == HIGH)
            {
                if (Oto_Algoritma_Sayac <= Kapi_Oto_Kapanma_Sure * 60)
                {
                    Oto_Algoritma_Sayac = Kapi_Oto_Kapanma_Sure * 60;
                }
            }
            if (millis() - lastTime_Oto_Algoritma_Sayac >= 1000)
            {
                lastTime_Oto_Algoritma_Sayac = millis();
                if (Oto_Algoritma_Sayac > 0)
                {
                    Oto_Algoritma_Sayac--;
                }
            }
            if (Oto_Algoritma_Sayac == 0)
            {
                if (SeriPort_Durum == 1)
                {
                    Serial.println();
                    Serial.println("***[SİSTEM]: Kapı otomatik kapatılıyor..");
                    Serial.println();
                }
                Oto_Algoritma_Sayac = Kapi_Oto_Kapanma_Sure * 60;
                Kapi_Komut = 2;
            }
        }

        else if (Kapi_Durum == 5 or Kapi_Durum == 2)
        {
            if (digitalRead(IN_3_Cisim_39) == HIGH)
            {
                if (Oto_Algoritma_Sayac <= Kapi_Oto_Kapanma_Sure * 30)
                {
                    Oto_Algoritma_Sayac = Kapi_Oto_Kapanma_Sure * 30;
                }
            }
            if (millis() - lastTime_Oto_Algoritma_Sayac >= 1000)
            {
                lastTime_Oto_Algoritma_Sayac = millis();
                if (Oto_Algoritma_Sayac > 0)
                {
                    Oto_Algoritma_Sayac--;
                }
            }
            if (Oto_Algoritma_Sayac == 0)
            {
                if (SeriPort_Durum == 1)
                {
                    Serial.println();
                    Serial.println("***[SİSTEM]: Kapı otomatik kapatılıyor..");
                    Serial.println();
                }
                Oto_Algoritma_Sayac = Kapi_Oto_Kapanma_Sure * 60;
                Kapi_Komut = 2;
            }
        }
        else
        {
            Oto_Algoritma_Sayac = Kapi_Oto_Kapanma_Sure * 60;
        }
    }
    else
    {
        Oto_Algoritma_Sayac = Kapi_Oto_Kapanma_Sure * 60;
    }
}