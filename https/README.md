# HTTPS Example   
This is LoRa and HTTPS gateway application.   
Receive from LoRa and send to HTTPS Server.   
ESP32 acts as HTTPS Client.   

```
           +-----------+           +-----------+            +------------+
           |           |           |           |            |            |
==(LoRa)==>|  SX127x   |--(SPI)--->|   ESP32   |--(HTTPS)-->|HTTPS Server|
           |           |           |           |            |            |
           +-----------+           +-----------+            +------------+
```

# Installation
```
git clone https://github.com/nopnop2002/esp-idf-sx127x
cd esp-idf-sx127x/https/
chmod 777 mkkey.sh
./mkkey.sh
idf.py menuconfig
idf.py flash
```

mkkey.sh creates a server certificate file.   
The server certificate file is associated with the HTTPS server's IP address.   
mkkey.sh automatically retrieves the IP address of the server on which the script is executed and treats that address as an HTTPS server.   
To manually configure the HTTPS server's IP address, modify the script as follows:   
```
IP="192.168.0.123"
openssl req -x509 -new -nodes -key server.key -subj "/CN=${IP}" -days 10000 -out server.crt
```


# Configuration
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/40908d37-c394-46dc-ba2e-b3687cd5024e" />
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/070dfad9-ee16-45ea-850b-55f050054757" />

## WiFi Setting
Set the information of your access point.   
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/ddfc0879-b20e-4ef3-91a9-2d8967766935" />

## Server Setting
Set the information of your HTTPS server.   
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/7e931a84-657b-48f5-8b71-e0b44eac15b7" />

# Start the HTTPS server
```
python3 https-server.py
```
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/f1324a5d-c89d-4765-a9ef-ed1bb4254672" />

