// Created by Hi-Tech Software modified for SparkFun Character OLED
// https://github.com/technobly/Adafruit_CharacterOLED/blob/master/Adafruit_CharacterOLED.cpp
// https://www.sparkfun.com/products/11987
// Datasheet:
// http://www.winstar.com.tw/products/oled-module/oled-character-display/weh001602b.html

#include	<xc.h>

/*Configuration Settings*/
#define CLRDISPLAY     0x01    //clear display: clear, move cursor home
#define ENTRYMODE      0x06    //auto increment cursor after each char sent
#define FUNCSET        0x28    //function set: 4-bit mode, 2 lines, 5x7 pixels
#define DISPSET        0x0C    //no cursor, no blink
#define DISPOFF        0x08    //turn off display
#define HOME           0x02    //set cursor at home

/*Pin Assignments*/
#define RS             RE0         
#define RW             RE1        
#define EN             RE2        
#define DB_PINS        PORTC   //DB4-7 are on RC0-3
/* We change between reading and writing for while waiting on the busy flag 
 * these two macros allow us to keep easier track
 */
#define TRIS_BUSY       TRISC3  
#define BUSY_PIN       RC3

/*Direction*/
#define CMD            0           
#define DATA           1       

/*Assuming 4MHz internal clock this will yield a timer 0 tick of 4us*/
void timer0_init() {
  T0CS = 0;
  PSA = 0;
  PS0 =  1;
  PS1 =  0;
  PS2 =  0;
}

/*Since we have 4us per tick, we need 250 ticks for a ms, start TMR0 at 6*/
void delay_ms(unsigned int ms) {
  while (ms != 0) {
    TMR0 = 6;
    T0IF = 0;
    while(T0IF == 0);
    ms--;
  }
}

/* Need to delay at least 50us for timing, here we'll be cheap*/
inline void delay_50us() {
  char x;
  for(x=0; x<50; x++);
}
  
void oled_pulse() {
  EN = 1;
  delay_50us();
  EN = 0;
  delay_50us();
}

/* Wait until the display is free */
void oled_checkbusy() {
  char busy = 1;
  RW = 1;
  RS = 0;
  TRIS_BUSY = 1; //Change DB7 to input      
  do {
    oled_pulse();  //Grab byte containing busy pin
    busy = BUSY_PIN;
    oled_pulse();  //Grab rest of byte
  }while(busy);
  TRIS_BUSY = 0;
  RW = 0;
}

/*Send only 4 bits, used during initialization*/
void oled_4bits(char data) {
  DB_PINS = (DB_PINS&0xF0)|((data>>4)&0x0F);
  delay_50us();
  oled_pulse();
  oled_checkbusy();
}

/*Send a command or data, split up for 8 bits*/
void oled_send(char mode, char output) {
  RS = mode;
  delay_50us();
  DB_PINS = (DB_PINS&0xF0)|((output>>4)&0x0F); //Send upper nibble first
  delay_50us();
  oled_pulse(); 
  DB_PINS = (DB_PINS&0xF0)|(output&0x0F); //Lower nibble comes next
  delay_50us();
  oled_pulse();
  oled_checkbusy();
}

void oled_clear(void) {
  oled_send(CMD, CLRDISPLAY);
  delay_ms(2);
}

/* Write one character to the LCD */
void oled_outc(char c) {
  oled_send(DATA, c);
}

/* Write a string of chars to the display */
void oled_outs(const char *str) {
  while (*str != 0)
      oled_outc(*str++);
}

/*
 * Moves cursor to desired position, locations are as follows:
 *     Row 1: 0x00-0x10
 *     Row 2: 0x40-0x50
 */
void oled_cursor(char pos) {
  oled_send(CMD, 0x80 + pos);
  delay_ms(2);
}

void oled_home() {
  oled_send(CMD, HOME);
}

/* TODO: still need to jam reset... */
void oled_init() {
  unsigned char cmd;
  TRISC0 = 0;
  TRISC1 = 0;
  TRISC2 = 0;
  TRISC3 = 0;
  TRISE0 = 0;
  TRISE1 = 0;
  TRISE2 = 0;
  DB_PINS = DB_PINS&0xF0;//zero out data pins
  timer0_init();
  RS = CMD;
  EN = 0;
  delay_ms(100);

  /* 4-bit initialization sequence from datasheet */
  oled_4bits(2); //Set 4-bit mode
  delay_ms(5);
  oled_send(CMD, FUNCSET);
  delay_ms(5);
  oled_checkbusy();

  oled_send(CMD, DISPSET);
  delay_ms(5);
  oled_checkbusy();

  oled_clear();
  oled_checkbusy();

  oled_send(CMD, ENTRYMODE);
  oled_checkbusy();
}
