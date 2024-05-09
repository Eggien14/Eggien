# Trans/receive data with Module RF Zigbee UART CC2530 V2 and ESP32
1. Config module [Reference](http://arduino.vn/tutorial/1479-gioi-thieu-module-thu-phat-rf-cc2530-tam-xa-plus-cc2591-cua-zigbee)
- Baudrate : 9600
- Chanel: same for both
- Point to Point (Red,Green)
2. Pins Connection
- CC2530(Red) : Rx - Tx0, Tx - Rx0
- CC2530(Green) : Rx - Tx2, Tx - Rx2

# Serial Monitor
```
Send data: Ax
Received data: A
Received data: 

Send data: Ax
Received data: A

```
