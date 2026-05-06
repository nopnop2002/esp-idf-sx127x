# Web Form Example   
Data entered through the web form is sent to LoRa.   
Data received from LoRa is displayed in a web form.   
```
           +-----------+              +-----------+              +-----------+
           |           |              |           |              |           |
           |  WebForm  |-(WebSocket)->|   ESP32   |----(SPI)---->|  SX127x   |==(LoRa)==>
           |           |              |           |              |           |
           +-----------+              +-----------+              +-----------+

           +-----------+              +-----------+              +-----------+
           |           |              |           |              |           |
==(LoRa)==>|  SX127x   |----(SPI)---->|   ESP32   |-(WebSocket)->|  WebForm  |
           |           |              |           |              |           |
           +-----------+              +-----------+              +-----------+
```

I used [this](https://github.com/Molorius/esp32-websocket) component.   
This component can communicate directly with the browser.   
It's a great job.   
<img width="844" height="779" alt="Image" src="https://github.com/user-attachments/assets/eec0862a-d599-4b93-ada2-1844a2369ff5" />

# Configuration
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/6b93ad2b-9a41-4976-ab28-302758dd299c" />
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/581da032-a266-4958-ba4d-d15b960b7931" />

## WiFi Setting
Set the information of your access point.   
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/4e99f6ea-af86-4207-9d17-b0be39805a17" />

## Radio Setting
Set the wireless communication direction.   

### Web to LoRa
Data entered through the web form is sent to LoRa.   
```
           +-----------+              +-----------+              +-----------+
           |           |              |           |              |           |
           |  WebForm  |-(WebSocket)->|   ESP32   |----(SPI)---->|  SX127x   |==(LoRa)==>
           |           |              |           |              |           |
           +-----------+              +-----------+              +-----------+
```

<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/5bc4bc57-c8c4-41db-b34a-e56369d5e67c" />

### LoRa to Web
Data received from LoRa is displayed in a web form.   

```
           +-----------+              +-----------+              +-----------+
           |           |              |           |              |           |
==(LoRa)==>|  SX127x   |----(SPI)---->|   ESP32   |-(WebSocket)->|  WebForm  |
           |           |              |           |              |           |
           +-----------+              +-----------+              +-----------+
```

<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/e3ffa440-4b28-47e6-aa5e-182cc17a019d" />

# Launch a web browser
Enter the following in the address bar of your web browser.   
```
http:://{IP of ESP32}/
or
http://esp32-server.local/
```
<img width="816" height="811" alt="Image" src="https://github.com/user-attachments/assets/6957cd33-2270-47d2-8667-63d0cbf08c0c" />
<img width="816" height="811" alt="Image" src="https://github.com/user-attachments/assets/35fba682-454f-4972-85a8-7154729ea275" />

### Web to LoRa
Enter the data to send in the TextBox and press the Send button.   
<img width="816" height="811" alt="Image" src="https://github.com/user-attachments/assets/d555f253-a8bd-49df-a55d-1178d1cf3666" />

### LoRa to Web
The received data will be displayed in the TextBox.   
The Change button changes the number of lines displayed.   
The Copy button copies the received data to the clipboard.   
<img width="816" height="811" alt="Image" src="https://github.com/user-attachments/assets/e7a2aa05-ec21-4cba-a6f2-5d751f34c743" />
<img width="816" height="875" alt="Image" src="https://github.com/user-attachments/assets/3d1a0486-acb9-47a0-8ab9-65cf6a218df7" />

# WEB Pages
WEB Pages are stored in the html folder.   
I used [this](https://bulma.io/) open source css.   
You can change root.html as you like.   


