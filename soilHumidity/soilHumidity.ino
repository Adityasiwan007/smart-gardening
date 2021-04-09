#include "WiFi.h"
#include <HTTPClient.h>
#define SensorPin 32 
const char* ssid = "Wifi Name";
const char* password =  "Password";
int didMyOneTimeAction = 0;
int didMyOneTimeAction2 = 0;

String serverName = "Your_BaseURL/garden";

unsigned long lastTime = 0;
unsigned long timerDelay = 500;

void setup() { 
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
}

void loop() {
  float sensorValue = analogRead(SensorPin);
  Serial.println(sensorValue);
  delay(1000);
  
  if(sensorValue>=3000)
  {
    didMyOneTimeAction2=0;
  }

  // Action when soil is dry
  if (didMyOneTimeAction == 0 && sensorValue>=3000)
  {
    didMyOneTimeAction = 1;
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      
      String serverPath = serverName + "?id="+"Soil_is_Dry,_Now_Watering.";
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();

      // DO your action stuuf here to start the watering the soil.

      
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }

  // Action when soil is hummid/Wet
  if ((millis() - lastTime) > timerDelay && sensorValue<3000 && didMyOneTimeAction2==0) {
    didMyOneTimeAction2=1;
    didMyOneTimeAction = 0;
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      
      String serverPath = serverName + "?id="+"Soil_is_wet.";
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();



      // DO your action stuuf here to stop the watering the soil.
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
} 
