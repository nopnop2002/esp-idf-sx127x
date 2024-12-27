# HTTP Example   
This is LoRa and HTTP gateway application.   
```
           +-----------+           +-----------+           +-----------+
           |           |           |           |           |           |
           |HTTP Client|--(HTTP)-->|   ESP32   |--(SPI)--->|  SX127x   |==(LoRa)==>
           |           |           |           |           |           |
           +-----------+           +-----------+           +-----------+

           +-----------+           +-----------+           +-----------+
           |           |           |           |           |           |
==(LoRa)==>|  SX127x   |--(SPI)--->|   ESP32   |--(HTTP)-->|HTTP Server|
           |           |           |           |           |           |
           +-----------+           +-----------+           +-----------+
```



# Configuration
![config-top](https://github.com/nopnop2002/esp-idf-sx127x/assets/6020549/70cf366a-03dd-4500-8be3-d73434eed3d9)
![config-app](https://github.com/nopnop2002/esp-idf-sx127x/assets/6020549/a6b508f2-4501-4d48-9b70-bdfae1bd8932)

## WiFi Setting

![config-wifi](https://github.com/nopnop2002/esp-idf-sx127x/assets/6020549/594779a0-698b-4afd-b119-230e0477c362)


## Radio Setting

### HTTP to LoRa
Subscribe with HTTP and send to LoRa.   
ESP32 acts as HTTP Server.   
You can use curl as HTTP Client.   
```sh ./http-client.sh```

```
           +-----------+           +-----------+           +-----------+
           |           |           |           |           |           |
           |HTTP Client|--(HTTP)-->|   ESP32   |--(SPI)--->|  SX127x   |==(LoRa)==>
           |           |           |           |           |           |
           +-----------+           +-----------+           +-----------+
```

![config-radio-1](https://github.com/user-attachments/assets/df15b3d0-1d80-4f81-bec1-8b5b3b59314a)

Communicate with Arduino Environment.   
Run this sketch.   
ArduinoCode\LoRaReceiver   


### LoRa to HTTP
Receive from LoRa and publish as HTTP.   
ESP32 acts as HTTP Client.   
You can use nc(netcat) as HTTP Server.   
```sh ./http-server.sh```

```
           +-----------+           +-----------+           +-----------+
           |           |           |           |           |           |
==(LoRa)==>|  SX127x   |--(SPI)--->|   ESP32   |--(HTTP)-->|HTTP Server|
           |           |           |           |           |           |
           +-----------+           +-----------+           +-----------+
```

![config-radio-2](https://github.com/user-attachments/assets/b4df6c8f-8cf4-4c40-93f0-265ae1951ef3)

Communicate with Arduino Environment.   
Run this sketch.   
ArduinoCode\LoRaSender   


### Specifying an HTTP Server   
You can specify your HTTP Server in one of the following ways:   
- IP address   
 ```192.168.10.20```   
- mDNS host name   
 ```http-server.local```   
- Fully Qualified Domain Name   
 ```http-server.public.io```


