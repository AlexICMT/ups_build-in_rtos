/* ups_build-in
 * /ups_build-in/src/i2c_src/i2c.cpp
 * i2c.cpp
 * 
 *  Created on: 26 нояб. 2021 г.
 *  	  Time: 08:28:54
 *      Author: AlexICMT
 */

#include "i2c.hpp"
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_i2c.h"
#include "MDR32F9Qx_port.h"

namespace kip5 {
namespace i2c {

const uint32_t I2c_time 	= 30;
const uint32_t I2c_timeout 	= 1;
const uint32_t NACK 		= 2;

static void initInternalI2CPorts()
{
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTC, ENABLE);

		PORT_InitTypeDef portInit;

		portInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
		portInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
		portInit.PORT_PD_SHM = PORT_PD_SHM_ON;
		portInit.PORT_PD = PORT_PD_DRIVER;
		//portInit.PORT_GFEN = PORT_GFEN_OFF;
		portInit.PORT_SPEED = PORT_SPEED_MAXFAST;
		portInit.PORT_MODE = PORT_MODE_DIGITAL;

		// PC0.SCL:
		portInit.PORT_Pin = PORT_Pin_0;
		portInit.PORT_FUNC = PORT_FUNC_ALTER;
		portInit.PORT_OE = PORT_OE_OUT;
		PORT_Init(MDR_PORTC, &portInit);

		// PC0.SDA:
		portInit.PORT_Pin = PORT_Pin_1;
		portInit.PORT_FUNC = PORT_FUNC_ALTER;
		portInit.PORT_OE = PORT_OE_OUT;//IN;
		PORT_Init(MDR_PORTC, &portInit);
}

I2c::I2c()
{
	initInternalI2CPorts();
	init();
}

uint8_t I2c::send(uint8_t addr, const uint8_t* data, int size)
{
	uint16_t I2C_Timeout;

    I2C_Timeout = I2c_time;
	while(!I2C_CheckEvent(I2C_EVENT_BUS_FREE))
	{
		if((I2C_Timeout--) == 0)
			return I2c_timeout;
	}

	I2C_Timeout = I2c_time;
	I2C_Send7bitAddress(addr, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C_EVENT_ACK_FOUND))
	{
		if(I2C_CheckEvent(I2C_EVENT_NACK_FOUND))
		{
			I2C_SendSTOP();
			return NACK;
		}
		if((I2C_Timeout--) == 0)
		{
			I2C_SendSTOP();
			return I2c_timeout;
		}
	}
	for(int i = 0; i < size; ++i)
	{
		I2C_Timeout = I2c_time;
		I2C_SendByte(*(data+i));
		while(!I2C_CheckEvent(I2C_EVENT_ACK_FOUND))
		{

			if(I2C_CheckEvent(I2C_EVENT_NACK_FOUND))
			{
				I2C_SendSTOP();
				return NACK;
			}
			if((I2C_Timeout--) == 0)
			{
				I2C_SendSTOP();
				return I2c_timeout;
			}
		}
	}

	I2C_Timeout = I2c_time;
	while(!I2C_CheckEvent(I2C_EVENT_BUS_FREE))
	{
		if((I2C_Timeout--) == 0)
			break;
	}
	I2C_SendSTOP();
	return 0;
}

