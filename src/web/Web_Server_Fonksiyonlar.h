#include <functions/Degisken_Fonksiyonlar.h>

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    if (type == WS_EVT_CONNECT)
    {
        Serial.println("WebSoket Bağlantısı Kuruldu");
    }
    else if (type == WS_EVT_DISCONNECT)
    {
        Serial.println("WebSoket Bağlantısı Kapatıldı");
    }
    else if (type == WS_EVT_DATA)
    {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        if (info->opcode == WS_TEXT)
        {
            data[len] = '\0';
            String receivedMessage = String((char *)data);
            if (receivedMessage == "OtoModOn")
            {
                Oto_Mod_Durum = true;
            }
            else if (receivedMessage == "OtoModOff")
            {
                Oto_Mod_Durum = false;
            }
        }
    }
    if (Oto_Mod_Durum)
    {
        server->textAll("OtoModOn");
    }
    else
    {
        server->textAll("OtoModOff");
    }
}

String generateMainPageForm()
{

    String MainPageScript = String("<script>var ws=new WebSocket('ws://' + window.location.hostname + ':80/ws');") +

                            // WebSocket ile server'dan güncelleme alındığında çalışacak fonksiyon
                            "ws.onmessage = function (event) {var data = event.data;" +
                            "   if      (data === 'OtoModOn') {document.getElementById('otoModSwitch').checked = true}" +
                            "   else if (data === 'OtoModOff') {document.getElementById('otoModSwitch').checked = false;}};" +

                            "ws.onerror = function(event) {" +
                            "alert('Bağlantı hatası! Sayfa yeniden yüklenecek.'); window.location.reload();};" +

                            "function sendRequest(url) {" +
                            "   fetch(url).then(response => {" +
                            "   if (response.ok) {console.log('İstek başarıyla gönderildi!');}" +
                            "   else {console.error('İstekte bir hata oluştu.');}}).catch(error => {console.error('Bir hata oluştu:', error);});}" +

                            "function openDoor() {" +
                            "   sendRequest('/kapiac');}" +

                            "function closeDoor() {" +
                            "   sendRequest('/kapikapat');}"

                            "function confirmReset() {" +
                            "   return confirm('Fabrika ayarlarına dönmek istediğinize emin misiniz? Onay vermeniz durumunda hafızada kayıtlı olan tüm bilgiler silinecek ve cihazı tekrar kullanabilmek için yeniden internet yapılandırması yapmanız gerekecektir.');}" +

                            "function resetDevice() {" +
                            "   if (confirmReset()) {sendRequest('/reset');window.location.href = '/reset';}}" +

                            "function confirmRestart() {" +
                            "   return confirm('Sistem Yeniden Başlatılacaktır. Onaylıyor musunuz?');}" +

                            "function restartDevice() {" +
                            "   if (confirmRestart()) {sendRequest('/restart');var countDown = 1;var countdownInterval = setInterval(function() {" +
                            "   if (countDown > 0) {alert('Sistem yeniden başlatılıyor. Sayfa otomatik olarak yeniden yüklenecektir.');countDown--;} " +
                            "   else {clearInterval(countdownInterval);" +
                            "   alert('Sistem yeniden başlatıldı.'); window.location.reload();}}, 2500);}}" +

                            "function clearInputFields() {" +
                            "   document.getElementById('username').value = ''; document.getElementById('password').value = '';}" +

                            "function authenticateUser() {" +
                            "   var username = document.getElementById('username').value; var password = document.getElementById('password').value;" +
                            "   if (username === 'admin' && password === 'admin1234') {clearInputFields();document.getElementById('loginPopup').style.display = 'none';document.getElementById('advancedSettingsPopup').style.display = 'block';}" +
                            "   else {clearInputFields();alert('Kullanıcı adı veya şifre yanlış!');}}" +

                            // OtoMod durumunun değişmesini takip eden fonksiyon
                            "function changeOtoMod() {var otoModSwitch = document.getElementById('otoModSwitch'); var isChecked = otoModSwitch.checked;" +
                            "if (isChecked) {ws.send('OtoModOn');} " +
                            "else { ws.send('OtoModOff');}}" +

                            // "function checkFile() {" +
                            // "   const fileInput = document.getElementById('fileInput');" +
                            // "   const uploadButton = document.getElementById('uploadButton');" +
                            // "   const allowedExtension = '.bin'; // İzin verilen dosya uzantısı" +
                            // "   if (fileInput.files.length > 0) {const fileName = fileInput.files[0].name; const fileExtension = fileName.slice(fileName.lastIndexOf('.')).toLowerCase(); " +
                            // "       if (fileExtension === allowedExtension) {uploadButton.disabled = false;}" +
                            // "       else {alert('Lütfen .bin uzantılı bir dosya seçin.'); uploadButton.disabled = true;}}" +
                            // "   else {uploadButton.disabled = true;}}" +

                            "function closeAdvancedSettingsPopup() {" +
                            "   document.getElementById('advancedSettingsPopup').style.display = 'none';}" +

                            "function updateCPUTemperature() {" +
                            "   fetch('/getCPUTemperature').then(response => response.text()).then(data => {document.getElementById('cpuTemp').innerText = data + ' °C';}).catch(error => {console.error('Hata:', error);});}" +
                            "   setInterval(updateCPUTemperature, 500);" +

                            "function updateRAMUsage() {" +
                            "   fetch('/getRAMUsage').then(response => response.text()).then(data => {document.getElementById('RAMUsage').innerText = data;}).catch(error => {console.error('Hata:', error);});}" +
                            "   setInterval(updateRAMUsage, 500);" +

                            "function updateROMUsage() {" +
                            "   fetch('/getROMUsage').then(response => response.text()).then(data => {document.getElementById('ROMUsage').innerText = data;}).catch(error => {console.error('Hata:', error);});}" +
                            "   setInterval(updateROMUsage, 500);" +

                            "function updateDoorStatus() {" +
                            "   fetch('/updateDoorStatus').then(response => response.text()).then(data => {document.getElementById('DoorStatus').innerText = data;}).catch(error => {console.error('Hata:', error);});}" +
                            "   setInterval(updateDoorStatus, 500);" +

                            "function changeOtoMod() {" +
                            "var otoModSwitch = document.getElementById('otoModSwitch');" +
                            "var isChecked = otoModSwitch.checked;" +
                            "if (isChecked){ ws.send('OtoModOn'); }" +
                            "else{ ws.send('OtoModOff');} }" +

                            "function updateOtoSayacStatus() {" +
                            "   fetch('/updateOtoSayacStatus').then(response => response.text()).then(data => {document.getElementById('OtoSayacStatus').innerText = data;}).catch(error => {console.error('Hata:', error);});}" +
                            "   setInterval(updateOtoSayacStatus, 500);" +

                            "function updateWorkingTime() {" +
                            "   fetch('/updateWorkingTime').then(response => response.text()).then(data => {document.getElementById('sysWorkingTime').innerText = data;}).catch(error => {console.error('Hata:', error);});}" +
                            "   setInterval(updateWorkingTime, 500);" +

                            "function updateminSwitchStatus() {" +
                            "   fetch('/updateminSwitchStatus').then(response => response.text()).then(data => {document.getElementById('minSwitchStatus').innerText = data;}).catch(error => {console.error('Hata:', error);});}" +
                            "   setInterval(updateminSwitchStatus, 500);" +

                            "function updatemaxSwitchStatus() {" +
                            "   fetch('/updatemaxSwitchStatus').then(response => response.text()).then(data => {document.getElementById('maxSwitchStatus').innerText = data;}).catch(error => {console.error('Hata:', error);});}" +
                            "   setInterval(updatemaxSwitchStatus, 500);" +

                            "function updateobjdetectStatus() {" +
                            "   fetch('/updateobjdetectStatus').then(response => response.text()).then(data => {document.getElementById('objdetectStatus').innerText = data;}).catch(error => {console.error('Hata:', error);});}" +
                            "   setInterval(updateobjdetectStatus, 500);" +

                            "</script>";

    String MainPagecss = String("<style>") +
                         "body { font-family: Arial, sans-serif; background-color: #f4f4f4; text-align: center; }" +
                         "h1 { color: #333; }" +
                         "button { padding: 10px 20px; font-size: 16px; margin: 5px; border-radius: 5px; border: none; cursor: pointer; }" +
                         "button.button-open { background-color: #4CAF50; color: white; }" +  // Kapıyı Aç butonu (yeşil)
                         "button.button-close { background-color: #f44336; color: white; }" + // Kapıyı Kapat butonu (kırmızı)
                         "button:hover { background-color: #777; }" +
                         ".form-container { max-width: 750px; margin: 0 auto; padding: 20px; background: rgba(255, 255, 255, 0.8); border-radius: 5px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.3); }" +
                         ".popup { display: none; position: fixed; left: 50%; top: 50%; transform: translate(-50%, -50%); z-index: 9999; background-color: rgba(255, 255, 255, 0.9); border: 1px solid #ccc; border-radius: 5px; padding: 20px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.3); }" +
                         ".popup form label { display: inline-block; width: 80px; }" +                                         // Ayarlarınızı yapabilirsiniz
                         ".popup form input[type='text'], .popup form input[type='password'] { width: calc(100% - 100px); }" + // inputların boyutu
                         ".close { position: absolute; top: 10px; right: 10px; cursor: pointer; font-size: 24px; }" +
                         ".switch {position: relative; display: inline-block; width: 60px; height: 34px;}" +
                         ".switch input {display: none;}" +
                         ".slider {position: absolute; cursor: pointer; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; transition: .4s; border-radius: 34px;}" +
                         ".slider:before {position: absolute; content: ''; height: 26px; width: 26px; left: 4px; bottom: 4px; background-color: white; transition: .4s; border-radius: 50%;}" +
                         "input:checked + .slider {background-color: #2196F3;}" +
                         "input:checked + .slider:before {transform: translateX(26px);}" +
                         ".slider.round {border-radius: 34px;}" +
                         ".slider.round:before {border-radius: 50%;}" +

                         "</style>";

    String MainPagehtlm = String("<!DOCTYPE html><html><head><meta charset='UTF-8'><title>" + UYGULAMA + " Anasayfa</title>") +
                          MainPageScript + MainPagecss + "</head><body>" +

                          "<div class='form-container'>" +
                          "<h2>Smart Door Kontrol Sayfası</h2>" +

                          "<h3>- Kapı Kontrol -</h3>" +
                          "<button id='btnKapiAc' class='button button-open' onclick='openDoor()'>Kapıyı Aç</button>" +
                          "<button id='btnKapiKapat' class='button button-close' onclick='closeDoor()'>Kapıyı Kapat</button>" +

                          "<p>Kapı Durumu: <strong><span id='DoorStatus'></span></strong></p>" +

                          "<h3>- Otomatik Mod -</h3>" +
                          "<label class='switch'>" +
                          "<input type='checkbox' id='otoModSwitch' onchange='changeOtoMod()' " + (Oto_Mod_Durum ? "checked" : "") + ">" +
                          "<span class='slider round'></span></label>" +
                          "<p>Oto Kapanma Süresi: <strong><span id='OtoSayacStatus'></span></strong></p>" +
                          "<br></div>" +
                          "<br>" +

                          "<div class='form-container'>" +
                          "<h2>Cihaz Bilgileri</h2>" +
                          "<h3>- Wi-Fi Bilgileri -</h3>" +
                          "<p> <strong>Bağlı Ağ:</strong> " + ssid_STA + "</p>" +
                          "<p> <strong>IP Adresi:</strong> " + WiFi.localIP().toString() + "</p>" +
                          "<p> <strong>MAC Adresi:</strong> " + WiFi.macAddress() + "</p>" +
                          "<p> <strong>Host Adı:</strong> " + hostname + "</p>" +

                          "<h3>- CPU Bilgileri -</h3>" +
                          "<p> <strong>CPU: </strong> " + CPU + String((F_CPU / 1000000)) + " Mhz" + "</p>" +
                          "<p> <strong>RAM: </strong><span id='RAMUsage'></span></p>" +
                          "<p> <strong>ROM: </strong><span id='ROMUsage'></span></p>" +
                          "<p> <strong>CPU Temp.:</strong> <span id='cpuTemp'></span></p>" +

                          "<h3>- Sensör Bilgileri -</h3>" +
                          "<p> <strong>~ Min Switch:</strong> <span id='minSwitchStatus'></span></p>" +
                          "<p> <strong>~ Max Switch:</strong> <span id='maxSwitchStatus'></span></p>" +
                          "<p> <strong>~ Cisim Sensoru:</strong> <span id='objdetectStatus'></span></p>" +

                          "<h3>- Diğer -</h3>" +
                          "<p> <strong>Sistem Çalışma Süresi.:</strong> <span id='sysWorkingTime'></span></p>" +

                          "</div>" +
                          "<br>" +

                          "<div class='form-container'>" +
                          "<h2>Ayarlar</h2>" +
                          "<button onclick='document.getElementById(\"loginPopup\").style.display = \"block\";'>Yönetici Ayarları Girişi</button>" +
                          "<p> <em>Made in <strong>Kule Kalıp</strong>. Design by <strong>S. KAYA</strong></em></p>" +
                          "</div>" +

                          "<div id='loginPopup' class='popup'>" +
                          "<span class='close' onclick='document.getElementById(\"loginPopup\").style.display = \"none\";'>&times;</span>" +
                          "<h2>Yönetici Girişi</h2>" +
                          "<form>" +
                          "<label for='username'>Kullanıcı Adı:</label>" +
                          "<input type='text' id='username' name='username' required><br><br>" +
                          "<label for='password'>Şifre:</label>" +
                          "<input type='password' id='password' name='password' required><br><br>" +
                          "<input type='button' value='Giriş Yap' onclick='authenticateUser()'>" +
                          "</form>" +
                          "</div>" +
                          "<div id='advancedSettingsPopup' class='popup' style='display: none;'>" +
                          "<span class='close' onclick='closeAdvancedSettingsPopup()' style='font-size: 28px;'>&times;</span>" +
                          "<h2>Gelişmiş Ayarlar</h2>" +
                          "<h3>- Yazılım Güncelleme -</h3>" +

                          //"<form id='uploadForm' method='POST' action ='/update' enctype='multipart/form-data'>" +
                          //"<input type='file' id='fileInput' name='update'onchange='checkFile()'>" +
                          //"<input type='submit' value='Yazılımı Yükle' class='button' id='uploadButton' disabled>" +
                          //"</form>" +

                          "<form method='POST' action='/update' enctype='multipart/form-data'>" +
                          "<input type='file' name='update'>" +
                          "<input type='submit' value='Yazılımı Yükle' class='button'>" +
                          "</form>" +

                          "<p> <strong>Mevcut Versiyon:</strong> " + VERSIYON + "</p>" +
                          "<button onclick='resetDevice()' class='button'>Fabrika Ayarlarına Dön</button>" +
                          "<h3>- Sistem -</h3>" +
                          "<button onclick='restartDevice()' class='button'>Yeniden Başlat</button>" +
                          "</div>" +

                          "</body></html>";
    return MainPagehtlm;
}

