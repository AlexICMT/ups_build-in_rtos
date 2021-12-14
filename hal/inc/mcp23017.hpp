/* ups_build-in
 * /ups_build-in/inc/mcp23017_inc/mcp23017.hpp
 * mcp23017.hpp
 * 
 *  Created on: 28 нояб. 2021 г.
 *  	  Time: 15:08:14
 *      Author: AlexICMT
 */

#ifndef INC_MCP23017_INC_MCP23017_HPP_
#define INC_MCP23017_INC_MCP23017_HPP_

//#include "MDR32F9Qx_config.h"
#include "i2c.hpp"
#include "helper.hpp"

namespace kip5 {
namespace mcp23017 {

class MCP23017
{
	enum MCP23017_Registers : uint8_t
	{
		ADDR_IODIR		= 0x00,		///<
		ADDR_IPOL		= 0x02,		///<
		ADDR_GPINTEN	= 0x04,		///<
		ADDR_DEFVAL		= 0x06,		///<
		ADDR_INTCON		= 0x08,		///<
		ADDR_IOCON		= 0x0A,		///<
		ADDR_GPPU		= 0x0C,		///<
		ADDR_INTF		= 0x0E,		///<
		ADDR_INTCAP		= 0x10,		///<
		ADDR_GPIO		= 0x12,		///<
		ADDR_OLAT		= 0x14,		///<

		ADDR_IODIR_B	= 0x01,		///<
		ADDR_IPOL_B		= 0x03,		///<
		ADDR_GPINTEN_B	= 0x05,		///<
		ADDR_DEFVAL_B	= 0x07,		///<
		ADDR_INTCON_B	= 0x09,		///<
		ADDR_IOCON_B	= 0x0B,		///<
		ADDR_GPPU_B		= 0x0D,		///<
		ADDR_INTF_B		= 0x0F,		///<
		ADDR_INTCAP_B	= 0x11,		///<
		ADDR_GPIO_B		= 0x13,		///<
		ADDR_OLAT_B		= 0x15		///<
	};

	struct BitsIOCON
	{
		uint8_t			: 1;	///< not use
		uint8_t INTPOL	: 1;	///<
		uint8_t ODR		: 1;	///<
		uint8_t HAEN	: 1;	///<
		uint8_t DISSLW	: 1;	///<
		uint8_t SEQOP	: 1;	///<
		uint8_t MIRROR	: 1;	///<
		uint8_t BANK	: 1;	///<
	};

	struct BitsIODIR : helper::Bits8 {};

	using RegIODIRA = helper::ByteUnion<BitsIODIR, 0xFF>;
	using RegIODIRB = helper::ByteUnion<BitsIODIR, 0xFF>;

	using RegIOCONA = helper::Union<uint8_t, BitsIOCON>;
	using RegIOCONB = helper::Union<uint8_t, BitsIOCON>;

public:
	enum Pin : uint8_t
		{
			GP0		= 0b00000001,
			GP1		= 0b00000010,
			GP2		= 0b00000100,
			GP3		= 0b00001000,
			GP4		= 0b00010000,
			GP5		= 0b00100000,
			GP6		= 0b01000000,
			GP7		= 0b10000000,
			GPAll	= 0b11111111
		};

	enum MCP23017_ErrorCodes : uint8_t
		{
			MCP_ERR_OK			= 0,
			MCP_ERR_NOT_WORK	= 1,
			MCP_ERR_IO			= 2,
			MCP_ERR_WRONG_CONF	= 3
		};

	MCP23017(i2c::I2c& i2c, uint8_t addr);
	MCP23017(const MCP23017&) = delete;
	uint8_t On(uint8_t addr);
	uint8_t Off();
	void init();

	//------------------------------------------------------------------------
		/**
		 * Set pins to input mode
		 *
		 * This function is used to set which pins are inputs (if any). Example:
		 * set_inputs ( Pin_GP0 | Pin_GP1 | Pin_GP2 );
		 * Note that these are set to input in addition to the previously set.
		 * In other words, the following:
		 * set_inputs ( Pin_GP1 );
		 * set_inputs ( Pin_GP2 );
		 * Results in at least two pins set to input.
		 *
		 * @param pins A bitmask of pins to set to input mode.
		 * @return
		 */
		uint8_t Set_input_pins(uint8_t pins);

