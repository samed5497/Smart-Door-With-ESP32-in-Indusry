#include <core/Kutuphaneler.h>

String UYGULAMA = "Smart DOOR ";
String VERSIYON = "v0.8_beta-by S.KAYA";

//////////////////////////////////////////////////////////////  PINLER  //////////////////////////////////////////////////////////////

int ROLE_1_ISIK = 33;
int ROLE_2_Buzzer = 32;

int M1_OUT = 27;
int M2_OUT = 26;

int IN_1_SW_MIN_35 = 35;
int IN_2_SW_MAX_34 = 34;
int IN_3_Cisim_39 = 39;
int IN_4_ManuelButon_36 = 36;
int IN_5_RF_23 = 23;
int IN_BUTON_0 = 0;
int IN_BUTON_16 = 16;

//////////////////////////////////////////////////////////////  TEMEL SİSTEM DEĞİŞKELER  //////////////////////////////////////////////////////////////

//////////////  Zaman değişkenleri - sabit değişkenler

long unsigned int lasttime_RF, lasttime_ISIK, lasttime_ManuelButon, lastTime_DONGU, lastTime_Update, lastTime_Oto_Algoritma_Sayac, RAPOR = 0;
long unsigned int lasttime_Buzzer = 0;
int gecensure_sn, gecensure_dk, gecensure_saat, gecensure_gun, gecensure_hafta = 0;
long unsigned int AP_MOD_ResTime = 10; //dakika   // AP Modunda X dakika kalırsa sistemi kontrol etmek için tekrardan sistemi başlat. 
    //////////////  Zaman değişkenleri - Ayarlanabilir

long unsigned int seriport_sure = 1; // saniye
long unsigned int aydinlatma_sure = 2500;
long unsigned int RF_algilama_sure = 2000;
long unsigned int ManuelButon_algilama_sure = 1000;
int buzzer_sure = 200;
int Kapi_Oto_Kapanma_Sure = 3;
int Oto_Algoritma_Sayac = (Kapi_Oto_Kapanma_Sure * 60); // Geri sayım değeri (saniye cinsinden)

int Kapi_Durum, Kapi_Komut, Isik_Durumu, IN4_deger, RF_KOMUT, Manuel_KOMUT, Update_Mod, progress = 0;
int SeriPort_Durum = 1;
int last_eeprom = 300;

bool Oto_Mod_Durum = true;

const char *hostname = UYGULAMA.c_str();
const char *ssid_AP = UYGULAMA.c_str();
const char *pass_AP = "";
String AdminUser = "admin";
String AdminPass = "admin1234";

boolean Kurulum_Modu;

String ssidList, Kapi_Durum_String, Isik_Durumu_String, IN4_deger_String, Sistem_Calisma_Zamani = "";
String ssid_STA = "";
String pass_STA = "";
String APP_KEY = "";
String APP_SECRET = "";
String DEVICE_ID = "";
const IPAddress apIP(192, 168, 4, 1);

IPAddress local_IP(192, 168, 1, 184);
IPAddress subnet(255, 255, 0, 0);
IPAddress gateway(192, 168, 1, 1);
IPAddress primaryDNS(8, 8, 8, 8);   // optional
IPAddress secondaryDNS(8, 8, 4, 4); // optional

DNSServer dnsServer;
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

//////////////////////////////////////////////////////////////  AAAAAAAAAAAAAAAAAAAA  //////////////////////////////////////////////////////////////
