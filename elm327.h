extern void elm327_init(void);
extern char uart_inc(void);
extern void uart_outc(char byte);
extern void uart_outs(const char *str);
extern void elm_reset(void);
extern void elm_vin(void);
extern void elm_wait_for_msg(void);
extern void handle_elm_interrupt(void);

//All commands must end with CR, can also be used to repeat commands
#define END_CMD() uart_outc(0x0D)
#define END_CHAR 0x3E //The ELM327 is ready for commands when we see '>'

/* UART receive buffer */
#define BUFLEN 32
