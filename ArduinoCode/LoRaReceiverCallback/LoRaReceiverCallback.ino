#include <SPI.h>
#include <LoRa.h>

#ifdef ARDUINO_SAMD_MKRWAN1300
#error "This example is not compatible with the Arduino MKR WAN 1300 board!"
#endif

/*
 * VCC    3V3
 * GND    GND
 * SCK    D13
 * MISO   D12
 * MOSI   D11
 * NSS    D10
 * RST    D9
 * DIO0   D2
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

  Serial.println("LoRa Receiver Callback");

  if (!LoRa.begin(RF_FREQUENCY)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.setSignalBandwidth(LORA_BANDWIDTH);
  LoRa.setSpreadingFactor(LORA_SPREADING_FACTOR);
  LoRa.setCodingRate4(LORA_CODINGRATE);    

  // Uncomment the next line to disable the default AGC and set LNA gain, values between 1 - 6 are supported
  // LoRa.setGain(6);
  
  // register the receive callback
  LoRa.onReceive(onReceive);

  // put the radio into receive mode
  LoRa.receive();
}

void loop() {
  // do nothing
}

void onReceive(int packetSize) {
  // received a packet
  Serial.print("Received packet '");

  // read packet
  for (int i = 0; i < packetSize; i++) {
    Serial.print((char)LoRa.read());
  }

  // print RSSI of packet
  Serial.print("' with RSSI ");
  Serial.println(LoRa.packetRssi());
}
