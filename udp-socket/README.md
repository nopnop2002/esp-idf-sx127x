# UDP-SOCKET Example   
This is LoRa and UDP-SOCKET gateway application.   
```
           +------------+           +------------+           +------------+
           |            |           |            |           |            |
           | UDP Client |-(UDP/IP)->|    ESP32   |--(SPI)--->|   SX127x   |==(LoRa)==>
           |            |           |            |           |            |
           +------------+           +------------+           +------------+

           +------------+           +------------+           +------------+
           |            |           |            |           |            |
==(LoRa)==>|   SX127x   |--(SPI)--->|    ESP32   |-(UDP/IP)->| UDP Server |
           |            |           |            |           |            |
           +------------+           +------------+           +------------+
```



# Configuration
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/7523fd86-eb28-4f67-a060-5b4ad2ce4021" />
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/a71533a8-3973-4d50-84a5-48b7a47991f5" />

## WiFi Setting
Set the information of your access point.   
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/fdc2f0a2-5493-4be0-ae29-c6cc1bf85dc7" />

## Radio Setting
Set the wireless communication direction.   

### UDP to LoRa
Receive from UDP and send to LoRa.   
ESP32 acts as UDP Server.   
You can use this script as UDP Client.   
```python3 ./udp-client.py```

```
           +------------+           +------------+           +------------+
           |            |           |            |           |            |
           | UDP Client |-(UDP/IP)->|    ESP32   |--(SPI)--->|   SX127x   |==(LoRa)==>
           |            |           |            |           |            |
           +------------+           +------------+           +------------+
```

<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/7407c394-a147-4706-9501-d1ff8e9e4e49" />

Communicate with Arduino Environment.   
I tested it with [this](https://github.com/nopnop2002/esp-idf-sx127x/tree/main/ArduinoCode/Ra01S-RX).   

### LoRa to UDP
Receive from LoRa and send to UDP.   
ESP32 acts as UDP Client.   
You can use this script as UDP Server.   
```python3 ./udp-server.py```

```
           +------------+           +------------+           +------------+
           |            |           |            |           |            |
==(LoRa)==>|   SX127x   |--(SPI)--->|    ESP32   |-(UDP/IP)->| UDP Server |
           |            |           |            |           |            |
           +------------+           +------------+           +------------+
```

<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/048e281c-47c7-422f-8de1-1e79db881e9f" />

Communicate with Arduino Environment.   
I tested it with [this](https://github.com/nopnop2002/esp-idf-sx127x/tree/main/ArduinoCode/Ra01S-TX).   

### Specifying an UDP Address   
There are the following four methods for specifying the UDP Address.   

- Limited broadcast address   
	The address represented by 255.255.255.255, or \<broadcast\>, cannot cross the router.   
	Both the sender and receiver must specify a Limited broadcast address.   
	<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/d2a26c4b-26d1-45d6-bc0f-39dd394a2e2d" />

- Directed broadcast address   
	It is possible to cross the router with an address that represents only the last octet as 255, such as 192.168.10.255.   
	Both the sender and receiver must specify the Directed broadcast address.   
	__Note that it is possible to pass through the router.__   
	<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/3b535eda-9705-4cee-843f-e0279cc03840" />

- Multicast address   
	Data is sent to all PCs belonging to a specific group using a special address (224.0.0.0 to 239.255.255.255) called a multicast address.   
	I've never used it, so I don't know anything more.
	<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/74df801f-c4d6-42ee-b1ca-da07e9b540a3" />

	- Unicast address   
	It is possible to cross the router with an address that specifies all octets, such as 192.168.10.41.   
	Both the sender and receiver must specify the Unicast address.
	<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/79383a16-aaf8-41c1-a02a-d16eeec9d965" />

