/* ups_build-in
 * /ups_build-in/src/dma_src/dma.cpp
 * dma.cpp
 * 
 *  Created on: 19 нояб. 2021 г.
 *  	  Time: 15:29:46
 *      Author: AlexICMT
 */

#include "dma.hpp"
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_dma.h"
#include "adc.hpp"
#include "dac.hpp"
#include <math.h>
//#include "stdint.h"
//#include <cstdint>

static uint32_t ADC_Buffer[10];
static uint32_t DAC_Buffer[8];
static uint32_t DAC_Buffer1 = 3600;
namespace kip5 {
namespace dma {

// Задание Функции генератора гармонического (синусоидального) сигнала
static void Function_Sin_Setup()
{
	uint32_t i;
	float w;

	// Заполнить массив с отсчетами синусоиды
  w = 2 * 3.1415926535 / 32;

	for (i = 0; i < 32; i++)
	{
		DAC_Buffer[i] = 500 + 3600 * sin(w * i);
  }
}

static void Function_Const_Setup()
{
	uint32_t i;
	//float w;

	// Заполнить массив с отсчетами синусоиды
 // w = 2 * 3.1415926535 / 32;

	for (i = 0; i < 8; i++)
	{
		DAC_Buffer[i] = 1300;
  }
}

static DMA_CtrlDataInitTypeDef 	DMA_InitStruct_ADC;
static DMA_ChannelInitTypeDef 	DMA_Ch_InitStruct_ADC;
//static DMA_CtrlDataInitTypeDef 	DMA_InitStruct_DAC;
//static DMA_ChannelInitTypeDef 	DMA_Ch_InitStruct_DAC;
//static DMA_CtrlDataTypeDef 		DMA_CtrlDataType_DAC;

Dma::Dma()
{
	//Function_Sin_Setup();
	Function_Const_Setup();
	Dma_config();
}

void Dma::Dma_config()
{
	RST_CLK_PCLKcmd (RST_CLK_PCLK_DMA | RST_CLK_PCLK_SSP1 | RST_CLK_PCLK_SSP2, ENABLE);
	NVIC->ICPR[0] = 0xFFFFFFFF;
	NVIC->ICER[0] = 0xFFFFFFFF;

	DMA_DeInit();

	DMA_StructInit (&DMA_Ch_InitStruct_ADC);
	//DMA_StructInit (&DMA_Ch_InitStruct_DAC);
	//ADC Struct
	DMA_InitStruct_ADC.DMA_SourceBaseAddr = (uint32_t)(&(MDR_ADC->ADC1_RESULT));	// Базовый адрес регистра результата преобразования АЦП1, откуда будем читать
	DMA_InitStruct_ADC.DMA_DestBaseAddr = (uint32_t) &ADC_Buffer;					// Базовый адрес в памяти, куда будем писать результаты работы АЦП
	DMA_InitStruct_ADC.DMA_CycleSize = 10;							      			// Размер буфера
	DMA_InitStruct_ADC.DMA_SourceIncSize = DMA_SourceIncNo;				 		  	// Запретить автоувеличение адреса для периферии
	DMA_InitStruct_ADC.DMA_DestIncSize = DMA_DestIncWord;							// Разрешить автоувеличение адреса для памяти на 2
	DMA_InitStruct_ADC.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;    			// Передаем данные по 2 байта (полслова)
	DMA_InitStruct_ADC.DMA_NumContinuous = DMA_Transfers_1;				    		// Сколько раз повоторяем совокупность циклов - 1 раз
	DMA_InitStruct_ADC.DMA_SourceProtCtrl = DMA_SourcePrivileged;					// Режим защиты источника
	DMA_InitStruct_ADC.DMA_DestProtCtrl = DMA_DestPrivileged;			    		// Режим защиты приемника
	DMA_InitStruct_ADC.DMA_Mode = DMA_Mode_Basic;									// Режим работы DMA - базовый

	/*//DAC Struct
	DMA_InitStruct_DAC.DMA_SourceBaseAddr = (uint32_t) &DAC_Buffer;
	DMA_InitStruct_DAC.DMA_DestBaseAddr = (uint32_t)(&(MDR_DAC->DAC2_DATA));
	DMA_InitStruct_DAC.DMA_CycleSize = 8;
	DMA_InitStruct_DAC.DMA_SourceIncSize = DMA_SourceIncHalfword;
	DMA_InitStruct_DAC.DMA_DestIncSize = DMA_DestIncNo;
	DMA_InitStruct_DAC.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStruct_DAC.DMA_NumContinuous = DMA_Transfers_8;
	DMA_InitStruct_DAC.DMA_SourceProtCtrl = DMA_SourcePrivileged;
	DMA_InitStruct_DAC.DMA_DestProtCtrl = DMA_DestPrivileged;
	DMA_InitStruct_DAC.DMA_Mode = DMA_Mode_Basic;
	*/
	//ADCx Struct
	DMA_Ch_InitStruct_ADC.DMA_PriCtrlData = &DMA_InitStruct_ADC;
	DMA_Ch_InitStruct_ADC.DMA_Priority = DMA_Priority_Default;
	DMA_Ch_InitStruct_ADC.DMA_UseBurst = DMA_BurstClear;
	DMA_Ch_InitStruct_ADC.DMA_SelectDataStructure = DMA_CTRL_DATA_PRIMARY;

	/*//DACx Struct
	DMA_Ch_InitStruct_DAC.DMA_PriCtrlData = &DMA_InitStruct_DAC;
	DMA_Ch_InitStruct_DAC.DMA_Priority = DMA_Priority_Default;
	DMA_Ch_InitStruct_DAC.DMA_UseBurst = DMA_BurstClear;
	DMA_Ch_InitStruct_DAC.DMA_SelectDataStructure = DMA_CTRL_DATA_PRIMARY;
	*/
	// Инициализировать канал DMA
	DMA_Init(DMA_Channel_ADC1, &DMA_Ch_InitStruct_ADC);//DMA_Channel_ADC2
	//DMA_Init(DMA_Channel_TIM1, &DMA_Ch_InitStruct_DAC);
	// DMA for working with DAC

	// Снять запрет на запросы и одиночные запросы к DMA
	MDR_DMA->CHNL_REQ_MASK_CLR = 1 << DMA_Channel_ADC1;
	MDR_DMA->CHNL_USEBURST_CLR = 1 << DMA_Channel_ADC1;
	//MDR_DMA->CHNL_REQ_MASK_CLR = 1 << DMA_Channel_TIM1;
	//MDR_DMA->CHNL_USEBURST_CLR = 1 << DMA_Channel_TIM1;
	// Разрешить работу DMA
	DMA_Cmd (DMA_Channel_ADC1, ENABLE);
	//DMA_Cmd (DMA_Channel_TIM1, ENABLE);
	// Задать приоритет аппаратного прерывания от DMA
	NVIC_SetPriority (DMA_IRQn, 1);
}

void Dma::Dma_launch_adc()
{
	DMA_Cmd (DMA_Channel_ADC1, ENABLE);
	//DMA_Cmd (DMA_Channel_TIM1, ENABLE);
	//MDR_DMA->CHNL_SW_REQUEST = 1<<DMA_Channel_SW1;
}

uint16_t Dma::get_adc()
{
	return static_cast<uint16_t>(DAC_Buffer[0]);
}

} // namespace dma

namespace handlers {
extern "C" void DMA_IRQHandler()
{
	dma::DMA_InitStruct_ADC.DMA_CycleSize = 10;
	//dma::DMA_InitStruct_DAC.DMA_CycleSize = 8;
	DMA_Init(DMA_Channel_ADC1, &dma::DMA_Ch_InitStruct_ADC);
	//DMA_Init(DMA_Channel_TIM1, &dma::DMA_Ch_InitStruct_DAC);
		for (int i = 0; i<10 ; i++)
		{
		uint32_t ch1 = ADC_Buffer[i] >> 16;
		switch(ch1)
				{
				case 0:
					adc::Adc::adcBuffer1.adc_ch0 = ADC_Buffer[i];
					break;
				case 1:
					adc::Adc::adcBuffer1.adc_ch1 = ADC_Buffer[i];
					break;
				case 2:
					adc::Adc::adcBuffer1.adc_ch2 = ADC_Buffer[i];
					break;
				case 3:
					adc::Adc::adcBuffer1.adc_ch3 = ADC_Buffer[i];
					break;
				case 4:
					adc::Adc::adcBuffer1.adc_ch4 = ADC_Buffer[i];
					break;
				case 5:
					adc::Adc::adcBuffer1.adc_ch5 = ADC_Buffer[i];
					break;
				case 6:
					adc::Adc::adcBuffer1.adc_ch6 = ADC_Buffer[i];
					break;
				case 7:
					adc::Adc::adcBuffer1.adc_ch7 = ADC_Buffer[i];
					break;
				case 30:
					adc::Adc::adcaddBuffer1.adc_chV = ADC_Buffer[i];
					break;
				case 31:
					adc::Adc::adcaddBuffer1.adc_chT = ADC_Buffer[i];
					break;
				}
		}
	DMA_Cmd (DMA_Channel_ADC1, DISABLE);
	//DMA_Cmd (DMA_Channel_TIM1, DISABLE);
}
} // namespace handlers
} // namespace kip5


