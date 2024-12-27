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
![config-pingpong-1](https://github.com/user-attachments/assets/c9b339cf-528a-4712-b32d-7d1da3d5521e)

# Screen Shot of Primary side   
![pingpong-1](https://user-images.githubusercontent.com/6020549/152316130-784d49eb-a5d9-4858-af54-0979af1948c0.jpg)

# Response time with varying BW (Unit=MillSecs)   
ESP32@160/CR=1/SF=7   
|Payload Size|BW=62.5KHz|BW=125KHz|BW=250KHz|BW=500KHz|
|:-:|:-:|:-:|:-:|:-:|
|31Bytes|330|190|120|80|
|63Bytes|510|280|160|100|
|127Bytes|900|470|260|160|
|255Bytes|1680|880|480|290|
