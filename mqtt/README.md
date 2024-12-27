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
![config-top](https://user-images.githubusercontent.com/6020549/152316024-73f1aab9-fb2a-4729-8683-fbcdae3dcc71.jpg)
![config-app](https://github.com/user-attachments/assets/16a98416-0797-441d-b84b-efd6a036854a)

## WiFi Setting
![config-wifi](https://github.com/user-attachments/assets/95e73a2a-e40f-4b76-9451-43894dc01ad9)

## Radio Setting


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

![config-radio-1](https://github.com/user-attachments/assets/f0fdb99e-7ffa-4160-a7cd-999050501556)

Communicate with Arduino Environment.   
Run this sketch.   
ArduinoCode\LoRaReceiver   

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

![config-radio-2](https://github.com/user-attachments/assets/b1539ff0-7a37-4d03-9b3a-4c02c0128718)

Communicate with Arduino Environment.   
Run this sketch.   
ArduinoCode\LoRaSender   

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

### Secure Option
Specifies the username and password if the server requires a password when connecting.   
[Here's](https://www.digitalocean.com/community/tutorials/how-to-install-and-secure-the-mosquitto-mqtt-messaging-broker-on-debian-10) how to install and secure the Mosquitto MQTT messaging broker on Debian 10.   
![config-radio-3](https://github.com/user-attachments/assets/7318534c-895f-4e5e-ae90-e875da8e92f8)

