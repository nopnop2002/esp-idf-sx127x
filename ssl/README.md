# SSL Example   
This is LoRa and SSL gateway application.   
Receive from LoRa and send to SSL Server.   
ESP32 acts as SSL Client.   

```
           +-----------+           +-----------+           +-----------+
           |           |           |           |           |           |
==(LoRa)==>|  SX127x   |--(SPI)--->|   ESP32   |--(SSL)--->| SSL Server|
           |           |           |           |           |           |
           +-----------+           +-----------+           +-----------+
```

# Installation
```
git clone https://github.com/nopnop2002/esp-idf-sx127x
cd esp-idf-sx127x/ssl/
chmod 777 mkkey.sh
./mkkey.sh
idf.py menuconfig
idf.py flash
```

mkkey.sh creates a server certificate file.   
The server certificate file is associated with the SSL server's IP address.   
mkkey.sh automatically retrieves the IP address of the server on which the script is executed and treats that address as an SSL server.   
To manually configure the SSL server's IP address, modify the script as follows:   
```
IP="192.168.0.123"
openssl req -x509 -new -nodes -key server.key -subj "/CN=${IP}" -days 10000 -out server.crt
```


# Configuration
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/3904d130-319e-4353-aede-3113e2382660" />
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/bf7de489-b97b-40c6-99a8-fd019281ea62" />

## WiFi Setting
Set the information of your access point.   
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/81d9de33-35f1-4cad-848c-8fc3bc8d5365" />

## Server Setting
Set the information of your SSL server.   
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/ebe9ba19-8813-4acb-9002-3b7a16bde76c" />

# Start the SSL server
- C language
	```
	cd clang-tls-communication
	make
	./server
	```
	<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/f817ade0-90c6-4bee-ba74-a35cd2fd272e" />

- python script
	```
	cd python-tls-communication
	python3 server.py
	```
	<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/a7e59d47-b1bf-4dd2-a10b-3c7dcc558887" />

