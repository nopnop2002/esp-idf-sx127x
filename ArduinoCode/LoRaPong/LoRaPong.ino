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

#define RF_FREQUENCY                                433000000 // Hz  center frequency
//#define RF_FREQUENCY                                866000000 // Hz  center frequency
//#define RF_FREQUENCY                                915000000 // Hz  center frequency

//#define LORA_BANDWIDTH                              7.8E3     // bandwidth 0
//#define LORA_BANDWIDTH                             10.4E3     // bandwidth 1
//#define LORA_BANDWIDTH                             15.6E3     // bandwidth 2
//#define LORA_BANDWIDTH                             20.8E3     // bandwidth 3
//#define LORA_BANDWIDTH                             31.2E3     // bandwidth 4
//#define LORA_BANDWIDTH                             41.7E3     // bandwidth 5
//#define LORA_BANDWIDTH                             62.5E3     // bandwidth 6
#define LORA_BANDWIDTH                             125E3      // bandwidth 7
//#define LORA_BANDWIDTH                             250E3      // bandwidth 8
//#define LORA_BANDWIDTH                             500E3      // bandwidth 9

#define LORA_SPREADING_FACTOR                       7         // spreading factor [SF6..SF12]

#define LORA_CODINGRATE                             5         // Error coding rate
                                                              //  5: 4/5
                                                              //  6: 4/6
                                                              //  7: 4/7
                                                              //  8: 4/8


void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(RF_FREQUENCY)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
  LoRa.setSignalBandwidth(LORA_BANDWIDTH);
  LoRa.setSpreadingFactor(LORA_SPREADING_FACTOR);
  LoRa.setCodingRate4(LORA_CODINGRATE); 
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

    // send packet
    LoRa.beginPacket();
    LoRa.write(txData, rxLen);
    LoRa.endPacket();
    Serial.println("Sending packet");
  }
}
