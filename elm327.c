/*
 * Uses: ESUART
 */
#define BAUD_N 103 //9600
//#define BAUD_N 25 //38400 (or 26, depending on drift)

//Use delay functions
#include <xc.h>
#include "oled.h"
#include "elm327.h"

char recv_buf[BUFLEN+1], buf_pos; 
/* The cheapest of non-blocking locks */
#define buf_lock() RCIE=0;
#define buf_unlock() RCIE=1;

void elm327_init(void) {
  SPEN = 1; SYNC = 0; TXEN = 1; CREN = 1; //UART enable, transmit and receive
  BRG16 = 1; BRGH=1; SPBRGH = 0; SPBRG = BAUD_N; //Baud rate generator
  PEIE = 1; GIE = 1; RCIE = 1; //Use interrupts for receive
  buf_pos=0;
  recv_buf[BUFLEN] = NULL; //Just in case
}

void uart_outc(char byte) { 
    while(TXIF == 0); //Wait until transmit register is empty
    TXREG = byte;
}

//Outputs a NULL-terminated string through the serial port
void uart_outs(const char *str) {
    for(; *str != 0x0; str++) uart_outc(*str);
}

/* Resets the elm327 */
void elm_reset(void) {
  uart_outs("ATZ");
  END_CMD();
}

/*Unfortunately, since the 887 has only one interrupt handler, we have this
 * total hack of a solution to have this be a separate function. 
 * Hopefully we don't blow up the stack
 */
void handle_elm_interrupt(void) {
  if(RCIF) {
    /* Very simple buffer, just circle back if overrun */
    recv_buf[buf_pos]=RCREG;  
    buf_pos++;
    if(buf_pos>=BUFLEN) {
      buf_pos = 0;
    }
  }
}

void output_buffer(void) {
  char i;
  buf_lock();
  for(i=0;i<buf_pos;i++) {
    oled_outc(recv_buf[i]);
  }
  buf_pos = 0;
  buf_unlock();
}

void clear_buffer(void) {
  buf_lock();
  buf_pos = 0;
  buf_unlock();
}

void elm_vin(void) {
  uart_outs("09 02");
  END_CMD();
}

void obd_connect(void) {
  char i;
  uart_outs("ATSP0");
  uart_outc(0x0D);
  delay_ms(100);
  clear_buffer();
  uart_outs("01 00");
  uart_outc(0x0D);
  delay_ms(100);
  //Block until we find a protocol
  while(1) {
    buf_lock();
    for(i=0;i<buf_pos;i++) {
      if(recv_buf[i] == END_CHAR) {
        buf_unlock();
        return;
      }
    }
    clear_buffer();
    buf_unlock();
    for(i=0;i<10;i++); //Wait for a new character
  }
}

/*Wait until a complete message is in the buffer*/
void elm_wait_for_msg() {
}
