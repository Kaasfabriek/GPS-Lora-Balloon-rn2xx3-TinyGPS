# GPS-Lora-Balloon-rn2xx3-TinyGPS
Lora location transmittor with TinyGPS and RN2483<br/>
Connect the RN2xx3 as follows:<br/>
 * RN2xx3 -- Arduino
 * Uart TX -- 11
 * Uart RX -- 12
 * Reset -- 10
 * Vcc -- 3.3V
 * Gnd -- Gnd
<br/>
Connect the GPS on ports 0 and 1 (HardwareSerial)<br/>
Cross the Rx and Tx <br/>
<br/>
Change the Device address, App and Networkkey <br/>
To do this create a file keys.h <br/>
In keys.h you write: (with your own keys ofcourse)<br/>
<br/>
String addr = "AAAAAAAA";<br/>
String AppSKey = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";<br/>
String NwkSKey = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";<br/>
<br/>
Libraries required:<br/>
TinyGPS:<br/>
https://github.com/mikalhart/TinyGPS<br/>
RN2483 Arduino Library:<br/>
https://github.com/jpmeijers/RN2483-Arduino-Library<br/>

