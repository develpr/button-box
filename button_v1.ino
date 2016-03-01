 
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
#include <EEPROM.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>



#include "helpers.h"
#include "global.h"
/*
Include the HTML, STYLE and Script "Pages"
*/
#include "Page_Admin.h"
#include "Page_Information.h"
#include "PAGE_NetworkConfiguration.h"

#define ACCESS_POINT_NAME  "HACK THE PLANET"
#define ACCESS_POINT_PASSWORD  "password123" 
#define AdminTimeOut 180  // Defines the Time in Seconds, when the Admin-Mode will be diabled
#define CH_PD_CONTROL_PIN 14  // Defines the Time in Seconds, when the Admin-Mode will be diabled
#define TEST_PIN 16

bool sent = false;
long startTime;
        
void setup ( void ) {
  
  startTime = millis();
  pinMode(CH_PD_CONTROL_PIN, OUTPUT);
  pinMode(TEST_PIN, OUTPUT);
  digitalWrite(CH_PD_CONTROL_PIN, HIGH);

  EEPROM.begin(512);
  Serial.begin(115200);
  
  if (!ReadConfig())
  {
    // DEFAULT CONFIG
    config.ssid = "Wind-Up Bird Chronicle Supplier";
    config.password = "password123";
    config.dhcp = true;
    config.IP[0] = 192;config.IP[1] = 168;config.IP[2] = 1;config.IP[3] = 100;
    config.Netmask[0] = 255;config.Netmask[1] = 255;config.Netmask[2] = 255;config.Netmask[3] = 0;
    config.Gateway[0] = 192;config.Gateway[1] = 168;config.Gateway[2] = 1;config.Gateway[3] = 1;
    config.iftttKey = "<iftttkeyhere>";
    config.trigger1 = "";
    config.trigger2 = "";
    config.trigger3 = "";
    
    WriteConfig();
    Serial.println("General config applied");
  }

  WiFi.mode(WIFI_STA);

  ConfigureWifi();

  digitalWrite(TEST_PIN, HIGH);
  delay(250);
  digitalWrite(TEST_PIN, LOW);
  delay(250);
  
  digitalWrite(TEST_PIN, HIGH);
  delay(250);
  digitalWrite(TEST_PIN, LOW);
  delay(250);
  
  digitalWrite(TEST_PIN, HIGH);
  delay(250);
  digitalWrite(TEST_PIN, LOW);
  delay(250);
  
  digitalWrite(TEST_PIN, HIGH);
  delay(250);
  digitalWrite(TEST_PIN, LOW);
  delay(250);
  

}


void loop ( void ) {
  
  digitalWrite(CH_PD_CONTROL_PIN, HIGH);
  
  digitalWrite(TEST_PIN, HIGH);
  delay(2000);
  digitalWrite(TEST_PIN, LOW);
  delay(2000);
      
  if ( ! AdminEnabled)
  {
    if ( (millis() - startTime) > 15000)
    {
       AdminEnabled = true;
       Serial.println("Admin Mode enabled!");
       WiFi.mode(WIFI_AP_STA);
       WiFi.softAP( ACCESS_POINT_NAME , ACCESS_POINT_PASSWORD);
       setupServer();
       server.handleClient();
    }
    
  }
  else{
    digitalWrite(TEST_PIN, HIGH);
    delay(100);
    digitalWrite(TEST_PIN, LOW);
    delay(100);
    digitalWrite(TEST_PIN, HIGH);
    delay(100);
    digitalWrite(TEST_PIN, LOW);
    
    server.handleClient();
  }
  
  if(WiFi.status() == WL_CONNECTED && sent == false){
    sent = true;
    digitalWrite(TEST_PIN, HIGH);
    delay(100);
    digitalWrite(TEST_PIN, LOW);
    delay(100);
    digitalWrite(TEST_PIN, HIGH);
    delay(100);
    digitalWrite(TEST_PIN, LOW);
    delay(100);
    digitalWrite(TEST_PIN, HIGH);
    delay(100);
    digitalWrite(TEST_PIN, LOW);
    delay(100);
    digitalWrite(TEST_PIN, HIGH);
    delay(100);
    digitalWrite(TEST_PIN, LOW);
    if(config.trigger1.length() > 0){
      Serial.println("Sending trigger 1: " + config.trigger1);
      sendTrigger(config.iftttKey, config.trigger1);  
    }
    if(config.trigger2.length() > 0){
      Serial.println("Sending trigger 2: " + config.trigger2);
      sendTrigger(config.iftttKey, config.trigger2);  
    }
    if(config.trigger3.length() > 0){
      Serial.println("Sending trigger 3: " + config.trigger3);
      sendTrigger(config.iftttKey, config.trigger3);  
    }
    //messages were sent, so turn everything off again
    digitalWrite(CH_PD_CONTROL_PIN, LOW);
    Serial.println("Turning off - send messages...");
  }
  
  if( (millis() - startTime) > 99000 ){
    Serial.println("Turning off - timeout...");
    digitalWrite(CH_PD_CONTROL_PIN, LOW);
  }
  
}


void setupServer( void ) {

  
  server.on ( "/", send_network_configuration_html  );
  
  server.on ( "/favicon.ico",   []() { Serial.println("favicon.ico"); server.send ( 200, "text/html", "" );   }  );

  server.on ( "/admin.html", []() { Serial.println("admin.html"); server.send_P ( 200, "text/html", PAGE_AdminMainPage );   }  );
  server.on ( "/config.html", send_network_configuration_html );
  server.on ( "/info.html", []() { Serial.println("info.html"); server.send_P ( 200, "text/html", PAGE_Information );   }  );
  server.on ( "/admin/values", send_network_configuration_values_html );
  server.on ( "/admin/connectionstate", send_connection_state_values_html );
  server.on ( "/admin/infovalues", send_information_values_html );

  server.onNotFound ( []() { Serial.println("Page Not Found"); server.send_P ( 400, "text/html", "Page not Found" );   }  );
  server.begin();
  
}
  

// Perform an HTTP GET request to a remote page
bool sendTrigger(String key, String trigger) {
  HTTPClient http;
  Serial.println("Key: " + key);
  Serial.println("Trigger: " + trigger);
  Serial.println("path: /trigger/" + trigger + "/with/key/" + key);
  http.begin("maker.ifttt.com", 80, "/trigger/" + trigger + "/with/key/" + key); //HTTP
  int httpCode = http.GET();
  if(httpCode) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if(httpCode == 200) {
          return true;
      }
  } else {
      int httpCode = http.GET();
  }

  return true;
  
}

