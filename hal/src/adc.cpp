/* ups_build-in
 * /ups_build-in/src/adc_src/adc.cpp
 * adc.cpp
 * 
 *  Created on: 17 нояб. 2021 г.
 *  	  Time: 11:31:17
 *      Author: AlexICMT
 */

//#include "MDR32Fx.h"
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_adc.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include "adc.hpp"

namespace kip5 {
namespace adc {

/*
 * АЦП MDR32F9Qx
 * Для осуществления преобразования требуется не менее 28 тактов.
 * Максимальная частота CLK не может превышать 14 МГц.
 * 14 МГц / 28 тактов = 500 кГц
 *
 * 80 МГц / ADC_CLK_div_8 / 28 тактов = 10 МГц / 28 тактов = 357.143 кГц
 * 80 МГц / ADC_CLK_div_16 / 28 тактов = 5 МГц / 28 тактов = 178.571 кГц
 * 80 МГц / ADC_CLK_div_32 / 28 тактов = 2.5 МГц / 28 тактов = 89.286 кГц
 * 80 МГц / ADC_CLK_div_64 / 28 тактов = 1.25 МГц / 28 тактов = 44.643 кГц
 * 80 МГц / ADC_CLK_div_128 / 28 тактов = 0.625 МГц / 28 тактов = 22.321 кГц
 * 80 МГц / ADC_CLK_div_256 / 28 тактов = 0.3125 МГц / 28 тактов = 11.161 кГц
 */

/*Adc::Adc()
{
	initAdcForSwitchingConvertion();
}*/

Adc::Adc()
{
	initInternalADCPorts();
	initAdcForSwitchingConvertion();
	//initAdcForSwitchingConvertionSynch();
	//initAdcForSingleConvertion();
}

void Adc::initInternalADCPorts()
{
	PORT_InitTypeDef PortInit;

	PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
	PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PortInit.PORT_PD_SHM = PORT_PD_SHM_OFF;
	PortInit.PORT_PD = PORT_PD_OPEN;
	PortInit.PORT_GFEN = PORT_GFEN_OFF;
	PortInit.PORT_FUNC = PORT_FUNC_PORT;
	PortInit.PORT_OE = PORT_OE_IN;
	PortInit.PORT_SPEED = PORT_OUTPUT_OFF;
	PortInit.PORT_MODE = PORT_MODE_ANALOG;
	PortInit.PORT_Pin = (PORT_Pin_0 | PORT_Pin_1 | PORT_Pin_2 | PORT_Pin_3 | PORT_Pin_4 | PORT_Pin_5 | PORT_Pin_6 | PORT_Pin_7);
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTD , ENABLE);
	PORT_Init(MDR_PORTD, &PortInit);
}

void Adc::initAdcForSingleConvertion()
{
	RST_CLK_PCLKcmd(RST_CLK_PCLK_ADC, ENABLE);

	ADC_DeInit();
	ADC_InitTypeDef ADC_InitStruct;
	ADC_StructInit(&ADC_InitStruct);
	ADC_InitStruct.ADC_SynchronousMode      = ADC_SyncMode_Independent;
	ADC_InitStruct.ADC_StartDelay           = 10;	// 0..15
	ADC_InitStruct.ADC_TempSensor           = ADC_TEMP_SENSOR_Enable;
	ADC_InitStruct.ADC_TempSensorAmplifier  = ADC_TEMP_SENSOR_AMPLIFIER_Enable;
	ADC_InitStruct.ADC_TempSensorConversion = ADC_TEMP_SENSOR_CONVERSION_Enable;
	ADC_InitStruct.ADC_IntVRefConversion    = ADC_VREF_CONVERSION_Enable;
	ADC_InitStruct.ADC_IntVRefTrimming      = 0;	// 0..7
	ADC_Init(&ADC_InitStruct);

	ADC_SetTrim(4);		// 0..7

	ADCx_InitTypeDef ADCx_InitStruct;
	ADCx_StructInit(&ADCx_InitStruct);
	ADCx_InitStruct.ADC_ClockSource         = ADC_CLOCK_SOURCE_CPU;
	ADCx_InitStruct.ADC_SamplingMode        = ADC_SAMPLING_MODE_CICLIC_CONV;
	ADCx_InitStruct.ADC_ChannelSwitching    = ADC_CH_SWITCHING_Disable;
	ADCx_InitStruct.ADC_ChannelNumber       = ADC_CH_TEMP_SENSOR;//ADC_CH_ADC0
	ADCx_InitStruct.ADC_Channels            = 0;
	ADCx_InitStruct.ADC_LevelControl        = ADC_LEVEL_CONTROL_Disable;
	ADCx_InitStruct.ADC_LowLevel            = 0;								// 0..4095
	ADCx_InitStruct.ADC_HighLevel           = 0;								// 0..4095
	ADCx_InitStruct.ADC_VRefSource          = ADC_VREF_SOURCE_INTERNAL;
	ADCx_InitStruct.ADC_IntVRefSource       = ADC_INT_VREF_SOURCE_EXACT;		// TODO ADC_IntVRefSource
	ADCx_InitStruct.ADC_Prescaler           = ADC_CLK_div_64;					// 80МГц / 14МГц = 5.7, but /64 is minimal prescaler (by experience) => 80M/64 = 1.25 MHz
	ADCx_InitStruct.ADC_DelayGo             = 2;								// 0..7

	ADC1_Init(&ADCx_InitStruct);
	ADC2_Init(&ADCx_InitStruct);

	//ADC1_ITConfig(ADCx_IT_END_OF_CONVERSION, ENABLE);
	//ADC2_ITConfig(ADCx_IT_END_OF_CONVERSION, DISABLE);

	ADC1_Cmd(ENABLE);
	ADC2_Cmd(DISABLE);

	NVIC_EnableIRQ(DMA_IRQn);
}

void Adc::initAdcForSwitchingConvertion()
{
	RST_CLK_PCLKcmd(RST_CLK_PCLK_ADC, ENABLE);
	ADC_DeInit();
	ADC_InitTypeDef ADC_InitStruct;
	ADC_StructInit(&ADC_InitStruct);
	ADC_InitStruct.ADC_SynchronousMode      = ADC_SyncMode_Independent;
	ADC_InitStruct.ADC_StartDelay           = 0;	// 0..15
	ADC_InitStruct.ADC_TempSensor           = ADC_TEMP_SENSOR_Enable;
	ADC_InitStruct.ADC_TempSensorAmplifier  = ADC_TEMP_SENSOR_AMPLIFIER_Enable;
	ADC_InitStruct.ADC_TempSensorConversion = ADC_TEMP_SENSOR_CONVERSION_Enable;
	ADC_InitStruct.ADC_IntVRefConversion    = ADC_VREF_CONVERSION_Enable;
	ADC_InitStruct.ADC_IntVRefTrimming      = 0;	// 0..7
	ADC_Init(&ADC_InitStruct);

	ADC_SetTrim(4);		// 0..7

	ADCx_InitTypeDef ADCx_InitStruct;
	ADCx_StructInit(&ADCx_InitStruct);
	ADCx_InitStruct.ADC_ClockSource         = ADC_CLOCK_SOURCE_CPU;
	ADCx_InitStruct.ADC_SamplingMode        = ADC_SAMPLING_MODE_CICLIC_CONV;
	ADCx_InitStruct.ADC_ChannelNumber       = ADC_CH_ADC0;
	ADCx_InitStruct.ADC_LevelControl        = ADC_LEVEL_CONTROL_Disable;
	ADCx_InitStruct.ADC_LowLevel            = 0;								// 0..4095
	ADCx_InitStruct.ADC_HighLevel           = 0;								// 0..4095
	ADCx_InitStruct.ADC_VRefSource          = ADC_VREF_SOURCE_INTERNAL;
	ADCx_InitStruct.ADC_IntVRefSource       = ADC_INT_VREF_SOURCE_EXACT;		// TODO ADC_IntVRefSource
	ADCx_InitStruct.ADC_Prescaler           = ADC_CLK_div_64;					//
	ADCx_InitStruct.ADC_DelayGo             = 4;								// 0..7

	ADCx_InitStruct.ADC_ChannelSwitching    = ADC_CH_SWITCHING_Enable;
	ADCx_InitStruct.ADC_Channels            = ADC_CH_ADC0_MSK | ADC_CH_ADC1_MSK | ADC_CH_ADC2_MSK | ADC_CH_ADC3_MSK | ADC_CH_ADC4_MSK | ADC_CH_ADC5_MSK | ADC_CH_ADC6_MSK | ADC_CH_ADC7_MSK | ADC_CH_TEMP_SENSOR_MSK | ADC_CH_INT_VREF_MSK;
	ADC1_Init(&ADCx_InitStruct);

	ADCx_InitStruct.ADC_ChannelSwitching    = ADC_CH_SWITCHING_Disable;
	ADCx_InitStruct.ADC_Channels            = 0;
	ADC2_Init(&ADCx_InitStruct);

	//ADC1_ITConfig(ADCx_IT_END_OF_CONVERSION, DISABLE);
	//ADC2_ITConfig(ADCx_IT_END_OF_CONVERSION, DISABLE);

	ADC1_Cmd(ENABLE);
	ADC2_Cmd(DISABLE);

	//NVIC_SetPriority(ADC_IRQn, 1);
	//NVIC_EnableIRQ(ADC_IRQn);
	NVIC_EnableIRQ(DMA_IRQn);
}

void Adc::initAdcForSwitchingConvertionSynch()
{
	RST_CLK_PCLKcmd(RST_CLK_PCLK_ADC, ENABLE);
	ADC_DeInit();
	ADC_InitTypeDef ADC_InitStruct;
	ADC_StructInit(&ADC_InitStruct);
	ADC_InitStruct.ADC_SynchronousMode      = ADC_SyncMode_Synchronous;
	ADC_InitStruct.ADC_StartDelay           = 7;	// 0..15
	ADC_InitStruct.ADC_TempSensor           = ADC_TEMP_SENSOR_Enable;
	ADC_InitStruct.ADC_TempSensorAmplifier  = ADC_TEMP_SENSOR_AMPLIFIER_Enable;
	ADC_InitStruct.ADC_TempSensorConversion = ADC_TEMP_SENSOR_CONVERSION_Enable;
	ADC_InitStruct.ADC_IntVRefConversion    = ADC_VREF_CONVERSION_Enable;
	ADC_InitStruct.ADC_IntVRefTrimming      = 0;	// 0..7
	ADC_Init(&ADC_InitStruct);

	ADC_SetTrim(4);		// 0..7

	ADCx_InitTypeDef ADCx_InitStruct;
	ADCx_StructInit(&ADCx_InitStruct);
	ADCx_InitStruct.ADC_ClockSource         = ADC_CLOCK_SOURCE_CPU;
	ADCx_InitStruct.ADC_SamplingMode        = ADC_SAMPLING_MODE_CICLIC_CONV;
	ADCx_InitStruct.ADC_ChannelNumber       = ADC_CH_ADC0;
	ADCx_InitStruct.ADC_LevelControl        = ADC_LEVEL_CONTROL_Disable;
	ADCx_InitStruct.ADC_LowLevel            = 0;								// 0..4095
	ADCx_InitStruct.ADC_HighLevel           = 0;								// 0..4095
	ADCx_InitStruct.ADC_VRefSource          = ADC_VREF_SOURCE_INTERNAL;
	ADCx_InitStruct.ADC_IntVRefSource       = ADC_INT_VREF_SOURCE_EXACT;		// TODO ADC_IntVRefSource
	ADCx_InitStruct.ADC_Prescaler           = ADC_CLK_div_64;					//
	ADCx_InitStruct.ADC_DelayGo             = 4;								// 0..7

	ADCx_InitStruct.ADC_ChannelSwitching    = ADC_CH_SWITCHING_Enable;
	ADCx_InitStruct.ADC_Channels            = ADC_CH_ADC0_MSK | ADC_CH_ADC1_MSK | ADC_CH_ADC2_MSK | ADC_CH_ADC3_MSK | ADC_CH_ADC4_MSK | ADC_CH_ADC5_MSK | ADC_CH_ADC6_MSK | ADC_CH_ADC7_MSK | ADC_CH_TEMP_SENSOR_MSK | ADC_CH_INT_VREF_MSK;
	ADC1_Init(&ADCx_InitStruct);
	ADCx_InitStruct.ADC_Channels            = ADC_CH_ADC0_MSK | ADC_CH_ADC1_MSK | ADC_CH_ADC2_MSK | ADC_CH_ADC3_MSK | ADC_CH_ADC4_MSK | ADC_CH_ADC5_MSK | ADC_CH_ADC6_MSK | ADC_CH_ADC7_MSK;
	ADC2_Init(&ADCx_InitStruct);

	//ADCx_InitStruct.ADC_ChannelNumber       = ADC_CH_ADC5;
	//ADCx_InitStruct.ADC_ChannelSwitching    = ADC_CH_SWITCHING_Enable;
	//ADCx_InitStruct.ADC_Channels            = ADC_CH_ADC5_MSK | ADC_CH_ADC6_MSK | ADC_CH_ADC7_MSK | ADC_CH_TEMP_SENSOR_MSK | ADC_CH_INT_VREF_MSK;
	//ADC1_Init(&ADCx_InitStruct);



	//ADC1_ITConfig(ADCx_IT_END_OF_CONVERSION, DISABLE);
	//ADC2_ITConfig(ADCx_IT_END_OF_CONVERSION, DISABLE);

	ADC1_Cmd(ENABLE);
	ADC2_Cmd(ENABLE);

	//NVIC_SetPriority(ADC_IRQn, 1);
	//NVIC_EnableIRQ(ADC_IRQn);
	NVIC_EnableIRQ(DMA_IRQn);
}

void Adc::initAdcForSwitchingConvertion_2048()
{
	RST_CLK_PCLKcmd(RST_CLK_PCLK_ADC, ENABLE);
	ADC_DeInit();
	ADC_InitTypeDef ADC_InitStruct;
	ADC_StructInit(&ADC_InitStruct);
	ADC_InitStruct.ADC_SynchronousMode      = ADC_SyncMode_Independent;
	ADC_InitStruct.ADC_StartDelay           = 0;	// 0..15
	ADC_InitStruct.ADC_TempSensor           = ADC_TEMP_SENSOR_Enable;
	ADC_InitStruct.ADC_TempSensorAmplifier  = ADC_TEMP_SENSOR_AMPLIFIER_Enable;
	ADC_InitStruct.ADC_TempSensorConversion = ADC_TEMP_SENSOR_CONVERSION_Enable;
	ADC_InitStruct.ADC_IntVRefConversion    = ADC_VREF_CONVERSION_Enable;
	ADC_InitStruct.ADC_IntVRefTrimming      = 0;	// 0..7
	ADC_Init(&ADC_InitStruct);

	ADC_SetTrim(4);		// 0..7

	ADCx_InitTypeDef ADCx_InitStruct;
	ADCx_StructInit(&ADCx_InitStruct);
	ADCx_InitStruct.ADC_ClockSource         = ADC_CLOCK_SOURCE_CPU;
	ADCx_InitStruct.ADC_SamplingMode        = ADC_SAMPLING_MODE_CICLIC_CONV;
	ADCx_InitStruct.ADC_ChannelNumber       = ADC_CH_ADC0;
	ADCx_InitStruct.ADC_LevelControl        = ADC_LEVEL_CONTROL_Disable;
	ADCx_InitStruct.ADC_LowLevel            = 0;								// 0..4095
	ADCx_InitStruct.ADC_HighLevel           = 0;								// 0..4095
	ADCx_InitStruct.ADC_VRefSource          = ADC_VREF_SOURCE_INTERNAL;
	ADCx_InitStruct.ADC_IntVRefSource       = ADC_INT_VREF_SOURCE_EXACT;		// TODO ADC_IntVRefSource
	ADCx_InitStruct.ADC_Prescaler           = ADC_CLK_div_2048;
	ADCx_InitStruct.ADC_DelayGo             = 4;								// 0..7

	ADCx_InitStruct.ADC_ChannelSwitching    = ADC_CH_SWITCHING_Enable;
	ADCx_InitStruct.ADC_Channels            = ADC_CH_ADC0_MSK | ADC_CH_ADC1_MSK | ADC_CH_ADC2_MSK | ADC_CH_ADC3_MSK | ADC_CH_ADC4_MSK | ADC_CH_ADC5_MSK | ADC_CH_ADC6_MSK | ADC_CH_ADC7_MSK;
	ADC1_Init(&ADCx_InitStruct);

	ADCx_InitStruct.ADC_ChannelSwitching    = ADC_CH_SWITCHING_Disable;
	ADCx_InitStruct.ADC_Channels            = 0;
	ADC2_Init(&ADCx_InitStruct);

	ADC1_ITConfig(ADCx_IT_END_OF_CONVERSION, ENABLE);
	ADC2_ITConfig(ADCx_IT_END_OF_CONVERSION, DISABLE);

	ADC1_Cmd(ENABLE);
	ADC2_Cmd(ENABLE);

	NVIC_SetPriority(ADC_IRQn, 3);
	NVIC_EnableIRQ(ADC_IRQn);
}

void Adc::startConversionAdc1(int channel)
{
	ADC1_SetChannel((uint32_t)channel);
	ADC1_Start();
}

void Adc::startConversionAdc2(int channel)
{
	ADC2_SetChannel((uint32_t)channel);
	ADC2_Start();
}

int Adc::waitConversionAndReadAdc1()
{
	while((ADC1_GetStatus() & ADCx_FLAG_END_OF_CONVERSION) == 0);
	return ADC1_GetResult() & 0xFFF;
}

int Adc::waitConversionAndReadAdc2()
{
	while((ADC2_GetStatus() & ADCx_FLAG_END_OF_CONVERSION) == 0);
	return ADC2_GetResult() & 0xFFF;
}

int Adc::readAdc1(int channel)
{
	startConversionAdc1(channel);
	return waitConversionAndReadAdc1();
}

int Adc::readAdc2(int channel)
{
	startConversionAdc2(channel);
	return waitConversionAndReadAdc2();
}

void Adc::readAdc()
{
	adc_ch0 = 0;
	adc_ch1 = 0;
	adc_ch2 = 0;
	adc_ch3 = 0;
	adc_ch4 = 0;
	adc_ch5 = 0;
	adc_ch6 = 0;
	adc_ch7 = 0;
	adc_chT = 0;
	adc_chV = 0;

	//const int n = Adc::adcHalfBufferSize;
	//const Adc::AdcData* buf = Adc::getBuffer();
		adc_ch0 = adcBuffer1.adc_ch0;
		adc_ch1 = adcBuffer1.adc_ch1;
		adc_ch2 = adcBuffer1.adc_ch2;
		adc_ch3 = adcBuffer1.adc_ch3;
		adc_ch4 = adcBuffer1.adc_ch4;
		adc_ch5 = adcBuffer1.adc_ch5;
		adc_ch6 = adcBuffer1.adc_ch6;
		adc_ch7 = adcBuffer1.adc_ch7;
		adc_chT = adcaddBuffer1.adc_chT;
		adc_chV = adcaddBuffer1.adc_chV;
}

Adc::AdcData Adc::adcBuffer[adcBufferSize];
Adc::AdcData Adc::adcBuffer1;
Adc::AdcDataAdd Adc::adcaddBuffer1;
int Adc::adcBufferIndex = 0;
int Adc::ADC_READY_4_SAMPLES;



} // namespace adc

