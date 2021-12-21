/* ups_build_in_rtos
 * /ups_build_in_rtos/src/app.cpp
 * app.cpp
 * 
 *  Created on: 15 дек. 2021 г.
 *  	  Time: 11:14:02
 *      Author: AlexICMT
 */

#include "app.hpp"
#include "framework.hpp"

extern "C" int __write(char* ptr, int len)
{
	while(len--)
		charPainterMin->putChar(*ptr++);
	return 0;
}

namespace kip5 {
namespace app {

App* App::inst = nullptr;

App* App::instance()
{
	if (inst == nullptr)
		inst = new App;
	return inst;
}

void App::run()
{
	charPainterMin->clear();
	charPainterMin->update();

	vTaskStartScheduler();

	for(;;);
}

} // namespace app

extern "C" void vApplicationIdleHook(void)
{
	//app::App::ulIdleCC++;
}

extern "C" void vApplicationMallocFailedHook(void)
{
    for(;;);
}

extern "C" void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName)
{
    (void)pcTaskName;
    (void)pxTask;
    for(;;);
}


} // namespace kip5

