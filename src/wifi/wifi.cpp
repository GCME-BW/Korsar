#include "wifi.h"

bool connected = true;
String initial_mdns_addr = DEFAULT_LOCAL_DOMAIN;
int initial_wifi_mode = 0;
String initial_soft_ap_ssid = DEFAULT_SOFTAPP_SSID;
String initial_soft_ap_pwd = DEFAULT_SOFTAPP_PASSWORD;

WiFiManager wm;
WiFiManagerParameter mdns_addr;
WiFiManagerParameter wifi_mode;
WiFiManagerParameter soft_ap_ssid;
WiFiManagerParameter soft_ap_pwd;

String getParam(String name)
{
    // read parameter from server, for customhmtl input
    String value;
    if (wm.server->hasArg(name))
    {
        value = wm.server->arg(name);
    }
    return value;
}

void saveConfigCallback()
{
    Serial.print("mdns: ");
    Serial.println(getParam("mdns_addr"));
    Serial.print("wifi_mode: ");
    Serial.println(getParam("wifi_mode"));
    Serial.print("soft_ap_ssid: ");
    Serial.println(getParam("soft_ap_ssid"));
    Serial.print("soft_ap_pwd: ");
    Serial.println(getParam("soft_ap_pwd"));

    uint8_t buffer[256];
    WifiSettings data = WifiSettings_init_zero;
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    data.mode = getParam("wifi_mode") == "0" ? WifiMode_STA : WifiMode_SoftAP;
    getParam("mdns_addr").toCharArray(data.mdns_domain, 64);
    getParam("soft_ap_ssid").toCharArray(data.softap_ssid, 64);
    getParam("soft_ap_pwd").toCharArray(data.softap_password, 64);
    pb_encode(&stream, WifiSettings_fields, &data);

    File file = SPIFFS.open("/wifi_config.bin", FILE_WRITE);
    if (!file)
    {
        Serial.println("- failed to open file for writing");
        return;
    }
    if (file.write(buffer, stream.bytes_written))
    {
        Serial.println("- file written");
    }
    else
    {
        Serial.println("- write failed");
    }
    file.close();
    if (getParam("wifi_mode") != "0")
    {
        ESP.restart();
    }
}

void loadSettings()
{
    File file = SPIFFS.open("/wifi_config.bin");
    if (!file || file.isDirectory())
    {
        Serial.println("No saved configs file");
        return;
    }

    uint8_t buffer[256];
    int bytes = 0;
    Serial.println("- read from file:");
    while (file.available())
    {
        buffer[bytes] = file.read();
        bytes++;
    }
    file.close();

    WifiSettings settings = WifiSettings_init_zero;
    pb_istream_t stream = pb_istream_from_buffer(buffer, bytes);
    bool status = pb_decode(&stream, WifiSettings_fields, &settings);
    if (!status) // TODO delete broken file
    {
        Serial.println("No saved configs file");
        return;
    }

    initial_mdns_addr = settings.mdns_domain;
    initial_wifi_mode = settings.mode;
    initial_soft_ap_ssid = settings.softap_ssid;
    initial_soft_ap_pwd = settings.softap_password;
    Serial.print("mdns: ");
    Serial.println(settings.mdns_domain);
    Serial.print("wifi_mode: ");
    Serial.println(settings.mode);
    Serial.print("soft_ap_ssid: ");
    Serial.println(settings.softap_ssid);
    Serial.print("soft_ap_pwd: ");
    Serial.println(settings.softap_password);
}

void setupWifi()
{
    pinMode(BUTTON_PIN, INPUT);
    loadSettings();
    if (initial_wifi_mode == 0)
    {
        WiFi.mode(WIFI_STA);
        delay(3000);
        wm.setSaveParamsCallback(saveConfigCallback);
        new (&mdns_addr) WiFiManagerParameter("mdns_addr", "mDNS local domain address", initial_mdns_addr.c_str(), 64);
        const char *wifi_mode_str = "<br><label for='wifi_mode'>WiFi connection mode</label><br/><input type='radio' name='wifi_mode' value='0' checked> Connect to existing network<br><input type='radio' name='wifi_mode' value='1'> Host standalone WiFi network (enter SSID and password in following fields):<br>";
        new (&wifi_mode) WiFiManagerParameter(wifi_mode_str);
        new (&soft_ap_ssid) WiFiManagerParameter("soft_ap_ssid", "Access Point SSID", initial_soft_ap_ssid.c_str(), 64);
        new (&soft_ap_pwd) WiFiManagerParameter("soft_ap_pwd", "Access Point Password", initial_soft_ap_pwd.c_str(), 64);
        wm.addParameter(&mdns_addr);
        wm.addParameter(&wifi_mode);
        wm.addParameter(&soft_ap_ssid);
        wm.addParameter(&soft_ap_pwd);

        wm.setConfigPortalBlocking(false);

        std::vector<const char *> menu = {"wifi", "info", "param", "sep", "restart", "exit"};
        wm.setMenu(menu);

        // set dark theme
        wm.setClass("invert");

        wm.setAPStaticIPConfig(IPAddress(192, 168, 0, 1), IPAddress(192, 168, 0, 1), IPAddress(255, 255, 255, 0)); // set static ip,gw,sn

        wm.setConnectTimeout(20);
        wm.setConfigPortalTimeout(300);

        connected = wm.autoConnect();

        if (!connected)
        {
            Serial.println("Failed to connect or hit timeout");
            // ESP.restart();
        }
        else
        {
            Serial.println("connected...yeey :)");
            if (!MDNS.begin(initial_mdns_addr))
            {
                Serial.println("Error setting up MDNS responder!");
            }
        }
    }
    else
    {
        IPAddress AP_LOCAL_IP(192, 168, 0, 1);
        IPAddress AP_GATEWAY_IP(192, 168, 0, 1);
        IPAddress AP_NETWORK_MASK(255, 255, 255, 0);
        WiFi.softAPConfig(AP_LOCAL_IP, AP_GATEWAY_IP, AP_NETWORK_MASK);
        WiFi.softAP(initial_soft_ap_ssid, initial_soft_ap_pwd);
        MDNS.begin(initial_mdns_addr);
        connected = true;
    }
}

int buttonState;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;

void checkButton()
{
    int reading = digitalRead(BUTTON_PIN);

    if (reading != lastButtonState)
    {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > RESET_BUTTON_TIME)
    {
        if (reading != buttonState)
        {
            buttonState = reading;
            if (buttonState == LOW)
            {
                wm.resetSettings();
                SPIFFS.remove("/wifi_config.bin");
                ESP.restart();
            }
        }
    }

    lastButtonState = reading;
}

void processWifi()
{
    if (!connected)
    {
        wm.process();
    }
    else
    {
        checkButton();
    }
}
