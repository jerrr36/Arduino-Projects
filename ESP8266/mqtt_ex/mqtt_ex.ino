#include <PubSubClient.h>
#include <ESP8266WiFi.h>


String ssid = "****";
String pwd = "*****";
String mqtt_server = "broker.hivemq.com";

WiFiClient client;
PubSubClient psclient(client);

#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

void wifiConnect(String ssid, String pwd);

void setup() {
  // put your setup code here, to run once:
  wifiConnect(ssid, pwd);
  delay(2000);
  //tcp connexction to mqttserver
  psclient.setServer(mqtt_server, 1883);

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
