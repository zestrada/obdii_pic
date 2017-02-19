/*Cursor Positions*/
#define OLED_ROW1 0x0
#define OLED_ROW2 0x40

extern void oled_init(void);
extern void delay_ms(unsigned int);
extern void oled_clear(void);
extern void oled_send(char, char);
extern void oled_outs(const char *s);
extern void oled_outc(char);
extern void oled_cursor(char pos);