String generateFirstPageForm()
{
    String FirstPageScript = String("<script> </script>");

    String FirstPagecss = String("<style>") +
                          "body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }" +
                          "h1 { color: #333; } p { color: #555; }" +
                          ".settings-btn { background-color: #4caf50; color: white; padding: 10px 20px; text-decoration: none; border-radius: 5px; }" +
                          ".content { max-width: 400px; margin: 0 auto; background-color: #fff; padding: 20px; border-radius: 5px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); }" +
                          "</style>";

    String FirstPagehtml = "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>" + UYGULAMA + " Wi-Fi Yapılandırma</title>" +
                           FirstPageScript + FirstPagecss + "</head><body>" +
                           "<div class='content'>" +
                           "<h1>" + UYGULAMA + "</h1>" +
                           "<p>Cihazınız için özel Wi-Fi ağınızın ayarlarını yapılandırmak için aşağıdaki butona tıklayın.</p><br>" +
                           "<p><a href=\"/settings\" class='settings-btn'>Yapılandırma</a></p></div></body></html>";

    return FirstPagehtml;
}

String generateSettingsForm()
{

    String SettingsPageScript = String("<style>") +
                                "body { font-family: Arial, sans-serif; background-color: #f4f4f4; }" +
                                "h1 { color: #333; text-align: center; }" +
                                "form { margin: 0 auto; max-width: 400px; padding: 20px; background: #fff; border-radius: 5px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); position: relative; }" +
                                "label { display: block; margin-bottom: 5px; }" +
                                "input[type='password'], input[type='text'], select { width: calc(100% - 12px); padding: 5px; margin-bottom: 10px; border: 1px solid #ccc; border-radius: 3px; }" +
                                ".toggle-password { position: absolute; top: 50%; right: 5px; transform: translateY(-50%); width: 20px; height: 20px; background: #ccc; border: none; cursor: pointer; padding: 0; }" +
                                "input[type='submit'] { background-color: #4caf50; color: white; padding: 10px 20px; border: none; border-radius: 3px; cursor: pointer; display: block; margin: 0 auto; }" +
                                "</style>";

    String SettingsPagecss = String("<script>") +
                             "function togglePassword() {var passField = document.getElementById('pass_STA');" +
                             "  if (passField.type === 'password') {passField.type = 'text';}" +
                             "  else {passField.type = 'password';}}" +
                             "</script>";

    String SettingsPagehtml = String("<!DOCTYPE html><html><head><meta charset='UTF-8'><title>" + UYGULAMA + " Wi-Fi Yapılandırma</title>") +
                              SettingsPageScript + SettingsPagecss + "</head><body>" +
                              "<h1>Wi-Fi Yapılandırma</h1>" +
                              "<form method='post' action='setap'>" +
                              "<label for='ssid_STA'>SSID:</label><select name='ssid_STA'>" + ssidList + "</select><br><br>" +
                              "<label for='pass_STA'>Şifre:</label><div style='position: relative;'>" +
                              "<input name='pass_STA' id='pass_STA' maxlength='64' type='password'>" +
                              "<input type='button' class='toggle-password' onclick='togglePassword()' value='👁️'></div>" +
                              "<br><br>" +
                              "<label for='APP_KEY'>APP Key:</label>"
                              "<input name='APP_KEY' length=64 type='text'><br><br>"
                              "<label for='APP_SECRET'>APP Secret:</label>"
                              "<input name='APP_SECRET' length=128 type='text'><br><br>"
                              "<label for='DEVICE_ID'>Device ID:</label>"
                              "<input name='DEVICE_ID' length=64 type='text'><br><br>"
                              "<input type='submit' value='Gönder'></form>" +
                              "</body></html>";

    return SettingsPagehtml;
}

