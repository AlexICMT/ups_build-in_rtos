/* ups_build_in_rtos
 * /ups_build_in_rtos/hacks/src/thread.cpp
 * thread.cpp
 * 
 *  Created on: 15 дек. 2021 г.
 *  	  Time: 11:29:39
 *      Author: AlexICMT
 */

#include "thread.hpp"

namespace kip5 {
namespace thread {

// TODO newlib _impure_ptr each thread can have its own reentrancy structure

Thread::Thread(const char* name, int stackDepth, int priority)
{
	if(xTaskCreate(run, name, stackDepth, this, priority, &xHandle) != pdPASS)
		for(;;);
}

Thread::~Thread()
{
#if INCLUDE_vTaskDelete
	if(xHandle != nullptr)
	{
		vTaskDelete(xHandle);
	}
#endif
}

void Thread::run(void* param)
{
	// Если Thread::execute() определен как = 0, то код увеличивается примерно на 40кб.
	static_cast<Thread*>(param)->execute();
}

} // namespace thread
} // namespace kip5


