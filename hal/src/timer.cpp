/* ups_build-in
 * /ups_build-in/src/tim_src/timer.cpp
 * timer.cpp
 * 
 *  Created on: 23 нояб. 2021 г.
 *  	  Time: 10:25:09
 *      Author: AlexICMT
 */

#include "timer.hpp"
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_timer.h"
#include "MDR32F9Qx_port.h"
//#include "meas_frq.hpp"

namespace kip5 {
namespace timer {
static uint32_t cnt_pls = 0;
constexpr uint16_t period = static_cast<uint16_t>(80000000/1000);//32
constexpr uint16_t prescaler = static_cast<uint16_t>((80000000/16000000)-1);
constexpr uint16_t pulse = static_cast<uint16_t>((16000000/100000)-1);
//static TIMER_CntInitTypeDef TIM_CntInit;

Timer::Timer()
{
	pwm_tim_config();
	//meas_tim_config();
}

void Timer::tim_config()
{
	TIMER_CntInitTypeDef TIM_CntInit;
	TIMER_DeInit (MDR_TIMER1);
	RST_CLK_PCLKcmd (RST_CLK_PCLK_TIMER1, ENABLE);
	TIMER_BRGInit (MDR_TIMER1, TIMER_HCLKdiv1);// 80MHz
	TIM_CntInit.TIMER_Period = period;
	TIM_CntInit.TIMER_Prescaler = 0;
	TIM_CntInit.TIMER_CounterDirection = TIMER_CntDir_Up;
	TIM_CntInit.TIMER_CounterMode = TIMER_CntMode_ClkFixedDir;
	TIM_CntInit.TIMER_FilterSampling = TIMER_FDTS_TIMER_CLK_div_1;
	TIM_CntInit.TIMER_EventSource = TIMER_EvSrc_None;

	TIM_CntInit.TIMER_ARR_UpdateMode           = TIMER_ARR_Update_Immediately;
	TIM_CntInit.TIMER_ETR_FilterConf           = TIMER_Filter_1FF_at_TIMER_CLK;
	TIM_CntInit.TIMER_ETR_Prescaler            = TIMER_ETR_Prescaler_None;
	TIM_CntInit.TIMER_ETR_Polarity             = TIMER_ETRPolarity_NonInverted;
	TIM_CntInit.TIMER_BRK_Polarity             = TIMER_BRKPolarity_NonInverted;

	TIMER_CntInit (MDR_TIMER1, &TIM_CntInit);
	TIMER_DMACmd (MDR_TIMER1, TIMER_STATUS_CNT_ARR, ENABLE);

	TIMER_Cmd (MDR_TIMER1, ENABLE);
}

void Timer::pwm_port_tim_config()
{
	RST_CLK_PCLKcmd (RST_CLK_PCLK_PORTE, ENABLE);
	PORT_InitTypeDef PortInitStructure;
	PORT_StructInit(&PortInitStructure);
	PortInitStructure.PORT_FUNC  = PORT_FUNC_ALTER;
	PortInitStructure.PORT_OE    = PORT_OE_OUT;
	PortInitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
	PortInitStructure.PORT_Pin   = PORT_Pin_0;
	PortInitStructure.PORT_SPEED = PORT_SPEED_MAXFAST;
	PORT_Init (MDR_PORTE, &PortInitStructure);
}

void Timer::pwm_tim_config()
{
	pwm_port_tim_config();
	TIMER_CntInitTypeDef TIM2_Init;
	TIMER_ChnInitTypeDef TIM2_Ch1Init;
	TIMER_ChnOutInitTypeDef	TIM2_Ch1OutInitE0;
	TIMER_DeInit (MDR_TIMER2);
	RST_CLK_PCLKcmd (RST_CLK_PCLK_TIMER2, ENABLE);
	TIMER_BRGInit (MDR_TIMER2, TIMER_HCLKdiv1);// 80MHz
	TIM2_Init.TIMER_Period = pulse;
	TIM2_Init.TIMER_Prescaler = prescaler;
	TIM2_Init.TIMER_CounterDirection = TIMER_CntDir_Up;
	TIM2_Init.TIMER_CounterMode = TIMER_CntMode_ClkFixedDir;
	TIM2_Init.TIMER_FilterSampling = TIMER_FDTS_TIMER_CLK_div_1;
	TIM2_Init.TIMER_EventSource = TIMER_EvSrc_None;

	TIM2_Init.TIMER_ARR_UpdateMode           = TIMER_ARR_Update_Immediately;
	TIM2_Init.TIMER_ETR_FilterConf           = TIMER_Filter_1FF_at_TIMER_CLK;
	TIM2_Init.TIMER_ETR_Prescaler            = TIMER_ETR_Prescaler_None;
	TIM2_Init.TIMER_ETR_Polarity             = TIMER_ETRPolarity_NonInverted;
	TIM2_Init.TIMER_BRK_Polarity             = TIMER_BRKPolarity_NonInverted;

	TIMER_CntInit (MDR_TIMER2, &TIM2_Init);

	TIMER_ChnStructInit (&TIM2_Ch1Init);
	TIM2_Ch1Init.TIMER_CH_Number = TIMER_CHANNEL1;
	TIM2_Ch1Init.TIMER_CH_Mode = TIMER_CH_MODE_PWM;
	TIM2_Ch1Init.TIMER_CH_REF_Format = TIMER_CH_REF_Format6;
	TIMER_ChnInit (MDR_TIMER2, &TIM2_Ch1Init);

	TIMER_ChnOutStructInit (&TIM2_Ch1OutInitE0);
	TIM2_Ch1OutInitE0.TIMER_CH_Number = TIMER_CHANNEL1;
	TIM2_Ch1OutInitE0.TIMER_CH_DirOut_Polarity = TIMER_CHOPolarity_NonInverted;
	TIM2_Ch1OutInitE0.TIMER_CH_DirOut_Source = TIMER_CH_OutSrc_REF;
	TIM2_Ch1OutInitE0.TIMER_CH_DirOut_Mode = TIMER_CH_OutMode_Output;
	TIM2_Ch1OutInitE0.TIMER_CH_NegOut_Polarity = TIMER_CHOPolarity_NonInverted;
	TIM2_Ch1OutInitE0.TIMER_CH_NegOut_Source = TIMER_CH_OutSrc_REF;
	TIM2_Ch1OutInitE0.TIMER_CH_NegOut_Mode = TIMER_CH_OutMode_Output;
	TIMER_ChnOutInit (MDR_TIMER2, &TIM2_Ch1OutInitE0);

	//TIMER_DMACmd (MDR_TIMER2, TIMER_STATUS_CNT_ARR, ENABLE);
	TIMER_SetChnCompare(MDR_TIMER2, TIMER_CHANNEL1, (pulse/2));

	TIMER_Cmd (MDR_TIMER2, ENABLE);
}

void Timer::meas_tim_config()
{
	PORT_InitTypeDef PortInitStructure;
	TIMER_CntInitTypeDef TIM2_Init;
	TIMER_ChnInitTypeDef TIM2_Ch1Init;
	TIMER_ChnOutInitTypeDef	TIM2_Ch1OutInitE0;

	RST_CLK_PCLKcmd (RST_CLK_PCLK_PORTE, ENABLE);
	RST_CLK_PCLKcmd (RST_CLK_PCLK_TIMER2, ENABLE);

	TIMER_BRGInit (MDR_TIMER2, TIMER_HCLKdiv1);// 80MHz

	PORT_StructInit (&PortInitStructure);
	PortInitStructure.PORT_FUNC  = PORT_FUNC_ALTER;
	PortInitStructure.PORT_OE    = PORT_OE_IN;
	PortInitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
	PortInitStructure.PORT_Pin   = PORT_Pin_0;
	PORT_Init (MDR_PORTE, &PortInitStructure);

	TIMER_DeInit (MDR_TIMER2);

	TIM2_Init.TIMER_Period 					 = 65535;
	TIM2_Init.TIMER_Prescaler 				 = 0;
	TIM2_Init.TIMER_CounterDirection		 = TIMER_CntDir_Up;
	TIM2_Init.TIMER_CounterMode 			 = TIMER_CntMode_EvtFixedDir;
	TIM2_Init.TIMER_FilterSampling 			 = TIMER_FDTS_TIMER_CLK_div_1;
	TIM2_Init.TIMER_EventSource 			 = TIMER_EvSrc_CH1;

	TIM2_Init.TIMER_ARR_UpdateMode           = TIMER_ARR_Update_Immediately;
	TIM2_Init.TIMER_ETR_FilterConf           = TIMER_Filter_1FF_at_TIMER_CLK;
	TIM2_Init.TIMER_ETR_Prescaler            = TIMER_ETR_Prescaler_None;
	TIM2_Init.TIMER_ETR_Polarity             = TIMER_ETRPolarity_NonInverted;
	TIM2_Init.TIMER_BRK_Polarity             = TIMER_BRKPolarity_NonInverted;

	TIMER_CntInit (MDR_TIMER2, &TIM2_Init);

	TIMER_ChnStructInit (&TIM2_Ch1Init);
	TIM2_Ch1Init.TIMER_CH_Number = TIMER_CHANNEL1;
	TIM2_Ch1Init.TIMER_CH_Mode = TIMER_CH_MODE_CAPTURE;
	TIM2_Ch1Init.TIMER_CH_REF_Format = TIMER_CH_EvSrc_PE;

	TIMER_ChnInit (MDR_TIMER2, &TIM2_Ch1Init);

	TIMER_ChnOutStructInit (&TIM2_Ch1OutInitE0);
	TIM2_Ch1OutInitE0.TIMER_CH_Number = TIMER_CHANNEL1;
	TIM2_Ch1OutInitE0.TIMER_CH_DirOut_Polarity = TIMER_CHOPolarity_NonInverted;
	TIM2_Ch1OutInitE0.TIMER_CH_DirOut_Source = TIMER_CH_OutSrc_Only_0;
	TIM2_Ch1OutInitE0.TIMER_CH_DirOut_Mode = TIMER_CH_OutMode_Input;

	TIMER_ChnOutInit (MDR_TIMER2, &TIM2_Ch1OutInitE0);

	TIMER_ITConfig (MDR_TIMER2, TIMER_STATUS_CNT_ARR, ENABLE);

	TIMER_Cmd (MDR_TIMER2, ENABLE);

	NVIC_SetPriority (TIMER2_IRQn, 0x02);

	NVIC_EnableIRQ (TIMER2_IRQn);
}

void Timer::read()
{
	cnt_pls_loc = cnt_pls;
}

void Timer::tim_config_rtos()
{
	RST_CLK_PCLKcmd (RST_CLK_PCLK_PORTF, ENABLE);
	PORT_InitTypeDef PortInitStructure;
	PORT_StructInit(&PortInitStructure);
	PortInitStructure.PORT_FUNC  = PORT_FUNC_ALTER;
	PortInitStructure.PORT_OE    = PORT_OE_OUT;
	PortInitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
	PortInitStructure.PORT_Pin   = PORT_Pin_6;
	PortInitStructure.PORT_SPEED = PORT_SPEED_MAXFAST;
	PORT_Init (MDR_PORTE, &PortInitStructure);

	TIMER_CntInitTypeDef TIM1_Init;
	TIMER_ChnInitTypeDef TIM1_Ch1Init;
	TIMER_ChnOutInitTypeDef	TIM1_Ch1OutInitF6;
	TIMER_DeInit (MDR_TIMER1);
	RST_CLK_PCLKcmd (RST_CLK_PCLK_TIMER1, ENABLE);
	TIMER_BRGInit (MDR_TIMER1, TIMER_HCLKdiv1);// 80 MHz
	TIM1_Init.TIMER_Period = 10000;
	TIM1_Init.TIMER_Prescaler = (8000 - 1); //10 KHz
	TIM1_Init.TIMER_CounterDirection = TIMER_CntDir_Up;
	TIM1_Init.TIMER_CounterMode = TIMER_CntMode_ClkFixedDir;
	TIM1_Init.TIMER_FilterSampling = TIMER_FDTS_TIMER_CLK_div_1;
	TIM1_Init.TIMER_EventSource = TIMER_EvSrc_None;

	TIM1_Init.TIMER_ARR_UpdateMode           = TIMER_ARR_Update_Immediately;
	TIM1_Init.TIMER_ETR_FilterConf           = TIMER_Filter_1FF_at_TIMER_CLK;
	TIM1_Init.TIMER_ETR_Prescaler            = TIMER_ETR_Prescaler_None;
	TIM1_Init.TIMER_ETR_Polarity             = TIMER_ETRPolarity_NonInverted;
	TIM1_Init.TIMER_BRK_Polarity             = TIMER_BRKPolarity_NonInverted;

	TIMER_CntInit (MDR_TIMER1, &TIM1_Init);
	/*
	TIMER_ChnStructInit (&TIM1_Ch1Init);
	TIM1_Ch1Init.TIMER_CH_Number = TIMER_CHANNEL1;
	TIM1_Ch1Init.TIMER_CH_Mode = TIMER_CH_MODE_PWM;
	TIM1_Ch1Init.TIMER_CH_REF_Format = TIMER_CH_REF_Format6;
	TIMER_ChnInit (MDR_TIMER1, &TIM1_Ch1Init);

	TIMER_ChnOutStructInit (&TIM1_Ch1OutInitF6);
	TIM1_Ch1OutInitF6.TIMER_CH_Number = TIMER_CHANNEL1;
	TIM1_Ch1OutInitF6.TIMER_CH_DirOut_Polarity = TIMER_CHOPolarity_NonInverted;
	TIM1_Ch1OutInitF6.TIMER_CH_DirOut_Source = TIMER_CH_OutSrc_REF;
	TIM1_Ch1OutInitF6.TIMER_CH_DirOut_Mode = TIMER_CH_OutMode_Output;
	TIM1_Ch1OutInitF6.TIMER_CH_NegOut_Polarity = TIMER_CHOPolarity_NonInverted;
	TIM1_Ch1OutInitF6.TIMER_CH_NegOut_Source = TIMER_CH_OutSrc_REF;
	TIM1_Ch1OutInitF6.TIMER_CH_NegOut_Mode = TIMER_CH_OutMode_Output;
	TIMER_ChnOutInit (MDR_TIMER1, &TIM1_Ch1OutInitF6);
	*/
	//TIMER_DMACmd (MDR_TIMER2, TIMER_STATUS_CNT_ARR, ENABLE);
	//TIMER_SetChnCompare(MDR_TIMER1, TIMER_CHANNEL1, (pulse/2));

	TIMER_ITConfig (MDR_TIMER1, TIMER_STATUS_CNT_ARR, ENABLE);

	NVIC_SetPriority (TIMER1_IRQn, 0x01);

	NVIC_EnableIRQ (TIMER1_IRQn);

	TIMER_Cmd (MDR_TIMER1, ENABLE);
}

void Timer::start_tim()
{
	TIMER_Cmd (MDR_TIMER1, ENABLE);
}

} // namespace timer

namespace handlers {
extern "C" void Timer2_IRQHandler()
{
uint32_t cnt_pls_irq = MDR_TIMER2->CNT;
timer::cnt_pls = cnt_pls_irq;
//timer::Timer::cnt_pls_loc = timer::cnt_pls;
TIMER_Cmd (MDR_TIMER2, DISABLE);
TIMER_ClearITPendingBit (MDR_TIMER2, TIMER_STATUS_CNT_ARR);
//kip5::meas::Meas_frq_by_frq::launch_meas();
}

extern "C" void Timer1_IRQHandler()
{
TIMER_Cmd (MDR_TIMER1, DISABLE);
TIMER_ClearITPendingBit (MDR_TIMER1, TIMER_STATUS_CNT_ARR);
}
} // namespace handlers


} // namespace kip5


