/* ups_build-in
 * /ups_build-in/src/dac_src/dac.cpp
 * dac.cpp
 * 
 *  Created on: 22 нояб. 2021 г.
 *  	  Time: 12:00:59
 *      Author: AlexICMT
 */

#include "dac.hpp"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_dac.h"

namespace kip5 {
namespace dac {

Dac::Dac()
{
	dac_init();
}

void Dac::dac_init()
{
	RST_CLK_PCLKcmd(RST_CLK_PCLK_DAC, ENABLE);
	port_dac_init();
	DAC_DeInit();
	DAC_Init(DAC_SYNC_MODE_Independent, DAC1_AVCC, DAC2_AVCC);
	DAC1_Cmd(DISABLE);
	DAC2_Cmd(ENABLE);
	//dac_launch();
}

void Dac::port_dac_init()
{
	PORT_InitTypeDef PortInitStructure;
	PORT_StructInit(&PortInitStructure);
	PortInitStructure.PORT_FUNC  = PORT_FUNC_ALTER;
	PortInitStructure.PORT_OE    = PORT_OE_OUT;
	PortInitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
	PortInitStructure.PORT_Pin   = PORT_Pin_0;
	PortInitStructure.PORT_SPEED = PORT_SPEED_MAXFAST;
	PORT_Init (MDR_PORTE, &PortInitStructure);
}

void Dac::dac_launch()
{
	DAC2_SetData(1300);
}

void Dac::setDac(int data)
{
	DAC2_SetData((uint32_t)data);
}

int Dac::getDac()
{
	return (int)DAC2_GetData();
}

} // namespace dac
} // namespace kip5


