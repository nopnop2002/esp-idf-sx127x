# esp-idf-sx127x
SX1276/77/78/79 Low Power Long Range Transceiver driver for esp-idf.


I based on [this](https://github.com/Inteform/esp32-lora-library).

# Changes from the original   
- Changed make to cmake.   
- Added support for ESP32S2, ESP32S3, ESP32C2, ESP32C3 and ESP32C6.   
- I left the control of CS to the driver.   
- Added a sample of ping-pong/http/mqtt.   
- Added some API functions.   

# Software requirements
ESP-IDF V4.4/V5.x.   
ESP-IDF V5.0 is required when using ESP32-C2.   
ESP-IDF V5.1 is required when using ESP32-C6.   

# Installation

```Shell
git clone https://github.com/nopnop2002/esp-idf-sx127x
cd esp-idf-sx127x/basic/
idf.py set-target {esp32/esp32s2/esp32s3/esp32c2/esp32c3/esp32c6}
idf.py menuconfig
idf.py flash
```

__Note for ESP32C3__   
For some reason, there are development boards that cannot use GPIO06, GPIO08, GPIO09, GPIO19 for SPI clock pins.   
According to the ESP32C3 specifications, these pins can also be used as SPI clocks.   
I used a raw ESP-C3-13 to verify that these pins could be used as SPI clocks.   


# Configuration for Transceiver   

![config-lora-1](https://user-images.githubusercontent.com/6020549/152313802-d88ed3ab-dff5-4fe5-a05f-742c2e6e0aa4.jpg)
![config-lora-2](https://user-images.githubusercontent.com/6020549/168179087-8cba5e92-4519-461a-934e-bf05853c6959.jpg)


# SPI BUS selection   
![config-lora-3](https://user-images.githubusercontent.com/6020549/168179130-7f69187a-a44d-45b1-8d39-ca09205c1a4a.jpg)

The ESP32 series has three SPI BUSs.   
SPI1_HOST is used for communication with Flash memory.   
You can use SPI2_HOST and SPI3_HOST freely.   
When you use SDSPI(SD Card via SPI), SDSPI uses SPI2_HOST BUS.   
When using this module at the same time as SDSPI or other SPI device using SPI2_HOST, it needs to be changed to SPI3_HOST.   
When you don't use SDSPI, both SPI2_HOST and SPI3_HOST will work.   
Previously it was called HSPI_HOST / VSPI_HOST, but now it is called SPI2_HOST / SPI3_HOST.   

# Wirering

|SX127X||ESP32|ESP32-S2/S3|ESP32-C2/C3/C6|
|:-:|:-:|:-:|:-:|:-:|
|RST|--|GPIO16|GPIO38|GPIO3|
|MISO|--|GPIO19|GPIO37|GPIO4|
|SCK|--|GPIO18|GPIO36|GPIO5|
|MOSI|--|GPIO23|GPIO35|GPIO6|
|NSS|--|GPIO15|GPIO34|GPIO7|
|GND|--|GND|GND|GND|
|VCC|--|3.3V|3.3V|3.3V|

__You can change it to any pin using menuconfig.__   


# Communication with SX126X
LoRa's packet format is strictly specified.   
Therefore, if the following three parameters are the same, they can communicate with each other.   
- Signal Bandwidth (= BW)   
- Error Cording Rate (= CR)   
- Spreading Factor (= SF)   

# Datasheet
Everything is in the datasheet.   
https://github.com/jgromes/RadioLib/files/8646997/DS_SX1276-7-8-9_W_APP_V7.pdf   


# Reference

https://github.com/nopnop2002/esp-idf-sx126x

