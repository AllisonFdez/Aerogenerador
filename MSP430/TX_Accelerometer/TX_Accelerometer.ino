#include <SPI.h>
#include <Enrf24.h>
#include <nRF24L01.h>
#include <string.h>

#include "MspFlash.h"

const unsigned char data[2*SEGMENT_SIZE] = {0};
#define flash SEGPTR(data)

Enrf24 radio(P2_0, P2_1, P2_2);  // P2.0=CE, P2.1=CSN, P2.2=IRQ
const uint8_t txaddr[] = { 0xE7, 0xD3, 0xF0, 0x35, 0xC2 };
 
const short xpin = A0; 
const short ypin = A3;
const short zpin = A4;
short xvalue[43] = {0};        // value read from xpin
short yvalue[43] = {0};        // value read from ypin
short zvalue[43] = {0};        // value read from zpin
short bufer = 0;
unsigned char p;

void setup() {
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(1); // MSB-first
 
  radio.begin(1000000);  // Defaults 1Mbps, channel 0, max TX power
  radio.setChannel(76);
  radio.setCRC(1,1); 
  radio.setTXaddress((void*)txaddr);
  Flash.erase(flash);
}
 
void loop() {
    
   Flash.erase(flash);
   //Aqui con flash
   for (int x = 0; x<60; x++){
     bufer = analogRead(xpin);
     Flash.write(flash+(9*x), (unsigned char*) String(bufer).c_str() ,3); 
     bufer = analogRead(ypin);
     Flash.write(flash+(9*x)+3, (unsigned char*) String(bufer).c_str() ,3); 
     bufer = analogRead(zpin);
     Flash.write(flash+(9*x)+6, (unsigned char*) String(bufer).c_str() ,3); 
     delay(15);
   }

   // Toma 43 medidas de acelaración
   for (int x = 0; x <43; x++){
     xvalue[x] = analogRead(xpin);
     yvalue[x] = analogRead(ypin);
     zvalue[x] = analogRead(zpin);
      delay(10);
   };
   
   //Mando datos en flash
for(int x = 0; x<19; x++){
  bufer = 27*x;
  radio.print("a");
  do
  {
    Flash.read(flash+bufer, &p, 1);
    radio.write(p);
   } while ( p && (bufer++ < (27*(x+1) - 1)) );
   radio.flush();
   sleep(1000);
}
   Flash.erase(flash);
   sleep(1500);
 //Mando datos en ram
 for(int y = 0; y<14; y++){
  radio.print("a");   
  for (int x = 3*y; x<((3*y)+3); x++){
    radio.print(xvalue[x]);
    radio.print(yvalue[x]);
    radio.print(zvalue[x]);
  }
 radio.flush();
 sleep(1000);
}
 radio.deepsleep();
 sleep(120000); //Optimizar con Pardi
 radio.begin(1000000);  // Defaults 1Mbps, channel 0, max TX power
 radio.setChannel(76);
 radio.setCRC(1,1); 
 radio.setTXaddress((void*)txaddr);
}
