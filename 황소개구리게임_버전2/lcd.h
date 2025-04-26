// Lcd.a (Library)
// SPI1 @36MHz (PCLK2=72MHz)

#ifdef __cplusplus
 extern "C" {
#endif 

#define BLACK	0x0000
#define WHITE	0xffff
#define BLUE	0x001f
#define GREEN	0x07e0
#define RED		0xf800
#define YELLOW	0xffe0
#define VIOLET	0xf81f

extern void Lcd_Init(void);
extern void Lcd_Set_Display_Mode(int mode);
extern void Lcd_Set_Cursor(unsigned short x, unsigned short y);
extern void Lcd_Set_Windows(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
extern void Lcd_Put_Pixel(unsigned short x, unsigned short y, unsigned short color);
extern void Lcd_Clr_Screen(void);
extern void Lcd_Draw_Back_Color(unsigned short color);
extern void Lcd_Write_Data_16Bit(unsigned short color);
extern void Lcd_Draw_Box(int xs, int ys, int w, int h, unsigned short Color);

#ifdef __cplusplus
}
#endif