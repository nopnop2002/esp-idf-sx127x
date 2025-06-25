# WebSocket Example   
This is LoRa and WebSocket gateway application.   
```
           +-----------+              +-----------+              +-----------+
           |           |              |           |              |           |
           | Web Client|-(WebSocket)->|   ESP32   |----(SPI)---->|  SX127x   |==(LoRa)==>
           |           |              |           |              |           |
           +-----------+              +-----------+              +-----------+

           +-----------+              +-----------+              +-----------+
           |           |              |           |              |           |
==(LoRa)==>|  SX127x   |----(SPI)---->|   ESP32   |-(WebSocket)->| Web Server|
           |           |              |           |              |           |
           +-----------+              +-----------+              +-----------+
```



# Configuration
![config-top](https://github.com/user-attachments/assets/ab7b2141-48e6-423d-98d2-7730eb17bc90)
![config-app](https://github.com/user-attachments/assets/8c513633-4144-4fc8-bddb-9f8c346d9980)

## WiFi Setting
Set the information of your access point.   
![config-wifi](https://github.com/user-attachments/assets/51bf839d-8d2f-41ab-b3a7-dbea09836ce0)

## Radio Setting
Set the wireless communication direction.   

### WebSocket to LoRa
Receive from WebSocket and send to LoRa.   
ESP32 acts as Web Server.   
You can use ws-client.py as Web Client.   
You need to install websockets from here.   
https://github.com/python-websockets/websockets   

```
           +-----------+              +-----------+              +-----------+
           |           |              |           |              |           |
           | Web Client|-(WebSocket)->|   ESP32   |----(SPI)---->|  SX127x   |==(LoRa)==>
           |           |              |           |              |           |
           +-----------+              +-----------+              +-----------+
```

![config-radio-1](https://github.com/user-attachments/assets/50296104-d749-427f-9c61-a8e36a7a4431)

Communicate with Arduino Environment.   
I tested it with [this](https://github.com/nopnop2002/esp-idf-sx127x/tree/main/ArduinoCode/LoRaReceiver).   

### LoRa to WebSocket
Receive from LoRa and send to WebSocket.   
ESP32 acts as Web Client.   
You can use ws-server.py as Web Server.   
You need to install websocket-server from here.   
https://github.com/Pithikos/python-websocket-server   

```
           +-----------+              +-----------+              +-----------+
           |           |              |           |              |           |
==(LoRa)==>|  SX127x   |----(SPI)---->|   ESP32   |-(WebSocket)->| Web Server|
           |           |              |           |              |           |
           +-----------+              +-----------+              +-----------+
```

![config-radio-2](https://github.com/user-attachments/assets/da7bb160-0932-47a3-96b5-b072d0861c6f)

Communicate with Arduino Environment.   
I tested it with [this](https://github.com/nopnop2002/esp-idf-sx127x/tree/main/ArduinoCode/LoRaSender).   

### Specifying an WebSocket Server   
You can specify your WebSocket Server in one of the following ways:   
- IP address   
 ```192.168.10.20```   
- mDNS host name   
 ```http-server.local```   


