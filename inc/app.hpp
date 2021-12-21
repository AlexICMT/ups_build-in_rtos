/* ups_build_in_rtos
 * /ups_build_in_rtos/inc/app.hpp
 * app.hpp
 * 
 *  Created on: 15 дек. 2021 г.
 *  	  Time: 11:13:53
 *      Author: AlexICMT
 */

#ifndef INC_APP_HPP_
#define INC_APP_HPP_

#include "eclipse_header_wrapper.hpp"

C_HEADERS_WRAPPER_BEGIN(freertos)
#include "FreeRTOS.h"
#include "semphr.h"
C_HEADERS_WRAPPER_END(freertos)

namespace kip5 {
namespace app {

class App
{
	App() = default;
public:
	static App* instance();
	//static unsigned long ulIdleCC = 0UL; //ulIdleCycleCount
	void run();
private:
	static App* inst;
};

} // namespace app
} // namespace kip5



#endif /* INC_APP_HPP_ */
