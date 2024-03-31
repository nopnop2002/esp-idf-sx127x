# Ping-Pong Example   
Send data from primary to secondary.   
In the secondary, the characters are converted and sent back.   

- ESP32 is Primary   
When using Arduino, use Ra01S-Pong.   
```
+-----------+            +-----------+           +-----------+
|           |            |           |           |           |
| Secondary |<==(LoRa)===|  SX127x   |<--(SPI)---|   ESP32   |
|           |            |           |           |  Primary  |
|           |            |           |           |           |
|           |===(LoRa)==>|           |---(SPI)-->|           |
|           |            |           |           |           |
+-----------+            +-----------+           +-----------+
```

- ESP32 is Secondary   
When using Arduino, use Ra01S-Ping.   
```
+-----------+            +-----------+           +-----------+
|           |            |           |           |           |
|  Primary  |===(LoRa)==>|  SX127x   |---(SPI)-->|   ESP32   |
|           |            |           |           | Secondary |
|           |            |           |           |           |
|           |<==(LoRa)===|           |<--(SPI)---|           |
|           |            |           |           |           |
+-----------+            +-----------+           +-----------+
```

# Configuration   
![config-top](https://user-images.githubusercontent.com/6020549/152316024-73f1aab9-fb2a-4729-8683-fbcdae3dcc71.jpg)
![config-pingpong-1](https://user-images.githubusercontent.com/6020549/153731874-ca03ed3c-aba0-4273-b0b7-bff611a570c5.jpg)

When using a transceiver other than 169MHz / 433MHz / 470MHz / 866MHz / 915MHz:   
![config-pingpong-2](https://user-images.githubusercontent.com/6020549/153731877-2a72fcca-69ab-4822-8be3-1042ff8e092b.jpg)

When using advanced settings:   
![config-pingpong-3](https://user-images.githubusercontent.com/6020549/153731879-9cfc7705-b7f4-4b2e-9237-cf2eccc54d51.jpg)

# Screen Shot of Primary side   
![pingpong-1](https://user-images.githubusercontent.com/6020549/152316130-784d49eb-a5d9-4858-af54-0979af1948c0.jpg)

# Response time (Unit=MillSecs)   
It depends on the payload size.   
|Payload Size|BW=62.5KHz|BW=125KHz|BW=250KHz|BW=500KHz|
|:-:|:-:|:-:|:-:|:-:|
|31Bytes|330|190|120|80|
|63Bytes|510|280|160|100|
|127Bytes|900|470|260|160|
|255Bytes|1680|880|480|290|
