extern void elm327_init(void);
extern char uart_inc(void);
extern void uart_outc(char byte);
extern void uart_outs(const char *str);
extern void elm_reset(void);
extern void elm_vin(void);
