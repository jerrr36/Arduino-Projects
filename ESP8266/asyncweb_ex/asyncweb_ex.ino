#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266WiFi.h>
#include <FS.h>

#define LED D0

const char* ssid = "";
const char* pwd = "";
bool toggle = HIGH;
String ledState;



AsyncWebServer server(80);

void wifiConnect(String ssid, String pwd);
String processor(const String& var);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, toggle);

  SPIFFS.begin();

  wifiConnect(ssid, pwd);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/toggle", HTTP_GET, [](AsyncWebServerRequest *request){
    toggle = !toggle;
    digitalWrite(LED, toggle);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.begin();

}

void loop() {
  // put your main code here, to run repeatedly:

}

void wifiConnect(String ssid, String pwd){
 
  WiFi.begin(ssid, pwd);   // add Wi-Fi networks you want to connect to
 
  Serial.println("Connecting ...");
  
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
  
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer 
}

String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTON"){
    if(digitalRead(LED)){
      ledState = "OFF";
    }
    else{
      ledState = "ON";
    }
    return ledState;
  }
  return String();
}
