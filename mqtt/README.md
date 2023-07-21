# mqtt Example   
This is LoRa and MQTT gateway application.   
```
           +----------+           +----------+           +----------+           +----------+
           |          |           |          |           |          |           |          |
==(LoRa)==>|  SX127x  |--(SPI)--->|  ESP32   |--(MQTT)-->|  Broker  |--(MQTT)-->|Subscriber|
           |          |           |          |           |          |           |          |
           +----------+           +----------+           +----------+           +----------+

           +----------+           +----------+           +----------+           +----------+
           |          |           |          |           |          |           |          |
           |Publisher |--(MQTT)-->|  Broker  |--(MQTT)-->|  ESP32   |--(SPI)--->|  SX127x  |==(LoRa)==>
           |          |           |          |           |          |           |          |
           +----------+           +----------+           +----------+           +----------+
```


# Configuration   
![config-top](https://user-images.githubusercontent.com/6020549/152316024-73f1aab9-fb2a-4729-8683-fbcdae3dcc71.jpg)
![config-mqtt-1](https://github.com/nopnop2002/esp-idf-sx127x/assets/6020549/0de324fa-6e1b-4721-b56b-0a7e1483a36d)

### WiFi Setting
![config-mqtt-2](https://github.com/nopnop2002/esp-idf-sx127x/assets/6020549/22c3bdb0-8ac3-4ddf-9e79-1e77fc4855d5)

### Radio Setting

- As the receiver   
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

![config-mqtt-3](https://github.com/nopnop2002/esp-idf-sx127x/assets/6020549/f3d35df3-8579-4fee-838a-594f21a576f1)

- As the sender   
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


![config-mqtt-4](https://github.com/nopnop2002/esp-idf-sx127x/assets/6020549/81ef2e9e-6620-4a37-b6db-79409c825e6d)


MQTT broker is specified by one of the following.
- IP address   
 ```192.168.10.20```   
- mDNS host name   
 ```mqtt-broker.local```   
- Fully Qualified Domain Name   
 ```broker.emqx.io```


