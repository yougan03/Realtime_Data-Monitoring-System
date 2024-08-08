#define CS0 (1<<7)
typedef unsigned int u32;
typedef unsigned int s32;
typedef unsigned char u8;
typedef unsigned char s8;
typedef unsigned short int u16;

extern void delay_sec(unsigned int sec); ///////////// delay
extern void delay_ms(unsigned int ms);

extern void lcd_cmd(unsigned char cmd);	   //////////// lcd
extern void lcd_init(void);
extern void lcd_string(char *ptr);
extern void lcd_int(int num);
extern void lcd_float(float f);
extern void lcd_data(unsigned char data);
extern void lcd_cgram(s8 *a);

extern void i2c_byte_write_frame(u8 sa,u8 mr,u8 data);	 ////////// i2c
extern u8 i2c_byte_read_frame(u8 sa, u8 mr);
extern void i2c_init(void);


extern void uart0_init(u32 baud);		  /////// uart0
extern void uart0_tx(u8 data);
extern unsigned char uart0_rx(void);
extern void uart0_tx_string(char *ptr);
extern void uart0_rx_string(s8 *ptr, u8 len);
extern void uart0_tx_integer(s32 num);
extern void uart0_tx_float(float num);


extern void spi0_init(void);  //// spi
extern u8 spi0(u8 data);

extern u16 read_mcp3204(u8 ch_num);	 //// mcp3204


extern void adc_init(void);						 ////////////// adc
extern unsigned int adc_read(unsigned char ch_num);


