# GPS-Lora-Balloon-rn2xx3-TinyGPS
Lora location transmittor with TinyGPS and RN2483
Connect the RN2xx3 as follows:
 * RN2xx3 -- Arduino
 * Uart TX -- 11
 * Uart RX -- 12
 * Reset -- 10
 * Vcc -- 3.3V
 * Gnd -- Gnd

Connect the GPS on ports 0 and 1 (HardwareSerial)
Cross the Rx and Tx 

Change the Device address, App and Networkkey

Libraries required:
TinyGPS:
https://github.com/mikalhart/TinyGPS
RN2483 Arduino Library:
https://github.com/jpmeijers/RN2483-Arduino-Library

