/*
 * initialization.cpp
 *
 *  Created on: Oct 27, 2021
 *      Author: AlexICMT
 */

#include "initialization.hpp"

namespace kip5 {
namespace init {

void Initialization::init()
{
	low_level_init();
	frequencyClkPll(1,9);
	SystemCoreClockUpdate();
}

} //namespace mdr32
} //namespace kip5

