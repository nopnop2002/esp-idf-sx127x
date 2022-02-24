# ping-pong Example   
Send data from primary to secondary.   
In the secondary, the characters are converted and sent back.   


# Configuration   
![config-top](https://user-images.githubusercontent.com/6020549/152316024-73f1aab9-fb2a-4729-8683-fbcdae3dcc71.jpg)
![config-pingpong-1](https://user-images.githubusercontent.com/6020549/153731874-ca03ed3c-aba0-4273-b0b7-bff611a570c5.jpg)

When using a transceiver other than 169MHz / 433MHz / 470MHz / 866MHz / 915MHz:   
![config-pingpong-2](https://user-images.githubusercontent.com/6020549/153731877-2a72fcca-69ab-4822-8be3-1042ff8e092b.jpg)

When using advanced settings:   
![config-pingpong-3](https://user-images.githubusercontent.com/6020549/153731879-9cfc7705-b7f4-4b2e-9237-cf2eccc54d51.jpg)

LoRa mode has the following three communication parameters.   
1.Signal Bandwidth (= BW)   
2.Error Cording Rate (= CR)   
3.Spreading Factor (= SF)   
The communication speed is faster when BW is large, CR is small, and SF is small.   
However, as the communication speed increases, the reception sensitivity deteriorates, so select the one that best suits your needs.   

- Error coding rate   
001:4/5(Default)   
010:4/6   
011:4/7   
100:4/8   

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

- SF rate (expressed as a base-2 logarithm)   
6:64 chips / symbol   
7:128 chips / symbol(Default)   
8:256 chips / symbol   
9:512 chips / symbol   
10:1024 chips / symbol   
11:2048 chips / symbol   
12:4096 chips / symbol   

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
