#include <core/Tanimlamalar.h>

boolean Baglanti_Kontrol()
{
    Serial.printf("\r\n[Wifi]: Bağlantı için bekleniyor..");

    int count = 0;
    while (count < 60)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println();
            Serial.printf("[WiFi]: Bağlantı Başarılı!\r\n[WiFi]: Yerel IP-Addresi: %s\r\n", WiFi.localIP().toString().c_str());
            Serial.printf("[WiFi]: MAC Addresi     : %s\r\n", WiFi.macAddress().c_str());

            return (true);
        }
        delay(500);
        Serial.print(".");
        count++;
    }
    Serial.println("");

    Serial.println("[WiFi]: Zaman aşımına uğrandı.");
    return false;
}

boolean Hafizadan_Yukle()
{
    Serial.println("[EEPROM]: Kayıtlı ağ okunuyor..");

    if (EEPROM.read(0) != 0)
    {
        for (int i = 0; i < 32; ++i)
        {
            char c = EEPROM.read(i);
            if (c == '\0')
            {
                break; // Null karakteri bulduğumuzda okumayı durdur
            }
            ssid_STA += c; // Karakteri String'e ekleyin
        }
        ssid_STA.trim();
        Serial.print("  SSID Adı  (");
        Serial.print(ssid_STA.length());
        Serial.print(") : ");
        Serial.println(ssid_STA);
        for (int i = 32; i < 96; ++i)
        {
            char c = EEPROM.read(i);
            if (c == '\0')
            {
                break; // Null karakteri bulduğumuzda okumayı durdur
            }
            pass_STA += c; // Karakteri String'e ekleyin
        }
        pass_STA.trim();
        Serial.print("  Şifre     (");
        Serial.print(pass_STA.length());
        Serial.print(") : ");
        Serial.println(pass_STA);

        for (int i = 96; i < 140; ++i)
        {
            char c = EEPROM.read(i);
            if (c == '\0')
            {
                break; // Null karakteri bulduğumuzda okumayı durdur
            }
            APP_KEY += c; // Karakteri String'e ekleyin
        }
        APP_KEY.trim();
        Serial.print("  APP_KEY   (");
        Serial.print(APP_KEY.length());
        Serial.print(") : ");
        Serial.println(APP_KEY);

        for (int i = 140; i < 250; ++i)
        {
            char c = EEPROM.read(i);
            if (c == '\0')
            {
                break; // Null karakteri bulduğumuzda okumayı durdur
            }
            APP_SECRET += c; // Karakteri String'e ekleyin
        }
        APP_SECRET.trim();

        Serial.print("  APP_SECRET(");
        Serial.print(APP_SECRET.length());
        Serial.print(") : ");
        Serial.println(APP_SECRET);

        for (int i = 250; i < last_eeprom; ++i)
        {
            char c = EEPROM.read(i);
            if (c == '\0')
            {
                break; // Null karakteri bulduğumuzda okumayı durdur
            }
            DEVICE_ID += c; // Karakteri String'e ekleyin
        }
        DEVICE_ID.trim();
        Serial.print("  DEVICE_ID (");
        Serial.print(DEVICE_ID.length());
        Serial.print(") : ");
        Serial.println(DEVICE_ID);
        /*
            if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
            {
              Serial.println("STA Failed to configure");
            }
        */
        WiFi.setSleep(false);
        WiFi.setAutoReconnect(true);
        WiFi.setHostname(hostname);
        WiFi.begin(ssid_STA.c_str(), pass_STA.c_str());

        return true;
    }
    else
    {
        Serial.println("[EEPROM]: Kayıtlı ağ bulunamadı!");
        return false;
    }
}

void Durum_Guncellemeleri()
{
    if (Kapi_Durum == 0)
    {
        Kapi_Durum_String = "Belirsiz Durumda";
    }
    if (Kapi_Durum == 1)
    {
        Kapi_Durum_String = "Kapı Kapanıyor..";
    }
    if (Kapi_Durum == 2)
    {
        Kapi_Durum_String = "Kapanırken Durduruldu!";
    }
    if (Kapi_Durum == 3)
    {
        Kapi_Durum_String = "Kapı Kapalı Durumda";
    }
    if (Kapi_Durum == 4)
    {
        Kapi_Durum_String = "Kapı Açılıyor..";
    }
    if (Kapi_Durum == 5)
    {
        Kapi_Durum_String = "Açılırken Durduruldu!";
    }

    if (Kapi_Durum == 6)
    {
        Kapi_Durum_String = "Kapı Açık Durumda";
    }

    if (Isik_Durumu == 0)
    {
        Isik_Durumu_String = "Kapalı";
    }
    if (Isik_Durumu == 1)
    {
        Isik_Durumu_String = "Açık";
    }
    if (Isik_Durumu == 3)
    {
        Isik_Durumu_String = "Manuel Modda Açık";
    }
    if (IN4_deger == 0)
    {
        IN4_deger_String = "Kapalı";
    }
    if (IN4_deger == 1)
    {
        IN4_deger_String = "Açık";
    }
}

