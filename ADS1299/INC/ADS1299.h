#ifndef __ADS1299_H__
#define __ADS1299_H__

#include "rt/rt_api.h"

/************ADS1299命令定义****************/
/*系统命令*/
#define WAKEUP 0X02    //从待机模式唤醒
#define STANDBY 0X04   //进入待机模式
#define ADS_RESET 0X06 //复位ADS1292R
#define START 0X08     //启动或转换
#define STOP 0X0A      //停止转换

/*数据读取命令*/
#define RDATAC 0X10 //启用连续的数据读取模式,默认使用此模式
#define SDATAC 0X11 //停止连续的数据读取模式
#define RDATA 0X12  //通过命令读取数据;支持多种读回。

/*寄存器读取命令*/
#define RREG 0X20 //读取001r rrrr 000n nnnn  这里定义的只有高八位，低8位在发送命令时设置
#define WREG 0X40 //写入010r rrrr 000n nnnn
/*	r rrrr=要读、写的寄存器地址
		n nnnn=要读、写的数据*/

/*********ADS1299内部寄存器地址定义*********/
#define ID 0X00         //ID控制寄存器
#define CONFIG1 0X01    //配置寄存器1
#define CONFIG2 0X02    //配置寄存器2
#define CONFIG3 0X03    //配置寄存器3
#define LOFF 0X04       //导联脱落控制寄存器
#define CH1SET 0X05     //通道1设置寄存器
#define CH2SET 0X06     //通道2设置寄存器
#define CH3SET 0X07     //通道3设置寄存器
#define CH4SET 0X08     //通道4设置寄存器
#define CH5SET 0X09     //通道5设置寄存器
#define CH6SET 0X0A     //通道6设置寄存器
#define CH7SET 0X0B     //通道7设置寄存器
#define CH8SET 0X0C     //通道8设置寄存器
#define BIAS_SENSP 0X0D //
#define BIAS_SENSN 0X0E
#define LOFF_SENSP 0X0F
#define LOFF_SENSN 0X10
#define LOFF_FLIP 0X11

#define LOFF_STATP 0X12
#define LOFF_STATN 0X13 //掉电状态寄存器地址（只读）

#define MISC1 0x15 //SRB1 control register
/*********ADS1299 Mode Configure********/
/*
 * define CHxSET_Signal 0X0E    电路非差分设计时，（共N 或 共P）
 * define CHxSET_Signal 0X00    电路差分输入时
 */

//#define CHxSET_Signal 0X06     // BIAS_DRP (positive electrode is the driver)模式  手册50页
// #define CHxSET_Signal 0X00       // Normal electrode input                           手册50页
// #define CHxSET_Signal 0X01     // Input shorted (for offset or noise measurements) 手册50页
#define CHxSET_Signal 0x05 // Setting CHnSET[2:0] = 001 sets the common-mode voltage of [(VVREFP + VVREFN) / 2] to both channel inputs.
                           // This setting can be used to test inherent device noise in the user system.
//#define DAISY_DISEN            // 是否使能菊花链  寄存器配置手册46页 模式介绍手册65页

//------------------------------------------------------------------------------------------------
//rt_spim_t spim;

//----------------------------------------------------------------------------------------------

void ADS1299_Check(void);
void ADS1299_Init(void);

unsigned char ADS_REG(unsigned char com, unsigned data); //对ADS1292内部寄存器进行操作
void ADS_Read(unsigned char *data);                      //读取72位的数据
//void SamplingRateSelection(void);

unsigned char ADS_SPI(unsigned char TxData);

#endif
