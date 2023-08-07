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
![config-mqtt-1](https://github.com/nopnop2002/esp-idf-sx127x/assets/6020549/0de324fa-6e1b-4721-b56b-0a7e1483a36d)

## WiFi Setting
![config-mqtt-2](https://github.com/nopnop2002/esp-idf-sx127x/assets/6020549/22c3bdb0-8ac3-4ddf-9e79-1e77fc4855d5)

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

![config-mqtt-4](https://github.com/nopnop2002/esp-idf-sx127x/assets/6020549/3f2ce0d2-6240-4437-a01c-6cbf75503942)

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

![config-mqtt-3](https://github.com/nopnop2002/esp-idf-sx127x/assets/6020549/cfd1f9f6-8a62-4457-b749-11d60e0d5ab9)


### Specifying an MQTT Broker   
MQTT broker is specified by one of the following.
- IP address   
 ```192.168.10.20```   
- mDNS host name   
 ```mqtt-broker.local```   
- Fully Qualified Domain Name   
 ```broker.emqx.io```