void Zaman_Guncellemesi()
{

    if (Kurulum_Modu and (millis() > (60000 * AP_MOD_ResTime)))
    {
        ESP.restart(); // Resetle
    }

    
    if (millis() - lasttime_RF > RF_algilama_sure) // son rf sinyalinden bu yana X saniye geçtiyse sinyal okunabilir olsun.
    {
        RF_KOMUT = 0;
    }

    if (millis() - lasttime_ManuelButon > ManuelButon_algilama_sure) // son rf sinyalinden bu yana X saniye geçtiyse sinyal okunabilir olsun.
    {
        Manuel_KOMUT = 0;
    }

    if (millis() - lasttime_ISIK > aydinlatma_sure and Isik_Durumu != 3) // KAPI HAREKETİNDEN 30 saniye sonra ısık sonsun. manuel acıldıysa sonmesın.
    {
        digitalWrite(ROLE_1_ISIK, LOW);
        Isik_Durumu = 0;
    }

    if (millis() - lastTime_Update > 2500 and Update_Mod == 2) // Güncelleme sonrası bilgilendirmeden sonra res at.
    {
        ESP.restart(); // Resetle
    }
}

void Motor_Dur() // Motorları durdurma fonksiyonu
{
    digitalWrite(M1_OUT, LOW);
    digitalWrite(M2_OUT, LOW);
}

void Sistem_Saat_Guncelleme()
{
    gecensure_sn = lastTime_DONGU / 1000;
    gecensure_dk = lastTime_DONGU / 60000;
    gecensure_saat = lastTime_DONGU / 3600000;
    gecensure_gun = lastTime_DONGU / 86400000;
    gecensure_hafta = lastTime_DONGU / 604800000;
    Sistem_Calisma_Zamani = "";
    Serial.println();
    Serial.print("---------------------------------  ");

    if (gecensure_hafta > 0)
    {
        // Serial.print(gecensure_hafta);
        // Serial.print(" hafta ");
        Sistem_Calisma_Zamani = String(gecensure_hafta) + " hafta ";
    }
    if (gecensure_gun > 0)
    {
        if (gecensure_gun >= 7)
        {
            // Serial.print(gecensure_gun - (gecensure_hafta * 7));
            Sistem_Calisma_Zamani = Sistem_Calisma_Zamani + String(gecensure_gun - (gecensure_hafta * 7)) + " gun ";
        }
        else
        {
            // Serial.print(gecensure_saat);
            Sistem_Calisma_Zamani = Sistem_Calisma_Zamani + String(gecensure_saat) + " gun ";
        }
        // Serial.print(" gun ");
    }
    if (gecensure_saat > 0)
    {
        if (gecensure_saat >= 24)
        {
            // Serial.print(gecensure_saat - (gecensure_gun * 24));
            Sistem_Calisma_Zamani = Sistem_Calisma_Zamani + String(gecensure_saat - (gecensure_gun * 24)) + " saat ";
        }
        else
        {
            // Serial.print(gecensure_saat);
            Sistem_Calisma_Zamani = Sistem_Calisma_Zamani + String(gecensure_saat) + " saat ";
        }
        // Serial.print(" saat ");
    }

    if (gecensure_dk > 0)
    {
        if (gecensure_dk >= 60)
        {
            // Serial.print(gecensure_dk - (gecensure_saat * 60));
            Sistem_Calisma_Zamani = Sistem_Calisma_Zamani + String(gecensure_dk - (gecensure_saat * 60)) + " dk ";
        }
        else
        {
            // Serial.print(gecensure_dk);
            Sistem_Calisma_Zamani = Sistem_Calisma_Zamani + String(gecensure_dk) + " dk ";
        }
        // Serial.print(" dk ");
    }

    if (gecensure_sn >= 60)
    {
        // Serial.print(gecensure_sn - (gecensure_dk * 60));
        Sistem_Calisma_Zamani = Sistem_Calisma_Zamani + String(gecensure_sn - (gecensure_dk * 60)) + " sn ";
    }
    else
    {
        // Serial.print(gecensure_sn);
        Sistem_Calisma_Zamani = Sistem_Calisma_Zamani + String(gecensure_sn) + " sn ";
        // Serial.print(" sn ");
    }

    // Sistem_Calisma_Zamani = String(String(gecensure_hafta) + " hafta " + String(gecensure_gun) + " gun " + String(gecensure_saat) + " saat " + String(gecensure_dk) + " dk " + String(gecensure_sn) + " sn");
    Serial.print(Sistem_Calisma_Zamani);

    Serial.println("  ---------------------------------");
    Serial.println("");
}

void Buzzer_Ton()
{
    if (lasttime_Buzzer <= buzzer_sure)
    {
        digitalWrite(ROLE_2_Buzzer, HIGH);
    }

    if (lasttime_Buzzer > buzzer_sure and lasttime_Buzzer <= (2 * buzzer_sure))
    {
        digitalWrite(ROLE_2_Buzzer, LOW);
    }
    if (lasttime_Buzzer > (2 * buzzer_sure))
    {
        lasttime_Buzzer = 0;
    }
    lasttime_Buzzer = lasttime_Buzzer + 1;
}