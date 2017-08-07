#include <SPI.h>
#include <Enrf24.h>
#include <nRF24L01.h>
#include <string.h>


#define WHITE_LED P2_3
#define YELLOW_LED P2_4

Enrf24 radio(P2_0, P2_1, P2_2);  // P2.0=CE, P2.1=CSN, P2.2=IRQ
const uint8_t txaddr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x01 };

const char *str_on = "ON";
const char *str_off = "OFF";

void dump_radio_status_to_serialport(uint8_t);

void setup() {
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(1); // MSB-first
  
  radio.begin();  // Defaults 1Mbps, channel 0, max TX power
  dump_radio_status_to_serialport(radio.radioState());
  radio.setTXaddress((void*)txaddr);
  
  pinMode(RED_LED, OUTPUT); 
  digitalWrite(RED_LED, LOW);
  pinMode(YELLOW_LED, OUTPUT);
  digitalWrite(YELLOW_LED, LOW);  
  pinMode(WHITE_LED, OUTPUT);
  digitalWrite(WHITE_LED, LOW);  
  
}

void loop() {
  digitalWrite(WHITE_LED, HIGH);
  radio.print(str_off);
  radio.flush();  // Force transmit (don't wait for any more data)
  dump_radio_status_to_serialport(radio.radioState());  // Should report IDLE
  digitalWrite(WHITE_LED, LOW);
  delay(200);
  
  digitalWrite(WHITE_LED, HIGH);
  radio.print(str_on);
  radio.flush(); 
  dump_radio_status_to_serialport(radio.radioState());  // Should report IDLE
  digitalWrite(WHITE_LED, LOW);
  delay(200);
}

void dump_radio_status_to_serialport(uint8_t status)
{
  switch (status) {
    case ENRF24_STATE_NOTPRESENT:
      digitalWrite(RED_LED, LOW);
      digitalWrite(YELLOW_LED, LOW);
      break;

    case ENRF24_STATE_DEEPSLEEP:
      digitalWrite(RED_LED, HIGH);
       digitalWrite(YELLOW_LED, LOW);
      break;

    case ENRF24_STATE_IDLE:
      digitalWrite(RED_LED, LOW); 
      digitalWrite(YELLOW_LED, HIGH);
      break;

    case ENRF24_STATE_PTX:
      digitalWrite(RED_LED, HIGH);
      digitalWrite(YELLOW_LED, LOW);
      break;

    case ENRF24_STATE_PRX:
      digitalWrite(RED_LED, LOW);
      digitalWrite(YELLOW_LED, HIGH);
      break;

    default:
      digitalWrite(RED_LED, HIGH);
      digitalWrite(YELLOW_LED, HIGH);
  }
}
