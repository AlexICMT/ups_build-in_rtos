/* ups_build-in
 * /ups_build-in/src/test/help.cpp
 * help.cpp
 * 
 *  Created on: 22 нояб. 2021 г.
 *  	  Time: 14:29:28
 *      Author: AlexICMT
 */
#include "help.hpp"

	kip5::view_ctrl_bus::View_Ctrl_Bus bus{MDR_PORTA};
	kip5::button::Button buttons;

	volatile uint32_t keyboardPort = 0;

#ifdef USE_FREERTOS_0


	void (*handler)() = nullptr;
	volatile ticks_t ms_counter = 0;


	namespace kip5::handlers {
	extern "C" void SysTick_Handler(void)
	{
		tick();
	}
	}
#endif

void readButtonPort()
{
	bus.write(0x7F);
	bus.setMode(0x80);
	for(int i = 0; i < 100; ++i) __NOP();
	keyboardPort = bus.read();
	bus.inputMode();
	bus.write(0xFF);
}

void buttonsInit()
{
	if(!bus.locked())
	{
		readButtonPort();
	}
	uint32_t buf = keyboardPort;
	buf >>= 2;
	buf = ~buf;
	buf &= kip5::KEY::ALL;
	buttons.handler(buf);
}
