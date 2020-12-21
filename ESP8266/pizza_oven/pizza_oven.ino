#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include <max6675.h>
#include <PubSubClient.h>

// leds for timers
#define LED1 D0
#define LED2 D4

// wifi network and password
const char* ssid = "";
const char* pwd = "";

// mqtt server
const char* mqtt_server = "broker.hivemq.com";

// html parameter for timer #
const char* timerParam = "timer";

// pins for thermocouple
int thermoSO = 12;
int thermoCS = 13;
int thermoCLK = 14;

// init thermocouple
MAX6675 tc(thermoCLK, thermoCS, thermoSO);


// starting webserver and mqtt client
AsyncWebServer server(80);
WiFiClient espClient;
PubSubClient client(espClient);

// prototypes
void wifiConnect(String ssid, String pwd);
float getTemp();
void callback(char* topic, byte* payload, unsigned int length);
void mqttConnect();

void setup() {
  // for logging
  Serial.begin(9600);

  // Initializing leds
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, HIGH);

  pinMode(LED2, OUTPUT);
  digitalWrite(LED2, HIGH);

  // starting filesystem
  SPIFFS.begin();

  // connecting to wifi
  wifiConnect(ssid, pwd);

  // starting mdns currently not working
  if (!MDNS.begin("esp8266")) {             
    Serial.println("Error setting up MDNS responder!");
  }

  // starting mqtt connection
  mqttConnect();

  // responses to various http requests
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
    char temp[10];
    sprintf(temp, "%.02f", getTemp());
    client.publish("jsh/oventemp", temp);   
    request->send(200, "text/plain", temp);
  });
  
  server.begin();

}

void loop() {
  
}

// wifi connect function
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

// get temp from thermocouple
float getTemp() {
  float t = tc.readFahrenheit();
  return t;
}

// connect to mqtt broker
void mqttConnect() {
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  while(!client.connected()) {
    String clientid = "JSHESP8266";
    Serial.println("connecting to hivemq");
    if (client.connect(clientid.c_str())) {
      Serial.println("Connected");
    } else {
      Serial.print("failed =");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length){
  Serial.println(topic);
  Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
  
  
}
