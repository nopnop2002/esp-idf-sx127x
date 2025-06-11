# USB Serial Device Example   
This is LoRa and USB Serial Device gateway application.   

ESP-IDF supports USB Serial Device.   
As a USB stack, a TinyUSB component is used.   

```
           +---------------+          +---------------+          +---------------+
           |               |          |               |          |               |
           |USB Serial Host|--(USB)-->|     ESP32     |--(SPI)-->|     SX127x    |==(LoRa)==>
           |               |          |               |          |               |
           +---------------+          +---------------+          +---------------+

           +---------------+          +---------------+          +---------------+
           |               |          |               |          |               |
==(LoRa)==>|     SX127x    |--(SPI)-->|     ESP32     |--(USB)-->|USB Serial Host|
           |               |          |               |          |               |
           +---------------+          +---------------+          +---------------+
```

# Hardware requirements
1. ESP32-S2/S3 Development board   
Because the ESP32-S2/S3 does support USB OTG.   

2. USB Connector   
I used this USB Mini femail:   
![usb-connector](https://user-images.githubusercontent.com/6020549/124848149-3714ba00-dfd7-11eb-8344-8b120790c5c5.JPG)

```
ESP32-S2/S3 BOARD          USB CONNECTOR
                           +--+
                           | || VCC
    [GPIO 19]    --------> | || D-
    [GPIO 20]    --------> | || D+
    [  GND  ]    --------> | || GND
                           +--+
```


# Configuration
![config-top](https://github.com/user-attachments/assets/55d44b46-630a-45d7-afc7-a6841ae32298)

### USB to LoRa
Receive from USB and send to LoRa.   
ESP32 acts as USB Serial Device for reading.   
You can use this script as USB Serial Host for writing.   
```python3 ./write.py```

```
           +---------------+          +---------------+          +---------------+
           |               |          |               |          |               |
           |USB Serial Host|--(USB)-->|     ESP32     |--(SPI)-->|     SX127x    |==(LoRa)==>
           |               |          |               |          |               |
           +---------------+          +---------------+          +---------------+
```

![config-radio-1](https://github.com/user-attachments/assets/6e4b1fa8-b54e-48cb-b001-2a71aef22556)

Communicate with Arduino Environment.   
Run this sketch.   
ArduinoCode\Ra01S_RX   


### LoRa to USB
Receive from LoRa and send to  USB.   
ESP32 acts as USB Serial Device for writing.   
You can use this script as USB Serial Host for reading.   
```python3 ./read.py```

```
           +---------------+          +---------------+          +---------------+
           |               |          |               |          |               |
==(LoRa)==>|     SX127x    |--(SPI)-->|     ESP32     |--(USB)-->|USB Serial Host|
           |               |          |               |          |               |
           +---------------+          +---------------+          +---------------+
```

![config-radio-2](https://github.com/user-attachments/assets/b0ced381-2ec4-445c-9f97-6adf455c8bc1)

Communicate with Arduino Environment.   
Run this sketch.   
ArduinoCode\Ra01S_TX   


# Using Windows Terminal Software
When you connect the USB cable to the USB port on your Windows machine and build the firmware, a new COM port will appear.   
Open a new COM port in the terminal software.   
When set to LoRa to USB, the data received from LoRa will be displayed on the terminal.   
When set to USB to LoRa, input data from the keyboard is sent to LoRa.   
I used TeraTerm as terminal software.   
![tusb-TeraTerm](https://github.com/user-attachments/assets/5abec7b7-bfe8-42c1-b945-106e9357035a)

# Using Linux Terminal Software
When you connect the USB cable to the USB port on your Linux machine and build the firmware, a new /dev/tty device will appear.   
Open a new tty device in the terminal software.   
Most occasions, the device is /dev/ttyACM0.   
When set to LoRa to USB, the data received from LoRa will be displayed on the terminal.   
When set to USB to LoRa, input data from the keyboard is sent to LoRa.   
I used screen as terminal software.   
![tusb-screen](https://github.com/user-attachments/assets/dd6fb0bb-a0af-4ebe-9b1b-bb2991cf1b5e)

