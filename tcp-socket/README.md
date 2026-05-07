# TCP-SOCKET Example   
This is LoRa and TCP-SOCKET gateway application.   
```
           +------------+           +------------+           +------------+
           |            |           |            |           |            |
           | TCP Client |-(TCP/IP)->|    ESP32   |--(SPI)--->|   SX127x   |==(LoRa)==>
           |            |           |            |           |            |
           +------------+           +------------+           +------------+

           +------------+           +------------+           +------------+
           |            |           |            |           |            |
==(LoRa)==>|   SX127x   |--(SPI)--->|    ESP32   |-(TCP/IP)->| TCP Server |
           |            |           |            |           |            |
           +------------+           +------------+           +------------+
```



# Configuration
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/1c5a327d-8e93-4053-ae62-ec59deec27a5" />
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/69e7e22b-a728-4b39-9803-5e36236ff582" />

## WiFi Setting
Set the information of your access point.   
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/e3247353-0fb0-4066-8561-752182e8a5ba" />

## Radio Setting
Set the wireless communication direction.   

### TCP to LoRa
Receive from TCP and send to LoRa.   
ESP32 acts as TCP Server.   
You can use this script as TCP Client.   
```python3 ./tcp-client.py```

```
           +------------+           +------------+           +------------+
           |            |           |            |           |            |
           | TCP Client |-(TCP/IP)->|    ESP32   |--(SPI)--->|   SX127x   |==(LoRa)==>
           |            |           |            |           |            |
           +------------+           +------------+           +------------+
```

<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/d8ab22ce-46f4-43c3-bbea-ee1a598998c7" />

Communicate with Arduino Environment.   
I tested it with [this](https://github.com/nopnop2002/esp-idf-sx127x/tree/main/ArduinoCode/Ra01S-RX).   

### LoRa to TCP
Receive from LoRa and send to TCP.   
ESP32 acts as TCP Client.   
You can use this script as TCP Server.   
```python3 ./tcp-server.py```

```
           +------------+           +------------+           +------------+
           |            |           |            |           |            |
==(LoRa)==>|   SX127x   |--(SPI)--->|    ESP32   |-(TCP/IP)->| TCP Server |
           |            |           |            |           |            |
           +------------+           +------------+           +------------+
```

<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/de435083-8d0b-4672-a100-30561af052d4" />

Communicate with Arduino Environment.   
I tested it with [this](https://github.com/nopnop2002/esp-idf-sx127x/tree/main/ArduinoCode/Ra01S-TX).   

### Specifying an TCP Server   
You can specify your TCP Server in one of the following ways:   
- IP address   
 ```192.168.10.20```   
- mDNS host name   
 ```tcp-server.local```   