String generateSettingsFinishForm()
{
    String SettingsPageScript = String("<style>") +
                                "body { font-family: Arial, sans-serif; background-color: #f4f4f4; }" +
                                "h1 { color: #333; text-align: center; }" +
                                "form { margin: 0 auto; max-width: 400px; padding: 20px; background: #fff; border-radius: 5px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); position: relative; }" +
                                "label { display: block; margin-bottom: 5px; }" +
                                "input[type='password'], input[type='text'], select { width: calc(100% - 12px); padding: 5px; margin-bottom: 10px; border: 1px solid #ccc; border-radius: 3px; }" +
                                ".toggle-password { position: absolute; top: 50%; right: 5px; transform: translateY(-50%); width: 20px; height: 20px; background: #ccc; border: none; cursor: pointer; padding: 0; }" +
                                "input[type='submit'] { background-color: #4caf50; color: white; padding: 10px 20px; border: none; border-radius: 3px; cursor: pointer; display: block; margin: 0 auto; }" +
                                "</style>";

    String SettingsPagecss = String("<script>") +
                             "function togglePassword() {var passField = document.getElementById('pass_STA');" +
                             "  if (passField.type === 'password') {passField.type = 'text';}" +
                             "  else {passField.type = 'password';}}" +
                             "</script>";

    String SettingsPagehtml = String("<!DOCTYPE html><html><head><meta charset='UTF-8'><title>" + UYGULAMA + " Wi-Fi Yapılandırma</title>") +
                              SettingsPageScript + SettingsPagecss + "</head><body>" +
                              "<h1>Wi-Fi Yapılandırma</h1>" +
                              "<form method='post' action='setap'>" +
                              "<label for='ssid_STA'>SSID:</label><select name='ssid_STA'>" + ssidList + "</select><br><br>" +
                              "<label for='pass_STA'>Şifre:</label><div style='position: relative;'>" +
                              "<input name='pass_STA' id='pass_STA' maxlength =64 type='password'>" +
                              "<input type='button' class='toggle-password' onclick='togglePassword()' value='👁️'></div>" +
                              "<br><br>" +
                              "<label for='APP_KEY'>APP Key:</label><input name='APP_KEY' maxlength =64 type='text'><br><br>" +
                              "<label for='APP_SECRET'>APP Secret:</label><input name='APP_SECRET' maxlength =128 type='text'><br><br>" +
                              "<label for='DEVICE_ID'>Device ID:</label><input name='DEVICE_ID' maxlength =64 type='text'><br><br>" +
                              "<input type='submit' value='Gönder'></form>" +
                              "</body></html>";

    return SettingsPagehtml;
}

