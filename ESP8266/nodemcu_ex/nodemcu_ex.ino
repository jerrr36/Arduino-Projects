#include <ESP8266WiFi.h>


WiFiClient client;

String ssid = "SecretNinja007";
String pwd = "I<3Tango";

void wifiConnect(String ssid, String pwd);
void modbusSetup();

void setup(void){
  Serial.begin(9600);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  wifiConnect(ssid, pwd);
  modbusSetup(); 
}



void loop(void){

   //Checking is server disconnected
   if (!client.connected()){
    Serial.println("Lost connection :(");
    modbusSetup();
   }
   
   
   
   
   client.write(16);
   delay(500);
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


void modbusSetup(){
  while (!client.connect("192.168.86.44",5555)) {
      Serial.print(".");        
    }
    
  Serial.println('\n');
  Serial.println("Connected via modbus");
  //Serial.println(client.connect("192.168.86.44",5555));
}
