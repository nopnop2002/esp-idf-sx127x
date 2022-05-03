#include <SPI.h>
#include <LoRa.h> // https://github.com/sandeepmistry/arduino-LoRa

/*
 * VCC    3V3
 * GND    GND
 * SCK    D13
 * MISO   D12
 * MOSI   D11
 * NSS    D10
 * RST    D9
 */
 
int counter = 0;

//long frequency = 433E6;
long frequency = 915E6;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(frequency)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("Hello World!! ");
  LoRa.print(counter);
  LoRa.endPacket();

  // Wait for responce
  unsigned long startMills = millis();
  while(1) {
    // try to parse packet
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      // received a packet
      Serial.print("Received packet '");
  
      // read packet
      while (LoRa.available()) {
        Serial.print((char)LoRa.read());
      }
  
      // print RSSI of packet
      Serial.print("' with RSSI ");
      Serial.println(LoRa.packetRssi());
      break;
    }
    delay(1);
    
    // Check responce timeout
    if ( (millis() - startMills) > 5000) {
      Serial.println("No response within 5 seconds");
      break;
    }    
  } // end while

  counter++;

  delay(5000);
}
