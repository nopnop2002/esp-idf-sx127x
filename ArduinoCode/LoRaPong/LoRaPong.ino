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
  uint8_t txData[255];
  uint8_t rxData[255];

  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.println("Received packet");

    // read packet
    int8_t rxLen = LoRa.available();
    Serial.print("Received packet rxLen=");
    Serial.println(rxLen);
    for(int i=0;i<rxLen;i++) {
      rxData[i] = LoRa.read();
      char c = rxData[i];
      Serial.print(c);
    }
    Serial.println();

    for(int i=0;i<rxLen;i++) {
      if (isupper(rxData[i])) {
        txData[i] = tolower(rxData[i]);
      } else {
        txData[i] = toupper(rxData[i]);
      }
    }

#if 1
    // send packet
    LoRa.beginPacket();
    LoRa.write(txData, rxLen);
    LoRa.endPacket();
    Serial.println("LoRa.endPacket");
#endif
    
#if 0
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
#endif

  
  }
}
