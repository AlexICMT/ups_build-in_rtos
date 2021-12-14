/*
 * port.cpp
 *
 *  Created on: 28/10/2021.
 *      Author: AlexICMT
 */

#include "port.hpp"


namespace kip5{
namespace port{

static_assert(sizeof(PortBitBase) == 4, "sizeof(PortBit) must be 4 bytes");
static_assert(sizeof(OutPortBit) == 4, "sizeof(OPortBit) must be 4 bytes");
static_assert(sizeof(InPortBit) == 4, "sizeof(IPortBit) must be 4 bytes");

static_assert(sizeof(PortBitNullBase) == 1, "sizeof(PortBitNull) must be 1 byte");
static_assert(sizeof(OutPortBitNull) == 1, "sizeof(OPortBitNull) must be 1 byte");
static_assert(sizeof(InPortBitNull) == 1, "sizeof(IPortBitNull) must be 1 byte");

/*void enableClkToAllPorts(bool enable)
{
	constexpr uint32_t RST_CLK_PCLK = RST_CLK_PCLK_PORTA | RST_CLK_PCLK_PORTB | RST_CLK_PCLK_PORTC | RST_CLK_PCLK_PORTD | RST_CLK_PCLK_PORTE | RST_CLK_PCLK_PORTF;
	RST_CLK_PCLKcmd(RST_CLK_PCLK, enable ? ENABLE : DISABLE);
}*/

void enableClkToPort(MDR_PORT_TypeDef* port, bool enable)
{
	RST_CLK_PCLKcmd(PCLK_BIT(port), enable ? ENABLE : DISABLE);
}

OutPort::OutPort(MDR_PORT_TypeDef* port, unsigned bit) : Port(port, bit)
{
	PORT_InitTypeDef PORT_InitStruct;
	//RST_CLK_PCLKcmd(PCLK_BIT(port),ENABLE); //test
	PORT_StructInit(&PORT_InitStruct);
	PORT_InitStruct.PORT_Pin = bit;
	PORT_InitStruct.PORT_MODE  = PORT_MODE_DIGITAL;
	PORT_InitStruct.PORT_FUNC  = PORT_FUNC_PORT;
	PORT_InitStruct.PORT_OE    = PORT_OE_OUT;
	PORT_InitStruct.PORT_SPEED = PORT_SPEED_MAXFAST; //PORT_SPEED_MAXFAST
	PORT_Init(port, &PORT_InitStruct);
	clr();
}

InPort::InPort(MDR_PORT_TypeDef* port, unsigned bit) : Port(port, bit)
{
	PORT_InitTypeDef PORT_InitStruct;
	PORT_StructInit(&PORT_InitStruct);
	PORT_InitStruct.PORT_Pin = 1u << bit;
	PORT_InitStruct.PORT_OE = PORT_OE_IN;
	PORT_InitStruct.PORT_FUNC = PORT_FUNC_PORT;
	PORT_InitStruct.PORT_MODE = PORT_MODE_DIGITAL;
	PORT_InitStruct.PORT_SPEED = PORT_SPEED_MAXFAST;
	PORT_InitStruct.PORT_PULL_UP = PORT_PULL_UP_ON;
	PORT_InitStruct.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PORT_InitStruct.PORT_PD_SHM = PORT_PD_SHM_ON;
	PORT_Init(port, &PORT_InitStruct);
}

PortBitBase::PortBitBase(MDR_PORT_TypeDef* port, unsigned bit) : _portBit(utils::bit_banding(&port->RXTX, bit))
{
	enableClkToPort(port);
}

OutPortBit::OutPortBit(MDR_PORT_TypeDef* port, unsigned bit) : PortBitBase(port, bit)
{
	PORT_InitTypeDef PORT_InitStruct;
	PORT_StructInit(&PORT_InitStruct);
	PORT_InitStruct.PORT_Pin = 1u << bit;
	PORT_InitStruct.PORT_OE = PORT_OE_OUT;
	PORT_InitStruct.PORT_FUNC = PORT_FUNC_PORT;
	PORT_InitStruct.PORT_MODE = PORT_MODE_DIGITAL;
	PORT_InitStruct.PORT_SPEED = PORT_SPEED_MAXFAST;
	PORT_Init(port, &PORT_InitStruct);
	clr();
}

InPortBit::InPortBit(MDR_PORT_TypeDef* port, unsigned bit) : PortBitBase(port, bit)
{
	PORT_InitTypeDef PORT_InitStruct;
	PORT_StructInit(&PORT_InitStruct);
	PORT_InitStruct.PORT_Pin = 1u << bit;
	PORT_InitStruct.PORT_OE = PORT_OE_IN;
	PORT_InitStruct.PORT_FUNC = PORT_FUNC_PORT;
	PORT_InitStruct.PORT_MODE = PORT_MODE_DIGITAL;
	PORT_InitStruct.PORT_SPEED = PORT_SPEED_MAXFAST;
	PORT_InitStruct.PORT_PULL_UP = PORT_PULL_UP_ON;
	PORT_InitStruct.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PORT_InitStruct.PORT_PD_SHM = PORT_PD_SHM_ON;
	PORT_Init(port, &PORT_InitStruct);
}

} //namespace kip5
} //namespace port



