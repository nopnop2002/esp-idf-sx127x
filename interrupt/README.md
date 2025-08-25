# Interrupt Example   
The SX127x supports the following interrupts.   
This project demonstrates how to use TxDone or RxDone.   

|DIOx Mapping|DIO5|DIO4|DIO3|DIO2|DIO1|DIO0|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|0x00|ModeReady|CadDetected|CadDone|FhssChangeChannel|RxTimeout|RxDone|
|0x01|ClkOut|PllLock|ValidHeadere|FhssChangeChannel|FhssChangeChannel|TxDone|
|0x02|ClkOut|PllLock|PayloadCrcError|FhssChangeChannel|CadDetected|CadDone|
|0x03|-|-|-|-|-|-|

# Configuration   
![Image](https://github.com/user-attachments/assets/381a395e-e04f-4fdd-9579-f8353d44245b)
![Image](https://github.com/user-attachments/assets/c679652c-8957-4142-97df-22c361d89653)

# Wirering
Add a wire to DIO1.   
|SX127X||ESP32|ESP32-S2/S3|ESP32-C2/C3/C6|
|:-:|:-:|:-:|:-:|:-:|
|RST|--|GPIO16|GPIO38|GPIO3|
|MISO|--|GPIO19|GPIO37|GPIO4|
|SCK|--|GPIO18|GPIO36|GPIO5|
|MOSI|--|GPIO23|GPIO35|GPIO6|
|NSS|--|GPIO15|GPIO34|GPIO7|
|GND|--|GND|GND|GND|
|VCC|--|3.3V|3.3V|3.3V|
|DIO1|--|GPIO21|GPIO1|GPIO1|

# How to use interrupt
When TxDone occurs, DIO1 turns ON.   
```
    // Set up an interrupt for SPI transactions
    int dio = 0;
    int mode = 1;
    lora_set_dio_mapping(dio, mode)
```

When RxDone occurs, DIO1 turns ON.   
```
    // Set up an interrupt for SPI transactions
    int dio = 0;
    int mode = 0;
    lora_set_dio_mapping(dio, mode);
```

