#ifndef __OLED_H_
#define	__OLED_H_
#include "pmsis.h"

#define OLED_ADDRESS	0x78 //ͨ������0R����,������0x78��0x7A������ַ -- Ĭ��0x78

struct pi_device I2C;

void I2C_Configuration(void);
void I2C_WriteByte(uint8_t addr,uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);
void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);


void OLED_DrawPoint(unsigned char x0,unsigned char y0,uint8_t data);
void OLED_DrawLine(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1);
void OLED_Fill_Block(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t data);

void OLED_Refresh_Gram(void);




#endif