String generateResetForm()
{
    String ResetPageScript = String("<script> </script>");

    String ResetPagecss = String("<style>") +
                          "body { font-family: Arial, sans-serif; background-color: #f4f4f4; }" +
                          "body { font-family: Arial, sans-serif; background-color: #f4f4f4; text-align: center; }" +
                          "h1 { color: #333; } p { color: #555; }" +
                          ".content { max-width: 400px; margin: 0 auto; background: #fff; border-radius: 5px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); padding: 20px; }" +
                          "label { display: block; margin-bottom: 5px; }" +
                          "input[type='text'] { width: calc(100% - 12px); padding: 5px; margin-bottom: 10px; border: 1px solid #ccc; border-radius: 3px; }" +
                          ".submit-button { background-color: #4caf50; color: white; padding: 10px 20px; border: none; border-radius: 3px; cursor: pointer; }" +
                          ".button { padding: 10px 20px; font-size: 18px; background-color: #007bff; color: #fff; border: none; border-radius: 5px; cursor: pointer; text-decoration: none; }" +
                          "</style>";

    String ResetPagehtml = String("<!DOCTYPE html<html><head><meta charset='UTF-8'><title>" + UYGULAMA + " Fabrika Ayarları</title>") +
                           ResetPageScript + ResetPagecss + "</head><body>" +
                           "<div class='content'>" + "<h1>Wi-Fi ayarları sıfırlandı!</h1>" +
                           "<p>Değişiklikler cihazı yeniden başlattığınızda devreye alınacaktır. Yeniden başlatana kadar mevcut sistem çalışmaya devam edecektir. </p>" +
                           "<p>Cihaz yeniden başlatıldığında <strong><em>" + ssid_AP + "</strong></em> ismiyle Wifi paylaşımına başlayacaktır. </p><br>" +
                           "<a href='/' class='button'>Ana Sayfaya Dön</a>" + // Buton ile ana sayfaya yönlendirme
                           "<br></div></body></html>";

    return ResetPagehtml;
}

