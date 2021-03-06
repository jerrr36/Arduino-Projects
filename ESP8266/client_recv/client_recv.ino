#include <ESP8266WiFi.h>

#define LED 2
WiFiClient client;

String ssid = "";
String pwd = "";

void wifiConnect(String ssid, String pwd);
void modbusSetup();
int readTCP();

void setup(void){
  Serial.begin(9600);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  wifiConnect(ssid, pwd);
  modbusSetup(); 
  pinMode(LED, OUTPUT);
}



void loop(void){

   //Checking is server disconnected
   if (!client.connected()){
    Serial.println("Lost connection :(");
    modbusSetup();
   }
  
  int val = readTCP();  
  //Serial.println(val);
  
  if (val == 48){
    digitalWrite(LED, LOW);
  } else {
    digitalWrite(LED, HIGH);
  }
  delay(2500);
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


int readTCP(){
  client.write("Available");
  char ch;
  while (client.available()) {
    ch += static_cast<char>(client.read());
    
    
    
  }
  int x = (int) ch;

  //Serial.println(x);
  return x;
  
}
