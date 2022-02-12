# basic Example   
Simple send or receive.   

# Configuration   
![config-top](https://user-images.githubusercontent.com/6020549/152315401-0a3ea44f-d045-4b93-b988-b9ce9451f935.jpg)
![config-basic-1](https://user-images.githubusercontent.com/6020549/152459229-0c198ce6-f0e3-451c-8947-b205d9173b93.jpg)

When using a transceiver other than 169MHz / 433MHz / 470MHz / 866MHz / 915MHz:   
![config-basic-2](https://user-images.githubusercontent.com/6020549/152459233-42b48205-c32c-4947-8e05-09a9b2e27e4d.jpg)

- Signal Bandwidth   
0001:10.4 kHz   
0010:15.6 kHz   
0011:20.8kHz   
0100:31.25 kHz   
0101:41.7 kHz   
0110:62.5 kHz   
0111:125 kHz(Default)   
1000:250 kHz   
1001:500 kHz   
In the lower band (169MHz), signal bandwidths 8&9 are not supported.   

# Throughput(Unit=Bytes/Sec)   
It depends on the payload size.   
|Payload Size|BW=62.5KHz|BW=125KHz|BW=250KHz|BW=5000KHz|
|:-:|:-:|:-:|:-:|:-:|
|31Bytes|190.83|187.17|393.28|776.94|
|63Bytes|134.27|448.38|768.68|793.45|
|127Bytes|290.88|290.48|1064.54|1067.23|
|255Bytes|309.77|586.07|1107.24|1958.50|

# Packet Lost Rate(Unit=Percent)   
It depends on the payload size.   
0 is all packet received.   
50% means that you cannot get half of the packet.   
|Payload Size|BW=62.5KHz|BW=125KHz|BW=250KHz|BW=5000KHz|
|:-:|:-:|:-:|:-:|:-:|
|31Bytes|1|50|49|0|
|63Bytes|49|0|1|50|
|127Bytes|0|49|0|50|
|255Bytes|0|0|1|0|
