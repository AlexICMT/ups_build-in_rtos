/* ups_build-in
 * /ups_build-in/inc/test/help.hpp
 * help.hpp
 * 
 *  Created on: 22 нояб. 2021 г.
 *  	  Time: 14:08:59
 *      Author: AlexICMT
 */

#ifndef INC_TEST_HELP_HPP_
#define INC_TEST_HELP_HPP_

#include "view_ctrl_bus.hpp"
#include "button.hpp"
//#include "ow.hpp"

extern kip5::view_ctrl_bus::View_Ctrl_Bus bus;
extern kip5::button::Button buttons;

extern volatile uint32_t keyboardPort;
extern void readButtonPort();
extern void buttonsInit();

#ifdef USE_FREERTOS_0

extern void (*handler)();
typedef uint32_t ticks_t;
extern volatile ticks_t ms_counter;

namespace handlers {
extern "C" void SysTick_Handler(void);
} // namespace handlers

inline static void tick()
	{
		++ms_counter;
		if(handler)
			{
					handler();
			}
	}

#endif

#endif /* INC_TEST_HELP_HPP_ */
