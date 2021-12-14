/* ups_build-in
 * /ups_build-in/src/mcp23017_src/mcp23017.cpp
 * mcp23017.cpp
 * 
 *  Created on: 28 нояб. 2021 г.
 *  	  Time: 15:08:31
 *      Author: AlexICMT
 */

#include "mcp23017.hpp"
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_rst_clk.h"

namespace kip5 {
namespace mcp23017 {

void MCP23017::init()
{
	RegIOCONA con;
	con[0] = 255;
	con.bits.INTPOL		= 0;
	con.bits.ODR		= 1;
	con.bits.HAEN		= 0;
	con.bits.DISSLW		= 0;
	con.bits.SEQOP		= 1;
	con.bits.MIRROR		= 0;
	con.bits.BANK		= 0;
	Write_register(ADDR_IOCON, con);

	RegIODIRA dir;
	//dir.byte = 0xFF;
	Write_register(ADDR_IODIR, dir);//default: dir.byte = 0xFF
	dir.byte = 0x00;
	Write_register(ADDR_IODIR_B, dir);
}

MCP23017::MCP23017(i2c::I2c& i2c, uint8_t addr) : i2c(i2c), i2c_address(addr)
{
	init();
}

uint8_t MCP23017::On(uint8_t addr)
{
	i2c_address = addr & 0xFE;
	reg_subaddr = addr & 0x01;
	return SoftReset();
}

uint8_t MCP23017::Off()
{
	return SoftReset();
}

uint8_t MCP23017::Set_input_pins(uint8_t pins)
{
	uint8_t error = 0;
	uint8_t value;

	error = Read_register(ADDR_IODIR, &value);
	error |= Write_register(ADDR_IODIR, (value | pins));

	return error;
}

uint8_t MCP23017::Set_output_pins(uint8_t pins)
{
	uint8_t error = 0;
	uint8_t value;

	error = Read_register(ADDR_IODIR, &value);
	error |= Write_register(ADDR_IODIR, (value & ~pins));

	return error;
}

uint8_t MCP23017::Write_outputs(uint8_t values)
{
	uint8_t error = 0;
	error = Write_register(ADDR_GPIO_B, values);
	return error;
}

uint8_t MCP23017::Write_outputs1(uint8_t values)
{
	uint8_t error = 0;
	error = Write_register(ADDR_OLAT_B, values);
	return error;
}

uint8_t MCP23017::Write_outputs_mode(uint8_t values)
{
	uint8_t error = 0;
	error = Write_register_mode(ADDR_GPIO_B, values);
	return error;
}

uint8_t MCP23017::Write_outputs_mode1(uint8_t values)
{
	uint8_t error = 0;
	error = Write_register_mode(ADDR_OLAT_B, values);
	return error;
}

uint8_t MCP23017::Read_outputs(uint8_t *data)
{
	uint8_t error = 0;

	error = Read_register(ADDR_GPIO_B, data);

	return error;
}

uint8_t MCP23017::Read_outputs_mode(uint8_t *data)
{
	uint8_t error = 0;

	error = Read_register(ADDR_OLAT_B, data);

	return error;
}

uint8_t MCP23017::Read_inputs(uint8_t *data)
{
	uint8_t error = 0;

	error = Read_register(ADDR_GPIO, data);

	return error;
}


uint8_t MCP23017::Set_input_polarity(uint8_t values)
{
	uint8_t error = 0;

	error = Write_register(ADDR_IPOL, values);

	return error;
}

uint8_t MCP23017::Get_input_polarity(uint8_t *data)
{
	uint8_t error = 0;

	error = Read_register(ADDR_IPOL, data);

	return error;
}

uint8_t MCP23017::Set_pullups(uint8_t values)
{
	uint8_t error = 0;

	error = Write_register(ADDR_GPPU, values);

	return error;
}

uint8_t MCP23017::Get_pullups(uint8_t *data)
{
	uint8_t error = 0;

	error = Read_register(ADDR_GPPU, data);

	return error;
}

uint8_t MCP23017::Interrupt_on_changes(uint8_t pins)
{
	uint8_t error = 0;
	uint8_t value;

	error = Read_register(ADDR_INTCON, &value);
	value &= ~pins;
	error |= Write_register(ADDR_INTCON, value);
	error |= Read_register(ADDR_GPINTEN, &value);
	value |= pins;
	error |= Write_register(ADDR_GPINTEN, value);

	return error;
}

uint8_t MCP23017::Disable_interrupts(uint8_t pins)
{
	uint8_t error = 0;
	uint8_t value;

	error = Read_register(ADDR_GPINTEN, &value);
	value &= ~pins;
	error |= Write_register(ADDR_GPINTEN, value);

	return error;
}

uint8_t MCP23017::Acknowledge_interrupt(uint8_t *pin, uint8_t *values)
{
	uint8_t error = 0;

	error = Read_register(ADDR_INTF, pin);
	error |= Read_register(ADDR_INTCAP, values);

	return error;
}

uint8_t MCP23017::Read_register(uint8_t reg, uint8_t *data)
{
	uint8_t error = 0;
	error = i2c.receive_mcp23017(i2c_address, reg, data, 1);

	if(error > 0)
	{
		error = MCP_ERR_IO;
	}

	return error;
}

uint8_t MCP23017::Write_register(uint8_t reg, uint8_t value)
{
	uint8_t error = 0;
	uint8_t reg_data[2];

	reg_data[0] = reg;// | reg_subaddr;
	reg_data[1] = value;

	error = i2c.send(i2c_address, &reg_data[0], 2);

	if(error > 0)
	{
		error = MCP_ERR_IO;
	}

	return error;
}

uint8_t MCP23017::Write_register_mode(uint8_t reg, uint8_t value)
{
	uint8_t error = 0;
	uint8_t reg_data[2];

	reg_data[0] = reg;// | reg_subaddr;
	reg_data[1] = value;

	error = i2c.send_mcp23017(i2c_address, reg_data[0], &reg_data[1], 1);

	if(error > 0)
	{
		error = MCP_ERR_IO;
	}

	return error;
}

uint8_t MCP23017::Write_mask(uint8_t reg, uint8_t mask, bool value)
{
	uint8_t val;
	uint8_t error = 0;

	error = Read_register(reg, &val);

	if(value)
		val |= mask;
	else
		val &= ~mask;

	error |= Write_register(reg, val);

	return error;
}

uint8_t MCP23017::SoftReset()
{
	uint8_t error = 0;

	error = Write_register(ADDR_IODIR, 0xFF);
	for(uint8_t reg = ADDR_IPOL; reg <= ADDR_OLAT_B; reg++)
		error |= Write_register(reg, 0);

	return error;
}


} // namespace mcp23017
} // namespace kip5



