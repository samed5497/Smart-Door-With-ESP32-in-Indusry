#include <functions/Baslangic_Fonksiyonlari.h>


void setup()
{
    //////////////////////////////////////////////////////////////  PINLERİN TANIMLANMALARI  //////////////////////////////////////////////////////////////
    
    pinMode(ROLE_1_ISIK, OUTPUT);
    pinMode(ROLE_2_Buzzer, OUTPUT);
    pinMode(M1_OUT, OUTPUT);
    pinMode(M2_OUT, OUTPUT);

    pinMode(IN_BUTON_0, INPUT_PULLUP);
    pinMode(IN_BUTON_16, INPUT);
    pinMode(IN_1_SW_MIN_35, INPUT);
    pinMode(IN_2_SW_MAX_34, INPUT);
    pinMode(IN_3_Cisim_39, INPUT);
    pinMode(IN_4_ManuelButon_36, INPUT);
    pinMode(IN_5_RF_23, INPUT);

    //////////////////////////////////////////////////////////////  SETUP BOLUMLERI  //////////////////////////////////////////////////////////////

    Serial.begin(115200);
    Serial.println("HMI Başlatılıyor. ");
    Serial.println();
    Serial.println();
    Serial.print(" >>> ");
    Serial.print(UYGULAMA);
    Serial.print(" / ");
    Serial.println(VERSIYON);
    Serial.printf("\r\n");
    delay(10);

    EEPROM.begin(512);
    delay(10);

    digitalWrite(ROLE_1_ISIK, LOW);

    if (digitalRead(IN_1_SW_MIN_35) == LOW and digitalRead(IN_2_SW_MAX_34) == HIGH)
    {
        Kapi_Komut = 0;
        Kapi_Durum = 3;
        Serial.println("[BILGI]: Kapı Kapalı Durumda.");
        Motor_Dur();
    }
    else if (digitalRead(IN_2_SW_MAX_34) == LOW and digitalRead(IN_1_SW_MIN_35) == LOW)
    {
        Kapi_Komut = 0;
        Kapi_Durum = 6;
        Serial.println("[BILGI]: Kapı Açık Durumda.");
        Motor_Dur();
    }

    Baslatma_Fonksiyonlari();
}

void loop()
{
    if (Update_Mod != 1)
    {
        RF_Kontrol();
        Buton_Kontrol();
        Kapi_Fonksiyonlari();
        Zaman_Guncellemesi();
        Oto_Kontrol_Algoritmasi();
        Seriport_Rapor();
    }
    if (Kurulum_Modu)
    {
        dnsServer.processNextRequest();
    }
}
