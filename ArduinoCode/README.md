# Example of Arduino environment   
I use [this](https://github.com/sandeepmistry/arduino-LoRa).   

# Wirering

|SX127x||ATmega||
|:-:|:-:|:-:|:-:
|RST|--|D9|*1|
|NSS|--|D10|*1|
|MOSI|--|D11|*1|
|MISO|--|D12||
|SCK|--|D13|*1|
|DIO0|--|D2|*1 *2|
|VCC|--|3.3V||
|GND|--|GND||

(*1)   
SX127x is not 5V tolerant.   
You need level shift from 5V to 3.3V.   
I used [this](https://www.ti.com/lit/ds/symlink/txs0108e.pdf?ts=1647593549503) for a level shift.   


(*2)   
DIO0 pin is optional, it is only needed for receive callback mode.
