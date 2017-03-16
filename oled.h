/*Cursor Positions*/
#define OLED_ROW1 0x0
#define OLED_ROW2 0x40

/*Size of the display*/
#define NUMCOLS 16 //16x2 display

/*Just in case...*/
#ifndef NULL
#define NULL '\0'
#endif

extern void oled_init(void);
extern void delay_ms(unsigned int);
extern void oled_clear(void);
extern void oled_send(char, char);
extern void oled_outs(const char *s);
extern void oled_scroll_line2(const char *s);
extern void oled_outc(char);
extern void oled_cursor(char pos);
extern void handle_oled_interrupt(void);
