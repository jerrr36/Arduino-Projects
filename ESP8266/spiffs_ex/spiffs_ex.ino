#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>

WiFiClient client;
ESP8266WebServer server(80);

String ssid = "";
String pwd = "";

void wifiConnect(String ssid, String pwd);
void modbusSetup();
void handleroot();
String getContentType(String filename); // convert the file extension to the MIME type
bool handleFileRead(String path);  

void setup(void){
  Serial.begin(9600);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  wifiConnect(ssid, pwd);
  //modbusSetup();

  SPIFFS.begin();                           // Start the SPI Flash Files System
  
  server.onNotFound([]() {                              // If the client requests any URI
    if (!handleFileRead(server.uri()))                  // send it if it exists
      server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
  });

  server.begin();
}



void loop(void){

   /*Checking is server disconnected
   if (!client.connected()){
    Serial.println("Lost connection :(");
    modbusSetup();
   }
   */
   server.handleClient();
   
   
   //client.write(16);
   //delay(500);
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

void handleroot() {
  server.send(200, "text/html", "index.html");
}

String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";         // If a folder is requested, send the index file
  String contentType = getContentType(path);            // Get the MIME type
  if (SPIFFS.exists(path)) {                            // If the file exists
    File file = SPIFFS.open(path, "r");                 // Open it
    size_t sent = server.streamFile(file, contentType); // And send it to the client
    file.close();                                       // Then close the file again
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;                                         // If the file doesn't exist, return false
}
