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
 Subscribe using mosquitto_sub.   
 ```mosquitto_sub -h broker.emqx.io -p 1883 -t "/topic/lora/test"```

```
           +----------+           +----------+           +----------+           +----------+
           |          |           |          |           |          |           |          |
==(LoRa)==>|  SX127x  |--(SPI)--->|  ESP32   |--(MQTT)-->|  Broker  |--(MQTT)-->|Subscriber|
           |          |           |          |           |          |           |          |
           +----------+           +----------+           +----------+           +----------+
```

![config-mqtt-3](https://github.com/nopnop2002/esp-idf-sx127x/assets/6020549/88562fb1-4ae7-45f8-8043-8c3b2cebfe51)

- As the sender   
 Subscribe with MQTT and send to LoRa.   
 Publish using mosquitto_pub.   
 ```mosquitto_pub -h broker.emqx.io -p 1883 -t "/topic/lora/test" -m "test"```

```
           +----------+           +----------+           +----------+           +----------+
           |          |           |          |           |          |           |          |
           |Publisher |--(MQTT)-->|  Broker  |--(MQTT)-->|  ESP32   |--(SPI)--->|  SX127x  |==(LoRa)==>
           |          |           |          |           |          |           |          |
           +----------+           +----------+           +----------+           +----------+
```


![config-mqtt-4](https://github.com/nopnop2002/esp-idf-sx127x/assets/6020549/83f34dde-4a94-4408-95a6-d75051185797)




