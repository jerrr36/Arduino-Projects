#include <Arduino.h>
#include <SPI.h>
#include <Sdfat.h>

#define SD_FAT_VERSION 2
#define cs 4

SdFat SD;
File myFile;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  if (!SD.begin(cs)) {
    Serial.println("Initialization failed");
    return;
  }
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}


void loop() {
  
}