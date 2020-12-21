/************************************************************************************
*  Copyright (c), 2014, HelTec Automatic Technology co.,LTD.
*            All rights reserved.
*
* Http:    www.heltec.cn
* Email:   cn.heltec@gmail.com
* WebShop: heltec.taobao.com
*
* File name: OLED_I2C.c
* Project  : HelTec.uvprij
* Processor: STM32F103C8T6
* Compiler : MDK fo ARM
* 
* Author : С��
* Version: 1.00
* Date   : 2014.4.8
* Email  : hello14blog@gmail.com
* Modification: none
* 
* Description:128*64�����OLED��ʾ�������ļ����������ڻ����Զ���(heltec.taobao.com)��SD1306����IICͨ�ŷ�ʽ��ʾ��
*
* Others: none;
*
* Function List:
*	1. void I2C_Configuration(void) -- ����CPU��Ӳ��I2C
* 2. void I2C_WriteByte(uint8_t addr,uint8_t data) -- ��Ĵ�����ַдһ��byte������
* 3. void WriteCmd(unsigned char I2C_Command) -- д����
* 4. void WriteDat(unsigned char I2C_Data) -- д����
* 5. void OLED_Init(void) -- OLED����ʼ��
* 6. void OLED_SetPos(unsigned char x, unsigned char y) -- ������ʼ������
* 7. void OLED_Fill(unsigned char fill_Data) -- ȫ�����
* 8. void OLED_CLS(void) -- ����
* 9. void OLED_ON(void) -- ����
* 10. void OLED_OFF(void) -- ˯��
* 11. void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize) -- ��ʾ�ַ���(�����С��6*8��8*16����)
* 12. void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N) -- ��ʾ����(������Ҫ��ȡģ��Ȼ��ŵ�codetab.h��)
* 13. void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]) -- BMPͼƬ
*
* History: none;
*
*************************************************************************************/

#include "OLED.h"
#include "codetab.h"

uint8_t OLED_GRAM[128][8];

void OLED_Refresh_Gram(void)
{
	uint8_t i, n;
	for (i = 0; i < 8; i++)
	{
		WriteCmd(0xb0 + i);
		WriteCmd(0x00);
		WriteCmd(0x10);
		for (n = 0; n < 128; n++)
		{
			WriteDat(OLED_GRAM[n][i]);
		}
	}
}

void Single_Write(unsigned char REG_Address, unsigned char REG_data)
{
	unsigned char TxData[2] = {REG_Address, REG_data};

	pi_i2c_write(&I2C, (uint8_t *)TxData, 2, PI_I2C_XFER_STOP | PI_I2C_XFER_START);
	// pi_i2c_write(&I2C,(uint8_t*)TxData+1,1,PI_I2C_XFER_STOP |PI_I2C_XFER_NO_START);

	// while(HAL_I2C_Master_Transmit(&hi2c2,Slave_Address,(uint8_t*)TxData,2,1000) != HAL_OK)
	// {
	//     if (HAL_I2C_GetError(&hi2c2) != HAL_I2C_ERROR_AF)
	//             {
	//               Error_Handler();
	//             }
	// }
}
void WriteCmd(unsigned char I2C_Command) //д����
{
	Single_Write(0x00, I2C_Command);
}

void WriteDat(unsigned char I2C_Data) //д����
{
	Single_Write(0x40, I2C_Data);
}

void OLED_Init(void)
{
	pi_time_wait_us(100); //�������ʱ����Ҫ

	WriteCmd(0xAE); //display off
	WriteCmd(0x20); //Set Memory Addressing Mode
	WriteCmd(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0); //Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc0); //Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //���ȵ��� 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
}

void OLED_SetPos(unsigned char x, unsigned char y) //������ʼ������
{
	WriteCmd(0xb0 + y);
	WriteCmd(((x & 0xf0) >> 4) | 0x10);
	WriteCmd((x & 0x0f) | 0x01);
}

void OLED_Fill(unsigned char fill_Data) //ȫ�����
{
	unsigned char m, n;
	for (m = 0; m < 8; m++)
	{
		WriteCmd(0xb0 + m); //page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for (n = 0; n < 128; n++)
		{
			WriteDat(fill_Data);
		}
	}
}

void OLED_CLS(void) //����
{
	OLED_Fill(0x00);
}