uint8_t I2c::send_mcp23017(uint8_t addr, uint8_t reg, uint8_t* data, int size)
{
	uint16_t I2C_Timeout;

    I2C_Timeout = I2c_time;
	while(!I2C_CheckEvent(I2C_EVENT_BUS_FREE))
	{
		if((I2C_Timeout--) == 0)
			return I2c_timeout;
	}

	I2C_Timeout = I2c_time;
	I2C_Send7bitAddress(addr, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C_EVENT_ACK_FOUND))
	{
		if(I2C_CheckEvent(I2C_EVENT_NACK_FOUND))
		{
			I2C_SendSTOP();
			return NACK;
		}
		if((I2C_Timeout--) == 0)
		{
			I2C_SendSTOP();
			return I2c_timeout;
		}
	}
	I2C_SendByte(reg);
	while(!I2C_CheckEvent(I2C_EVENT_ACK_FOUND))
	{
		if(I2C_CheckEvent(I2C_EVENT_NACK_FOUND))
		{
			I2C_SendSTOP();
			return NACK;
		}
	}
	//I2C_StartReceiveData(I2C_Send_to_Slave_NACK); alexicmt: SR interrupts transmitting data
	I2C_Send7bitAddress(addr, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C_EVENT_ACK_FOUND))
		{
			if(I2C_CheckEvent(I2C_EVENT_NACK_FOUND))
			{
				I2C_SendSTOP();
				return NACK;
			}
		}
	//I2C_StartReceiveData(I2C_Send_to_Slave_NACK); alexicmt: SR interrupts transmitting data
	for(int i = 0; i < size; ++i)
	{
		I2C_Timeout = I2c_time;
		I2C_SendByte(*(data+i));
		while(!I2C_CheckEvent(I2C_EVENT_ACK_FOUND))
		{

			if(I2C_CheckEvent(I2C_EVENT_NACK_FOUND))
			{
				I2C_SendSTOP();
				return NACK;
			}
			if((I2C_Timeout--) == 0)
			{
				I2C_SendSTOP();
				return I2c_timeout;
			}
		}
	}

	I2C_Timeout = I2c_time;
	while(!I2C_CheckEvent(I2C_EVENT_BUS_FREE))
	{
		if((I2C_Timeout--) == 0)
			break;
	}
	I2C_SendSTOP();
	return 0;
}

void I2c::init()
{
	RST_CLK_PCLKcmd(RST_CLK_PCLK_I2C, ENABLE);

	I2C_DeInit();
	I2C_InitTypeDef I2C_InitStruct;
	I2C_InitStruct.I2C_ClkDiv = 25;
	I2C_InitStruct.I2C_Speed = I2C_SPEED_UP_TO_400KHz;
	I2C_Init(&I2C_InitStruct);

	I2C_Cmd(ENABLE);
}

uint8_t I2c::start(uint8_t addr, uint32_t dir)
{
uint16_t I2C_Timeout;
I2C_Timeout = I2c_time;
while(!I2C_CheckEvent(I2C_EVENT_BUS_FREE))
{
	if((I2C_Timeout--) == 0)
		return I2c_timeout;
}
I2C_Timeout = I2c_time;
I2C_Send7bitAddress(addr, dir);
while(!I2C_CheckEvent(I2C_EVENT_ACK_FOUND))
{
	if((I2C_Timeout--) == 0)
		return I2c_timeout;
	if(I2C_CheckEvent(I2C_EVENT_NACK_FOUND))
	{
		I2C_SendSTOP();
		return NACK;
	}
}
return 0;
}

uint8_t I2c::rw(uint32_t dir, uint8_t *data, int size)
{
//while(!I2C_CheckEvent(I2C_EVENT_TRANSFER_ENABLED));
uint16_t I2C_Timeout;
I2C_Timeout = I2c_time;
if(dir == Direction_Transmitter)
{
	for(int i = 0; i < size; ++i)
	{
		I2C_SendByte(*(data+i));
		while(!I2C_CheckEvent(I2C_EVENT_ACK_FOUND))//or I2C_EVENT_TRANSFER_ENABLED
		{
			if((I2C_Timeout--) == 0)
				return I2c_timeout;
			if(I2C_CheckEvent(I2C_EVENT_NACK_FOUND))
			{
				I2C_SendSTOP();
				return NACK;
			}
		}
	}
}
else
{
	I2C_Timeout = I2c_time;
	//while(!I2C_CheckEvent(I2C_EVENT_TRANSFER_ENABLED));
	I2C_StartReceiveData(I2C_Send_to_Slave_NACK);
	for(int i = 0; i < size; ++i)
	{
		while(!I2C_GetFlagStatus(I2C_FLAG_CMD_nRD))
		{
			if((I2C_Timeout--) == 0)
			{
				I2C_SendSTOP();
				return I2c_timeout;
			}
		}
		*(data+i) = I2C_GetReceivedData();
	}
	I2C_Timeout = I2c_time;//
	while(!I2C_CheckEvent(I2C_EVENT_BUS_FREE))
	{
		if((I2C_Timeout--) == 0)
			break;
	}
I2C_SendSTOP();
}
return 0;
}

