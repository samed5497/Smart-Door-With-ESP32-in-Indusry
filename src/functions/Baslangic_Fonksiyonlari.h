#include "web/Sunucu_mod_sayfalar.h"

void Web_Sunucu_Baslat()
{
  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);

  if (Kurulum_Modu)
  {
    Kurulum_mod_sayfalar();
  }

  else
  {
    Sunucu_mod_sayfalar();
  }

  server.begin();
}

void Kurulum_Modu_Fonksiyonu()
{
  WiFi.mode(WIFI_STA); // STA MODUNU devre dışı bırak.
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks(); // wifi ağlarını tara
  delay(100);
  Serial.println("");
  for (int i = 0; i < n; ++i)
  {
    ssidList += "<option value=\"";
    ssidList += WiFi.SSID(i);
    ssidList += "\">";
    ssidList += WiFi.SSID(i);
    ssidList += "</option>";
  }
  delay(100);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid_AP);

  dnsServer.start(53, "*", apIP);
  Web_Sunucu_Baslat();
  Serial.println("[Yerel Sunucu]: AP baglantisi baslatiliyor.");
  Serial.print("[Yerel Sunucu]: Varsayilan SSID:  \"");

  Serial.print(ssid_AP);
  Serial.println("\"");
}

void Baslatma_Fonksiyonlari() // İlk açılışta ilerleyecek fonksiyonlar.
{
  if (Hafizadan_Yukle()) // Hafzada kayıtlı bilgi varsa buraya gir.
  {
    if (Baglanti_Kontrol()) // Hafızadaki kayıtlı bilgilerle bağlantı kurmaya çalış.
    {
      Kurulum_Modu = false;
      Web_Sunucu_Baslat();         // Bağlantı kurulursa sunucuyu başlat.
      WiFi.softAPdisconnect(true); // AP Modunu devre dışı bırak.
      return;
    }
  }
  Kurulum_Modu = true; // Katıtlı veri yoksa kurulum modunu aktifleştir ve AP modunda devam et.
  Kurulum_Modu_Fonksiyonu();
}