# Example of Arduino environment   
I use [this](https://github.com/sandeepmistry/arduino-LoRa).   

Unfortunately, this library has incomplete mode switching.   
Therefore, Ping/Pong does not work reliably.   

# Wirering

|SX127x||ATmega328||
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

# Configuration

- Radio frequency   
	```
	#define RF_FREQUENCY                                433000000 // Hz  center frequency
	```

- Signal Bandwidth   
	```
	#define LORA_BANDWIDTH                             125E3      // bandwidth 7
	```

- Error Coding Rate   
	```
	#define LORA_CODINGRATE                             5         // Error coding rate
	```

- Spreading Factor (expressed as a base-2 logarithm)   
	```
	#define LORA_SPREADING_FACTOR                       7         // spreading factor [SF6..SF12]
	```

# Application   
- LoRaSender   
	Send only.   
- LoRaReceiver   
	Receive only.   
- LoRaReceiverCallback   
	Receive only using callback function .   
- LoRaPing   
	Wait for a response after sending.   
- LoRaPong   
	Send a response after receiving.   
