/*
 * low_level_init.cpp
 *
 *  Created on: Oct 27, 2021
 *      Author: AlexICMT
 */

#include <cstdint>
#include "low_level_init.hpp"

namespace kip5 {
namespace init {

/*constexpr uint32_t allPortsClk =
	RST_CLK_PCLK_PORTA | RST_CLK_PCLK_PORTB | RST_CLK_PCLK_PORTC |
	RST_CLK_PCLK_PORTD | RST_CLK_PCLK_PORTE | RST_CLK_PCLK_PORTF;
*/
/// ������������� ������� ����
void Low_init::frequencyClkPll(unsigned pll_on, unsigned int pll_mul)
{
	// pll_mul - ����������� ��������� ��� CPU PLL
	// pll_pld - ��� ����������� PLL
	// pll_on - ��� ��������� PLL
	MDR_RST_CLK->HS_CONTROL = 0b01;					// �������� ������� ��������� HSE

	// ������� ������� HSE � PLL
	while(MDR_RST_CLK->CLOCK_STATUS == 0b000)
		__NOP();

	MDR_RST_CLK->CPU_CLOCK = 0b0100000010;			// �������� CPU_CLK

	if(pll_on == 1)
	{
		MDR_RST_CLK->PLL_CONTROL = (pll_on << 2) | (pll_mul << 8);	// ��� PLL
		__attribute__((unused)) volatile unsigned int dddd = (pll_on << 2) | (pll_mul << 8);

		// ������� ������� HSE � PLL
		while(MDR_RST_CLK->CLOCK_STATUS < 0b110)
			__NOP();

		MDR_RST_CLK->CPU_CLOCK = 0b0100000110;		// �������� CPU_CLK
	}
}

/// �������� CLK �� ��� �����
/*void Low_init::initClkForAllPorts()
{
	MDR_RST_CLK->PER_CLOCK |= allPortsClk;			// �������� CLK �� �����
}

/// ������������� ���� ������ � ��������� ��� ����� ������ ��, �.�. � ����� ����������� �����������������
void Low_init::initAllPorts()
{
	RST_CLK_PCLKcmd(allPortsClk, ENABLE);			// �������� CLK �� �����

	PORT_InitTypeDef PORT_InitStructure;
	PORT_StructInit(&PORT_InitStructure);

	PORT_Init(MDR_PORTA, &PORT_InitStructure);
	PORT_Init(MDR_PORTB, &PORT_InitStructure);
	PORT_Init(MDR_PORTC, &PORT_InitStructure);
	PORT_Init(MDR_PORTD, &PORT_InitStructure);
	PORT_Init(MDR_PORTE, &PORT_InitStructure);
	PORT_Init(MDR_PORTF, &PORT_InitStructure);

	RST_CLK_PCLKcmd(allPortsClk, DISABLE);			// ��������� CLK �� �����
}*/

} //namespace mdr32
} //namespace kip5
