#include <PubSubClient.h>
#include <ESP8266WiFi.h>


String ssid = "";
String pwd = "";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

void wifiConnect(String ssid, String pwd);
void callback(char* topic, byte* payload, unsigned int length);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  wifiConnect(ssid, pwd);
  delay(2000);
  //tcp connexction to mqttserver
  client.setServer(mqtt_server, 1883);

  client.setCallback(callback);

  while(!client.connected()) {
    String clientid = "JSHESP8266";
    Serial.println("connecting to hivemq");
    if (client.connect(clientid.c_str())) {
      Serial.println("Connected");
      client.publish("esp/test","Hello");
      client.subscribe("esp/test");
    } else {
      Serial.print("failed =");
      Serial.println(client.state());
      delay(5000);
    }
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  client.loop();
  client.publish("esp/test","hello");
  delay(2000);

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

void callback(char* topic, byte* payload, unsigned int length){
  Serial.println(topic);
  Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
  
  
}
