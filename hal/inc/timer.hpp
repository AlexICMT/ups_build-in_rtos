/* ups_build-in
 * /ups_build-in/inc/tim_inc/timer.hpp
 * timer.hpp
 * 
 *  Created on: 23 нояб. 2021 г.
 *  	  Time: 10:24:59
 *      Author: AlexICMT
 */

#ifndef INC_TIM_INC_TIMER_HPP_
#define INC_TIM_INC_TIMER_HPP_

//#include "MDR32F9Qx_config.h"
#include <cstdint>

namespace kip5 {

namespace handlers {
extern "C" void Timer2_IRQHandler();

extern "C" void Timer1_IRQHandler();
} // namespace handlers

namespace timer {

class Timer
{
public:
	Timer();
	Timer(const Timer&) = delete;

	void tim_config();
	void pwm_port_tim_config();
	void pwm_tim_config();
	void meas_tim_config();//frq by frq
	inline uint16_t get_cnt_pls() const {return static_cast<uint16_t>(cnt_pls_loc);};
	void read();
	static void tim_config_rtos();
	static void start_tim();
	friend void handlers::Timer2_IRQHandler();
	friend void handlers::Timer1_IRQHandler();
private:
	int cnt_pls_loc = 0;
};

} // namespace timer
} // namespace kip5

#endif /* INC_TIM_INC_TIMER_HPP_ */
