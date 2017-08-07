#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <stdlib.h>

char text[255] = {0};
short xvec[99] = {0};
short yvec[99] = {0};
short zvec[99] = {0};
short rpm = 0;
RF24 radio(9, 10);

const uint64_t pipes[2] = {0xE7D3F035FF,  0xE7D3F035C2 }; 
void setup()
{
  while (!Serial);
  Serial.begin(9600);
  
  radio.begin();
  radio.setDataRate(RF24_1MBPS);
  radio.setChannel(76);
  radio.setPALevel(RF24_PA_MAX);  
  radio.enableDynamicPayloads();  
  radio.setCRCLength(RF24_CRC_16);  
  
  radio.openReadingPipe(1, pipes[0]);
  radio.openReadingPipe(2, pipes[1]);
  
  radio.startListening();
}

void loop()
{
if (radio.available()){
 for(int x = 0; x < 30; x++){
   if (radio.available())
  {
      radio.read(&text, sizeof(text));   
      Serial.println(text);
     if (text[0] == 'a'){
       String str(text);
       xvec[x*3] = str.substring(1, 4).toInt();
       yvec[x*3] = str.substring(4, 7).toInt();
       zvec[x*3] = str.substring(7, 10).toInt();
       xvec[(x*3)+1] = str.substring(10, 13).toInt();
       yvec[(x*3)+1] = str.substring(13, 16).toInt();
       zvec[(x*3)+1] = str.substring(16, 19).toInt();
       xvec[(x*3)+2] = str.substring(19, 22).toInt();
       yvec[(x*3)+2] = str.substring(22, 25).toInt();
       zvec[(x*3)+2] = str.substring(25, 28).toInt();
     } else if(text[0] == 'r')
     {  
        String str(text);
        rpm = str.substring(1, 5).toInt();
      }
     delay(50);
   }
 }
}
}

