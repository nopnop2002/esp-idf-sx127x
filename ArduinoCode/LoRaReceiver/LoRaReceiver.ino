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

long frequency = 433E6;
//long frequency = 915E6;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(frequency)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("Starting LoRa Receiver");
}

void loop() {
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
  }
}
