/* ups_build_in_rtos
 * /ups_build_in_rtos/hacks/inc/thread.hpp
 * thread.hpp
 * 
 *  Created on: 15 дек. 2021 г.
 *  	  Time: 11:29:22
 *      Author: AlexICMT
 *
 *
 */

#ifndef HACKS_INC_THREAD_HPP_
#define HACKS_INC_THREAD_HPP_

#include "eclipse_header_wrapper.hpp"

C_HEADERS_WRAPPER_BEGIN(freertos)
#include "FreeRTOS.h"
#include "task.h"
C_HEADERS_WRAPPER_END(freertos)

namespace kip5 {
namespace thread {

/**
 * @brief Поток FreeRTOS
 */
class Thread
{
public:
	Thread(const char* name = nullptr, int stackDepth = configMINIMAL_STACK_SIZE, int priority = tskIDLE_PRIORITY);
	virtual ~Thread();

	inline TaskHandle_t get_id() const {return xHandle;}
	inline uint32_t now() const {return xTaskGetTickCount();}

protected:
//	virtual void execute() = 0;		// Приводит к увеличению кода примерно на 40кб.
									// Предположительно генерируется исключение в static_cast.
	virtual void execute() {}

	inline void yield() {taskYIELD();}
	inline void sleep_for(uint32_t sleep_duration) {vTaskDelay(sleep_duration);}
	inline void sleep_until(uint32_t sleep_time) {vTaskDelayUntil(&previousWakeTime, sleep_time);}

private:
	static void run(void* param);

	TaskHandle_t xHandle {nullptr};
	TickType_t previousWakeTime {now()};
};

} // namespace thread
} // namespace kip5



#endif /* HACKS_INC_THREAD_HPP_ */