		/**
		 * Set pins to output mode
		 *
		 * This function is used to set which pins are outputs (if any). Example:
		 * set_outputs ( Pin_GP0 | Pin_GP1 | Pin_GP2 );
		 * Note that these are set to output in addition to the previously set.
		 * In other words, the following:
		 * set_outputs ( Pin_GP1 );
		 * set_outputs ( Pin_GP2 );
		 * Results in at least two pins set to output.
		 *
		 * @param pins A bitmask of pins to set to output mode.
		 * @return
		 */
		uint8_t Set_output_pins(uint8_t pins);

		/**
		 * Write to the output pins.
		 *
		 * This function is used to set output pins on or off.
		 *
		 * @param values A bitmask indicating whether a pin should be on or off.
		 * @return
		 */
		uint8_t Write_outputs(uint8_t values);
		uint8_t Write_outputs1(uint8_t values);
		uint8_t Write_outputs_mode(uint8_t values);
		uint8_t Write_outputs_mode1(uint8_t values);
		/**
		 * Read back the outputs.
		 *
		 * This function is used to read the last values written to the output pins.
		 *
		 * @param data
		 * @return The value from the OLAT register.
		 */
		uint8_t Read_outputs(uint8_t *data);
		uint8_t Read_outputs_mode(uint8_t *data);

		/**
		 * Read from the input pins.
		 *
		 * This function is used to read the values from the input pins.
		 *
		 * @param
		 * @return A bitmask of the current state of the input pins.
		 */
		uint8_t Read_inputs(uint8_t *data);

		/**
		 * Set the input pin polarity.
		 *
		 * This function sets the polarity of the input pins.
		 * A 1 bit is inverted polarity, a 0 is normal.
		 *
		 * @param values A bitmask of the input polarity.
		 * @return
		 */
		uint8_t Set_input_polarity(uint8_t values);

		/**
		 * Read back the current input pin polarity.
		 *
		 * This function reads the current state of the input pin polarity.
		 *
		 * @param
		 * @return The value from the IPOL register.
		 */
		uint8_t Get_input_polarity(uint8_t *data);

		/**
		 * Enable and disable the internal pull-up resistors for input pins.
		 *
		 * This function enables the internal 100 kOhm pull-up resistors.
		 * A 1 bit enables the pull-up resistor for the corresponding input pin.
		 *
		 * @param values A bitmask indicating which pull-up resistors should be enabled/disabled.
		 * @return
		 */
		uint8_t Set_pullups(uint8_t values);

		/**
		 * Get the current state of the internal pull-up resistors.
		 *
		 * @param
		 * @return The current state of the pull-up resistors.
		 */
		uint8_t Get_pullups(uint8_t *data);

		/**
		 * Generate an interrupt when a pin changes.
		 *
		 * This function enables interrupt generation for the specified pins.
		 * The interrupt is active-low by default.
		 * The function acknowledge_interrupt must be called before another
		 * interrupt will be generated.
		 * Example:
		 * @code
		 * InterruptIn in(p16);
		 * MCP23017 mcp(p9, p10, 0);
		 * in.fall (&interrupt);
		 * mcp.interrupt_on_changes(MCP23017::Pin_GP0);
		 * while(1)
		 * {
		 *      wait(1);
		 * }
		 * @endcode
		 *
		 * @param pins A bitmask of the pins that may generate an interrupt.
		 * @return
		 */
		uint8_t Interrupt_on_changes(uint8_t pins);

		/**
		 * Disables interrupts for the specified pins.
		 *
		 * @param pins A bitmask indicating which interrupts should be disabled.
		 * @return
		 */
		uint8_t Disable_interrupts(uint8_t pins);

		/**
		 * Acknowledge a generated interrupt.
		 *
		 * This function must be called when an interrupt is generated to discover
		 * which pin caused the interrupt and to enable future interrupts.
		 *
		 * @param pin An output parameter that specifies which pin generated the interrupt.
		 * @param values The current state of the input pins.
		 * @return
		 */
		uint8_t Acknowledge_interrupt(uint8_t *pin, uint8_t *values);

		uint8_t SoftReset();
	//	void 	HardReset();

	protected:
		uint8_t Read_register(uint8_t reg, uint8_t *data);
		uint8_t Write_register(uint8_t reg, uint8_t value);
		uint8_t Write_register_mode(uint8_t reg, uint8_t value);
		uint8_t Write_mask(uint8_t reg, uint8_t mask, bool value);

	private:
		i2c::I2c& i2c;
		uint8_t i2c_address;
		uint8_t reg_subaddr;
};

} // namespace mcp23017
} // namespace kip5

#endif /* INC_MCP23017_INC_MCP23017_HPP_ */
