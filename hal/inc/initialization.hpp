/*
 * initialization.hpp
 *
 *  Created on: Oct 27, 2021
 *      Author: AlexICMT
 */

#ifndef INC_INITIALIZATION_HPP_
#define INC_INITIALIZATION_HPP_

#include "low_level_init.hpp"

namespace kip5 {
namespace init {

class Initialization : public Low_init
{
public:
	Initialization() = default;
	Initialization(const Initialization&) = delete;
//	Low_init* low_init = nullptr;
	void init();
};

} //namespace mdr32
} //namespace kip5

#endif /* INC_INITIALIZATION_HPP_ */
