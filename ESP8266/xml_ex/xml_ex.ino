#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <FS.h>

#define LED1 D0
#define LED2 D4

const char* ssid = "";
const char* pwd = "";
const char* timerParam = "timer";



AsyncWebServer server(80);

void wifiConnect(String ssid, String pwd);
long sendTemp();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Initializing leds
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, HIGH);

  pinMode(LED2, OUTPUT);
  digitalWrite(LED2, HIGH);

  SPIFFS.begin();

  wifiConnect(ssid, pwd);

  if (!MDNS.begin("esp8266")) {             // Start the mDNS responder for esp8266.local
    Serial.println("Error setting up MDNS responder!");
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false);
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/startTimer", HTTP_GET, [](AsyncWebServerRequest *request){   
    String timer;

    // getting parameters from get request
    if (request->hasParam(timerParam)) {
      timer = request->getParam(timerParam)->value();
      int t = timer.toInt();
      if (t == 1) {
        digitalWrite(LED1, LOW);
        
      }
      else {
        digitalWrite(LED2, LOW);
      }
      Serial.println(t);
    }
    else {
      Serial.println("Issue with timer leds get request");
    }
    request->send(200, "text/plain", "OK");
  });

  // turning off led when timer is complete
  server.on("/stopTimer", HTTP_GET, [](AsyncWebServerRequest *request){   
    String timer;

    // getting parameters from get request
    if (request->hasParam(timerParam)) {
      timer = request->getParam(timerParam)->value();
      int t = timer.toInt();
      if (t == 1) {
        digitalWrite(LED1, HIGH);
      }
      else {
        digitalWrite(LED2, HIGH);
      }
    }
    else {
      Serial.println("Issue with timer leds get request");
    }
    request->send(200, "text/plain", "OK");
  });

  server.on("/getTemp", HTTP_GET, [](AsyncWebServerRequest *request){   
    long t = sendTemp();
    char temp[50];
    sprintf(temp, "%lu", t);
    
    request->send(200, "text/plain", temp);
  });
  
  server.begin();

}

void loop() {
  
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

long sendTemp() {
  long randN = random(1, 24);

  return randN;
}
