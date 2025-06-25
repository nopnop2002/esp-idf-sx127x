# MQTT Example   
This is LoRa and MQTT gateway application.   
```
           +----------+           +----------+           +----------+           +----------+
           |          |           |          |           |          |           |          |
           |Publisher |--(MQTT)-->|  Broker  |--(MQTT)-->|  ESP32   |--(SPI)--->|  SX127x  |==(LoRa)==>
           |          |           |          |           |          |           |          |
           +----------+           +----------+           +----------+           +----------+

           +----------+           +----------+           +----------+           +----------+
           |          |           |          |           |          |           |          |
==(LoRa)==>|  SX127x  |--(SPI)--->|  ESP32   |--(MQTT)-->|  Broker  |--(MQTT)-->|Subscriber|
           |          |           |          |           |          |           |          |
           +----------+           +----------+           +----------+           +----------+
```


# Configuration
![Image](https://github.com/user-attachments/assets/815a3629-324e-4ee8-bf90-e8b72a889866)
![Image](https://github.com/user-attachments/assets/0b89ee93-c4d4-4562-9dc5-81401cf7b6cc)

## WiFi Setting
Set the information of your access point.   
![config-wifi](https://github.com/user-attachments/assets/95e73a2a-e40f-4b76-9451-43894dc01ad9)

## Radio Setting
Set the wireless communication direction.   
![Image](https://github.com/user-attachments/assets/063d2d26-8945-4354-8b5b-ab73ace96090)

### MQTT to LoRa   
 Subscribe with MQTT and send to LoRa.   
 You can use mosquitto_pub as Publisher.   
 ```mosquitto_pub -h broker.emqx.io -p 1883 -t "/topic/lora/test" -m "test"```

```
           +----------+           +----------+           +----------+           +----------+
           |          |           |          |           |          |           |          |
           |Publisher |--(MQTT)-->|  Broker  |--(MQTT)-->|  ESP32   |--(SPI)--->|  SX127x  |==(LoRa)==>
           |          |           |          |           |          |           |          |
           +----------+           +----------+           +----------+           +----------+
```

Communicate with Arduino Environment.   
I tested it with [this](https://github.com/nopnop2002/esp-idf-sx127x/tree/main/ArduinoCode/LoRaReceiver).   

### LoRa to MQTT   
 Receive from LoRa and publish as MQTT.   
 You can use mosquitto_sub as Subscriber.   
 ```mosquitto_sub -h broker.emqx.io -p 1883 -t "/topic/lora/test"```

```
           +----------+           +----------+           +----------+           +----------+
           |          |           |          |           |          |           |          |
==(LoRa)==>|  SX127x  |--(SPI)--->|  ESP32   |--(MQTT)-->|  Broker  |--(MQTT)-->|Subscriber|
           |          |           |          |           |          |           |          |
           +----------+           +----------+           +----------+           +----------+
```

Communicate with Arduino Environment.   
I tested it with [this](https://github.com/nopnop2002/esp-idf-sx127x/tree/main/ArduinoCode/LoRaSender).   

## Broker Setting
Set the information of your MQTT broker.   
![Image](https://github.com/user-attachments/assets/b4c57d56-b050-41c5-95ff-609e6848cf95)

### Select Transport   
This project supports TCP,SSL/TLS,WebSocket and WebSocket Secure Port.   

- Using TCP Port.   
	TCP Port uses the MQTT protocol.   

- Using SSL/TLS Port.   
	SSL/TLS Port uses the MQTTS protocol instead of the MQTT protocol.   

- Using WebSocket Port.   
	WebSocket Port uses the WS protocol instead of the MQTT protocol.   

- Using WebSocket Secure Port.   
	WebSocket Secure Port uses the WSS protocol instead of the MQTT protocol.   

__Note for using secure port.__   
The default MQTT server is ```broker.emqx.io```.   
If you use a different server, you will need to modify ```getpem.sh``` to run.   
```
chmod 777 getpem.sh
./getpem.sh
```

WebSocket/WebSocket Secure Port may differ depending on the broker used.   
If you use a different MQTT server than the default, you will need to change the port number from the default.   

### Specifying an MQTT Broker   
You can specify your MQTT broker in one of the following ways:   
- IP address   
 ```192.168.10.20```   
- mDNS host name   
 ```mqtt-broker.local```   
- Fully Qualified Domain Name   
 ```broker.emqx.io```

You can use this as broker.   
https://github.com/nopnop2002/esp-idf-mqtt-broker

### Select MQTT Protocol   
This project supports MQTT Protocol V3.1.1/V5.   
![Image](https://github.com/user-attachments/assets/07773bd6-ebb6-4a4c-a3a6-4eb3e383dd7e)

### Secure Option
Specifies the username and password if the server requires a password when connecting.   
[Here's](https://www.digitalocean.com/community/tutorials/how-to-install-and-secure-the-mosquitto-mqtt-messaging-broker-on-debian-10) how to install and secure the Mosquitto MQTT messaging broker on Debian 10.   
![Image](https://github.com/user-attachments/assets/8d73bf16-b080-4638-bb6b-21d6002d3138)

