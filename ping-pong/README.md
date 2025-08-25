# Ping-Pong Example   
Send data from primary to secondary.   
In the secondary, the characters are converted and sent back.   

- ESP32 is Primary   
When using Arduino, use Ra01S-Pong.   
```
+-----------+           +-----------+            +-----------+           +-----------+
|           |           |           |            |           |           |           |
|  Primary  |===(SPI)==>|  SX127x   |---(LoRa)-->|  SX127x   |===(SPI)==>| Secondary |
|   ESP32   |           |           |            |           |           |           |
|           |           |           |            |           |           |           |
|           |<==(SPI)===|           |<--(LoRa)---|           |<==(SPI)===|           |
|           |           |           |            |           |           |           |
+-----------+           +-----------+            +-----------+           +-----------+
```

- ESP32 is Secondary   
When using Arduino, use Ra01S-Ping.   
```
+-----------+           +-----------+            +-----------+           +-----------+
|           |           |           |            |           |           |           |
|  Primary  |===(SPI)==>|  SX127x   |---(LoRa)-->|  SX127x   |===(SPI)==>| Secondary |
|           |           |           |            |           |           |   ESP32   |
|           |           |           |            |           |           |           |
|           |<==(SPI)===|           |<--(LoRa)---|           |<==(SPI)===|           |
|           |           |           |            |           |           |           |
+-----------+           +-----------+            +-----------+           +-----------+
```

# Configuration   
![config-top](https://user-images.githubusercontent.com/6020549/152316024-73f1aab9-fb2a-4729-8683-fbcdae3dcc71.jpg)
![config-pingpong-1](https://github.com/user-attachments/assets/c9b339cf-528a-4712-b32d-7d1da3d5521e)

# Screen Shot of Primary side   
![pingpong-1](https://user-images.githubusercontent.com/6020549/152316130-784d49eb-a5d9-4858-af54-0979af1948c0.jpg)

# Response time   
Time (Ticks) required from Start to Finish.   
1 Tick is 10 milliseconds.   
```
+-----------+           +-----------+            +-----------+           +-----------+
|           |Start      |           |            |           |           |           |
|  Primary  |===(SPI)==>|  SX127x   |---(LoRa)-->|  SX127x   |===(SPI)==>| Secondary |
|   ESP32   |           |           |            |           |           |   ESP32   |
|           |Finish     |           |            |           |           |           |
|           |<==(SPI)===|           |<--(LoRa)---|           |<==(SPI)===|           |
|           |           |           |            |           |           |           |
+-----------+           +-----------+            +-----------+           +-----------+
```

### With varying Signal Bandwidth(BW)   
ESP32@160/SX1278@433MHz/CR=1/SF=7   
|Payload|BW=0(7.8KHz)|BW=2(15.6KHz)|BW=4(31.25KHz)|BW=6(62.5KHz)|BW=7(125KHz)|BW=8(250KHz)|BW=9(500KHz)|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|32Bytes|236|120|62|33|18|11|7|
|64Bytes|384|193|99|51|28|15|10|
|128Bytes|695|349|176|90|47|26|14|
|255Bytes|1285|644|324|164|84|44|24|

### With varying Error coding rate(CR)   
ESP32@160/SX1278@433MHz/BW=7/SF=7   
|Payload|CR=1(4/5)|CR=2(4/6)|CR=3(4/7)|CR=4(4/8)|
|:-:|:-:|:-:|:-:|:-:|
|32Bytes|18|20|22|24|
|64Bytes|28|32|35|39|
|128Bytes|47|54|62|70|
|255Bytes|84|99|114|130|