//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          :
// Parameters     : none
// Description    : ��OLED�������л���
//--------------------------------------------------------------
void OLED_ON(void)
{
	WriteCmd(0X8D); //���õ�ɱ�
	WriteCmd(0X14); //������ɱ�
	WriteCmd(0XAF); //OLED����
}

//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          :
// Parameters     : none
// Description    : ��OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
//--------------------------------------------------------------
void OLED_OFF(void)
{
	WriteCmd(0X8D); //���õ�ɱ�
	WriteCmd(0X10); //�رյ�ɱ�
	WriteCmd(0XAE); //OLED����
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
// Calls          :
// Parameters     : x,y -- ��ʼ������(x:0~127, y:0~7); ch[] -- Ҫ��ʾ���ַ���; TextSize -- �ַ���С(1:6*8 ; 2:8*16)
// Description    : ��ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
//--------------------------------------------------------------
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
	//	OLED_CLS();
	unsigned char c = 0, i = 0, j = 0;
	switch (TextSize)
	{
	case 1:
	{
		while (ch[j] != '\0')
		{

			if (ch[j] == '\n')
			{
				x += 6;
				j++;
				continue;
			}
			c = ch[j] - 32;
			if (x > 126)
			{
				x = 0;
				y++;
			}
			OLED_SetPos(x, y);
			for (i = 0; i < 6; i++)
				WriteDat(F6x8[c][i]);
			x += 6;
			j++;
		}
	}
	break;
	case 2:
	{
		while (ch[j] != '\0')
		{
			c = ch[j] - 32;
			if (x > 120)
			{
				x = 0;
				y++;
			}
			OLED_SetPos(x, y);
			for (i = 0; i < 8; i++)
				WriteDat(F8X16[c * 16 + i]);
			OLED_SetPos(x, y + 1);
			for (i = 0; i < 8; i++)
				WriteDat(F8X16[c * 16 + i + 8]);
			x += 8;
			j++;
		}
	}
	break;
	}
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
// Calls          :
// Parameters     : x,y -- ��ʼ������(x:0~127, y:0~7); N:������codetab.h�е�����
// Description    : ��ʾcodetab.h�еĺ���,16*16����
//--------------------------------------------------------------
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm = 0;
	unsigned int adder = 32 * N;
	OLED_SetPos(x, y);
	for (wm = 0; wm < 16; wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x, y + 1);
	for (wm = 0; wm < 16; wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
}

//--------------------------------------------------------------
// Prototype      : void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
// Calls          :
// Parameters     : x0,y0 -- ��ʼ������(x0:0~127, y0:0~7); x1,y1 -- ���Խ���(������)������(x1:1~128,y1:1~8)
// Description    : ��ʾBMPλͼ
//--------------------------------------------------------------
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
	unsigned int j = 0;
	unsigned char x, y;

	if (y1 % 8 == 0)
		y = y1 / 8;
	else
		y = y1 / 8 + 1;
	for (y = y0; y < y1; y++)
	{
		OLED_SetPos(x0, y);
		for (x = x0; x < x1; x++)
		{
			WriteDat(BMP[j++]);
		}
	}
}
void OLED_DrawPoint(unsigned char x0, unsigned char y0, uint8_t data)
{

	uint8_t pos, bx, temp = 0;
	if (x0 > 127 || y0 > 63)
		return;
	pos = 7 - y0 / 8;
	bx = y0 % 8;
	temp = 1 << (7 - bx);
	if (data)
		OLED_GRAM[x0][pos] |= temp;
	else
	{
		OLED_GRAM[x0][pos] &= ~temp;
	}

	// OLED_Refresh_Gram();
	// OLED_SetPos(x0, y0 / 8 + 1);
	// uint8_t p2;
	// p2 = y0 % 8;
	// WriteDat(0x01 << p2);
}

void OLED_Fill_Block(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t data)
{
	uint8_t x, y;
	for (x = x0; x < x1; x++)
	{
		for (y = y0; y < y1; y++)
		{
			OLED_DrawPoint(x, y, data);
		}
	}
	OLED_Refresh_Gram();
}

void OLED_DrawLine(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1)
{
	int x;
	int k = (y1 - y0) / (x1 - x0);
	for (x = x0; x < x1; x++)
	{
		OLED_DrawPoint(x, k * x,1);
	}
}
