#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "MyAccessPoint";
const char* password = "MySecretPassword";

//variabls for blinking an LED with Millis
const short LED_BUILTIN_ESP01 = 1; //GPIO1
// const int led =LED_BUILTIN_ESP01; // ESP8266 Pin to which onboard LED is connected
const int led =LED_BUILTIN; // ESP8266 Pin to which onboard LED is connected
// const int nodemcu_led = D0; // GPIO 2
const int nodemcu_led = 16; // GPIO 2
const int gpio5_led = 5; // GPIO 5
const int gpio4_led = 4; // GPIO 4

unsigned long previousMillis = 0;  // will store last time LED was updated
const long interval = 1000;  // interval at which to blink (milliseconds)
int ledState = LOW;  // ledState used to set the LED
void setup() {
  pinMode(led, OUTPUT);
  pinMode(nodemcu_led, OUTPUT);
  pinMode(gpio5_led, OUTPUT);
  pinMode(gpio4_led, OUTPUT);
    
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("OTALedsBlink_ESP-12E");

  // No authentication by default
  ArduinoOTA.setPassword("drachenzahne");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

  void loop() {
    ArduinoOTA.handle();
  
  //loop to blink without delay
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      // if the LED is off turn it on and vice-versa:
      ledState = not(ledState);
      // set the LED with the ledState of the variable:
      Serial.print("Writing to LED: ");
      Serial.println(ledState); 
      digitalWrite(led,  ledState);
      // digitalWrite(nodemcu_led,  not(ledState));
      digitalWrite(nodemcu_led,  ledState);
      digitalWrite(gpio5_led,  ledState);
      digitalWrite(gpio4_led,  ledState);
    }
  }
