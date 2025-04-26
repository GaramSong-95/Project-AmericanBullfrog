#ifdef __cplusplus
 extern "C" {
#endif 

// graphics.c

extern void Lcd_Eng_Putch(int x,int y,int color,int bkcolor,int data, int zx, int zy);
extern void Lcd_Puts(int x, int y, int color, int bkcolor, const char *str, int zx, int zy);
extern void Lcd_Printf(int x, int y, int color, int bkcolor, int zx, int zy, const char *fmt, ...);

#ifdef __cplusplus
}
#endif