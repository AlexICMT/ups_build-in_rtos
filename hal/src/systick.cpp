/* ups_build_in_rtos
 * /ups_build_in_rtos/hal/src/systick.cpp
 * systick.cpp
 * 
 *  Created on: 15 дек. 2021 г.
 *  	  Time: 10:23:25
 *      Author: AlexICMT
 */

#include "systick.hpp"

#include "eclipse_header_wrapper.hpp"

#if USE_FREERTOS != 0
C_HEADERS_WRAPPER_BEGIN(freertos)
#include "FreeRTOS.h"
#include "task.h"
C_HEADERS_WRAPPER_END(freertos)
#endif

namespace kip5 {
namespace systick {

SysTickTimer systick;

#if USE_FREERTOS == 0
volatile SysTickTimer::ticks_t SysTickTimer::ms_delayCount = 0;
#endif
volatile SysTickTimer::ticks_t SysTickTimer::ms_counter = 0;

//std::function<void(void)> SysTickTimer::handler;
void (*SysTickTimer::handler)() = nullptr;

void SysTickTimer::start()
{
#if USE_FREERTOS == 0
	SysTick_Config(SystemCoreClock / FREQUENCY_HZ);
#endif
}

void SysTickTimer::sleep(ticks_t ticks)
{
#if USE_FREERTOS == 0
	ms_delayCount = ticks;
	while(ms_delayCount != 0u);
#else
	vTaskDelay(ticks);
#endif
}

#if USE_FREERTOS == 0
namespace handlers {
extern "C" void SysTick_Handler(void)
{
	SysTickTimer::tick();
}
} // namespace handlers
#else
extern "C" void vApplicationTickHook(void)
{
	SysTickTimer::tick();
}
#endif


#include <sys/time.h>

extern "C" int _gettimeofday(struct timeval *tv, void *tzvp)
{
	uint64_t t_ms = systick.getMsCounter();
	tv->tv_sec = t_ms / 1000;
	tv->tv_usec = (t_ms % 1000) * 1000;
	(void)tzvp;
	return 0;
}

} // namespace systick
} // namespace kip5

