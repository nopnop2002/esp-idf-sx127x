# esp-idf-sx127x
SX1276/77/78/79 Low Power Long Range Transceiver driver for esp-idf.


I based on [this](https://github.com/Inteform/esp32-lora-library).

# Changes from the original   
- Changed make to cmake.   
- Added support for ESP32S2, ESP32S3, ESP32C2, ESP32C3 and ESP32C6.   
- I left the control of CS to the driver.   
- Added a sample of ping-pong/http/mqtt/tusb-serial/ws.   
- Added some API functions.   

# Software requirements
ESP-IDF V5.0 or later.   
ESP-IDF V4.4 release branch reached EOL in July 2024.   

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

## Frequency used   
![config-lora-2](https://github.com/user-attachments/assets/91c4b8b8-e18c-4dbb-b880-40d2cd460272)

## Using a transceiver other than 433MHz / 866MHz / 915MHz   
![config-lora-3](https://github.com/user-attachments/assets/62984a47-681e-48f4-a408-d8429fceea58)

## advanced settings   
![Image](https://github.com/user-attachments/assets/1aa25d32-5d32-4831-b305-d5b56ad55eb2)

LoRa mode has the following three communication parameters.   
1.Error Coding Rate (= CR)   
2.Signal Bandwidth (= BW)   
3.Spreading Factor (= SF)   
The communication speed is faster when BW is large, CR is small, and SF is small.   
However, as the communication speed increases, the reception sensitivity deteriorates, so select the one that best suits your needs.   

- Error coding rate   
1:4/5(Default)   
2:4/6   
3:4/7   
4:4/8   

- Signal Bandwidth   
0:7.8 kHz   
1:10.4 kHz   
2:15.6 kHz   
3:20.8kHz   
4:31.25 kHz   
5:41.7 kHz   
6:62.5 kHz   
7:125 kHz(Default)   
8:250 kHz   
9:500 kHz   
In the lower band (169MHz), signal bandwidths 8&9 are not supported.   

- Spreading Factor (expressed as a base-2 logarithm)   
6:64 chips / symbol   
7:128 chips / symbol(Default)   
8:256 chips / symbol   
9:512 chips / symbol   
10:1024 chips / symbol   
11:2048 chips / symbol   
12:4096 chips / symbol   

## SPI BUS selection   
![config-lora-5](https://github.com/user-attachments/assets/f3dcf76e-1bf4-4c05-98ac-f9174f52820e)

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

# About communication speed and maximum reception sensitivity   
In LoRa modulation mode, the communication speed (bps) and maximum reception sensitivity (RSSI) are determined by a combination of spreading factor (SF), bandwidth (BW), and coding rate (CDR).   
- SF   
Increasing SF increases the spreading factor and improves noise resistance.   
This improves reception sensitivity and extends communication distance, but communication speed decreases.   
- BW   
Bandwidth sets the width of the communication band. Setting a larger bandwidth will improve communication speed.   
However, the radio reception sensitivity (RSSI) will decrease.   
- CDR   
CDR (CodingRate) sets the level of error correction rate.   
The larger the number, the better the correction rate, but the amount of information per packet increases.   
(No effect on maximum reception sensitivity)   
You can set whether to use Optimaise for each CDR, and enabling it will improve the correction rate, but will reduce communication speed.   

# Throughput
- With varying Signal Bandwidth(BW) (Unit=Bytes/Sec)   
	ESP32@160/433MHz/CR=1/SF=7   
	|BW=0(7.8KHz)|BW=2(15.6KHz)|BW=4(31.25KHz)|BW=6(62.5KHz)|BW=7(125KHz)|BW=8(250KHz)|BW=9(500KHz)|
	|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
	|37.50|83.18|165.75|296.32|618.93|1153.85|2031.87|

- With varying Error coding rate(CR) (Unit=Bytes/Sec)   
	ESP32@160/433MHz/BW=7/SF=7   
	|CR=1(4/5)|CR=2(4/6)|CR=3(4/7)|CR=4(4/8)|
	|:-:|:-:|:-:|:-:|
	|618.93|543.13|468.32|410.63|


# How to use this component in your project   
Create idf_component.yml in the same directory as main.c.   
```
YourProject --+-- CMakeLists.txt
              +-- main --+-- main.c
                         +-- CMakeLists.txt
                         +-- idf_component.yml
```

Contents of idf_component.yml.
```
dependencies:
  nopnop2002/sx127x:
    path: components/lora/
    git: https://github.com/nopnop2002/esp-idf-sx127x.git
```

When you build a projects esp-idf will automaticly fetch repository to managed_components dir and link with your code.   
```
YourProject --+-- CMakeLists.txt
              +-- main --+-- main.c
              |          +-- CMakeLists.txt
              |          +-- idf_component.yml
              +-- managed_components ----- nopnop2002__sx127x
```

# Datasheet
Datasheet is [here](https://github.com/jgromes/RadioLib/files/8646997/DS_SX1276-7-8-9_W_APP_V7.pdf).   

# Reference
https://github.com/nopnop2002/esp-idf-sx126x