namespace handlers {
extern "C" void ADC_IRQHandler()
{
	if(MDR_ADC->ADC1_STATUS & ADC_STATUS_FLG_REG_EOCIF)
	{
		uint32_t v1 = ADC1_GetResult();
		//uint32_t v2 = ADC2_GetResult();
		uint32_t ch = v1 >> 16;
		switch(ch)
		{
		case 0:
			adc::Adc::adcBuffer1.adc_ch0 = v1;
			break;
		case 1:
			adc::Adc::adcBuffer1.adc_ch1 = v1;
			break;
		case 2:
			adc::Adc::adcBuffer1.adc_ch2 = v1;
			break;
		case 3:
			adc::Adc::adcBuffer1.adc_ch3 = v1;
			break;
		case 4:
			adc::Adc::adcBuffer1.adc_ch4 = v1;
			break;
		case 5:
			adc::Adc::adcBuffer1.adc_ch5 = v1;
			break;
		case 6:
			adc::Adc::adcBuffer1.adc_ch6 = v1;
			break;
		case 7:
			adc::Adc::adcBuffer1.adc_ch7 = v1;
			break;
		case 30:
			adc::Adc::adcaddBuffer1.adc_chV = v1;
			break;
		case 31:
			adc::Adc::adcaddBuffer1.adc_chT = v1;
			break;
		}
	}
}

} // namespace handlers

} // namespace kip5
