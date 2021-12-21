/* ups_build_in_rtos
 * /ups_build_in_rtos/hal/inc/systick.hpp
 * systick.hpp
 * 
 *  Created on: 15 дек. 2021 г.
 *  	  Time: 10:23:00
 *      Author: AlexICMT
 *
 * History:
 *
 * @file hal_systick.hpp
 * @date 13.07.2015
 * @author vlunyov
 * @brief HAL systick timer
 *
 * Last modified by: $Author: vlunyov $
 * $Revision: 1571 $
 * $Date: 2017-03-05 17:35:14 +0300 (Вс, 05 мар 2017) $
 *
 */

#ifndef HAL_INC_SYSTICK_HPP_
#define HAL_INC_SYSTICK_HPP_

#include "hal_defs.hpp"
#include "defs.hpp"

#include "eclipse_header_wrapper.hpp"

C_HEADERS_WRAPPER_BEGIN(cortex)
#include "core_cm3.h"
C_HEADERS_WRAPPER_END(cortex)

namespace kip5 {
namespace systick {
#if USE_FREERTOS == 0
namespace handlers {
extern "C" void SysTick_Handler(void);
} // namespace handlers
#else
extern "C" void vApplicationTickHook(void);
#endif
/**
 * @brief Системный таймер ядра Cortex
 */
class SysTickTimer
{
public:
	typedef uint32_t ticks_t;
//	static std::function<void(void)> handler;
	static void (*handler)();

private:
	static constexpr ticks_t FREQUENCY_HZ = 1000u;
#if USE_FREERTOS == 0
	static volatile ticks_t ms_delayCount;
#endif
	static volatile ticks_t ms_counter;

public:
	SysTickTimer() = default;
	SysTickTimer(const SysTickTimer&) = delete;

	static void start();

	static void sleep(ticks_t ticks);

	constexpr ticks_t getSysTickTimerResolution() const
	{
		return SysTick_LOAD_RELOAD_Msk + 1;
	}

	constexpr ticks_t getFrequency() const
	{
		return FREQUENCY_HZ;
	}

	inline static ticks_t getTicksTop()
	{
		return SysTick->LOAD;
	}

	inline static ticks_t getTicks()
	{
		return SysTick->VAL;
	}

	inline static ticks_t getMsCounter()
	{
		return ms_counter;
	}

	inline static ticks_t getClkCounter()
	{
		ticks_t timer;
		ticks_t counter;

//		__disable_irq();
//		timer = SysTick->VAL;
//		counter = ms_counter;
//		__enable_irq();
		do {
			counter = ms_counter;
			timer = SysTick->VAL;
		} while(counter != ms_counter);

		ticks_t top = SysTick->LOAD;
		timer = top - timer;
		return counter * (top + 1) + timer;
	}

private:
#if USE_FREERTOS == 0
	friend void handlers::SysTick_Handler(void);
#else
	friend void vApplicationTickHook(void);
#endif
	inline static void tick()
	{
		++ms_counter;
#if USE_FREERTOS == 0
		ticks_t buf = ms_delayCount;
		if(buf != 0u)
		{
			--buf;
			ms_delayCount = buf;
		}
#endif
		if(handler)
		{
			handler();
		}
	}
};

extern SysTickTimer systick;

} // namespace systick
} // namespace kip5

#endif /* HAL_INC_SYSTICK_HPP_ */
