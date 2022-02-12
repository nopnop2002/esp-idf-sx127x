# ping-pong Example   
Send data from primary to secondary.   
In the secondary, the characters are converted and sent back.   


# Configuration   
![config-top](https://user-images.githubusercontent.com/6020549/152316024-73f1aab9-fb2a-4729-8683-fbcdae3dcc71.jpg)
![config-pingpong-1](https://user-images.githubusercontent.com/6020549/152459294-9d603be1-d6d2-465e-81a5-85b70480c8b8.jpg)

When using a transceiver other than 169MHz / 433MHz / 470MHz / 866MHz / 915MHz:   
![config-pingpong-2](https://user-images.githubusercontent.com/6020549/152459300-5d837868-421b-4971-ae10-6725e91643fe.jpg)

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

# Screen Shot of Primary side   
![pingpong-1](https://user-images.githubusercontent.com/6020549/152316130-784d49eb-a5d9-4858-af54-0979af1948c0.jpg)

# Response time (Unit=MillSecs)   
It depends on the payload size.   
|Payload Size|BW=62.5KHz|BW=125KHz|BW=250KHz|BW=5000KHz|
|:-:|:-:|:-:|:-:|:-:|
|31Bytes|330|190|120|80|
|63Bytes|510|280|160|100|
|127Bytes|900|470|260|160|
|255Bytes|1680|880|480|290|
