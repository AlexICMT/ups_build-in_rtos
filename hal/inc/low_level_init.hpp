/*
 * low_level_init.hpp
 *
 *  Created on: Oct 27, 2021
 *      Author: AlexICMT
 */

#ifndef INC_LOW_LEVEL_INIT_HPP_
#define INC_LOW_LEVEL_INIT_HPP_

#include "MDR32Fx.h"
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"

namespace kip5 {
namespace init {

extern "C" void low_level_init(void);

class Low_init
{
public:

	Low_init() = default;
	Low_init(const Low_init&) = delete;

	void frequencyClkPll(unsigned pll_on, unsigned int pll_mul);
	void initClkForAllPorts();
	void initAllPorts();
};

} //namespace mdr32
} //namespace kip5





#endif /* INC_LOW_LEVEL_INIT_HPP_ */
