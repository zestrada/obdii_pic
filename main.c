/*
 * OBDII Reader
 * Output on OLED
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF       // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF        // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)


#include <xc.h>
#include "oled.h"
#include "elm327.h"

/*Note: you need to wait at least 100msec between commands*/
void main(void) {
  ANSEL = 0x0;
  ANSELH = 0x0;
  oled_init();
  oled_init(); //HACK for cold boots
  oled_clear();
  elm327_init();
  while(1) {
    oled_scroll_line2("Testing 1... 2... 3...");
    /*
    oled_clear();
    oled_home();
    oled_outs("Resetting...");
    oled_cursor(OLED_ROW2);
    elm_reset();
    delay_ms(100);
    output_buffer();
    delay_ms(1000);

    oled_clear();
    oled_home();
    oled_outs("Connecting...");
    oled_cursor(OLED_ROW2);
    obd_connect();
    output_buffer();
    oled_clear();
    oled_home();
    output_buffer();

    //FIXME: get another command working, my car doesn't support VIN
    oled_clear();
    oled_home();
    oled_outs("Reading VIN...");
    elm_vin();
    oled_cursor(OLED_ROW2);
    delay_ms(100);
    output_buffer();
    delay_ms(1000);
    */
  }
  return;
}

void interrupt interrupt_handler(void) {
  handle_elm_interrupt();
  handle_oled_interrupt();
}