uint8_t I2c::rw(uint32_t dir, uint8_t *data, int size, int ack)
{
//while(!I2C_CheckEvent(I2C_EVENT_TRANSFER_ENABLED));
uint16_t I2C_Timeout;
I2C_Timeout = I2c_time;
if(dir == Direction_Transmitter)
{
	for(int i = 0; i < size; ++i)
	{
		I2C_SendByte(*(data+i));
		while(!I2C_CheckEvent(I2C_EVENT_ACK_FOUND))//or I2C_EVENT_TRANSFER_ENABLED
		{
			if((I2C_Timeout--) == 0)
				return I2c_timeout;
			if(I2C_CheckEvent(I2C_EVENT_NACK_FOUND))
			{
				I2C_SendSTOP();
				return NACK;
			}
		}
	}
}
else
{
	I2C_Timeout = I2c_time;
	//while(!I2C_CheckEvent(I2C_EVENT_TRANSFER_ENABLED));
	I2C_StartReceiveData(ack);
	for(int i = 0; i < size; ++i)
	{
		while(!I2C_GetFlagStatus(I2C_FLAG_CMD_nRD))
		{
			if((I2C_Timeout--) == 0)
			{
				I2C_SendSTOP();
				return I2c_timeout;
			}
		}
		*(data+i) = I2C_GetReceivedData();
	}
	I2C_Timeout = I2c_time;//
	while(!I2C_CheckEvent(I2C_EVENT_BUS_FREE))
	{
		if((I2C_Timeout--) == 0)
			break;
	}
I2C_SendSTOP();
}
return 0;
}

void I2c::stop()
{
	I2C_SendSTOP();
}

uint8_t I2c::receive_mcp23017(uint8_t addr, uint8_t reg, uint8_t* data, int size)
{
	uint16_t I2C_Timeout;
	I2C_Timeout = I2c_time;
	while(!I2C_CheckEvent(I2C_EVENT_BUS_FREE))
		{
			if((I2C_Timeout--) == 0)
				return I2c_timeout;
		}
	I2C_Timeout = I2c_time;
	I2C_Send7bitAddress(addr, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C_EVENT_ACK_FOUND))
		{
			if(I2C_CheckEvent(I2C_EVENT_NACK_FOUND))
			{
				I2C_SendSTOP();
				return NACK;
			}
			if((I2C_Timeout--) == 0)
			{
				I2C_SendSTOP();
				return I2c_timeout;
			}

		}
	I2C_SendByte(reg);
	while(!I2C_CheckEvent(I2C_EVENT_ACK_FOUND))
		{
			if(I2C_CheckEvent(I2C_EVENT_NACK_FOUND))
			{
				I2C_SendSTOP();
				return NACK;
			}
		}
	//I2C_StartReceiveData(I2C_Send_to_Slave_NACK);
	I2C_Send7bitAddress(addr, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C_EVENT_ACK_FOUND))
		{
			if(I2C_CheckEvent(I2C_EVENT_NACK_FOUND))
			{
				I2C_SendSTOP();
				return NACK;
			}
		}
	I2C_StartReceiveData(I2C_Send_to_Slave_NACK);

	for(int i = 0; i < size; ++i)
	{
		while(!I2C_GetFlagStatus(I2C_FLAG_CMD_nRD));
		*(data+i) = I2C_GetReceivedData();
	}
	I2C_Timeout = I2c_time;
	while(!I2C_CheckEvent(I2C_EVENT_BUS_FREE))
	{
		if((I2C_Timeout--) == 0)
			break;
	}
	I2C_SendSTOP();
	return 0;
}



} // namespace i2c
} // namespace kip5


