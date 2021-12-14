/* ups_build-in
 * /ups_build-in/inc/i2c_inc/i2c.hpp
 * i2c.hpp
 * 
 *  Created on: 26 нояб. 2021 г.
 *  	  Time: 08:28:41
 *      Author: AlexICMT
 */

#ifndef INC_I2C_INC_I2C_HPP_
#define INC_I2C_INC_I2C_HPP_

//#include "MDR32F9Qx_config.h"
#include <cstdint>

namespace kip5 {
namespace i2c {

class I2c
{
public:
	const uint32_t Direction_Transmitter = 0x0;
	const uint32_t Direction_Receiver = 0x1;

	I2c();
	I2c(const I2c&) = delete;

uint8_t send(uint8_t addr, const uint8_t* data, int size);
uint8_t send_mcp23017(uint8_t addr, uint8_t reg, uint8_t* data, int size);
uint8_t start(uint8_t addr, uint32_t dir);
uint8_t rw(uint32_t dir, uint8_t *data, int size);
uint8_t rw(uint32_t dir, uint8_t *data, int size, int ack);
void stop();
uint8_t receive_mcp23017(uint8_t addr, uint8_t reg, uint8_t* data, int size);
private:
	void init();
};

} // namespace i2c
} // namespace kip5

#endif /* INC_I2C_INC_I2C_HPP_ */