String generateUpdateFailForm(String errorCode)
{

    String UpdateFailPageScript = String("<script> </script>");

    String UpdateFailPagecss = String("<style>") +
                               "body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }" +
                               ".fail-msg { color: red; font-size: 24px; }" +
                               ".button { padding: 10px 20px; font-size: 18px; background-color: #007bff; color: #fff; border: none; border-radius: 5px; cursor: pointer; text-decoration: none; }" +
                               "</style>";

    String UpdateFailPagehtml = String("<!DOCTYPE html<html><head><meta charset='UTF-8'><title>" + UYGULAMA + " Güncelleme Başarısız</title>") +
                                UpdateFailPageScript + UpdateFailPagecss + "</head><body>" +
                                "<h1>Güncelleme Başarısız!</h1>" +
                                "<p class='fail-msg'>Hata Kodu: " + errorCode + "</p>" +
                                "<a href='/' class='button'>Ana Sayfa</a>" + // Buton ile ana sayfaya yönlendirme
                                "</body></html>";

    return UpdateFailPagehtml;
}

String generateUpdateSuccessForm()
{

    String UpdateSuccessPageScript = String("<script> </script>");

    String UpdateSuccessPagecss = String("<style>") +
                                  "body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; background-image: url('arka_plan_resmi.png'); background-size: cover; }" +
                                  ".success-msg { color: green; font-size: 24px; }" +
                                  ".button { padding: 10px 20px; font-size: 18px; background-color: #007bff; color: #fff; border: none; border-radius: 5px; cursor: pointer; text-decoration: none; }" +
                                  "</style>";

    String UpdateSuccessPagehtml = String("<!DOCTYPE html<html><head><meta charset='UTF-8'><title>" + UYGULAMA + " Güncelleme Başarılı</title>") +
                                   UpdateSuccessPageScript + UpdateSuccessPagecss + "</head><body>" +
                                   "<h1>Güncelleme Başarılı!</h1>" +
                                   "<p class='success-msg'>Güncelleme başarıyla tamamlandı! Cihaz yeniden başlatılıyor..</p>" +
                                   "<a href='/' class='button'>Ana Sayfa</a>" + // Buton ile ana sayfaya yönlendirme
                                   "</body></html>";
    return UpdateSuccessPagehtml;
}

String generateUpdateInProgressForm()
{
    String UpdateInProgressPageScript = String("<script> </script>");

    String UpdateInProgressPagecss = String("<style>") +
                                     "body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }" +
                                     "</style>";

    String UpdateInProgressPagehtml = String("<!DOCTYPE html><html><head><meta charset='UTF-8'><title>" + UYGULAMA + " Güncelleme İlerliyor</title>" +
                                             UpdateInProgressPageScript + UpdateInProgressPagecss + "</head><body>" +
                                             "</head><body><h1>Güncelleme İlerliyor</h1><p>Lütfen bekleyiniz.</p></body></html>");
    return UpdateInProgressPagehtml;
}
