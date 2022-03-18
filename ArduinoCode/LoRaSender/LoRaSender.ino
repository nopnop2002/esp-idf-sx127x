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

long frequency = 433E6;
//long frequency = 915E6;

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

  counter++;

  delay(5000);
}
