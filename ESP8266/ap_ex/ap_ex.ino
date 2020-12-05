#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ESP8266mDNS.h>


ESP8266WebServer server(80);

String ssid = "esp";
String pwd = "jis#1";

#define LED D0


IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);


bool toggle;
void toggleled();
void handleroot();
String getContentType(String filename); // convert the file extension to the MIME type
bool handleFileRead(String path);  

void setup(void){
  Serial.begin(9200);         // Start the Serial communication to send messages to the computer
  delay(10);
  pinMode(LED, OUTPUT);
  toggle = false;
  Serial.println('\n');

  while (WiFi.softAP("esp", "jis#1son") == false){
    Serial.println("Initiating wifi");
    delay(5000);
    
  }
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  if (!MDNS.begin("esp8266", WiFi.softAPIP())) {             // Start the mDNS responder for esp8266.local
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS responder started");
  
  SPIFFS.begin();                           // Start the SPI Flash Files System
  
  server.onNotFound([]() {                              // If the client requests any URI
    if (!handleFileRead(server.uri()))                  // send it if it exists
      server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
  });
  server.on("/toggleled", HTTP_POST, toggleled);
  server.begin();
  Serial.println(WiFi.softAPIP());
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



void toggleled(){
  toggle = !toggle;
  if (toggle == false) {
    digitalWrite(LED, HIGH);
  }
  else {
    digitalWrite(LED, LOW);
  }
  server.send(200, "text/html", "toggleled.html");
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
