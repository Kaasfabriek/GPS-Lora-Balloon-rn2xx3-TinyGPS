/*
 * Author: JP Meijers
 * Date: 2016-02-07
 * Previous filename: TTN-Mapper-TTNEnschede-V1
 * 
 * This program is meant to be used with an Arduino UNO or NANO, conencted to an RNxx3 radio module.
 * It will most likely also work on other compatible Arduino or Arduino compatible boards, like The Things Uno, but might need some slight modifications.
 * 
 * Transmit a one byte packet via TTN. This happens as fast as possible, while still keeping to 
 * the 1% duty cycle rules enforced by the RN2483's built in LoRaWAN stack. Even though this is 
 * allowed by the radio regulations of the 868MHz band, the fair use policy of TTN may prohibit this.
 * 
 * CHECK THE RULES BEFORE USING THIS PROGRAM!
 * 
 * CHANGE ADDRESS!
 * Change the device address, network (session) key, and app (session) key to the values 
 * that are registered via the TTN dashboard.
 * The appropriate line is "myLora.initABP(XXX);" or "myLora.initOTAA(XXX);"
 * When using ABP, it is advised to enable "relax frame count".
 * 
 * Connect the RN2xx3 as follows:
 * RN2xx3 -- Arduino
 * Uart TX -- 11
 * Uart RX -- 12
 * Reset -- 10
 * Vcc -- 3.3V
 * Gnd -- Gnd
 * 
 * If you use an Arduino with a free hardware serial port, you can replace 
 * the line "rn2xx3 myLora(mySerial);"
 * with     "rn2xx3 myLora(SerialX);"
 * where the parameter is the serial port the RN2xx3 is connected to.
 * Remember that the serial port should be initialised before calling initTTN().
 * For best performance the serial port should be set to 57600 baud, which is impossible with a software serial port.
 * If you use 57600 baud, you can remove the line "myLora.autobaud();".
 * 
 */
#include <rn2xx3.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include "keys.h"

SoftwareSerial mySerial(11, 12); // RX, TX

//create an instance of the rn2xx3 library, 
//giving the software serial as port to use
rn2xx3 myLora(mySerial);


HardwareSerial &ss = Serial;
TinyGPS gps;
    
// the setup routine runs once when you press reset:
void setup() 
{
  ss.begin(9600);
  //output LED pin
  pinMode(13, OUTPUT);
  led_on();
  
  // Open serial communications and wait for port to open:
  
  mySerial.begin(9600); //serial port to radio
  //Serial.println("Startup");

  initialize_radio();

  //transmit a startup message
  myLora.tx("a!");

  led_off();
  delay(2000);
}

void initialize_radio()
{
  //reset rn2483
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
  delay(500);
  digitalWrite(10, HIGH);

  delay(100); //wait for the RN2xx3's startup message
  mySerial.flush();

  //Autobaud the rn2483 module to 9600. The default would otherwise be 57600.
  myLora.autobaud();

  //check communication with radio
  String hweui = myLora.hweui();
  while(hweui.length() != 16)
  {
    //Serial.println("Communication with RN2xx3 unsuccesful. Power cycle the board.");
    //Serial.println(hweui);
    delay(10000);
    hweui = myLora.hweui();
  }

  //print out the HWEUI so that we can register it via ttnctl
  //Serial.println("When using OTAA, register this DevEUI: ");
  //Serial.println(myLora.hweui());
  //Serial.println("RN2xx3 firmware version:");
  //Serial.println(myLora.sysver());

  //configure your keys and join the network
  //Serial.println("Trying to join TTN");
  bool join_result = false;
  
  //ABP: initABP(String addr, String AppSKey, String NwkSKey);
  
  join_result = myLora.initABP(addr, AppSKey, NwkSKey);
  
  //OTAA: initOTAA(String AppEUI, String AppKey);
  //join_result = myLora.initOTAA("???", "???");

  while(!join_result)
  {
    //Serial.println("Unable to join. Are your keys correct, and do you have TTN coverage?");
    delay(60000); //delay a minute before retry
    join_result = myLora.init();
  }
  //Serial.println("Successfully joined TTN");
  
}

// the loop routine runs over and over again forever:
void loop() 
{    
  
   float flat, flon, alt;
    unsigned long age, date, time, chars = 0;
    unsigned short sentences = 0, failed = 0;
    static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
    int hdopNumber; 
    
    delay(1000);    
    led_on();
    
  
    unsigned long start = millis();
    do 
    {
      while (ss.available())
        gps.encode(ss.read());
    } while (millis() - start < 2000);
    
    
    
    hdopNumber = gps.hdop();
    gps.f_get_position(&flat, &flon, &age);
    
    alt = gps.f_altitude();
    gps.stats(&chars, &sentences, &failed);
    myLora.tx("" + String(long(flat*1000000)) + " " + String(long(flon*1000000)) + " " + String(int(alt)) + " " + String(int(hdopNumber*100)) ); //one byte, blocking function
    
    led_off();
    delay(1000);
}

void led_on()
{
  digitalWrite(13, 1);
}

void led_off()
{
  digitalWrite(13, 0);
}

