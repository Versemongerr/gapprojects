#include "ADS1299.h"
#include "USER_SPI.h"

/*
 *  SPIx 读写一个字节  
 *  TxData:要写入的字节 
 *  返回值:读取到的字节
 */
// unsigned char ADS_SPI(unsigned char TxData)
// {
// 	unsigned char TX_Buffer[1]={TxData};
// 	unsigned char RX_Buffer[1];

// 	pi_spi_transfer_async(&spi, (void*)TX_Buffer,(void*)RX_Buffer,8,PI_SPI_CS_KEEP|PI_SPI_LINES_SINGLE,NULL);

// 	return RX_Buffer[0];
// }
/*
 * 对ADS129x内部寄存器进行操作 
 */
unsigned char ADS_REG(unsigned char com, unsigned data)
{
	SPI_CS_HIGH;
	SPI_CS_LOW;
	unsigned char i, data_return = 0;

	for (i = 0; i < 55; i++)
		;
	ADS_SPI(com);
	for (i = 0; i < 55; i++)
		;
	ADS_SPI(0X00);
	for (i = 0; i < 55; i++)
		;
	if ((com & 0x20) == 0x20) //判断是否为读寄存器指令
	{
		data_return = ADS_SPI(0X00);
	}
	if ((com & 0x40) == 0x40)
	{
		data_return = ADS_SPI(data); //判断是否为写寄存器指令
	}
	SPI_CS_HIGH;
	return (data_return);
}
/*
 * 一次读3位 + 8 * 3位 共27位数据
 */
void ADS_Read(unsigned char *data)
{
	SPI_CS_HIGH;
	SPI_CS_LOW;
	unsigned char i;
	for (i = 0; i < 27; i++)
	{
		*data = ADS_SPI(0X00);
		data++;
	}
}
/*
*	读取ADS1299设备ID、CONFIG1寄存器，确认是否工作正常
*/
void ADS1299_Check(void)
{
	printf("checking");
	// SPI_CS_H
	ADS_SPI(ADS_RESET); //reset ADS1299
	rt_time_wait_us(1000);

	// SPI_CS_HIGH;
	// SPI_CS_LOW;
	ADS_SPI(SDATAC); //暂停读数据连续模式
	rt_time_wait_us(100);

	unsigned char a = ADS_REG(RREG | ID, 0x00); //Read ID register
	printf("ID:0x%02x\r\n", a);

	pmsis_exit(-1);

	// SPI_CS_HIGH;
	// SPI_CS_LOW;

	unsigned char b = ADS_REG(RREG | CONFIG1, 0x00); //Read CONFIG1 register, the default value should be 0x96
	printf("Config1 Reg:0x%02x\r\n", b);

	// pmsis_exit(-1);

	rt_time_wait_us(1000);
}
/*
*	初始化ADS1299
*/
void ADS1299_Init(void)
{
	/* 
	 * 第一步：电路图把 CLKSEL_Pin =0  选择外部晶振2.048MHz
	 *         等待晶振起震
	 */
	rt_time_wait_us(1000); //等待ADS1299晶振起震

	/* 
	 * 第二步：电路图把 PDWN =1  RESET=1 发送ADS1299复位信号
	 *         等待一段时间ADS1299上电复位
	 */
	// 	SPI_CS_HIGH;
	// SPI_CS_LOW;
	ADS_SPI(ADS_RESET);	   //复位ADS1299
	rt_time_wait_us(1000); //等待ADS1299复位后稳定

	/* 
	 * 第三步：发送SDATAC命令 因为ADS1299从RDATAC模式唤醒
	 */
	// 	SPI_CS_HIGH;
	// SPI_CS_LOW;
	ADS_SPI(SDATAC); //暂停读数据连续模式
	rt_time_wait_us(100);

	/* 
	 * 第四步：配置CONFIG3寄存器 使用内部参考电压 ADS1299手册49页 其他设置待议！！！！！
	 */
	ADS_REG(WREG | CONFIG3, 0XEC); //使用内部参考电压
	rt_time_wait_us(10);		   //等待内部参考电压稳定

	// unsigned char c = ADS_REG(RREG | CONFIG3, 0x00); //Read CONFIG1 register, the default value should be 0x96
	// printf("Config3 Reg:0x%02x\r\n", c);

	// pmsis_exit(-1);

	/* 
	 * 第五步：配置MISC寄存器，使用SRB1  	 
	 */

	ADS_REG(WREG | MISC1, 0X20);
	rt_time_wait_us(10);

	/* 
	 * 第六步：配置CONFIG3寄存器 使用内部参考电压 ADS1299手册49页 其他设置待议！！！！！
	 */
	ADS_REG(WREG | BIAS_SENSN, 0xff);
	rt_time_wait_us(10);
	ADS_REG(WREG | BIAS_SENSP, 0xff);
	rt_time_wait_us(10);

/* 
	 * 第七步：配置CONFIG1寄存器 设置ADS1299采样速率 菊花链是否是能 ADS1299手册46页
	 *         配置CONFIG2寄存器 设置内部测试信号                   ADS1299手册47页 
	 *         菊花链控制位 [6] 写0使能 0x96
	 */
#ifdef DAISY_DISEN				   //如果使用菊花链
	ADS_REG(WREG | CONFIG1, 0xD5); //250Hz 0x96;500hz,0x95,1KHz 0x94,2KHz 0x93
#else
	ADS_REG(WREG | CONFIG1, 0x95); //250Hz 0x96;500hz,0x95,1KHz 0x94,2KHz 0x93
#endif

	rt_time_wait_us(1);
	ADS_REG(WREG | CONFIG2, 0XD0 | 0x01); //测试信号内部产生0XD0 外部0XC0
	rt_time_wait_us(1);

	/* 
	 * 第八步：配置CHXSET寄存器 0X00:正常输入，增益=1
	 */
	ADS_REG(WREG | CH1SET, CHxSET_Signal); //amplified x1
	ADS_REG(WREG | CH2SET, CHxSET_Signal); //amplified x1
	ADS_REG(WREG | CH3SET, CHxSET_Signal); //amplified x1
	ADS_REG(WREG | CH4SET, CHxSET_Signal); //amplified x1
	ADS_REG(WREG | CH5SET, CHxSET_Signal); //amplified x1
	ADS_REG(WREG | CH6SET, CHxSET_Signal); //amplified x1
	ADS_REG(WREG | CH7SET, CHxSET_Signal); //amplified x1
	ADS_REG(WREG | CH8SET, CHxSET_Signal); //amplified x1

	// unsigned char b = ADS_REG(RREG | CONFIG1, 0x00); //Read CONFIG1 register, the default value should be 0x96
	// printf("Config1 Reg:0x%02x\r\n", b);

	// pmsis_exit(-1);

	/* 
	 * 第九步：发送START命令，ADS1299开始转换
	 */
	// SPI_CS_HIGH;
	// SPI_CS_LOW;
	ADS_SPI(START); //开始转换
	rt_time_wait_us(1);

	/* 
	 * 第十步：发送RDATAC命令，设置ADS1299为读数据连续模式
	 */
	// SPI_CS_HIGH;
	// SPI_CS_LOW;
	ADS_SPI(RDATAC); //读数据连续模式
	rt_time_wait_us(1);

	printf("ADS1299 Ready!\r\n");
}
